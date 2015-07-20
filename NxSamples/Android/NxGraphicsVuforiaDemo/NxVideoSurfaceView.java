package com.hotstuff.main;

import java.io.IOException;
import java.lang.reflect.Array;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.hotstuff.sexy3d.R;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.PixelFormat;
import android.graphics.SurfaceTexture;
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.media.MediaFormat;
import android.media.MediaPlayer;
import android.media.MediaCodecInfo.CodecCapabilities;
import android.net.Uri;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.opengl.GLUtils;
import android.opengl.Matrix;
import android.os.Parcel;
import android.util.Log;
 
import android.view.Surface;
 
public class NxVideoSurfaceView extends GLSurfaceView {

    VideoRender mRenderer;
    private MediaPlayer mMediaPlayer = null;
    protected Resources mResources;
    private Context mContext;

    public NxVideoSurfaceView(Context context) {
        super(context);
        
        mContext = context;

        setEGLContextClientVersion(2);
        
        mResources = getResources();
        mMediaPlayer = new MediaPlayer();
 	
		setEGLConfigChooser(8, 8, 8, 8, 16, 0); 
		getHolder().setFormat(PixelFormat.RGBA_8888); 
		

		
		/*
		String mimeType = "video/x-vnd.on2.vp8";
		MediaCodec mediaCodec = MediaCodec.createDecoderByType(mimeType);
		MediaFormat mediaFormat = MediaFormat.createVideoFormat(mimeType, 256, 256 );
	    mediaCodec.configure(mediaFormat, surface, null, 0); // this is where it is crashing.

		see http://stackoverflow.com/questions/22197475/decoding-with-omx-sec-vp8-dec-and-omx-google-vpx-decoder-in-samsung-galaxy-y-duo
		*/

        mRenderer = new VideoRender(context);
        setRenderer(mRenderer);
        
        mRenderer.setMediaPlayer( mMediaPlayer );
        
        
        
        
        
        
        
        int numCodecs = MediaCodecList.getCodecCount();
        for (int i = 0; i < numCodecs; i++) {
            MediaCodecInfo codecInfo = MediaCodecList.getCodecInfoAt(i);
            String name = codecInfo.getName();
            Log.i("CODEC", "Examinig " + (codecInfo.isEncoder() ? "encoder" : "decoder") + ": " + name);
            for(String type: codecInfo.getSupportedTypes()) {	
            CodecCapabilities cap = codecInfo.getCapabilitiesForType(type);
            int formats [] = cap.colorFormats;
            int num = Array.getLength( formats );
            	for( int x = 0 ; x < num ; x++ ) {
            		if( formats[x] == cap.COLOR_FormatYUV420Planar  ) {
            			Log.i("CODEC", "--> COLOR_FormatYUV420Planar");
       				}            		
            		else{ Log.i("CODEC", "--> OTHER SUPPORTED COLOR : " +  formats[x] ); } 
            	}     	 
            }
        }
        
        int dede = 0 ;
        dede =2;
       
        
 
    }
    
    public void OpenVideoFileInt( int res ) {
    	
    	Uri video = Uri.parse("android.resource://" + mContext.getPackageName() + "/" + res ); 
 
    	try {
			mMediaPlayer.setDataSource( mContext, video  );
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (SecurityException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IllegalStateException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	
    	//mMediaPlayer.setVideoURI(video);
    	
    }
    
    public void OpenVideoFile( String name  ){ 
 
        try {
        	AssetFileDescriptor afd =   mResources.getAssets().openFd(name); 
        	mMediaPlayer.setDataSource(
        	afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength());
        	afd.close();
        	} catch (Exception e) {
        		Log.e("NxVideoSurfaceView", e.getMessage(), e);
        	}	
    }

    @Override
    public void onResume() {
        queueEvent(new Runnable(){
                public void run() {
                    mRenderer.setMediaPlayer(mMediaPlayer);
                }});

        super.onResume();
    }

    private static class VideoRender implements GLSurfaceView.Renderer, SurfaceTexture.OnFrameAvailableListener {
        private static String TAG = "VideoRender";

        private static final int FLOAT_SIZE_BYTES = 4;
        private static final int TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
        private static final int TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
        private static final int TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;
         private final float[] mTriangleVerticesData = {
            // X, Y, Z, U, V
            -1.0f, -1.0f, 0, 0.f, 0.f,
            1.0f, -1.0f, 0, 1.f, 0.f,
            -1.0f,  1.0f, 0, 0.f, 1.f,
            1.0f,  1.0f, 0, 1.f, 1.f,
        }; 
        
 
        

        private FloatBuffer mTriangleVertices;

        private final String mVertexShader =
                "uniform mat4 uMVPMatrix;\n" +
                "uniform mat4 uSTMatrix;\n" +
                "attribute vec4 aPosition;\n" +
                "attribute vec4 aTextureCoord;\n" +
                "varying vec2 vTextureCoord;\n" +
                "void main() {\n" +
                "  gl_Position = uMVPMatrix * aPosition;\n" +
                "  vTextureCoord = (uSTMatrix * aTextureCoord).xy;\n" +
                "}\n";

  /*      private final String mFragmentShader =
                "#extension GL_OES_EGL_image_external : require\n" +
                "precision mediump float;\n" +
                "varying vec2 vTextureCoord;\n" +
                "uniform samplerExternalOES sTexture;\n" +
                "void main() {\n" +
                "gl_FragColor = texture2D(sTexture, vTextureCoord);\n" +
                // "gl_FragColor = vec4(1.0, 0.0, 0.0, 0.5);\n" + // << -- will show transparency
                "}\n";
   */
        
        
        
        
        private final String mFragmentShader =
        		"#version 300 es\n"+
                "#extension GL_OES_EGL_image_external : require\n" +
                "precision mediump float;\n" +
                "varying vec2 vTextureCoord;\n" +
                "uniform samplerExternalOES sTexture;\n" +
                "float Brightness( float R, float G, float B) { return sqrt( R * R * .241 +  G * G * .691 + B * B * .068 );}\n"+
                "void main() {\n" +
                "if( vTextureCoord.x <= 0.5 ){;\n" +
                "vec4 res = texture2D(sTexture, vTextureCoord);\n" + 
                "vec2 pos = vec2( vTextureCoord.x + 0.5 , vTextureCoord.y );\n" +                  
                "vec4 tmp = texture2D(sTexture, pos) ;\n" +  
                "float alpha = Brightness(tmp.r, tmp.g, tmp.b );\n" +      
                "gl_FragColor = vec4(res.r, res.g, res.b, alpha);\n" +                
                " }else{ discard;  };\n" +
                "}\n";  
        
        
        


        private float[] mMVPMatrix = new float[16];
        private float[] mSTMatrix = new float[16];

        private int mProgram;
        private int mTextureID;
        private int muMVPMatrixHandle;
        private int muSTMatrixHandle;
        private int maPositionHandle;
        private int maTextureHandle;

        private SurfaceTexture mSurface;
        private boolean updateSurface = false;

        private static int GL_TEXTURE_EXTERNAL_OES = 0x8D65;

        private MediaPlayer mMediaPlayer;

        public VideoRender(Context context) {
            mTriangleVertices = ByteBuffer.allocateDirect(
                mTriangleVerticesData.length * FLOAT_SIZE_BYTES)
                    .order(ByteOrder.nativeOrder()).asFloatBuffer();
            mTriangleVertices.put(mTriangleVerticesData).position(0);

            Matrix.setIdentityM(mSTMatrix, 0);
        }

        public void setMediaPlayer(MediaPlayer player) {
            mMediaPlayer = player;
        }

        public void onDrawFrame(GL10 glUnused) {
            synchronized(this) {
                if (updateSurface) {
                    mSurface.updateTexImage();
                    mSurface.getTransformMatrix(mSTMatrix);
                    updateSurface = false;
                }
            }

            GLES20.glClear( GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
            GLES20.glUseProgram(mProgram);
            checkGlError("glUseProgram");
  
       	 	GLES20.glEnable(GLES20.GL_BLEND); 
       	 	GLES20.glBlendFunc(GLES20.GL_SRC_ALPHA, GLES20.GL_ONE_MINUS_SRC_ALPHA); 
 
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
            GLES20.glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureID);
 
            mTriangleVertices.position(TRIANGLE_VERTICES_DATA_POS_OFFSET);
            GLES20.glVertexAttribPointer(maPositionHandle, 3, GLES20.GL_FLOAT, false, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
            checkGlError("glVertexAttribPointer maPosition");
            GLES20.glEnableVertexAttribArray(maPositionHandle);
            checkGlError("glEnableVertexAttribArray maPositionHandle");

            mTriangleVertices.position(TRIANGLE_VERTICES_DATA_UV_OFFSET);
            GLES20.glVertexAttribPointer(maTextureHandle, 3, GLES20.GL_FLOAT, false,TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices);
            checkGlError("glVertexAttribPointer maTextureHandle");
            GLES20.glEnableVertexAttribArray(maTextureHandle);
            checkGlError("glEnableVertexAttribArray maTextureHandle");

            Matrix.setIdentityM(mMVPMatrix, 0);
            GLES20.glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix, 0);
            GLES20.glUniformMatrix4fv(muSTMatrixHandle, 1, false, mSTMatrix, 0);
 
            GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4);
 
            checkGlError("glDrawArrays");
            GLES20.glFinish();

        }

        public void onSurfaceChanged(GL10 glUnused, int width, int height) {

        }


        public void onSurfaceCreated(GL10 glUnused, EGLConfig config) {
 
            mProgram = createProgram(mVertexShader, mFragmentShader);
            if (mProgram == 0) {
                return;
            }
            maPositionHandle = GLES20.glGetAttribLocation(mProgram, "aPosition");
            checkGlError("glGetAttribLocation aPosition");
            if (maPositionHandle == -1) {
                throw new RuntimeException("Could not get attrib location for aPosition");
            }
            maTextureHandle = GLES20.glGetAttribLocation(mProgram, "aTextureCoord");
            checkGlError("glGetAttribLocation aTextureCoord");
            if (maTextureHandle == -1) {
                throw new RuntimeException("Could not get attrib location for aTextureCoord");
            }

            muMVPMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");
            checkGlError("glGetUniformLocation uMVPMatrix");
            if (muMVPMatrixHandle == -1) {
                throw new RuntimeException("Could not get attrib location for uMVPMatrix");
            }

            muSTMatrixHandle = GLES20.glGetUniformLocation(mProgram, "uSTMatrix");
            checkGlError("glGetUniformLocation uSTMatrix");
            if (muSTMatrixHandle == -1) {
                throw new RuntimeException("Could not get attrib location for uSTMatrix");
            }


            int[] textures = new int[1];
            GLES20.glGenTextures(1, textures, 0);

            mTextureID = textures[0];
            GLES20.glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureID);
            checkGlError("glBindTexture mTextureID");

            GLES20.glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
            GLES20.glTexParameterf(GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
            
            
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
            GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);            
            
 
            mSurface = new SurfaceTexture(mTextureID);
            mSurface.setOnFrameAvailableListener(this);
            Surface surface = new Surface(mSurface);

            mMediaPlayer.setSurface(surface);
            surface.release();

            try {
                mMediaPlayer.prepare();
                
                mMediaPlayer.setLooping(true);
            } catch (IOException t) {
                Log.e(TAG, "media player prepare failed");
            }

            synchronized(this) {
                updateSurface = false;
            }

            mMediaPlayer.start();
        }

        synchronized public void onFrameAvailable(SurfaceTexture surface) {
            updateSurface = true;
        }

        private int loadShader(int shaderType, String source) {
            int shader = GLES20.glCreateShader(shaderType);
            if (shader != 0) {
                GLES20.glShaderSource(shader, source);
                GLES20.glCompileShader(shader);
                int[] compiled = new int[1];
                GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
                if (compiled[0] == 0) {
                    Log.e(TAG, "Could not compile shader " + shaderType + ":");
                    Log.e(TAG, GLES20.glGetShaderInfoLog(shader));
                    GLES20.glDeleteShader(shader);
                    shader = 0;
                }
            }
            return shader;
        }

        private int createProgram(String vertexSource, String fragmentSource) {
            int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
            if (vertexShader == 0) {
                return 0;
            }
            int pixelShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
            if (pixelShader == 0) {
                return 0;
            }

            int program = GLES20.glCreateProgram();
            if (program != 0) {
                GLES20.glAttachShader(program, vertexShader);
                checkGlError("glAttachShader");
                GLES20.glAttachShader(program, pixelShader);
                checkGlError("glAttachShader");
                GLES20.glLinkProgram(program);
                int[] linkStatus = new int[1];
                GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
                if (linkStatus[0] != GLES20.GL_TRUE) {
                    Log.e(TAG, "Could not link program: ");
                    Log.e(TAG, GLES20.glGetProgramInfoLog(program));
                    GLES20.glDeleteProgram(program);
                    program = 0;
                }
            }
            return program;
        }

        private void checkGlError(String op) {
            int error;
            while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
                Log.e(TAG, op + ": glError " + error);
                throw new RuntimeException(op + ": glError " + error);
            }
        }
    }  // End of class VideoRender.
}  // End of class NxVideoSurfaceView.
