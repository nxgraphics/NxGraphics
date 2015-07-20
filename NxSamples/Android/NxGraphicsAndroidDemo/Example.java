package nx.graphics.main;
 
import android.app.NativeActivity;
import android.widget.Toast;

public class Example extends NativeActivity  
{
	static 
	{
	   System.loadLibrary("main");
	}	
	
	void ShowText( final String text ) { 
		
		runOnUiThread(new Runnable() {
			public void run() {
				Toast.makeText(Example.this, text, Toast.LENGTH_SHORT).show();
			}
		});
	}
	
	
	 

}

 
