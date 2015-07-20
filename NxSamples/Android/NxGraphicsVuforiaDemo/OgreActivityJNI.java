package com.hotstuff.main;

import javax.microedition.khronos.egl.EGLContext;

import android.R.bool;
import android.view.Surface;
import android.content.res.AssetManager;

public class OgreActivityJNI {	

	public native static void renderOneFrame();
	public native static void CreateEngine( Surface surface, AssetManager mgr );
	public native static void DeleteEngine();
	public native static EGLContext GetEngineContext();
	public native static int CreateBackGroundTexture( /*int width, int height, int pixelformat*/  );
	public native static void SetViewportSize(  float Left, float Top, float Width, float Height );
	public native static void SetScreenSize(  int x, int y  );	
	public native static void ViewportSetClearEveryFrame( boolean val );
	public native static void ViewportClear();
	public native static void ViewportUpdate();
	public native static void ViewportSetCurrent();
	public native static void ViewportSize( float x, float y, float width, float height );
	
	public native static void SetModelPose( float [] mat44  );
	public native static void SetTextureTransform(  float [] trans, float [] rotate, float [] scale ); 
	public native static void SetTextureFOV( double fovyRadians, double fovRadians ); 
	public native static void OpenTheoraVideo(String filepath);
	public native static void CloseTheoraVideo();
	public native static String[] CopyFilesToPath(String filepath, AssetManager mgr);
	public native static String[] GetFileList( String filepath );
	
	
	
}