package com.hotstuff.main;

import java.io.File;
import java.util.ArrayList;

import com.hotstuff.main.FileChooser.FileSelectedListener;
import com.hotstuff.main.R;

 

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.graphics.Rect;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Handler;
import android.preference.PreferenceManager;
import android.text.InputType;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.GestureDetector;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewTreeObserver;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewTreeObserver.OnGlobalLayoutListener;
import android.view.Window;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

 

//Handles the sample apps menu settings
public class VuforiaMenu
{
 
 protected static final String SwipeSettingsInterface = null;
 private GestureListener mGestureListener;
 private GestureDetector mGestureDetector;
 private VuforiaMenuAnimator mMenuAnimator;
 private Activity mActivity;
 private VuforiaMenuInterface mMenuInterface;
 private SurfaceView mMovableView;
 private VuforiaMenuView mParentMenuView;
 private LinearLayout mMovableListView;
 private ArrayList<VuforiaMenuGroup> mSettingsItems = new ArrayList<VuforiaMenuGroup>();
 
 private ArrayList<View> mAdditionalViews;
 private float mInitialAdditionalViewsX[];
 private int mScreenWidth;
 private int mListViewWidth = 0;
 
 // True if dragging and displaying the menu
 boolean mSwipingMenu = false;
 
 // True if menu is showing and docked
 boolean mStartMenuDisplaying = false;
 
 float mGingerbreadMenuClipping = 0;
 
 private static float SETTINGS_MENU_SCREEN_PERCENTAGE = .5f;//.80f;
 private static float SETTINGS_MENU_SCREEN_MIN_PERCENTAGE_TO_SHOW = .1f;
 
 boolean mIsBelowICS = Build.VERSION.SDK_INT < Build.VERSION_CODES.ICE_CREAM_SANDWICH;
 
 
 // Parameters:
 // menuInterface - Reference to the object which will be handling the
 // processes from the menu entries
 // activity - The activity where the swipe menu will be used
 // menuTitle - Title to be displayed
 // movableView - SurfaceView where the OpenGL rendering is done
 // listView - Parent view where the settings layout will be attached
 // additionalViewToHide - Additional view to move with openGl view
 public VuforiaMenu(VuforiaMenuInterface menuInterface,
     Activity activity, String menuTitle,  SurfaceView movableView,
     RelativeLayout parentView, ArrayList<View> additionalViewsToHide)
 {
	 
	 Log.e("VuforiaMenu : ", " Create ....");
	 
     mMenuInterface = menuInterface;
     mActivity = activity;
     mMovableView = movableView;
     mAdditionalViews = additionalViewsToHide;
     
     LayoutInflater inflater = LayoutInflater.from(mActivity);
     mParentMenuView = (VuforiaMenuView) inflater.inflate(R.layout.sample_app_menu_layer, null, false);
     parentView.addView(mParentMenuView);
     
     mMovableListView = (LinearLayout) mParentMenuView.findViewById(R.id.settings_menu);
     mMovableListView.setBackgroundColor(Color.BLACK);
     
     TextView title = (TextView) mMovableListView.findViewById(R.id.settings_menu_title);
     
     title.setText( "[ World Cup AR ]" );
     title.setTextSize( 10.0f );
     title.setBackgroundColor( Color.BLACK );
     title.setTextColor(  Color.WHITE );
     
     mMovableView.setVisibility(View.VISIBLE);
     
     if (mAdditionalViews != null && mAdditionalViews.size() > 0)
     {
         mInitialAdditionalViewsX = new float[mAdditionalViews.size()];
     }
     
     mGestureListener = new GestureListener();
     mGestureDetector = new GestureDetector(mActivity, mGestureListener);
     
     if (!mIsBelowICS)
         mMenuAnimator = new VuforiaMenuAnimator(this);
     
     DisplayMetrics metrics = new DisplayMetrics();
     activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
     mScreenWidth = metrics.widthPixels;
     
     // Used to set the listView length depending on the glView width
     ViewTreeObserver vto = mMovableView.getViewTreeObserver();
     vto.addOnGlobalLayoutListener(new OnGlobalLayoutListener()
     {
         @SuppressWarnings("deprecation")
         @Override
         public void onGlobalLayout()
         {
             int menuWidth = Math.min(mMovableView.getWidth(), mMovableView.getHeight());
             mListViewWidth = (int) (menuWidth * SETTINGS_MENU_SCREEN_PERCENTAGE);
             
             RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams(
                 mListViewWidth, RelativeLayout.LayoutParams.MATCH_PARENT);
             
             params.addRule(RelativeLayout.ALIGN_PARENT_TOP);
             params.addRule(RelativeLayout.ALIGN_PARENT_LEFT);
             mParentMenuView.setLayoutParams(params);
             
             setMenuDisplaying(false);
             mGestureListener.setMaxSwipe(mListViewWidth);
             
             LinearLayout.LayoutParams groupParams = new LinearLayout.LayoutParams(
                 mListViewWidth, LinearLayout.LayoutParams.WRAP_CONTENT);
             for (VuforiaMenuGroup group : mSettingsItems)
             {
                 group.getMenuLayout().setLayoutParams(groupParams);
             }
             
             mMovableView.getViewTreeObserver()
                 .removeGlobalOnLayoutListener(this);
             
         }
     });
     
     Log.e("VuforiaMenu : ", " Create DONE");
     
 }
 
 
 public boolean processEvent(MotionEvent event)
 {
     boolean result = false;
     result = mGestureDetector.onTouchEvent(event);
     
     if (event.getAction() == MotionEvent.ACTION_UP && !result)
     {
         setSwipingMenu(false);
         
         // Hides the menu if it is not docked when releasing
         if (!isMenuDisplaying() || getViewX(mMovableView) < (mScreenWidth * SETTINGS_MENU_SCREEN_PERCENTAGE))
         {
             if (isMenuDisplaying() || getViewX(mMovableView) < (mScreenWidth * SETTINGS_MENU_SCREEN_MIN_PERCENTAGE_TO_SHOW)) {
                 hideMenu();
             } else {
                 showMenu();
             }
         }
     }
     
     return result;
 }
 
 
 private void startViewsAnimation(boolean display)
 {
     float targetX = display ? mGestureListener.getMaxSwipe() : 0;
     
     mMenuAnimator.setStartEndX(getViewX(mMovableView), targetX);
     mMenuAnimator.start();
     
     if (mAdditionalViews != null)
     {
         for (int i = 0; i < mAdditionalViews.size(); i++)
         {
             setViewX(mAdditionalViews.get(i), mInitialAdditionalViewsX[i] + targetX);
         }
     }
 }
 
 
 public void setSwipingMenu(boolean isSwiping)
 {
     mSwipingMenu = isSwiping;
 }
 
 
 public boolean isMenuDisplaying()
 {
     return mStartMenuDisplaying;
 }
 
 
 public void setMenuDisplaying(boolean isMenuDisplaying)
 {
     // This is used to avoid the ListView to consume the incoming event when
     // the menu is not displayed.
     mParentMenuView.setFocusable(isMenuDisplaying);
     mParentMenuView.setFocusableInTouchMode(isMenuDisplaying);
     mParentMenuView.setClickable(isMenuDisplaying);
     mParentMenuView.setEnabled(isMenuDisplaying);
     
     mStartMenuDisplaying = isMenuDisplaying;
     
 }
 
 
 public void hide()
 {
     setViewX(mMovableView, 0);
     
     mParentMenuView.setHorizontalClipping(0);
     mParentMenuView.setVisibility(View.GONE);
     
     if (mAdditionalViews != null && !mIsBelowICS)
     {
         for (int i = 0; i < mAdditionalViews.size(); i++)
         {
             setViewX(mAdditionalViews.get(i), mInitialAdditionalViewsX[i]);
         }
     }
     
 }
 
 
 private void setViewX(View view, float x)
 {
     if (!mIsBelowICS)
         view.setX(x);
     else
         mGingerbreadMenuClipping = x;
 }
 
 
 private float getViewX(View view)
 {
     float x = 0;
     if (!mIsBelowICS)
         x = view.getX();
     else
         x = mGingerbreadMenuClipping;
     
     return x;
 }
 
// public boolean useLeftMenu = false;
 
 
 
 public void showMenu()
 {
	// if( !useLeftMenu ) return;
	 
	 
     if (!mIsBelowICS)
     {
         startViewsAnimation(true);
     } else
     {
         setAnimationX(mGestureListener.getMaxSwipe());
         setMenuDisplaying(true);
     }
 }
 
 
 public void hideMenu()
 {
     if (!mIsBelowICS) {
         if (!mMenuAnimator.isRunning())
         {
             startViewsAnimation(false);
             setMenuDisplaying(false);
         }
     } 
     else {
         hide();
         setMenuDisplaying(false);
     }
 }
 
 
 public VuforiaMenuGroup addGroup(String string, boolean hasTitle)
 {
	 VuforiaMenuGroup newGroup = new VuforiaMenuGroup(mMenuInterface, mActivity, this, hasTitle, string, 10);
     mSettingsItems.add(newGroup);
     return mSettingsItems.get(mSettingsItems.size() - 1);
 }
 
 
 public void attachMenu()
 {
     int size = mSettingsItems.size();
     for (VuforiaMenuGroup group : mSettingsItems)
     {
    	 
    	 LinearLayout lay = group.getMenuLayout();
         mMovableListView.addView( lay );
    	 //HERE MENU .. cose crashes on nexus...
     }
     
    // View newView = new View(mActivity);
    // newView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
    // newView.setBackgroundColor(Color.BLACK);
     
     
     //mMovableListView.addView(newView);
     hide();
     setMenuDisplaying(false);
     
 }
 
 
 public void setAnimationX(float animtationX)
 {
     mParentMenuView.setVisibility(View.VISIBLE);
     setViewX(mMovableView, animtationX);
     
     mParentMenuView.setHorizontalClipping((int) animtationX);
     
     if (mAdditionalViews != null)
     {
         for (int i = 0; i < mAdditionalViews.size(); i++)
         {
             setViewX(mAdditionalViews.get(i), mInitialAdditionalViewsX[i]
                 + animtationX);
         }
     }
 }
 
 
 public void setDockMenu(boolean isDocked)
 {
     setMenuDisplaying(isDocked);
     if (!isDocked)
         hideMenu();
 }
 
 // Process the gestures to handle the menu
 private class GestureListener extends GestureDetector.SimpleOnGestureListener
 {
     // Minimum distance to start displaying the menu
     int DISTANCE_TRESHOLD = 10;
     // Minimum velocity to display the menu upon fling
     int VELOCITY_TRESHOLD = 2000;
     
     // Maximum x to dock the menu
     float mMaxXSwipe;
     
     
     // Called when dragging
     @Override
     public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
    	 
    	 
    	 if( !Example.useLeftMenu ) return true;
    	 
         if (Math.abs(distanceX) > DISTANCE_TRESHOLD && !mSwipingMenu) {
             mSwipingMenu = true;
             mParentMenuView.setVisibility(View.VISIBLE);
             
             if (mAdditionalViews != null && !mIsBelowICS && !mStartMenuDisplaying) {
                 for (int i = 0; i < mAdditionalViews.size(); i++) {
                     mInitialAdditionalViewsX[i] = getViewX(mAdditionalViews.get(i));
                 }
             }
         }
         
         if (mSwipingMenu && mMovableView != null && (getViewX(mMovableView) - distanceX > 0))
         {
             float deltaX = Math.min(mMaxXSwipe, getViewX(mMovableView) - distanceX);
             
             setViewX(mMovableView, deltaX);
             
             mParentMenuView.setHorizontalClipping((int) deltaX);
             
             if (mAdditionalViews != null && !mIsBelowICS)
             {
                 for (int i = 0; i < mAdditionalViews.size(); i++)
                 {
                     setViewX(mAdditionalViews.get(i),
                         mInitialAdditionalViewsX[i] + deltaX);
                 }
             }
             
         }
         
         if (mMaxXSwipe <= getViewX(mMovableView))
         {
             Handler handler = new Handler();
             handler.postDelayed(new Runnable(){

                 @Override
                 public void run()
                 {
                     showMenu();
                 }
                 
             }, 100L);
             
         }
         
         return false;
     }
     
     
     @Override
     public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
     {
    	 
    	 if( !Example.useLeftMenu ) return true;
    	 
         if (velocityX > VELOCITY_TRESHOLD && !isMenuDisplaying())
         {
             showMenu();
         }
         return false;
     }
     
     
     @Override
     public boolean onSingleTapUp(MotionEvent e)
     {
         boolean consumeTapUp = isMenuDisplaying();
         hideMenu();
         
         return consumeTapUp;
     }
     
     
     
     public void showDialog() { 
    	 


		AlertDialog.Builder alert = new AlertDialog.Builder( mActivity  );
		alert.setTitle("Configuration");
		alert.setMessage("veuillez changer les valeurs :");
		
		SharedPreferences preferencesInput = PreferenceManager.getDefaultSharedPreferences( mActivity );
		
		// MAIN LAYOUT
		LinearLayout layMain = new LinearLayout( mActivity );
		//layMain.setLayoutParams(  new LayoutParams( LayoutParams.MATCH_PARENT , LayoutParams.MATCH_PARENT) );
		
		
		Rect displayRectangle = new Rect();
		Window window = mActivity.getWindow();
		window.getDecorView().getWindowVisibleDisplayFrame(displayRectangle);

		// inflate and adjust layout
		 
		layMain.setMinimumWidth((int)(displayRectangle.width() * 0.6f));
		layMain.setMinimumHeight((int)(displayRectangle.height() * 0.5f));		
		
		
		
		layMain.setOrientation( LinearLayout.VERTICAL );
		
		// IP
		// IP LAYOUT
		LinearLayout layFirst = new LinearLayout( mActivity );
		layFirst.setLayoutParams(  new LinearLayout.LayoutParams( LayoutParams.MATCH_PARENT , LayoutParams.WRAP_CONTENT  ) );
		layFirst.setWeightSum(10);
		// IP TEXT
		final EditText inputIpText = new EditText ( mActivity );
		inputIpText.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 5) );
		inputIpText.setText( preferencesInput.getString( "ip", null )  );
		layFirst.addView( inputIpText );
		// IP BUTTON
		final Button buttonIp = new Button( mActivity );
		buttonIp.setText("test ip");
		buttonIp.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 5) );
		layFirst.addView( buttonIp  );
		buttonIp.setOnClickListener(  new OnClickListener() {
			@Override
			public void onClick(View v) {
				Toast.makeText( mActivity , "Pinged !", Toast.LENGTH_SHORT  ).show();
			}
		});
 
		layMain.addView( layFirst );
		
		
		// MARKER
		// MARKER LAYOUT
		LinearLayout laySecond = new LinearLayout( mActivity );
		laySecond.setLayoutParams(  new LinearLayout.LayoutParams( LayoutParams.MATCH_PARENT , LayoutParams.WRAP_CONTENT  ) );
		laySecond.setWeightSum(10);
		// MARKER TEXT
		final TextView inputMarkerText = new TextView ( mActivity );
		inputMarkerText.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 5) );
		
		String tmp = preferencesInput.getString( "markerName", null );
		
		if( -1 == tmp.lastIndexOf("/") ) { // name only
			
			inputMarkerText.setText( tmp  );
		}else { //path
			String name =  tmp.substring(tmp.lastIndexOf("/"), tmp.length()-1);  
			inputMarkerText.setText( name  );
			
		}
 
		laySecond.addView( inputMarkerText );
		// MARKER BUTTON
		final Button chooseMarkerButton = new Button( mActivity );
		chooseMarkerButton.setText("Choose");
		chooseMarkerButton.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 5) );
		laySecond.addView( chooseMarkerButton  );
		chooseMarkerButton.setOnClickListener(  new OnClickListener() {
			@Override
			public void onClick(View v) {
				
				FileChooser choose = new FileChooser(mActivity, ".xml");
				choose.setFileListener(  new FileSelectedListener() {
					
					@Override
					public void fileSelected(File file) {
						
						String tmp = file.getAbsolutePath();
						String pathAndName = tmp.substring(0, tmp.length()-4);
						File testDat = new File( pathAndName + ".dat" );
						if( !testDat.exists() ) { 
							
							Toast.makeText( mActivity, "cant find associated .dat file !" , Toast.LENGTH_SHORT  ).show();			
						}else { // found .dat file
							
							String name =  pathAndName.substring(tmp.lastIndexOf("/")+1, pathAndName.length() ); 
							inputMarkerText.setText( name  );
							SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences( mActivity );
							SharedPreferences.Editor editor = preferences.edit();
						    editor.putString("markerName", pathAndName );
						    editor.putBoolean("markerFromPath", true );
						    editor.commit(); 						
							Toast.makeText( mActivity , "selected marker: "+pathAndName, Toast.LENGTH_SHORT  ).show();							
						}
 
					}
				} ).showDialog();
 	
				
			}
		});		
		
		layMain.addView( laySecond );		
		
		// SPLASH SCREEN
		 
		// mDataSplash = preferencesRead.getString("splashName", null );
		// mDataSplashFromPath = preferencesRead.getBoolean("splashNameFromPath", false );
		
		
		 
		// SPLASH LAYOUT
		LinearLayout layThird = new LinearLayout( mActivity );
		layThird.setLayoutParams(  new LinearLayout.LayoutParams( LayoutParams.MATCH_PARENT , LayoutParams.WRAP_CONTENT  ) );
		layThird.setWeightSum(10);
		// MARKER TEXT
		final TextView inputSplashText = new TextView ( mActivity );
		inputSplashText.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 5) );
		
		String tmpSplash = preferencesInput.getString( "splashName", null );
		
		if( -1 == tmpSplash.lastIndexOf("/") ) { // name only
			
			inputSplashText.setText( tmpSplash  );
		}else { //path
			String name =  tmpSplash.substring(tmpSplash.lastIndexOf("/")+1, tmpSplash.length());  
			inputSplashText.setText( name  );
			
		}
 
		
		layThird.addView( inputSplashText );
		// MARKER BUTTON
		final Button chooseSplashButton = new Button( mActivity );
		chooseSplashButton.setText("Choose");
		chooseSplashButton.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 5) );
		layThird.addView( chooseSplashButton  );
		chooseSplashButton.setOnClickListener(  new OnClickListener() {
			@Override
			public void onClick(View v) {
				
				FileChooser choose = new FileChooser(mActivity, ".jpg");
				choose.setFileListener(  new FileSelectedListener() {
					
					@Override
					public void fileSelected(File file) {
						
						String tmp = file.getAbsolutePath();	
						String name =  tmp.substring(tmp.lastIndexOf("/")+1, tmp.length() ); 
						inputSplashText.setText( name  );
						SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences( mActivity );
						SharedPreferences.Editor editor = preferences.edit();
					    editor.putString("splashName", tmp );
					    editor.putBoolean("splashNameFromPath", true );
					    editor.commit(); 						
						Toast.makeText( mActivity , "selected splash: "+ name, Toast.LENGTH_SHORT  ).show();							
						 
 
					}
				} ).showDialog();
 	
				
			}
		});		
		
		layMain.addView( layThird );	
		
		
		// threshold distance
		LinearLayout layFourth = new LinearLayout( mActivity );
		layFourth.setLayoutParams(  new LinearLayout.LayoutParams( LayoutParams.MATCH_PARENT , LayoutParams.WRAP_CONTENT  ) );
		layFourth.setWeightSum(10);
		// threshold TEXT
		final EditText inputthresholdText = new EditText ( mActivity );
		inputthresholdText.setInputType(InputType.TYPE_CLASS_NUMBER | InputType.TYPE_NUMBER_FLAG_DECIMAL | InputType.TYPE_NUMBER_FLAG_SIGNED);
		inputthresholdText.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 5) );
		layFourth.addView( inputthresholdText );
		float val = preferencesInput.getFloat("thresholdDistance", 1.0f );
		inputthresholdText.setText( String.valueOf( val ) );
		
		
		final Button applyThresholdButton = new Button( mActivity );
		applyThresholdButton.setText("Apply");
		applyThresholdButton.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 5) );
		layFourth.addView( applyThresholdButton  );
		applyThresholdButton.setOnClickListener( new OnClickListener() {
			@Override
			public void onClick(View v) { 
				Toast.makeText(mActivity, "TO DO apply",  Toast.LENGTH_LONG ).show();
			}
		});
		layMain.addView( layFourth );
		///////
		
	
		
		// threshold distance
		LinearLayout layFifth = new LinearLayout( mActivity );
		layFifth.setLayoutParams(  new LinearLayout.LayoutParams( LayoutParams.MATCH_PARENT , LayoutParams.WRAP_CONTENT  ) );
		layFifth.setWeightSum(10);
		 
		
		
		final Button applyCalibrateButton = new Button( mActivity );
		applyCalibrateButton.setText("CALIBRATE");
		applyCalibrateButton.setLayoutParams( new LinearLayout.LayoutParams( 0, LayoutParams.MATCH_PARENT, 10) );
		layFifth.addView( applyCalibrateButton  );
		applyCalibrateButton.setOnClickListener( new OnClickListener() {
			@Override
			public void onClick(View v) { 
				
				
				SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences( mActivity );
				SharedPreferences.Editor editor = preferences.edit();
			    editor.putFloat("calibrationOffset", OgreActivityJNI.GetGyroscopeY()  );
			    editor.commit(); 
	 
				OgreActivityJNI.SetGyroscopeOffset( OgreActivityJNI.GetGyroscopeY() );
				
				
				Toast.makeText(mActivity, "calibrated : " + String.valueOf( OgreActivityJNI.GetGyroscopeY() ),  Toast.LENGTH_LONG ).show();
			}
		});
		layMain.addView( layFifth );
		
		///////
 
		alert.setView( layMain );
		
		alert.setPositiveButton("Redemarrer", new DialogInterface.OnClickListener() {
		public void onClick(DialogInterface dialog, int whichButton) {
			
			try{
				Float val = Float.valueOf( inputthresholdText.getText().toString() ) ;
				SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences( mActivity );
				SharedPreferences.Editor editor = preferences.edit();
			    editor.putFloat("thresholdDistance", val );
			    editor.commit(); 
			    
			    // restart the app
			    Intent i = mActivity.getBaseContext().getPackageManager().getLaunchIntentForPackage( mActivity.getBaseContext().getPackageName() );
				i.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP|Intent.FLAG_ACTIVITY_NEW_TASK);
				mActivity.finish();
				mActivity.startActivity(i); 
			
				android.os.Process.killProcess(android.os.Process.myPid());
		        System.exit(0);		    
			
			
			}catch( NumberFormatException e ) { 
				
				Toast.makeText( mActivity , "selected distance not valid: "+ inputthresholdText.getText().toString(), Toast.LENGTH_SHORT  ).show();		
				
				
			}
				 
 

 
		  }
		});
		
		alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
		  public void onClick(DialogInterface dialog, int whichButton) {
		      // Canceled.
		  }
		});
		alert.show();

    	 
    	 
    	 
     }
    
     
     
     @Override
     public boolean onDoubleTap(MotionEvent e)
     {
    	 
    	 
    	 showDialog();
    	 
    	 if( !Example.useLeftMenu ) return true;
    	 
    	 
         if (!isMenuDisplaying())
         {
             if (!mIsBelowICS)
             {
                 startViewsAnimation(true);
             } else
             {
                 setAnimationX(mMaxXSwipe);
                 setMenuDisplaying(true);
             }
         }
         return true;
     }
     
     
     // Percentage of the screen to display and maintain the menu
     public void setMaxSwipe(float maxXSwipe)
     {
         mMaxXSwipe = maxXSwipe;
         if (!mIsBelowICS)
         {
             mMenuAnimator.setMaxX(mMaxXSwipe);
             mMenuAnimator.setStartEndX(0.0f, mMaxXSwipe);
         }
     }
     
     
     public float getMaxSwipe()
     {
         return mMaxXSwipe;
     }
     
 }
 
}