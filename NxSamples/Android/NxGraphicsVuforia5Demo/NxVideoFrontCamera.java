package com.hotstuff.main;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.List;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.SurfaceHolder;

public class NxVideoFrontCamera  implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {
	
 
	
	  private final String vss =
	      "attribute vec2 vPosition;\n" +
	      "attribute vec2 vTexCoord;\n" +
	      "varying vec2 texCoord;\n" +
	      "void main() {\n" +
	      "  texCoord = vTexCoord;\n" +
	      "  gl_Position = vec4 ( vPosition.x, vPosition.y, 0.0, 1.0 );\n" +
	      "}";
	 
	  private final String fss =
	      "#extension GL_OES_EGL_image_external : require\n" +
	      "precision mediump float;\n" +
	      "uniform samplerExternalOES sTexture;\n" +
	      "varying vec2 texCoord;\n" +
	      "void main() {\n" +
	      "  gl_FragColor = texture2D(sTexture,texCoord);\n" +
	      "}";

	  private int[] hTex;
	  private FloatBuffer pVertex;
	  private FloatBuffer pTexCoord;
	  private int hProgram;
	 
	  private Camera mCamera;
	  private SurfaceTexture mSTexture;
	 
	  private boolean mUpdateST = false;
	 
	  private NxVideoFrontCameraView mView;
	 
	  NxVideoFrontCamera ( NxVideoFrontCameraView view ) {
	    mView = view;
	    float[] vtmp = { 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f };
	    float[] ttmp = { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f };
	    pVertex = ByteBuffer.allocateDirect(8*4).order(ByteOrder.nativeOrder()).asFloatBuffer();
	    pVertex.put ( vtmp );
	    pVertex.position(0);
	    pTexCoord = ByteBuffer.allocateDirect(8*4).order(ByteOrder.nativeOrder()).asFloatBuffer();
	    pTexCoord.put ( ttmp );
	    pTexCoord.position(0);
	  }
	 
	  public void close()
	  {
	    mUpdateST = false;
	    mSTexture.release();
	    mCamera.stopPreview();
	    mCamera = null;
	    deleteTex();
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
	  
	  public void onSurfaceCreated ( GL10 unused, EGLConfig config ) {
	    //String extensions = GLES20.glGetString(GLES20.GL_EXTENSIONS);
	    //Log.i("mr", "Gl extensions: " + extensions);
	    //Assert.assertTrue(extensions.contains("OES_EGL_image_external"));
	        
	    initTex();
	    mSTexture = new SurfaceTexture ( hTex[0] );
	    mSTexture.setOnFrameAvailableListener(this);

	    mCamera = openFrontFacingCameraGingerbread();//Camera.open();
	    try {
	    	
	    	 
	    	 
	    	
	      mCamera.setPreviewTexture(mSTexture);
	    } catch ( IOException ioe ) {
	    	
	    	Log.i("", "ERRO CAMERA");
	    }

	    GLES20.glClearColor ( 1.0f, 1.0f, 0.0f, 1.0f );
	  
	    hProgram = loadShader ( vss, fss );
	  }
	 
	  public void onDrawFrame ( GL10 unused ) {
	    GLES20.glClear( GLES20.GL_COLOR_BUFFER_BIT );
	  
	    synchronized(this) {
	      if ( mUpdateST ) {
	        mSTexture.updateTexImage();
	        mUpdateST = false;
	      }
	    }

	    GLES20.glUseProgram(hProgram);

	    int ph = GLES20.glGetAttribLocation(hProgram, "vPosition");
	    int tch = GLES20.glGetAttribLocation ( hProgram, "vTexCoord" );
	    int th = GLES20.glGetUniformLocation ( hProgram, "sTexture" );
	  
	    GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
	    GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, hTex[0]);
	    GLES20.glUniform1i(th, 0);
	  
	    GLES20.glVertexAttribPointer(ph, 2, GLES20.GL_FLOAT, false, 4*2, pVertex);
	    GLES20.glVertexAttribPointer(tch, 2, GLES20.GL_FLOAT, false, 4*2, pTexCoord );
	    GLES20.glEnableVertexAttribArray(ph);
	    GLES20.glEnableVertexAttribArray(tch);
	  
	    GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
	    GLES20.glFlush();
	  }
	 
	  public void onSurfaceChanged ( GL10 unused, int width, int height ) {
	    GLES20.glViewport( 0, 0, width, height );
	    Camera.Parameters param = mCamera.getParameters();
	    List<Camera.Size> psize = param.getSupportedPreviewSizes();
	    if ( psize.size() > 0 ) {
	      int i;
	      for ( i = 0; i < psize.size(); i++ ) {
	        if ( psize.get(i).width < width || psize.get(i).height < height )
	          break;
	      }
	      if ( i > 0 )
	        i--;
	      param.setPreviewSize(psize.get(i).width, psize.get(i).height);
	      //Log.i("mr","ssize: "+psize.get(i).width+", "+psize.get(i).height);
	    }
	    param.set("orientation", "landscape");
	    mCamera.setParameters ( param );
	    mCamera.startPreview();
	  }
	 
	  private void initTex() {
	    hTex = new int[1];
	    
	    
	    GLES20.glGenTextures ( 1, hTex, 0 );
	    GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, hTex[0]);
	    GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
	    GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
	    GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
	    GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);
	  }
	 
	  private void deleteTex() {
	    GLES20.glDeleteTextures ( 1, hTex, 0 );
	  }
	 
	  public synchronized void onFrameAvailable ( SurfaceTexture st ) {
	    mUpdateST = true;
	    mView.requestRender();
	  }
	 
	  private static int loadShader ( String vss, String fss ) {
	    int vshader = GLES20.glCreateShader(GLES20.GL_VERTEX_SHADER);
	    GLES20.glShaderSource(vshader, vss);
	    GLES20.glCompileShader(vshader);
	    int[] compiled = new int[1];
	    GLES20.glGetShaderiv(vshader, GLES20.GL_COMPILE_STATUS, compiled, 0);
	    if (compiled[0] == 0) {
	      Log.e("Shader", "Could not compile vshader");
	      Log.v("Shader", "Could not compile vshader:"+GLES20.glGetShaderInfoLog(vshader));
	      GLES20.glDeleteShader(vshader);
	      vshader = 0;
	    }
	  
	    int fshader = GLES20.glCreateShader(GLES20.GL_FRAGMENT_SHADER);
	    GLES20.glShaderSource(fshader, fss);
	    GLES20.glCompileShader(fshader);
	    GLES20.glGetShaderiv(fshader, GLES20.GL_COMPILE_STATUS, compiled, 0);
	    if (compiled[0] == 0) {
	      Log.e("Shader", "Could not compile fshader");
	      Log.v("Shader", "Could not compile fshader:"+GLES20.glGetShaderInfoLog(fshader));
	      GLES20.glDeleteShader(fshader);
	      fshader = 0;
	    }

	    int program = GLES20.glCreateProgram();
	    GLES20.glAttachShader(program, vshader);
	    GLES20.glAttachShader(program, fshader);
	    GLES20.glLinkProgram(program);
	        
	    return program;
	  }
	}
