package com.hotstuff.main;

import android.content.Context;
import android.graphics.PixelFormat;
 
import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;

public class NxVideoFrontCameraView  extends GLSurfaceView {
	NxVideoFrontCamera mRenderer;
	 
	NxVideoFrontCameraView ( Context context ) {
	    super ( context );
	    mRenderer = new NxVideoFrontCamera(this);
	    
    setEGLContextClientVersion(2);
		setEGLConfigChooser(8, 8, 8, 8, 16, 0); 
		getHolder().setFormat(PixelFormat.RGBA_8888); 
		
		 setRenderer ( mRenderer );
	    
	    
	    /*
	    setEGLContextClientVersion ( 2 );
	    setRenderer ( mRenderer );
	    setRenderMode ( GLSurfaceView.RENDERMODE_WHEN_DIRTY );*/
	  }
	 
	  public void surfaceCreated ( SurfaceHolder holder ) {
	    super.surfaceCreated ( holder );
	  }
	  
	  
	  public NxVideoFrontCamera getRenderer() { 
		  
		  return mRenderer;
	  }
	 
	  public void surfaceDestroyed ( SurfaceHolder holder ) {
	    mRenderer.close();
	    super.surfaceDestroyed ( holder );
	  }
	 
	  public void surfaceChanged ( SurfaceHolder holder, int format, int w, int h ) {
	    super.surfaceChanged ( holder, format, w, h );
	  }
	}    
	
