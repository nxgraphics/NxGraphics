package com.hotstuff.main;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.Point;
//import android.opengl.EGLContext;
import android.os.AsyncTask;
import android.os.Build;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.WindowManager;

import com.qualcomm.vuforia.CameraCalibration;
import com.qualcomm.vuforia.CameraDevice;
import com.qualcomm.vuforia.Matrix44F;
import com.qualcomm.vuforia.PIXEL_FORMAT;
import com.qualcomm.vuforia.Renderer;
import com.qualcomm.vuforia.State;
import com.qualcomm.vuforia.Tool;
import com.qualcomm.vuforia.Vec2F;
import com.qualcomm.vuforia.Vec2I;
import com.qualcomm.vuforia.VideoBackgroundConfig;
import com.qualcomm.vuforia.VideoBackgroundTextureInfo;
import com.qualcomm.vuforia.VideoMode;
import com.qualcomm.vuforia.Vuforia;
import com.qualcomm.vuforia.Vuforia.UpdateCallbackInterface;


public class VuforiaSession implements UpdateCallbackInterface
{
    
    private static final String LOGTAG = "Vuforia_Sample_Applications";
    
    // Reference to the current activity
    private Activity m_activity;
    private VuforiaControl m_sessionControl;
    
    // Flags
    private boolean m_started = false;
    
    // Display size of the device:
    private int mScreenWidth = 0;
    private int mScreenHeight = 0;
    
    private int xSize = 0;
    private int ySize = 0;
    
    // The async tasks to initialize the Vuforia SDK:
    private InitVuforiaTask mInitVuforiaTask;
    private LoadTrackerTask mLoadTrackerTask;
    
    // An object used for synchronizing Vuforia initialization, dataset loading
    // and the Android onDestroy() life cycle event. If the application is
    // destroyed while a data set is still being loaded, then we wait for the
    // loading operation to finish before shutting down Vuforia:
    private Object mShutdownLock = new Object();
    
    // Vuforia initialization flags:
    private int mVuforiaFlags = 0;
    
    // Holds the camera configuration to use upon resuming
    private int mCamera = CameraDevice.CAMERA.CAMERA_DEFAULT;
    
    // Stores the projection matrix to use for rendering purposes
    private Matrix44F mProjectionMatrix;
    
    // Stores orientation
    private boolean mIsPortrait = false;
    
    
    public VuforiaSession( VuforiaControl sessionControl)
    {
        m_sessionControl = sessionControl;
    }
    
    
    // Initializes Vuforia and sets up preferences.
    public void initAR(Activity activity, int screenOrientation)
    {
    	VuforiaExceptions vuforiaException = null;
        m_activity = activity;
        
        if ((screenOrientation == ActivityInfo.SCREEN_ORIENTATION_SENSOR) && (Build.VERSION.SDK_INT > Build.VERSION_CODES.FROYO))
            screenOrientation = ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR;
        
        // Apply screen orientation
        m_activity.setRequestedOrientation(screenOrientation);
        
        updateActivityOrientation();
        
        // Query display dimensions:
        storeScreenDimensions();
        
        // As long as this window is visible to the user, keep the device's
        // screen turned on and bright:
        m_activity.getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
            WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        
        mVuforiaFlags = Vuforia.GL_20;
        
        // Initialize Vuforia SDK asynchronously to avoid blocking the
        // main (UI) thread.
        //
        // NOTE: This task instance must be created and invoked on the
        // UI thread and it can be executed only once!
        if (mInitVuforiaTask != null)
        {
            String logMessage = "Cannot initialize SDK twice";
            vuforiaException = new VuforiaExceptions(VuforiaExceptions.VUFORIA_ALREADY_INITIALIZATED,logMessage);
            Log.e(LOGTAG, logMessage);
        }
        
        if (vuforiaException == null)
        {
            try
            {
                mInitVuforiaTask = new InitVuforiaTask();
                mInitVuforiaTask.execute();
            } catch (Exception e)
            {
                String logMessage = "Initializing Vuforia SDK failed";
                vuforiaException = new VuforiaExceptions(VuforiaExceptions.INITIALIZATION_FAILURE,logMessage);
                Log.e(LOGTAG, logMessage);
            }
        }
        
        if (vuforiaException != null)
            m_sessionControl.onInitARDone( vuforiaException);
 
    }
    
    
    // Starts Vuforia, initialize and starts the camera and start the trackers
    public void startAR(int camera) throws VuforiaExceptions
    {
        String error;
        mCamera = camera;
        if (!CameraDevice.getInstance().init(camera))
        {
            error = "Unable to open camera device: " + camera;
            Log.e(LOGTAG, error);
            throw new VuforiaExceptions( VuforiaExceptions.CAMERA_INITIALIZATION_FAILURE, error);
        }
        
        Log.d(LOGTAG,  "startAR :  configureVideoBackground...  " );
        configureVideoBackground();
        
        if (!CameraDevice.getInstance().selectVideoMode(CameraDevice.MODE.MODE_DEFAULT))
        {
            error = "Unable to set video mode";
            Log.e(LOGTAG, error);
            throw new VuforiaExceptions(VuforiaExceptions.CAMERA_INITIALIZATION_FAILURE, error);
        }
        
        if (!CameraDevice.getInstance().start())
        {
            error = "Unable to start camera device: " + camera;
            Log.e(LOGTAG, error);
            throw new VuforiaExceptions(VuforiaExceptions.CAMERA_INITIALIZATION_FAILURE, error);
        }
        
        Log.d(LOGTAG, "Vuforiasession Vuforia.setFrameFormat... PIXEL_FORMAT.RGB565 ");
        //Vuforia.setFrameFormat(PIXEL_FORMAT.RGB565, true);
        
        Vuforia.setFrameFormat(PIXEL_FORMAT.RGB565, true);
        
        Log.d(LOGTAG, "Vuforiasession setProjectionMatrix(); ...");
        setProjectionMatrix();
        
        Log.d(LOGTAG, "Vuforiasession m_sessionControl.doStartTrackers()(); ...");
        m_sessionControl.doStartTrackers();
        
        
        
        try
        {
        	Log.d(LOGTAG, "Vuforiasession setFocusMode .. : FOCUS_MODE_TRIGGERAUTO ");
            setFocusMode(CameraDevice.FOCUS_MODE.FOCUS_MODE_TRIGGERAUTO);
        } catch (VuforiaExceptions exceptionTriggerAuto)
        {
        	Log.d(LOGTAG, "Vuforiasession setFocusMode Error switiching to: FOCUS_MODE_NORMAL ");
            setFocusMode(CameraDevice.FOCUS_MODE.FOCUS_MODE_NORMAL);
        }
    }
    
    
    // Stops any ongoing initialization, stops Vuforia
    public void stopAR() throws VuforiaExceptions
    {
        // Cancel potentially running tasks
        if (mInitVuforiaTask != null && mInitVuforiaTask.getStatus() != InitVuforiaTask.Status.FINISHED)
        {
            mInitVuforiaTask.cancel(true);
            mInitVuforiaTask = null;
        }
        
        if (mLoadTrackerTask != null && mLoadTrackerTask.getStatus() != LoadTrackerTask.Status.FINISHED)
        {
            mLoadTrackerTask.cancel(true);
            mLoadTrackerTask = null;
        }
        
        mInitVuforiaTask = null;
        mLoadTrackerTask = null;
        
        m_started = false;
        
        Log.e("stopCamera", "stopCamera");
        stopCamera();
        
        // Ensure that all asynchronous operations to initialize Vuforia
        // and loading the tracker datasets do not overlap:
        synchronized (mShutdownLock)
        {
            
            boolean unloadTrackersResult;
            boolean deinitTrackersResult;
            
            Log.e("doUnloadTrackersData", "doUnloadTrackersData");
            // Destroy the tracking data set:
            unloadTrackersResult = m_sessionControl.doUnloadTrackersData();
            
            Log.e("doDeinitTrackers", "doDeinitTrackers");
            // Deinitialize the trackers:
            deinitTrackersResult = m_sessionControl.doDeinitTrackers();
            
            Log.e("Vuforia.deinit()", "Vuforia.deinit()");
            // Deinitialize Vuforia SDK:
            Vuforia.deinit();
            
            Log.e("Vuforia.deinit() DONE", "Vuforia.deinit() DONE");
            
            if (!unloadTrackersResult)
                throw new VuforiaExceptions(
                		VuforiaExceptions.UNLOADING_TRACKERS_FAILURE,
                    "Failed to unload trackers\' data");
            
            if (!deinitTrackersResult)
                throw new VuforiaExceptions(
                		VuforiaExceptions.TRACKERS_DEINITIALIZATION_FAILURE,
                    "Failed to deinitialize trackers");
            
        }
        
        Log.e("stopAR END", "stopAR END");
    }
    
    
    // Resumes Vuforia, restarts the trackers and the camera
    public void resumeAR() throws VuforiaExceptions
    {
        // Vuforia-specific resume operation
        Vuforia.onResume();
        
        if (m_started)
            startAR(mCamera);
    }
    
    
    // Pauses Vuforia and stops the camera
    public void pauseAR() throws VuforiaExceptions
    {
        if (m_started)
            stopCamera();
        
        Vuforia.onPause();
    }
    
    
    // Gets the projection matrix to be used for rendering
    public Matrix44F getProjectionMatrix()
    {
        return mProjectionMatrix;
    }
    
    
    // Callback called every cycle
    @Override
    public void QCAR_onUpdate(State s)
    {
        m_sessionControl.onQCARUpdate(s);
    }
    
    
    // Manages the configuration changes
    public void onConfigurationChanged()
    {
        updateActivityOrientation();
        
        storeScreenDimensions();
        
        if (isARRunning())
        {
            // configure video background
            configureVideoBackground();
            
            // Update projection matrix:
            setProjectionMatrix();
        }
        
    }
    
    
    // Methods to be called to handle lifecycle
    public void onResume()
    {
        Vuforia.onResume();
    }
    
    
    public void onPause()
    {
        Vuforia.onPause();
    }
    
    
    public void onSurfaceChanged(int width, int height)
    {
        Vuforia.onSurfaceChanged(width, height);
    }
    
    
    public void onSurfaceCreated()
    {
        Vuforia.onSurfaceCreated();
    }
    
    // An async task to initialize Vuforia asynchronously.
    private class InitVuforiaTask extends AsyncTask<Void, Integer, Boolean>
    {
        // Initialize with invalid value:
        private int mProgressValue = -1;
        
        
        protected Boolean doInBackground(Void... params)
        {
            // Prevent the onDestroy() method to overlap with initialization:
            synchronized (mShutdownLock)
            {
                Vuforia.setInitParameters(m_activity, mVuforiaFlags, "AWXnSMf/////AAAAAcZf3u8xkEMgh9V0NLg1BVo90TQlLzf+hMh5xBnRy6LyQEjBXkwVRsnItW9L22Ub5IK2vgOIbW7WNkROnM3GKg6JvBoogRv8ADyWqIGHoDGkVO88gbJ6It6WEztDJOQmyR6NwJf4GXWZWsYQV31O1DmRQsGwIrRe54cn2IRUNo+Ft2TREHhK8kfQmVkDnw0I2xtsGgoJ4XULbejVtNWpoX7ii+1IzfmOSK9Rb7tK5IdJOjB9iWEHDv9ZwXqRLN4vlC2KKfnHcMquos+IuaLNZ4py1uc9tyT55C6zsNXeZPJEY2etDA3VhPtmZwPr07ZguBpRQ2oFmtrVwAdtkUb4iM6xv3zzrhqWf7EDR3ZT8V6I");
                
                do
                {
                    // Vuforia.init() blocks until an initialization step is
                    // complete, then it proceeds to the next step and reports
                    // progress in percents (0 ... 100%).
                    // If Vuforia.init() returns -1, it indicates an error.
                    // Initialization is done when progress has reached 100%.
                    mProgressValue = Vuforia.init();
                    
                    // Publish the progress value:
                    publishProgress(mProgressValue);
                    
                    // We check whether the task has been canceled in the
                    // meantime (by calling AsyncTask.cancel(true)).
                    // and bail out if it has, thus stopping this thread.
                    // This is necessary as the AsyncTask will run to completion
                    // regardless of the status of the component that
                    // started is.
                } while (!isCancelled() && mProgressValue >= 0
                    && mProgressValue < 100);
                
                return (mProgressValue > 0);
            }
        }
        
        
        protected void onProgressUpdate(Integer... values)
        {
            // Do something with the progress value "values[0]", e.g. update
            // splash screen, progress bar, etc.
        }
        
        
        protected void onPostExecute(Boolean result)
        {
            // Done initializing Vuforia, proceed to next application
            // initialization status:
            
        	VuforiaExceptions vuforiaException = null;
            
            if (result)
            {
                Log.d(LOGTAG, "InitVuforiaTask.onPostExecute: Vuforia "
                    + "initialization successful");
                
                boolean initTrackersResult;
                initTrackersResult = m_sessionControl.doInitTrackers();
                
                if (initTrackersResult)
                {
                    try
                    {
                        mLoadTrackerTask = new LoadTrackerTask();
                        mLoadTrackerTask.execute();
                    } catch (Exception e)
                    {
                        String logMessage = "Loading tracking data set failed";
                        vuforiaException = new VuforiaExceptions(
                        		VuforiaExceptions.LOADING_TRACKERS_FAILURE,
                            logMessage);
                        Log.e(LOGTAG, logMessage);
                        m_sessionControl.onInitARDone(vuforiaException);
                    }
                    
                } else
                {
                    vuforiaException = new VuforiaExceptions(
                    		VuforiaExceptions.TRACKERS_INITIALIZATION_FAILURE,
                        "Failed to initialize trackers");
                    m_sessionControl.onInitARDone(vuforiaException);
                }
            } else
            {
                String logMessage;
                
                // NOTE: Check if initialization failed because the device is
                // not supported. At this point the user should be informed
                // with a message.
                if (mProgressValue == Vuforia.INIT_DEVICE_NOT_SUPPORTED)
                {
                    logMessage = "Failed to initialize Vuforia because this "
                        + "device is not supported.";
                } else
                {
                    logMessage = "Failed to initialize Vuforia.";
                }
                
                // Log error:
                Log.e(LOGTAG, "InitVuforiaTask.onPostExecute: " + logMessage
                    + " Exiting.");
                
                // Send Vuforia Exception to the application and call initDone
                // to stop initialization process
                vuforiaException = new VuforiaExceptions(
                		VuforiaExceptions.INITIALIZATION_FAILURE,
                    logMessage);
                m_sessionControl.onInitARDone(vuforiaException);
            }
        }
    }
    
    // An async task to load the tracker data asynchronously.
    private class LoadTrackerTask extends AsyncTask<Void, Integer, Boolean>
    {
        protected Boolean doInBackground(Void... params)
        {
            // Prevent the onDestroy() method to overlap:
            synchronized (mShutdownLock)
            {
                // Load the tracker data set:
                return m_sessionControl.doLoadTrackersData();
            }
        }
        
        
        protected void onPostExecute(Boolean result)
        {
            
        	VuforiaExceptions vuforiaException = null;
            
            Log.d(LOGTAG, "LoadTrackerTask.onPostExecute: execution "
                + (result ? "successful" : "failed"));
            
            if (!result)
            {
                String logMessage = "Failed to load tracker data.";
                // Error loading dataset
                Log.e(LOGTAG, logMessage);
                vuforiaException = new VuforiaExceptions(
                		VuforiaExceptions.LOADING_TRACKERS_FAILURE,
                    logMessage);
            } else
            {
                // Hint to the virtual machine that it would be a good time to
                // run the garbage collector:
                //
                // NOTE: This is only a hint. There is no guarantee that the
                // garbage collector will actually be run.
                System.gc();
                
                Vuforia.registerCallback(VuforiaSession.this);
                
                m_started = true;
            }
            
            // Done loading the tracker, update application status, send the
            // exception to check errors
            m_sessionControl.onInitARDone(vuforiaException);
        }
    }
    
    
    // Stores screen dimensions
    private void storeScreenDimensions()
    {
        // Query display dimensions:
        DisplayMetrics metrics = new DisplayMetrics();
        m_activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
        mScreenWidth = metrics.widthPixels;
        mScreenHeight = metrics.heightPixels;
    }
    
    
    // Stores the orientation depending on the current resources configuration
    private void updateActivityOrientation()
    {
        Configuration config = m_activity.getResources().getConfiguration();
        
        switch (config.orientation)
        {
            case Configuration.ORIENTATION_PORTRAIT:
                mIsPortrait = true;
                break;
            case Configuration.ORIENTATION_LANDSCAPE:
                mIsPortrait = false;
                break;
            case Configuration.ORIENTATION_UNDEFINED:
            default:
                break;
        }
        
        Log.i(LOGTAG, "Activity is in "
            + (mIsPortrait ? "PORTRAIT" : "LANDSCAPE"));
    }
    
    
    // Method for setting / updating the projection matrix for AR content
    // rendering
    private void setProjectionMatrix()
    {
        CameraCalibration camCal = CameraDevice.getInstance().getCameraCalibration();
        
      


Vec2F size = camCal.getSize();
 Vec2F focalLength = camCal.getFocalLength();

double fovRadians = 2 * Math.atan(0.5f * size.getData()[1] / focalLength.getData()[1]);
double fovDegrees = fovRadians * 180.0f / Math.PI ;   

Log.d("=========>>> FOV IS : ",  Double.toString( fovDegrees  )  );
        
        

        mProjectionMatrix = Tool.getProjectionGL(camCal, 10.0f, 5000.0f);
    }
    
    
    private void stopCamera()
    {
        m_sessionControl.doStopTrackers();
        CameraDevice.getInstance().stop();
        CameraDevice.getInstance().deinit();
    }
    
    
    // Applies auto focus if supported by the current device
    private boolean setFocusMode(int mode) throws VuforiaExceptions
    {
        boolean result = CameraDevice.getInstance().setFocusMode(mode);
        
        if (!result)
            throw new VuforiaExceptions(
            		VuforiaExceptions.SET_FOCUS_MODE_FAILURE,
                "Failed to set focus mode: " + mode);
        
        return result;
    }
    
    
    
    private void configureVideoBackground()
    {
        CameraDevice cameraDevice = CameraDevice.getInstance();
        VideoMode vm = cameraDevice.getVideoMode(CameraDevice.MODE.MODE_DEFAULT);
        
        VideoBackgroundConfig config = new VideoBackgroundConfig();
        config.setEnabled(true);
        config.setPosition(new Vec2I(0, 0));
        
        int xSize = 0, ySize = 0;
        if (mIsPortrait)
        {
            xSize = (int) (vm.getHeight() * (mScreenHeight / (float) vm
                .getWidth()));
            ySize = mScreenHeight;
            
            if (xSize < mScreenWidth)
            {
                xSize = mScreenWidth;
                ySize = (int) (mScreenWidth * (vm.getWidth() / (float) vm
                    .getHeight()));
            }
        } else
        {
            xSize = mScreenWidth;
            ySize = (int) (vm.getHeight() * (mScreenWidth / (float) vm
                .getWidth()));
            
            if (ySize < mScreenHeight)
            {
                xSize = (int) (mScreenHeight * (vm.getWidth() / (float) vm
                    .getHeight()));
                ySize = mScreenHeight;
            }
        }
        
        config.setSize(new Vec2I(xSize, ySize));
        
        Log.i(LOGTAG, "Configure Video Background : Video (" + vm.getWidth()
            + " , " + vm.getHeight() + "), Screen (" + mScreenWidth + " , "
            + mScreenHeight + "), mSize (" + xSize + " , " + ySize + ")");
        
        Renderer.getInstance().setVideoBackgroundConfig(config);
        
    }    
    
    /*
    // Configures the video mode and sets offsets for the camera's image
    private void configureVideoBackground()
    {
        CameraDevice cameraDevice = CameraDevice.getInstance();
        VideoMode vm = cameraDevice.getVideoMode(CameraDevice.MODE.MODE_DEFAULT);
  
        VideoBackgroundConfig config = new VideoBackgroundConfig();
        config.setEnabled(true);
        config.setSynchronous(true);
        config.setPosition(new Vec2I(0, 0));
        
        xSize = 0;
        ySize = 0;
        if (mIsPortrait)
        {
        	 
           // xSize = (int) (vm.getHeight() * (mScreenHeight / (float) vm.getWidth()));
           // ySize = mScreenHeight;
            
           // if (xSize < mScreenWidth)
           // {
            //    xSize = mScreenWidth;
            //    ySize = (int) (mScreenWidth * (vm.getWidth() / (float) vm.getHeight()));
           // } 
        	
        	
        } else
        {
            xSize = mScreenWidth;
            ySize = (int) (vm.getHeight() * (mScreenWidth / (float) vm.getWidth()));
            
            if (ySize < mScreenHeight)
            {
                xSize = (int) (mScreenHeight * (vm.getWidth() / (float) vm.getHeight()));
                ySize = mScreenHeight;
            }
        }
        
        config.setSize(new Vec2I(xSize, ySize));
     
        DisplayMetrics display =  Example.GetInstance().getResources().getDisplayMetrics();
        int width = display.widthPixels;
        int height = display.heightPixels;
        
        Log.i(LOGTAG, "DEVICE SCREEN WIDTH : " + width  + " HEIGHT : " + height );
        
        
        Log.i(LOGTAG, "Configure Video Background : Video (" + vm.getWidth()
            + " , " + vm.getHeight() + "), Screen (" + mScreenWidth + " , "
            + mScreenHeight + "), mSize (" + xSize + " , " + ySize + ")");
        
        Renderer.getInstance().setVideoBackgroundConfig(config);
         
        
    }*/
    
    public int GetXSize() {
    	return xSize;
    }
    
    public int GetYSize() {
    	return ySize;
    } 
    
    public int GetVideoWidth() {
        CameraDevice cameraDevice = CameraDevice.getInstance();
        VideoMode vm = cameraDevice.getVideoMode(CameraDevice.MODE.MODE_DEFAULT);
    	return vm.getWidth();
    }
    
    public int GetVideoHeight() {
        CameraDevice cameraDevice = CameraDevice.getInstance();
        VideoMode vm = cameraDevice.getVideoMode(CameraDevice.MODE.MODE_DEFAULT);
    	return vm.getHeight();
    }    
    
    public int GetScreenWidth() {
    	return mScreenWidth;
    }
    
    public int GetScreenHeight() {
    	return mScreenHeight;
    }      
    
    
    
    
    // Returns true if Vuforia is initialized, the trackers started and the
    // tracker data loaded
    private boolean isARRunning()
    {
        return m_started;
    }
    
}