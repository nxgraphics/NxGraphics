package com.hotstuff.main;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.ShortBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;
import java.util.concurrent.ExecutionException;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.hotstuff.main.DownloadFilesTask.DownloadCallback;
import com.hotstuff.main.VuforiaGlView;
import com.hotstuff.main.R;




 

 

import com.qualcomm.vuforia.CameraCalibration;
import com.qualcomm.vuforia.CameraDevice;
import com.qualcomm.vuforia.DataSet;
import com.qualcomm.vuforia.Frame;
import com.qualcomm.vuforia.Image;
import com.qualcomm.vuforia.ImageTargetBuilder;
//import com.qualcomm.vuforia.ImageTracker;
import com.qualcomm.vuforia.Matrix44F;
import com.qualcomm.vuforia.ObjectTracker;
import com.qualcomm.vuforia.PIXEL_FORMAT;
import com.qualcomm.vuforia.Renderer;
import com.qualcomm.vuforia.STORAGE_TYPE;
import com.qualcomm.vuforia.State;
import com.qualcomm.vuforia.Tool;
import com.qualcomm.vuforia.Trackable;
import com.qualcomm.vuforia.TrackableResult;
import com.qualcomm.vuforia.Tracker;
import com.qualcomm.vuforia.TrackerManager;
import com.qualcomm.vuforia.VIDEO_BACKGROUND_REFLECTION;
import com.qualcomm.vuforia.Vec2F;
import com.qualcomm.vuforia.Vec2I;
import com.qualcomm.vuforia.VideoBackgroundConfig;
import com.qualcomm.vuforia.VideoBackgroundTextureInfo;
import com.qualcomm.vuforia.VideoMode;
import com.qualcomm.vuforia.Vuforia;
 
 
 

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Color;
import android.graphics.ImageFormat;
import android.graphics.PixelFormat;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.Camera.CameraInfo;
import android.net.Uri;
import javax.microedition.khronos.egl.EGLContext;

import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;

import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.Matrix;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.provider.MediaStore;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.ViewGroup;
import android.view.SurfaceHolder.Callback;
import android.view.ViewGroup.LayoutParams;
import android.view.SurfaceView;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.CheckBox;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;



 


public class Example extends Activity implements SensorEventListener, VuforiaControl, VuforiaMenuInterface
{

	
	 String ipServer = "10.0.0.3";
	
	protected Handler handler = null;
	protected SurfaceView  surfaceView = null;
	protected Surface lastSurface = null;
	 
	private List<String> mVideoFiles = null;

	private int viewportPosition_x;   
	private int viewportPosition_y;
	private int viewportSize_x;   
	private int viewportSize_y; 
 
	private Runnable renderer = null;
	private boolean paused = false;
	private boolean initOGRE = false;
	private boolean deleteOGRE = false;
	private AssetManager assetMgr = null;
	
	private boolean initvuforia = false;
	
	private VuforiaSession vuforiaAppSession = null; 
    private static final String LOGTAG = "UserDefinedTargets";
	// Our OpenGL view:
    //private VuforiaGlView mGlView;
    // Our renderer:
     //private VuforiaTargetRenderer mRenderer;
    // The textures we will use for rendering:
    private Vector<VuforiaTexture> mTextures;
    // View overlays to be displayed in the Augmented View
    private RelativeLayout mUILayout;
    private View mBottomBar;
    private View mCameraButton;
    // Alert dialog for displaying SDK errors
    private AlertDialog mDialog;
    int targetBuilderCounter = 1;
    DataSet dataSetUserDef = null;
    private GestureDetector mGestureDetector;
    private VuforiaMenu mSampleAppMenu;
    private ArrayList<View> mSettingsAdditionalViews;
    private boolean mFlash = false;
    private boolean mContAutofocus = false;
    private boolean mExtendedTracking = false;
    private View mFlashOptionView;
    
    byte [] mBuffer =null;
	   
   public void sendFile( String filepath ) { 
	   
	   NxFileUploader up = new NxFileUploader( "http://"+ipServer+"/upload.php", filepath );
	   
	   try {
		up.execute("" , null, null).get();
	} catch (InterruptedException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	} catch (ExecutionException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
   
   }
   
   
   public void downloadFiles(  String folderUrl , String whereLocal ) { 
	   
	   
		ArrayList<String> insertMediaFiles = new ArrayList<String>();
		ArrayList<String> insertMediaDstFolders = new ArrayList<String>();
		
		
		File f = new File( whereLocal );
		if( !f.exists() ) f.mkdirs();
		
		insertMediaFiles.add( folderUrl );
    	insertMediaDstFolders.add(  whereLocal );//Environment.getExternalStorageDirectory()+"/" );  
	   
	   boolean isFileToDownload = false;
	   try {
		new DownloadFilesTask(  isFileToDownload, insertMediaFiles, insertMediaDstFolders, null,
					new DownloadCallback(){
				    	@Override
				    	public void onMessage( String msg, int progress1, int progress2) {
				    	 		//sendTaskMsg( "", msg, progress1, progress2 );	
				    	}

				    	@Override
						public void OnFinished() { 
				    		Log.i("MEDIA INSERT", "===> Downloads Done !" );
						
						}
				    	
				    	@Override
				    	public void onError( int errorType ) { 
				    		
				    	}
					}
			 ).executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR).get() ;
	} catch (InterruptedException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	} catch (ExecutionException e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}// execute downloads 	   
	   
	   
   }


    	 

    
    
    private Camera openFrontFacingCameraGingerbread() {
	    int cameraCount = 0;
	    Camera cam = null;
	    Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
	    cameraCount = Camera.getNumberOfCameras();
	    for (int camIdx = 0; camIdx < cameraCount; camIdx++) {
	        Camera.getCameraInfo(camIdx, cameraInfo);
	        if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
	            try {
	                cam = Camera.open(camIdx);
	            } catch (RuntimeException e) {
	                Log.e("", "Camera failed to open: " + e.getLocalizedMessage());
	            }
	        }
	    }

	    return cam;
	}	
    
    HandlerThread mCameraThread;
    Handler mCameraHandler;
    
    
    
    
    
    class PreviewCallback implements Camera.PreviewCallback {

    	  //private static final String TAGd = PreviewCallback.class.getSimpleName();

    	  
    	  private Handler previewHandler;
    	  private int previewMessage;

    	  PreviewCallback( ) {
    		  
    		 
    	     
    	  }

    	  void setHandler(Handler previewHandler, int previewMessage) {
    	    this.previewHandler = previewHandler;
    	    this.previewMessage = previewMessage;
    	  }

    	  @Override
    	  public void onPreviewFrame(byte[] data, Camera camera) {
    		  
    		  
    		  Log.e("", "-------------------------------------");
    		  
    
    	  }

    	}    
     
    
    
    // NxVideoFrontCameraView mFrontView = null;
    
    private VuforiaLoadingDialogHandler loadingDialogHandler = new VuforiaLoadingDialogHandler(this);
    
    VuforiaFreeframe refFreeFrame;
    
    boolean mIsDroidDevice = false;	
 
    
    
    static Example mInstance;
	 
	 static Example GetInstance(){ 
		 
		 return  mInstance; 
	 }
	 
	 
	 @Override
		public void onBackPressed() {
		    final AlertDialog.Builder alert = new AlertDialog.Builder( this );
		    final TextView input = new TextView( this );
		    input.setText( " Close Application ? "    );
		    alert.setView(input);
		    alert.setPositiveButton( "Yes",  new AlertDialog.OnClickListener() {
		        public void onClick(DialogInterface dialog, int whichButton) {
		        	initOGRE = false;
		        	
		        	deleteOGRE = true;
		        	
		        	 
		        	
		        	 handler.removeCallbacks(renderer);
		        
		              finish();
		        }
		    });
				        
		    alert.setNegativeButton("No", new DialogInterface.OnClickListener() {
		        public void onClick(DialogInterface dialog, int whichButton) {
		
		        }
		    });
		    
		    alert.show(); 
		}	
	 
	 
	 public void CopyRessouresIfNeeded() {

	    	if (Environment.getExternalStorageState().equals( Environment.MEDIA_UNMOUNTED)) {
  	    		Log.d("==> UNMOUNTED","==> UNMOUNTED");
  	    	}
  	    	else if( Environment.getExternalStorageState().equals( Environment.MEDIA_MOUNTED)) {
  	    		Log.d("==> MOUNTED","==> MOUNTED");   		
  	    	}
  	    	else if( Environment.getExternalStorageState().equals( Environment.MEDIA_CHECKING)) {
  	    		Log.d("==> MEDIA_CHECKING","==> MEDIA_CHECKING");   
  	    	}
  	    	else if( Environment.getExternalStorageState().equals( Environment.MEDIA_MOUNTED_READ_ONLY)) {
  	    		Log.d("==> MEDIA_MOUNTED_READ_ONLY","==> MEDIA_MOUNTED_READ_ONLY");   
  	    	}
  	    	else if( Environment.getExternalStorageState().equals( Environment.MEDIA_REMOVED)) {
  	    		Log.d("==> MEDIA_REMOVED","==> MEDIA_REMOVED"); // NO SD CARD
  	    		//HERE PRoblem with htc desire C.....
  	    	}
	    	

	    	String FolderBase = getExternalFilesDir(null).getAbsolutePath();
  	    	System.out.println("--- IS : " + FolderBase );
			File file2 = new File( FolderBase + "/VideoData" );
			System.out.println("--- Folder exists ?");
			if (file2.exists()) {
				System.out.println("--- Folder exists : nothing to do");
				mVideoFiles = GetFileList( file2.getAbsolutePath()  );
				
				if( mVideoFiles.size() == 0 ) { //??? some bug 
					
					mVideoFiles = CopyAndGetFileListTo( file2.getAbsolutePath()  );
					
					
				}
				
				
			}else{ 
				file2.mkdirs();
				System.out.println("--- Folder doesnt exists : copy files");
				System.out.println("--- copy files to : " + file2.getAbsolutePath() );
				mVideoFiles = CopyAndGetFileListTo( file2.getAbsolutePath()  );
			 }   	

	 }
	 
	 public List<String> GetFileList( String dst ){

		 Log.d("Files", "Path: " +   dst);
		 File f = new File(  dst);        
		 File file[] = f.listFiles();
		 Log.d("Files", "Size: "+ file.length);
		 ArrayList<String> list = new ArrayList<String>();
		 for (int i=0; i < file.length; i++)
		 {
			 list.add(file[i].getAbsolutePath() ); 
		     Log.d("Files", "FileName:" + file[i].getAbsolutePath());
		 }	
		 return list; 
	 }

	 public List<String> CopyAndGetFileListTo( String dst ){
		 String [] dede = OgreActivityJNI.CopyFilesToPath( dst, getResources().getAssets() );
		 ArrayList<String> list = new ArrayList<String>();
		 for(int i = 0 ; i < dede.length ; i++ ){ 
			 list.add(dede[i]); 
			 Log.d("=========> CopyAndGetFileListTo found file : ", dede[i]  );
		 } 
		 return list;
	}  
	 
	 
	 
	 
	 
	 private class TakePictureTask extends AsyncTask<Void, Void, Void> {

		 Camera mCamera ;
		 @Override
		protected void onPreExecute() {
		// TODO Auto-generated method stub
		super.onPreExecute();
		
		     mCamera = openFrontFacingCameraGingerbread();//Camera.open();
		
		     Size previewSize=mCamera.getParameters().getPreviewSize();
			   int dataBufferSize=(int)(previewSize.height*previewSize.width*(ImageFormat.getBitsPerPixel(mCamera.getParameters().getPreviewFormat())/8.0));
			   mBuffer = new byte[dataBufferSize];
			   //mCamera.addCallbackBuffer( mBuffer ); 
			   PreviewCallback d = new PreviewCallback();
			  // mCamera.setPreviewCallbackWithBuffer(d);	
			   
			   mCamera.setPreviewCallback(d);
			   mCamera.startPreview();
		}
		 
		    @Override
		    protected void onPostExecute(Void result) {
		        // This returns the preview back to the live camera feed
		        //
		        
		    	  
		    	
		    	
		    	/*
		        mCamera.setPreviewCallback(new PreviewCallback() {
				    

					@Override
					public void onPreviewFrame(byte[] arg0, Camera arg1) {
						 Log.e("", "------------------------------------");
						
					}
					   });
		        
		        mCamera.startPreview();*/
		        
		    }

		    @Override
		    protected Void doInBackground(Void... params) {
		    	
		    	boolean vari = true;
		    	while( vari ) { 
		    		
			        // Sleep for however long, you could store this in a variable and
			        // have it updated by a menu item which the user selects.
			        try {
			            Thread.sleep(3000); // 3 second preview
			        } catch (InterruptedException e) {
			            // TODO Auto-generated catch block
			            e.printStackTrace();
			        }
		    		
		    		
		    	}
		       



		        return null;
		    }

		}
	 
	 int TAKE_PICTURE = 15;
	 
	 
	 String filePicture = null;
	 String folderPicture = null;
	 
	 
	 String folderWithPictures = Environment.getExternalStorageDirectory()+"/diego_img/";
	 
	 @Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		
		
		  if(requestCode == TAKE_PICTURE)
		     {
		        // Bitmap picture = (Bitmap) data.getExtras().get("data");
		        // ImageView image = (ImageView) findViewById(R.id.imageView1);
		        // image.setImageBitmap(picture);
		         
		         /*
		         Intent i=new Intent( this, Example.class);
	             //i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	             i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
	             i.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
	          
	             startActivity(i);
	             */
			  
			  BitmapFactory.Options options = new BitmapFactory.Options();
			//  options.inPreferredConfig = Bitmap.Config.
			  Bitmap photo = BitmapFactory.decodeFile( folderPicture + filePicture , options);
			  
			  
			  //here rotation 90 degrees
			  
			  
			//  Bitmap photo = (Bitmap) "your Bitmap image";
			  photo = Bitmap.createScaledBitmap(photo, 512, 512, false);
			  ByteArrayOutputStream bytes = new ByteArrayOutputStream();
			  photo.compress(Bitmap.CompressFormat.JPEG, 70, bytes);

			  File f = new File( folderPicture + filePicture );
			  f.delete();
			  try {
				f.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			  FileOutputStream fo = null;
			try {
				fo = new FileOutputStream(f);
			
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		
			  try {
				fo.write(bytes.toByteArray());
				fo.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			  
			  

			   
			   File to = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/" +"test.jpg");
			    
	     
			   try {
				copyDirectory(  f , to );
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			   
			  
			  
			  
			 
			  
			  /*
			  sendFile( folderPicture + filePicture );
			  
		        downloadFiles( "http://"+ipServer+"/img/" ,  folderWithPictures  );
			  */
		         
		     	mInstance = this;

				CopyRessouresIfNeeded();

				
				 
				handler = new Handler();
				sysInit();
		         
		         
		         
		         Toast.makeText( this, "vvvv", Toast.LENGTH_LONG ).show();
		     }//if		
		
		
		
		// Toast.makeText( this, "vvvv", Toast.LENGTH_LONG ).show();
		
	}
	 
	 
	 /*
	 public void onActivityResult(int req, int res, Intent data) throws NullPointerException
	 {
	     try{

	     super.onActivityResult(req, res, data);
	     
	     
	     if(req == TAKE_PICTURE)
	     {
	         Bitmap picture = (Bitmap) data.getExtras().get("data");
	        // ImageView image = (ImageView) findViewById(R.id.imageView1);
	        // image.setImageBitmap(picture);
	         
	         
	         Intent i=new Intent( this, Example.class);
             //i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
             i.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
             i.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
          
             startActivity(i);
	         
	         
	         
	         Toast.makeText( this, "vvvv", Toast.LENGTH_LONG ).show();
	     }//if
	     else
	     {
	         Toast.makeText(getApplicationContext(), "No picture taken", 4);
	     }//else
	     }catch (NullPointerException e){
	         String error = "null pointer exception";
	         Toast.makeText(getApplicationContext(), error, 4);
	     }
	 }//onActivityResult	
	 
	 */
	 public void copyDirectory(File sourceLocation , File targetLocation)
			 throws IOException {

			     if (sourceLocation.isDirectory()) {
			         if (!targetLocation.exists() && !targetLocation.mkdirs()) {
			             throw new IOException("Cannot create dir " + targetLocation.getAbsolutePath());
			         }

			         String[] children = sourceLocation.list();
			         for (int i=0; i<children.length; i++) {
			             copyDirectory(new File(sourceLocation, children[i]),
			                     new File(targetLocation, children[i]));
			         }
			     } else {

			         // make sure the directory we plan to store the recording in exists
			         File directory = targetLocation.getParentFile();
			         if (directory != null && !directory.exists() && !directory.mkdirs()) {
			             throw new IOException("Cannot create dir " + directory.getAbsolutePath());
			         }

			         InputStream in = new FileInputStream(sourceLocation);
			         OutputStream out = new FileOutputStream(targetLocation);

			         // Copy the bits from instream to outstream
			         byte[] buf = new byte[1024];
			         int len;
			         while ((len = in.read(buf)) > 0) {
			             out.write(buf, 0, len);
			         }
			         in.close();
			         out.close();
			     }
			 }
	 
	 
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		
		
		//File tempFolder = new File(Environment.getExternalStorageDirectory().getAbsolutePath()
                /*+ "/myfolder"*///);
        //tempFolder.mkdir();
        
		folderPicture = Environment.getExternalStorageDirectory().getAbsolutePath() + "/";
		filePicture =   String.valueOf(System.currentTimeMillis()) + ".jpg";
		 
		   File file = new File(folderPicture, filePicture);
		   
		   
		 
		   
		   /*
		   File from = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + String.valueOf(System.currentTimeMillis()) + ".jpg");
		   File to = new File(Environment.getExternalStorageDirectory().getAbsolutePath() + "/" +"test.jpg");
		   
		   try {
			to.createNewFile();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		   
		   from.renameTo(to);		   
		   */
		   
		   /*
		   File dst = new File( Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + "test.jpg");
		   if(!dst.exists()){ 
			   try {
				dst.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		   }
		   
		   try {
			
			copy( file , dst);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}*/
		   
		   
		   
			//folderPicture = Environment.getExternalStorageDirectory().getAbsolutePath() + "/";
			//filePicture =  "test"/*String.valueOf(System.currentTimeMillis())*/ + ".jpg";
		   
		   
	        Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
	        
	        //intent.setAction(Intent.ACTION_MAIN);
	        //intent.addCategory(Intent.CATEGORY_LAUNCHER);
	        
	        intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(file));
	        startActivityForResult(intent, TAKE_PICTURE); 	
		
	        
	        
		/*
	        
	      */  

		 
		
	
	    
	    
		 /* // param.setPreviewSize(preview_size.width,preview_size.height);
		  // mCamera.setParameters(param);
		   Size previewSize=mCamera.getParameters().getPreviewSize();
		   int dataBufferSize=(int)(previewSize.height*previewSize.width*(ImageFormat.getBitsPerPixel(mCamera.getParameters().getPreviewFormat())/8.0));
		   mBuffer = new byte[dataBufferSize];
		   //mCamera.addCallbackBuffer( mBuffer ); 
		   PreviewCallback d = new PreviewCallback();
		  // mCamera.setPreviewCallbackWithBuffer(d);	
		   
		   mCamera.setPreviewCallback(d);
		   mCamera.startPreview(); */
	 
		
	 
		/*
		mInstance = this;

		CopyRessouresIfNeeded();

		
		 
		handler = new Handler();
		sysInit(); */

	}

	@Override
	protected void onPause() {
		super.onPause();
		
		if( handler == null ) return;
		
		 handler.removeCallbacks(renderer);
		paused = true;
		 
		
		 ////////////
 
         if( vuforiaAppSession == null ) return;
         /*
        if (mGlView != null)
        {
            mGlView.setVisibility(View.INVISIBLE);
            mGlView.onPause();
        }*/
        
        // Turn off the flash
        if (mFlashOptionView != null && mFlash)
        {
            // OnCheckedChangeListener is called upon changing the checked state
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
            {
                ((Switch) mFlashOptionView).setChecked(false);
            } else
            {
                ((CheckBox) mFlashOptionView).setChecked(false);
            }
        }
        
        try
        {
            vuforiaAppSession.pauseAR();
        } catch (VuforiaExceptions e)
        {
            Log.e(LOGTAG, e.getString());
        }
         
		 
		
	}

	@Override
	protected void onResume() {
		super.onResume();
		
		if( handler == null ) return;
		
		 paused = false;
		handler.post(renderer); 
		

//////////////////////
		
		 if( vuforiaAppSession == null ) return;
         
        // This is needed for some Droid devices to force portrait
        if (mIsDroidDevice)
        {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        }
        
        try
        {
            vuforiaAppSession.resumeAR();
        } catch (VuforiaExceptions e)
        {
            Log.e(LOGTAG, e.getString());
        }
        
        /*
        // Resume the GL view:
        if (mGlView != null)
        {
            mGlView.setVisibility(View.VISIBLE);
            mGlView.onResume();
        }		 
		 */
		
	}
	
	@Override
	protected void onStop() {
	    super.onStop();
	    
	    if( handler == null ) return;

	    onDestroy();
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		
		if( handler == null ) return;
		
		
		 try
	        {
			 Log.e("stopar ...", "stopar ...");
	            vuforiaAppSession.stopAR();
	            vuforiaAppSession = null;
	        } catch (VuforiaExceptions e)
	        {
	            Log.e(LOGTAG, e.getString());
	            Log.e("ERRRORROOROROR", "COULD NOT FINISH...");
	        }
	        
	        // Unload texture:
	        mTextures.clear();
	        mTextures = null;
	        
	        
	        Log.e("FINISH", "FINISH");
	        
	        
			lastSurface = null;
			//OgreActivityJNI.ViewportSetCurrent(); 
			//OgreActivityJNI.DeleteEngine();
			
		        
			
			OgreActivityJNI.CloseTheoraVideo();
		        
		        
		        Log.e("EXIT", "EXIT");	        
	        
	        
	        finish();
	         System.exit(0);
		
		
/*
		Runnable destroyer = new Runnable() {
			public void run() {
				
				OgreActivityJNI.ViewportSetCurrent(); 
				 OgreActivityJNI.DeleteEngine();
				 
				
				 
			}
		};
		handler.post(destroyer);
		
		
		
     ////////////
		 
        try
        {
            vuforiaAppSession.stopAR();
        } catch (VuforiaExceptions e)
        {
            Log.e(LOGTAG, e.getString());
        }
        
        // Unload texture:
        mTextures.clear();
        mTextures = null;
        
        System.gc(); 
	      */
		
	
	}
	
	
	public void listfiles(String directoryName, ArrayList<File> files) {
		File directory = new File(directoryName);
		
		
		if(directory.exists()) { 
			
			// get all the files from a directory
			File[] fList = directory.listFiles();
			for (File file : fList) {
			    if (file.isFile()) {
			        files.add(file);
			    } else if (file.isDirectory()) {
			    	listfiles(file.getAbsolutePath(), files);
			    }
			}
			
		}


		}
	
	
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
	  // ignore orientation/keyboard change
	  super.onConfigurationChanged(newConfig);
	  
	   
     ////////////
	  if( vuforiaAppSession == null ) return;
        vuforiaAppSession.onConfigurationChanged();
        
        // Removes the current layout and inflates a proper layout
        // for the new screen orientation
        
        
        
        if (mUILayout != null)
        {
            mUILayout.removeAllViews();
            ((ViewGroup) mUILayout.getParent()).removeView(mUILayout);
            
        }
        
        addOverlayView(false);	
 
	}	
	
	public Boolean IsOgreInitialized() {
		return wndCreate;//initOGRE;
	}
	
	public void RenderFrame(){ 
		
		OgreActivityJNI.renderOneFrame();
	}
	    
public void DebugTextureFormatState( State state ){ 
	Frame fr = state.getFrame();
	for( int i = 0; i < fr.getNumImages(); i++) {
		Image e =   fr.getImage( i );
		int f = e.getFormat();
		if( f == PIXEL_FORMAT.GRAYSCALE ){ 
			Log.d(LOGTAG, "---> grayscale IS : " + Integer.toString( i ));
		}
		else if( f == PIXEL_FORMAT.INDEXED ){ 
			Log.d(LOGTAG, "---> INDEXED IS : " + Integer.toString( i ));
		}	
		else if( f == PIXEL_FORMAT.RGB565 ){ 
			Log.d(LOGTAG, "---> RGB565 IS : " + Integer.toString( i ));
		}		
		else if( f == PIXEL_FORMAT.RGB888 ){ 
			Log.d(LOGTAG, "---> RGB888 IS : " + Integer.toString( i ));	
		}										
		else if( f == PIXEL_FORMAT.RGBA8888 ){ 
			Log.d(LOGTAG, "---> RGBA IS : " + Integer.toString( i ));
		}
		else if( f == PIXEL_FORMAT.UNKNOWN_FORMAT ){ 
			Log.d(LOGTAG, "---> UNKNOWN_FORMAT IS : " + Integer.toString( i ));
		}
		else if( f == PIXEL_FORMAT.YUV ){ 
			Log.d(LOGTAG, "---> YUV IS : " + Integer.toString( i ));
		}	
		else{ Log.d(LOGTAG, "---> ERROR IS : " + Integer.toString( i )); }
		
	}
}		    
		
	static Boolean createdTexture = false;
	 static Boolean setsize = false;
	 
	 
	 Camera  mCamera;
 
	private void sysInit() {
		

		
		
		
		
 
		final Runnable initRunnable = new Runnable() {
			public void run() {
				if (!initOGRE) {
					initOGRE = true;
 
					renderer = new Runnable() {
						public void run() {

							if (paused)
								return;

							if (!wndCreate && lastSurface != null) {
								
								if(assetMgr == null) {
									assetMgr = getResources().getAssets();
									 
								}
								
								
							        
								
								
								// startCamera();
								
								
							 
					 
							   OgreActivityJNI.CreateEngine(lastSurface, assetMgr); 
							   OgreActivityJNI.ViewportSetClearEveryFrame( false );
			 
							   Vuforia.onSurfaceCreated();
							  
							      
								 

							   
							
							  
							//  mFrontView.requestRender();
							  
							   
							   mExtendedTracking = true;// ??? doesnt work
							   
								vuforiaAppSession = new VuforiaSession(mInstance);
								vuforiaAppSession.initAR(mInstance, ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
								// Load any sample specific textures:
								mTextures = new Vector<VuforiaTexture>();
								//loadTextures();
								mGestureDetector = new GestureDetector(mInstance, new GestureListener());
								mIsDroidDevice = android.os.Build.MODEL.toLowerCase().startsWith("droid");	
								wndCreate = true;
								handler.post(this);
								
								
								
						
								  
								
	 
								return;
							}
							
							if( deleteOGRE ){ 
								
								handler.removeCallbacks(renderer);
			 	
							}

							 if (initOGRE && wndCreate && initvuforia){ 
								 
								
								
								 if( !setsize ){
									 VideoBackgroundConfig  conf =   Renderer.getInstance().getVideoBackgroundConfig( );								
									 OgreActivityJNI.SetScreenSize(  conf.getSize().getData()[0] , conf.getSize().getData()[1]  );	
									 setsize = true; 
								  }			
								 
 
								/* never remove !! else context changes, creation of texture in another context.. bad */
								OgreActivityJNI.ViewportSetCurrent(); 
								// save
								int[] oldViewport = new int[4];
						        GLES20.glGetIntegerv( GLES20.GL_VIEWPORT, oldViewport, 0);
 
						        
						        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
					 
						        
						       // GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
						        
						        
						    	State state = Renderer.getInstance().begin( );
						    	
						    	
						    	//GLES20.glEnable(GLES20.GL_BLEND);
						    	
						    	//GLES20.glDepthMask(false);

						         if( !Renderer.getInstance().bindVideoBackground( 0 )) {
						            Log.e(LOGTAG, "Unable to bind video background texture!!");
						            Renderer.getInstance().end();
						             handler.post(this);
						            return;
						         }   
	
								if( !createdTexture ){
									
									ArrayList<File> files = new ArrayList<File>();
									listfiles("/sdcard/diego_img/", files);
									
									
									for(int i = 0 ; i < files.size(); i++) { 
										 Log.i( "","FOUND FILE = " + files.get(i).getAbsolutePath()  );
										OgreActivityJNI.CreateTextureFromPath( files.get(i).getAbsolutePath() ) ;
										
										
										
									}
									
									
									OgreActivityJNI.CreateBackGroundTexture();
									createdTexture = true;
									
									//regler perf video...
									
									// OgreActivityJNI.OpenTheoraVideo(mVideoFiles.get(0)); // enable when good tracking
									
									/////
									
														
									
									
									
								} 

								 // reset viewport
						         // GLES20.glViewport(oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
								//! important !
								 GLES20.glViewport(viewportPosition_x, viewportPosition_y, viewportSize_x, viewportSize_y);  
						         
								 //OgreActivityJNI.ViewportUpdate();
						         
								
								// DebugTextureFormatState( state );
								 
								 
								// dsqdqs mFrontView.
								 
								  //mFrontView.requestRender();
								 
								// here update here
				 
							    refFreeFrame.render();
	 
							      boolean ObjectVisible = false;
							     for (int tIdx = 0; tIdx < state.getNumTrackableResults(); tIdx++)
							     {
							    	 
							    	 ObjectVisible = true;
						        	//synchronized(this) {
						        	TrackableResult trackableResult = state.getTrackableResult(tIdx);
						        	Trackable trackable = trackableResult.getTrackable();
						        	
						        	 
						      
						        	Matrix44F modelViewMatrix_Vuforia = Tool.convertPose2GLMatrix(trackableResult.getPose());
						        	//float[] modelViewMatrix =  
 
						        	OgreActivityJNI.SetModelPose( modelViewMatrix_Vuforia.getData() ); 
						        	
						        	//Log.d("===> ", "VISIBLE" );
						        	
						 
						        	
							     } 
							     
							     //if( !ObjectVisible  ) Log.d("===> ", "NOT VISIBLE" );
	 
								 Renderer.getInstance().end();
								 
								 
								 //mFrontView.requestRender();
								 
								 
								 OgreActivityJNI.renderOneFrame();
								 
								//V needed adreno specific, fixing alpha blending!!! else white background or freezing
								 GLES20.glDisable( GLES20.GL_BLEND );
					 
							 }
								 

							if( !deleteOGRE ) handler.post(this);
							
						}
					};

					handler.post(renderer);
				}
			}

		};

		
		SurfaceView view = new  SurfaceView(this);
		
		SurfaceHolder holder = view.getHolder();
		//holder.setType(SurfaceHolder.SURFACE_TYPE_GPU);
		
		 
		
		 //view.setZOrderOnTop(true);    // necessary
		 // holder.setFormat(PixelFormat.TRANSPARENT);
		
		  
		
		surfaceView = view;
		
		 
 
		holder.addCallback(new Callback() {
			public void surfaceCreated(SurfaceHolder holder) {
				if (holder.getSurface() != null && holder.getSurface().isValid()) {
					lastSurface = holder.getSurface();
					
					//Vuforia.onSurfaceCreated();
					
					handler.post(initRunnable);
					
					
					
					
					
			
					
					
					
				}
			}

			public void surfaceDestroyed(SurfaceHolder holder) {
				if (initOGRE && wndCreate) {
					wndCreate = false;
					lastSurface = null;
					handler.post(new Runnable() {
						public void run() {
							//OgreActivityJNI.termWindow();
						}
					});
				}
			}

			public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
				if (initOGRE && wndCreate) {
			     updateRendering();
		        
		        // Call Vuforia function to handle render surface size changes:
		        vuforiaAppSession.onSurfaceChanged(width, height);
				}

			}
		});
		
	
		
		
		
		// addContentView( mView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		  
		 
		//setContentView(surfaceView); 
		
		 
		 
		 
		  //mFrontView = new NxVideoFrontCameraView( mInstance );
		
		 
		// addContentView(mFrontView, new LayoutParams(100, 100));
		 
		 
		 addContentView(surfaceView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
	

			//TakePictureTask takePictureTask = new TakePictureTask();
		    //takePictureTask.execute();
		 
		 
	/*
		 NxVideoSurfaceView test = new NxVideoSurfaceView(this);
		 test.setZOrderOnTop(true);
		SurfaceHolder testholder = test.getHolder();
		testholder.setFormat(PixelFormat.TRANSPARENT);		 
		 test.OpenVideoFileInt(  R.raw.tester ); 
		 addContentView(test, new LayoutParams(512, 512 ));
		 */
 
	}

	boolean wndCreate = false;

	public void onAccuracyChanged(Sensor sensor, int accuracy) {

	}

	public void onSensorChanged(SensorEvent event) {
		if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
		}
	}	
	 
 
	
	
    // Process Single Tap event to trigger autofocus
    private class GestureListener extends GestureDetector.SimpleOnGestureListener
    {
        // Used to set autofocus one second after a manual focus is triggered
        private final Handler autofocusHandler = new Handler();
        
        
        @Override
        public boolean onDown(MotionEvent e)
        {
            return true;
        }
        
        
        @Override
        public boolean onSingleTapUp(MotionEvent e)
        {
            // Generates a Handler to trigger autofocus
            // after 1 second
            autofocusHandler.postDelayed(new Runnable()
            {
                public void run()
                {
                    boolean result = CameraDevice.getInstance().setFocusMode(
                        CameraDevice.FOCUS_MODE.FOCUS_MODE_TRIGGERAUTO);
                    
                    if (!result)
                        Log.e("SingleTapUp", "Unable to trigger focus");
                }
            }, 1000L);
            
            return true;
        }
    }
    
    
    // We want to load specific textures from the APK, which we will later use
    // for rendering.
    private void loadTextures()
    {
        //mTextures.add(VuforiaTexture.loadTextureFromApk("TextureTeapotBlue.png",getAssets()));
    }
    
    
 
	
    
    // Shows error message in a system dialog box
    private void showErrorDialog()
    {
        if (mDialog != null && mDialog.isShowing())
            mDialog.dismiss();
        
        mDialog = new AlertDialog.Builder(Example.this).create();
        DialogInterface.OnClickListener clickListener = new DialogInterface.OnClickListener()
        {
            public void onClick(DialogInterface dialog, int which)
            {
                dialog.dismiss();
            }
        };
        
        mDialog.setButton(DialogInterface.BUTTON_POSITIVE,
            getString(R.string.button_OK), clickListener);
        
        mDialog.setTitle(getString(R.string.target_quality_error_title));
        
        String message = getString(R.string.target_quality_error_desc);
        
        // Show dialog box with error message:
        mDialog.setMessage(message);
        mDialog.show();
    }
    
    
    // Shows error message in a system dialog box on the UI thread
    void showErrorDialogInUIThread()
    {
        runOnUiThread(new Runnable()
        {
            public void run()
            {
                showErrorDialog();
            }
        });
    }
    
    
    // Initializes AR application components.
    private void initApplicationAR()
    {
        // Do application initialization
        refFreeFrame = new VuforiaFreeframe(this, vuforiaAppSession);
        refFreeFrame.init();
        
        // Create OpenGL ES view:
        int depthSize = 16;
        int stencilSize = 0;
        boolean translucent = Vuforia.requiresAlpha();
       
        
        /*
        mGlView = new VuforiaGlView(this);
        mGlView.init(translucent, depthSize, stencilSize);
        final SurfaceHolder GlHolder =  mGlView.getHolder();
      
       mRenderer = new VuforiaTargetRenderer(this, vuforiaAppSession);
       mRenderer.setTextures(mTextures);
       mGlView.setRenderer(mRenderer); */
       addOverlayView(true); 
        
        
    }
    
    
    // Adds the Overlay view to the GLView
    private void addOverlayView(boolean initLayout)
    {
    	
    	 
        // Inflates the Overlay Layout to be displayed above the Camera View
        LayoutInflater inflater = LayoutInflater.from(this);
        mUILayout = (RelativeLayout) inflater.inflate( R.layout.camera_overlay_udt, null, false);
        
        mUILayout.setVisibility(View.VISIBLE);
        
        // If this is the first time that the application runs then the
        // uiLayout background is set to BLACK color, will be set to
        // transparent once the SDK is initialized and camera ready to draw
        if (initLayout)
        {
            mUILayout.setBackgroundColor(Color.BLACK);
        }
        
        // Adds the inflated layout to the view
        addContentView(mUILayout, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        
        // Gets a reference to the bottom navigation bar
        mBottomBar = mUILayout.findViewById(R.id.bottom_bar);
        
        // Gets a reference to the Camera button
        mCameraButton = mUILayout.findViewById(R.id.camera_button);
        
        // Gets a reference to the loading dialog container
        loadingDialogHandler.mLoadingDialogContainer = mUILayout.findViewById(R.id.loading_layout);
        
        startUserDefinedTargets();
        initializeBuildTargetModeViews();
        
        mUILayout.bringToFront(); 
        
        mUILayout.setVisibility(View.GONE);//<< here to show the down menu
        
    }
    
    
    // Button Camera clicked
    public void onCameraClick(View v)
    {
        if (isUserDefinedTargetsRunning())
        {
            // Shows the loading dialog
            loadingDialogHandler.sendEmptyMessage(VuforiaLoadingDialogHandler.SHOW_LOADING_DIALOG);
            
            // Builds the new target
            startBuild();
        }
    }
    
    
    // Creates a texture given the filename
    VuforiaTexture createTexture(String nName)
    {
        return VuforiaTexture.loadTextureFromApk(nName, getAssets());
    }
    
    
    // Callback function called when the target creation finished
    void targetCreated()
    {
        // Hides the loading dialog
        loadingDialogHandler.sendEmptyMessage(VuforiaLoadingDialogHandler.HIDE_LOADING_DIALOG);
        
        if (refFreeFrame != null) {
            refFreeFrame.reset();
        }
        
    }
    
    
    // Initialize views
    private void initializeBuildTargetModeViews()
    {
        // Shows the bottom bar
        mBottomBar.setVisibility(View.VISIBLE);
        mCameraButton.setVisibility(View.VISIBLE);
    }
    
    // enable this to make work the left menu
    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        // Process the Gestures
        if (mSampleAppMenu != null && mSampleAppMenu.processEvent(event))
            return true;
        
        return mGestureDetector.onTouchEvent(event);
    } 
    
    
    boolean startUserDefinedTargets()
    {
        Log.d(LOGTAG, "startUserDefinedTargets");
        
        TrackerManager trackerManager = TrackerManager.getInstance();
        ObjectTracker objectTracker = (ObjectTracker) (trackerManager
            .getTracker(ObjectTracker.getClassType()));
        if (objectTracker != null)
        {
            ImageTargetBuilder targetBuilder = objectTracker
                .getImageTargetBuilder();
            
            if (targetBuilder != null)
            {
                // if needed, stop the target builder
                if (targetBuilder.getFrameQuality() != ImageTargetBuilder.FRAME_QUALITY.FRAME_QUALITY_NONE)
                    targetBuilder.stopScan();
                
                objectTracker.stop();
                
                targetBuilder.startScan();
                
            }
        } else
            return false;
        
        return true;
    }
    
    
    boolean isUserDefinedTargetsRunning()
    {
        TrackerManager trackerManager = TrackerManager.getInstance();
        ObjectTracker objectTracker = (ObjectTracker) trackerManager
            .getTracker(ObjectTracker.getClassType());
        
        if (objectTracker != null)
        {
            ImageTargetBuilder targetBuilder = objectTracker
                .getImageTargetBuilder();
            if (targetBuilder != null)
            {
                Log.e(LOGTAG, "Quality> " + targetBuilder.getFrameQuality());
                return (targetBuilder.getFrameQuality() != ImageTargetBuilder.FRAME_QUALITY.FRAME_QUALITY_NONE) ? true
                    : false;
            }
        }
        
        return false;
    }
    
    
    
    void startBuild()
    {
        TrackerManager trackerManager = TrackerManager.getInstance();
        ObjectTracker objectTracker = (ObjectTracker) trackerManager
            .getTracker(ObjectTracker.getClassType());
        
        if (objectTracker != null)
        {
            ImageTargetBuilder targetBuilder = objectTracker
                .getImageTargetBuilder();
            if (targetBuilder != null)
            {
                // Uncomment this block to show and error message if
                // the frame quality is Low
                //if (targetBuilder.getFrameQuality() == ImageTargetBuilder.FRAME_QUALITY.FRAME_QUALITY_LOW)
                //{
                //     showErrorDialogInUIThread();
                //}
                
                String name;
                do
                {
                    name = "UserTarget-" + targetBuilderCounter;
                    Log.d(LOGTAG, "TRYING " + name);
                    targetBuilderCounter++;
                } while (!targetBuilder.build(name, 320.0f));
                
                refFreeFrame.setCreating();
            }
        }
    }
    
    
    void updateRendering()
    {
        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        refFreeFrame.initGL(metrics.widthPixels, metrics.heightPixels);
    }
    
    
    @Override
    public boolean doInitTrackers()
    {
        // Indicate if the trackers were initialized correctly
        boolean result = true;
        
        // Initialize the image tracker:
        TrackerManager trackerManager = TrackerManager.getInstance();
        Tracker tracker = trackerManager.initTracker(ObjectTracker
            .getClassType());
        if (tracker == null)
        {
            Log.d(LOGTAG, "Failed to initialize ObjectTracker.");
            result = false;
        } else
        {
            Log.d(LOGTAG, "Successfully initialized ObjectTracker.");
        }
        
        return result;
    }
    
    
    @Override
    public boolean doLoadTrackersData()
    {
        TrackerManager tManager = TrackerManager.getInstance();
        ObjectTracker objectTracker = (ObjectTracker) tManager
            .getTracker(ObjectTracker.getClassType());
        if (objectTracker == null)
            return false;
        
        dataSetUserDef = objectTracker.createDataSet();
        if (dataSetUserDef == null)
            return false;
        
        if (!dataSetUserDef.load("Tarmac.xml",
            STORAGE_TYPE.STORAGE_APPRESOURCE))
            return false;
        
        if (!objectTracker.activateDataSet(dataSetUserDef))
            return false;
        
        int numTrackables = dataSetUserDef.getNumTrackables();
        for (int count = 0; count < numTrackables; count++)
        {
            String name = "Tarmac : "
                + dataSetUserDef.getTrackable(count).getName();
            dataSetUserDef.getTrackable(count).setUserData(name);
            Log.d(LOGTAG, "UserData:Set the following user data "
                + (String) dataSetUserDef.getTrackable(count).getUserData());
        }
        
        return true;
    }
    
    
   
    
    
    @Override
    public boolean doStartTrackers()
    {
        // Indicate if the trackers were started correctly
        boolean result = true;
        
        Tracker objectTracker = TrackerManager.getInstance().getTracker(
            ObjectTracker.getClassType());
        if (objectTracker != null)
            objectTracker.start();
        
        return result;
    }
    
    
    @Override
    public boolean doStopTrackers()
    {
        // Indicate if the trackers were stopped correctly
        boolean result = true;
        
        Tracker objectTracker = TrackerManager.getInstance().getTracker(
            ObjectTracker.getClassType());
        if (objectTracker != null)
            objectTracker.stop();
        
        return result;
    }
    
    @Override
    public boolean doUnloadTrackersData()
    {
        // Indicate if the trackers were unloaded correctly
        boolean result = true;
        
        // Get the image tracker:
        TrackerManager trackerManager = TrackerManager.getInstance();
        ObjectTracker objectTracker = (ObjectTracker) trackerManager
            .getTracker(ObjectTracker.getClassType());
        if (objectTracker == null)
        {
            result = false;
            Log.d(
                LOGTAG,
                "Failed to destroy the tracking data set because the ObjectTracker has not been initialized.");
        }
        
        if (dataSetUserDef != null)
        {
            if (objectTracker.getActiveDataSet() != null
                && !objectTracker.deactivateDataSet(dataSetUserDef))
            {
                Log.d(
                    LOGTAG,
                    "Failed to destroy the tracking data set because the data set could not be deactivated.");
                result = false;
            }
            
            if (!objectTracker.destroyDataSet(dataSetUserDef))
            {
                Log.d(LOGTAG, "Failed to destroy the tracking data set.");
                result = false;
            }
            
            Log.d(LOGTAG, "Successfully destroyed the data set.");
            dataSetUserDef = null;
        }
        
        return result;
    }
    
    
    @Override
    public boolean doDeinitTrackers()
    {
        // Indicate if the trackers were deinitialized correctly
        boolean result = true;
        
        if (refFreeFrame != null)
            refFreeFrame.deInit();
        
        TrackerManager tManager = TrackerManager.getInstance();
        tManager.deinitTracker(ObjectTracker.getClassType());
        
        return result;
    }
    
    
    public void configureViewport(int viewportPosition_x, int viewportPosition_y, int viewportSize_x, int viewportSize_y)
    {
            this.viewportPosition_x = viewportPosition_x;
            this.viewportPosition_y = viewportPosition_y;
            this.viewportSize_x = viewportSize_x;
            this.viewportSize_y = viewportSize_y;     
     }
    
    
    @Override
    public void onInitARDone(VuforiaExceptions exception)
    {
        
        if (exception == null)
        {
        	
  
            initApplicationAR(); // <<------
            
            // Activate the renderer
            //mRenderer.mIsActive = true;
            
            // Now add the GL surface view. It is important
            // that the OpenGL ES surface view gets added
            // BEFORE the camera is started and video background is configured.
            //addContentView(mGlView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
           
           // addContentView(surfaceView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
            //addContentView(surfaceView, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
            
            // Sets the UILayout to be drawn in front of the camera
            mUILayout.bringToFront();
            
            // Hides the Loading Dialog
            loadingDialogHandler.sendEmptyMessage(VuforiaLoadingDialogHandler.HIDE_LOADING_DIALOG);
            
            // Sets the layout background to transparent
            mUILayout.setBackgroundColor(Color.TRANSPARENT);
            
            
                 
            
            
            
            try
            {
                vuforiaAppSession.startAR(CameraDevice.CAMERA.CAMERA_BACK ); 
            } catch (VuforiaExceptions e)
            {
            	Log.e(LOGTAG,  "onInitARDone(VuforiaExceptions exception) --->  vuforiaAppSession.startAR EEROR " );
                Log.e(LOGTAG, e.getString());
            }
            
            
            
            VideoBackgroundConfig config = Renderer.getInstance().getVideoBackgroundConfig();
                
            DisplayMetrics metrics = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(metrics);
            
            
            int x = (((int) (metrics.widthPixels - config.getSize().getData()[0])) / (int) 2)
            + config.getPosition().getData()[0];
            
            int y =  (((int) (metrics.heightPixels - config.getSize().getData()[1])) / (int) 2)
                    + config.getPosition().getData()[1];
            
            int width = config.getSize().getData()[0];
            
            int height = config.getSize().getData()[1];
            
            
            Log.e(LOGTAG, "Viewport config : ");
            Log.e(LOGTAG, "X  : " + x );  
            Log.e(LOGTAG, "Y  : " + y );  
            Log.e(LOGTAG, "width  : " + width );   
            Log.e(LOGTAG, "height  : " + height );  
            
            configureViewport(x, y, width, height);
 
            
            boolean result = CameraDevice.getInstance().setFocusMode(CameraDevice.FOCUS_MODE.FOCUS_MODE_CONTINUOUSAUTO);
            
            if (result)
                mContAutofocus = true;
            else
                Log.e(LOGTAG, "Unable to enable continuous autofocus");
            
            setSampleAppMenuAdditionalViews();
              
            mSampleAppMenu = new VuforiaMenu(this, this,"User Defined Targets", surfaceView, mUILayout, mSettingsAdditionalViews);
   
            setSampleAppMenuSettings();
      
            initvuforia = true;
            
            
            /*
            Log.e(LOGTAG, "-----------CAMERAZ");
            mCamera = openFrontFacingCameraGingerbread();//Camera.open();
			
		     Size previewSize=mCamera.getParameters().getPreviewSize();
			   int dataBufferSize=(int)(previewSize.height*previewSize.width*(ImageFormat.getBitsPerPixel(mCamera.getParameters().getPreviewFormat())/8.0));
			   mBuffer = new byte[dataBufferSize];
			   //mCamera.addCallbackBuffer( mBuffer ); 
			   PreviewCallback d = new PreviewCallback();
			  // mCamera.setPreviewCallbackWithBuffer(d);	
			   
			   mCamera.setPreviewCallback(d);
			   mCamera.startPreview();   
			   Log.e(LOGTAG, "-----------END");*/
			   
    
        } else
        {
        	Log.e(LOGTAG,  "--->>> onInitARDone(VuforiaExceptions exception) ERROR " );
            Log.e(LOGTAG, exception.getString());
            finish();
        } 
    }
    

    @Override
    public void onQCARUpdate(State state)
    {
        TrackerManager trackerManager = TrackerManager.getInstance();
        ObjectTracker objectTracker = (ObjectTracker) trackerManager
            .getTracker(ObjectTracker.getClassType());
        
        if (refFreeFrame.hasNewTrackableSource())
        {
            Log.d(LOGTAG,
                "Attempting to transfer the trackable source to the dataset");
            
            // Deactivate current dataset
            objectTracker.deactivateDataSet(objectTracker.getActiveDataSet());
            
            // Clear the oldest target if the dataset is full or the dataset
            // already contains five user-defined targets.
            if (dataSetUserDef.hasReachedTrackableLimit()
                || dataSetUserDef.getNumTrackables() >= 5)
                dataSetUserDef.destroy(dataSetUserDef.getTrackable(0));
            
            if (mExtendedTracking && dataSetUserDef.getNumTrackables() > 0)
            {
                // We need to stop the extended tracking for the previous target
                // so we can enable it for the new one
                int previousCreatedTrackableIndex = 
                    dataSetUserDef.getNumTrackables() - 1;
                
                objectTracker.resetExtendedTracking();
                dataSetUserDef.getTrackable(previousCreatedTrackableIndex)
                    .stopExtendedTracking();
            }
            
            // Add new trackable source
            Trackable trackable = dataSetUserDef
                .createTrackable(refFreeFrame.getNewTrackableSource());
            
            // Reactivate current dataset
            objectTracker.activateDataSet(dataSetUserDef);
            
            if (mExtendedTracking)
            {
                trackable.startExtendedTracking();
            }
            
        }
    }
    
    final public static int CMD_BACK = -1;
    final public static int CMD_EXTENDED_TRACKING = 1;
    final public static int CMD_AUTOFOCUS = 2;
    final public static int CMD_FLASH = 3;
    final public static int CMD_CAMERA_FRONT = 4;
    final public static int CMD_CAMERA_REAR = 5;
    
    
    // This method sets the additional views to be moved along with the GLView
    private void setSampleAppMenuAdditionalViews()
    {
        mSettingsAdditionalViews = new ArrayList<View>();
        mSettingsAdditionalViews.add(mBottomBar);
    }
    
    
    public void ShowDialog(){ 
    	
    	
    	   final Dialog dialog = new Dialog( this);
    	  
    	  // Rect displayRectangle = new Rect();
    	   //Window window =  getWindow();
    	 //  window.getDecorView().getWindowVisibleDisplayFrame(displayRectangle);
 
    	   RelativeLayout layout = new RelativeLayout(this);
    	   layout.setBackgroundColor(Color.TRANSPARENT);
  
    	   ImageView img = new ImageView(this); 
    	   img.setImageResource(  R.drawable.inapp );
    	   img.setOnClickListener( new ImageView.OnClickListener(){

				@Override
				public void onClick(View arg0) {
					dialog.dismiss();
					
				}
   
    		   
    	   });
    	   
    	   layout.addView( img );
    	   
    	   dialog.setContentView( layout);
    	   
    	  
          // dialog.setTitle("This is my custom dialog box");
           dialog.setCancelable(true);
           //there are a lot of settings, for dialog, check them all out!
           
           DisplayMetrics displaymetrics = new DisplayMetrics();
            getWindowManager().getDefaultDisplay().getMetrics(displaymetrics);
           int width = (int) (displaymetrics.widthPixels * 0.85 );

           WindowManager.LayoutParams params = dialog.getWindow().getAttributes();
           params.width = width;
           dialog.getWindow().setAttributes(params);
           dialog.getWindow().setBackgroundDrawable(new ColorDrawable(android.graphics.Color.TRANSPARENT));
 
           dialog.show();
           
    	
    	
    }
    
    
    
    // This method sets the menu's settings
    private void setSampleAppMenuSettings()
    {
    	
    	Log.d(LOGTAG, " setSampleAppMenuSettings ...  ");
    	
        VuforiaMenuGroup group;
        
        group = mSampleAppMenu.addGroup("", false);
        /*
        group.addImageItem("", -20);
        group.addImageItem("", -30);
        group.addImageItem("", -40);
        group.addImageItem("", -50);
        group.addImageItem("", -60);
        group.addImageItem("", -70);*/
        
        
        
  
        
        
 
        
        
        group.getMenuLayout().setBackgroundColor(Color.BLACK);
        
        
    
        
        
        //group.addSelectionItem( "HELLO", CMD_EXTENDED_TRACKING, false);
        
       // group = mSampleAppMenu.addGroup("sdfsdfsdfsd", true);
      //  group.addSelectionItem( "HELLO", CMD_EXTENDED_TRACKING, false);
        
         
        
        
        
        //group = mSampleAppMenu.addGroup("", false);
        //group.addTextItem(getString(R.string.menu_back), -1);
        /*
        group = mSampleAppMenu.addGroup("", true);
        group.addSelectionItem(getString(R.string.menu_extended_tracking), CMD_EXTENDED_TRACKING, false);
        group.addSelectionItem(getString(R.string.menu_contAutofocus), CMD_AUTOFOCUS, mContAutofocus);
        mFlashOptionView = group.addSelectionItem( getString(R.string.menu_flash), CMD_FLASH, false);
        
        Log.d(LOGTAG, "  CameraInfo ci = new CameraInfo(); ");
        
        CameraInfo ci = new CameraInfo();
        boolean deviceHasFrontCamera = false;
        boolean deviceHasBackCamera = false;
        
        Log.d(LOGTAG, "  for (int i = 0; i < Camera.getNumberOfCameras(); i++) ");
        for (int i = 0; i < Camera.getNumberOfCameras(); i++)
        {
            Camera.getCameraInfo(i, ci);
            if (ci.facing == CameraInfo.CAMERA_FACING_FRONT) deviceHasFrontCamera = true;
            else if (ci.facing == CameraInfo.CAMERA_FACING_BACK) deviceHasBackCamera = true;
        }
        
        Log.d(LOGTAG, " if (deviceHasBackCamera && deviceHasFrontCamera) ");
        if (deviceHasBackCamera && deviceHasFrontCamera)
        {
            group = mSampleAppMenu.addGroup(getString(R.string.menu_camera), true);
            group.addRadioItem(getString(R.string.menu_camera_front), CMD_CAMERA_FRONT, false);
            group.addRadioItem(getString(R.string.menu_camera_back), CMD_CAMERA_REAR, true);
        }*/
        
        mSampleAppMenu.attachMenu();
        
         
        
        Log.d(LOGTAG, " setSampleAppMenuSettings . : OK ");
    }	
	
	
	
	   @Override
	    public boolean menuProcess(int command)
	    {
	        boolean result = true;
	        
	        switch (command)
	        {
 
	        case -20:
	        	//Toast.makeText(this, "other 1", Toast.LENGTH_LONG ).show();
	        	OgreActivityJNI.OpenTheoraVideo(mVideoFiles.get(0));
	        break;
	        case -30:
	        	//Toast.makeText(this, "Other 2", Toast.LENGTH_LONG ).show();
	        	OgreActivityJNI.OpenTheoraVideo(mVideoFiles.get(1));
	        break;	
	        case -40:
	        	Toast.makeText(this, "Comming soon !", Toast.LENGTH_LONG ).show();
	        	//OgreActivityJNI.OpenTheoraVideo(mVideoFiles.get(2));
	        break;
	        case -50:
	        	Toast.makeText(this, "Comming soon !", Toast.LENGTH_LONG ).show();
	        	//ShowDialog();
	        	//Toast.makeText(this, "other 4", Toast.LENGTH_LONG ).show();
	        	//OgreActivityJNI.OpenTheoraVideo(mVideoFiles.get(0));
	        break;	 
	        case -60:
	        	Toast.makeText(this, "Comming soon !", Toast.LENGTH_LONG ).show();
	        	//ShowDialog();
	        	//Toast.makeText(this, "other 4", Toast.LENGTH_LONG ).show();
	        	//OgreActivityJNI.OpenTheoraVideo(mVideoFiles.get(0));
	        break;	
	        case -70:
	        	Toast.makeText(this, "Comming soon !", Toast.LENGTH_LONG ).show();
	        	//ShowDialog();
	        	//Toast.makeText(this, "other 4", Toast.LENGTH_LONG ).show();
	        	//OgreActivityJNI.OpenTheoraVideo(mVideoFiles.get(0));
	        break;		        
	        
	        
	            case CMD_BACK:
	                finish();
	                break;
	            
	            case CMD_FLASH:
	                result = CameraDevice.getInstance().setFlashTorchMode(!mFlash);
	                
	                if (result)
	                {
	                    mFlash = !mFlash;
	                } else
	                {
	                    showToast(getString(mFlash ? R.string.menu_flash_error_off
	                        : R.string.menu_flash_error_on));
	                    Log.e(LOGTAG,
	                        getString(mFlash ? R.string.menu_flash_error_off
	                            : R.string.menu_flash_error_on));
	                }
	                break;
	            
	            case CMD_AUTOFOCUS:
	                
	                if (mContAutofocus)
	                {
	                    result = CameraDevice.getInstance().setFocusMode(
	                        CameraDevice.FOCUS_MODE.FOCUS_MODE_NORMAL);
	                    
	                    if (result)
	                    {
	                        mContAutofocus = false;
	                    } else
	                    {
	                        showToast(getString(R.string.menu_contAutofocus_error_off));
	                        Log.e(LOGTAG,
	                            getString(R.string.menu_contAutofocus_error_off));
	                    }
	                } else
	                {
	                    result = CameraDevice.getInstance().setFocusMode(
	                        CameraDevice.FOCUS_MODE.FOCUS_MODE_CONTINUOUSAUTO);
	                    
	                    if (result)
	                    {
	                        mContAutofocus = true;
	                    } else
	                    {
	                        showToast(getString(R.string.menu_contAutofocus_error_on));
	                        Log.e(LOGTAG,
	                            getString(R.string.menu_contAutofocus_error_on));
	                    }
	                }
	                
	                break;
	            
	            case CMD_EXTENDED_TRACKING:
	                if (dataSetUserDef.getNumTrackables() > 0)
	                {
	                    int lastTrackableCreatedIndex = 
	                        dataSetUserDef.getNumTrackables() - 1;
	                    
	                    Trackable trackable = dataSetUserDef
	                        .getTrackable(lastTrackableCreatedIndex);
	                    
	                    if (!mExtendedTracking) {
	                        if (!trackable.startExtendedTracking()) {
	                            Log.e(LOGTAG, "Failed to start extended tracking target");
	                            result = false;
	                        } else {
	                            Log.d(LOGTAG, "Successfully started extended tracking target");
	                        }
	                    } else
	                    {
	                        if (!trackable.stopExtendedTracking())
	                        {
	                            Log.e(LOGTAG,
	                                "Failed to stop extended tracking target");
	                            result = false;
	                        } else
	                        {
	                            Log.d(LOGTAG,
	                                "Successfully stopped extended tracking target");
	                        }
	                    }
	                }
	                
	                if (result)
	                    mExtendedTracking = !mExtendedTracking;
	                
	                break;
	            
	            case CMD_CAMERA_FRONT:
	            case CMD_CAMERA_REAR:
	                
	                // Turn off the flash
	                if (mFlashOptionView != null && mFlash)
	                {
	                    // OnCheckedChangeListener is called upon changing the checked state
	                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN)
	                    {
	                        ((Switch) mFlashOptionView).setChecked(false);
	                    } else
	                    {
	                        ((CheckBox) mFlashOptionView).setChecked(false);
	                    }
	                }
	                
	                doStopTrackers();
	                CameraDevice.getInstance().stop();
	                CameraDevice.getInstance().deinit();
	                try
	                {
	                    vuforiaAppSession
	                        .startAR(command == CMD_CAMERA_FRONT ? CameraDevice.CAMERA.CAMERA_FRONT : CameraDevice.CAMERA.CAMERA_BACK);
	                } catch (VuforiaExceptions e)
	                {
	                    showToast(e.getString());
	                    Log.e(LOGTAG, e.getString());
	                    result = false;
	                }
	                doStartTrackers();
	                break;
	        
	        }
	        
	        return result;
	    }
	    
	    
	    private void showToast(String text)
	    {
	        Toast.makeText(this, text, Toast.LENGTH_SHORT).show();
	    }	
	    
 
	
	
    static 
    {
    	System.loadLibrary("Vuforia");
       System.loadLibrary("NxGraphics");
    }	
 
}
