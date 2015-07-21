package nx.graphics.sound;

 
import java.io.File;
import java.io.FilenameFilter;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;

public class Example extends Activity 
{
	
	 private SeekBar mSeekbar;
	final int ACTIVITY_CHOOSE_FILE = 1;
	
    static 
    {
       System.loadLibrary("main");
    }	
    
	 public native void CreateEngine();
	 public native void DeleteEngine();
	 public native int GetNumDevices();
	 public native String GetDeviceName(int index);
	 public native void OpenDevice();
	 public native void CloseDevice();
	 public native void OpenFile( String file );
	 public native void SetRate( float rate );
    
	
    //private FMODAudioDevice mFMODAudioDevice = new FMODAudioDevice();
    
	private Handler mUpdateHandler = new Handler()
	{
		public void handleMessage(Message msg)
		{		
			/*
			cUpdate();
			
			if (cGetListenerMovement())
			{
				((SeekBar)findViewById(R.id.seekBar1)).setProgress((int)cGetListenerPosition() + 35);
			}
			else
			{
				cSetListenerPosition((float)(((SeekBar)findViewById(R.id.seekBar1)).getProgress() - 35));
			}
			*/
			removeMessages(0);
		    sendMessageDelayed(obtainMessage(0), 50);
		}
	};

	
    public void togglePause16bit3d(View view)
    {
    	Intent chooseFile;
   	 Intent intent;
   	 chooseFile = new Intent(Intent.ACTION_GET_CONTENT);
   	 chooseFile.setType("file/*");
   	 intent = Intent.createChooser(chooseFile, "Choose a file");
   	 startActivityForResult(intent, ACTIVITY_CHOOSE_FILE);
    }
    
    public void togglePause8bit3d(View view)
    {
    	//cTogglePause8bit3d();
    }
    
    public void play16bit2d(View view)
    {
    	//cPlay16bit2d();
    }    
	
    public void toggleListenerMovement(View view)
    {
    	//cToggleListenerMovement();
    }
    
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        
        mSeekbar = (SeekBar) findViewById(R.id.seekBar1);
        
        mSeekbar.setMax(200);
        mSeekbar.setProgress(200);
 
        mSeekbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
         
        	public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
        		
        		int value = progress - 100;
        		
        		float RateValue = (float)value / 100.0f;
        		Log.d("", "Setting rate value : " + RateValue );
        		SetRate( RateValue );
 
        		 
 
        	}
 
            public void onStartTrackingTouch(SeekBar seekBar) {}
            public void onStopTrackingTouch(SeekBar seekBar) {}
 
        });
         
         

        
        
       
        
        
    	CreateEngine();
    	String res = GetDeviceName(0);
    	Log.d("NXSOUND NUM DEVICES : ",  Integer.toString( GetNumDevices() )  + " Device Name : " + res );
    	
    	OpenDevice();        
        
    }
    
    @Override
    public void onDestroy()
    {
    	
    	Log.d("NxSound Java", "Closing device.." );
    	CloseDevice();
    	Log.d("NxSound Java", "Deleting engine.." );
    	DeleteEngine();    	
    	
    	
    }
    
    @Override
    public void onStart()
    {
    	super.onStart();   	

    	mUpdateHandler.sendMessageDelayed(mUpdateHandler.obtainMessage(0), 0);
    }
    
    
    @Override
    protected void onActivityResult( int requestCode, int resultCode, Intent data ) {
    	
      switch(requestCode) {
        case ACTIVITY_CHOOSE_FILE: {
          if (resultCode == RESULT_OK){
            Uri uri = data.getData();
            String filePath = uri.getPath();
            Log.d("", " Opening FILE :  " + filePath );
            OpenFile( filePath );
            mSeekbar.setProgress(200);
          }
        }
      }
    }    
    
    
    
    @Override
    public void onStop()
    {
 
    	mUpdateHandler.removeMessages(0);
    	//cEnd();
    	//mFMODAudioDevice.stop();
    	super.onStop();
    }
	

	@Override
	public void onBackPressed() {
        finish();  	
	}
	
	
 
	

}
