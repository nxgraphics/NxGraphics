package nx.graphics.device;

import android.app.Activity;
import android.os.Bundle;

public class NxDevice extends Activity 
{	
    static 
    {
       System.loadLibrary("NxDevice");
    }	
    
	 public native void createNxDevice();
 
 
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
  
    	createNxDevice(); 
    }
    
    @Override
    public void onDestroy() {
		super.onDestroy();
    	
    }
    
    @Override
    protected void onStart() {
    	super.onStart();
    }
    
    
    @Override
    public void onStop() {
 
    	super.onStop();
    }
	

	@Override
	public void onBackPressed() {
        finish();  	
	}
	
}
