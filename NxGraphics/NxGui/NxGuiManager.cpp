#include "NxGraphicsPreCompiled.h"

namespace Nx {

class ViewListener : public Ogre::Viewport::Listener
{	
public :
	ViewListener( NxGuiManager * watch ) : mManager( watch ) {

		mCamera = mManager->GetViewport()->GetViewport()->getCamera();
	}

	void viewportCameraChanged( Viewport * viewport ) {
		LogManager::getSingleton().logMessage("CAMERA VIEWPORT CHANGED to  : " + mCamera->getName() );
	}

	void viewportDimensionsChanged(Viewport* viewport) {
		mManager->OnViewportResized( viewport->getActualWidth(), viewport->getActualHeight() );
	}
private :
	 NxGuiManager * mManager;
	 Ogre::Camera * mCamera;
};

 

static std::vector<NxGuiManager*> setOverlays;
// a class to make sure that only the pointed-to overlay gets painted
class OverlayPreferer : public Ogre::RenderTargetListener
{
public:
	OverlayPreferer( NxGuiManager * watch , Ogre::Viewport* pMyViewport) : mManager(watch),m_pViewport(pMyViewport) {
		 setOverlays.push_back(watch);
	}
	virtual ~OverlayPreferer() {

	}
	/*virtual*/ void preViewportUpdate(const Ogre::RenderTargetViewportEvent & evt) /*override*/
	{
		if(evt.source == m_pViewport) {
			for(int i = 0; i < setOverlays.size() ; i++){
				 setOverlays[i]->Hide();
			}
			
			if( m_pViewport->getOverlaysEnabled() ) mManager->Show(); 
		}
	}
	/*virtual*/ void postViewportUpdate(const Ogre::RenderTargetViewportEvent & evt) /*override*/ {

	}
	
private:
	NxGuiManager * mManager;
	Ogre::Viewport* m_pViewport;
};
 
 
NxGuiManager::NxGuiManager( NxContentManager * pSceneManager, NxViewport * pViewport ) : mSceneManager( pSceneManager ), mViewport( pViewport )
{
	Ogre::Viewport * view = pViewport->GetViewport();
	view->getTarget()->addListener(new OverlayPreferer(this, pViewport->GetViewport()  ));
	mColours = new NxGuiColours();
	//mFontName ="AndaleMono";
 
	mScreenWidth = mScreenOriWidth = view->getActualWidth();
	mScreenHeight = mScreenOriHeight = view->getActualHeight();

	mFontName ="BlueHighway";
	mFontSize = 16.0f / (float)mScreenHeight; // or 32.0f on android ????

	view->addListener( new ViewListener(this) );

	Ogre::MaterialPtr RectangleMaterial = Ogre::MaterialManager::getSingleton().create( "ManualColours", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME  );
	Ogre::Pass * RectangleMaterialPass = RectangleMaterial->getTechnique(0)->getPass(0);
	RectangleMaterialPass->setVertexColourTracking( TVC_DIFFUSE | TVC_AMBIENT | TVC_SPECULAR  );
	RectangleMaterialPass->setLightingEnabled( false );
	RectangleMaterialPass->setSceneBlending( SBT_TRANSPARENT_ALPHA );

}

NxGuiManager::~NxGuiManager() {

}

NxContentManager * NxGuiManager::GetContentManager() {
	return mSceneManager;
}

void NxGuiManager::SetFontName( const std::string & FontName ) {
	mFontName = FontName;
}

const std::string & NxGuiManager::GetFontName() const {
	return mFontName;
}

float NxGuiManager::getDefaultFontSize() {
	return mFontSize;
}

void NxGuiManager::setDefaultFontSize( float size ) {
	mFontSize = size;
}

void NxGuiManager::OnViewportResized( unsigned int ScreenWidth, unsigned int ScreenHeight )
{
	mScreenWidth = ScreenWidth; mScreenHeight = ScreenHeight;
	for( int i = 0 ; i < mPanels.size(); i++ ) {
		mPanels[i]->OnViewportResized( mScreenWidth, mScreenHeight );
	}
	//LogManager::getSingleton().logMessage(  " ViewportWidth : " + Ogre::StringConverter::toString( mScreenWidth ) );
	//LogManager::getSingleton().logMessage(  " ViewportHeight: " + Ogre::StringConverter::toString( mScreenHeight ) );
}

NxViewport * NxGuiManager::GetViewport()
{
	return mViewport;
} 

unsigned int NxGuiManager::GetViewportOriginalWidth() {
	return mScreenOriWidth;
}

unsigned int NxGuiManager::GetViewportOriginalHeight() {
	return mScreenOriHeight;
}

unsigned int NxGuiManager::GetViewportWidth() {
	return mScreenWidth;
}
 
unsigned int NxGuiManager::GetViewportHeight() {
	return mScreenHeight;
}

void NxGuiManager::Show() {
	for( int i = 0 ; i < mPanels.size(); i++ ) {
		mPanels[i]->SetVisible(true, true );
	}
}

void NxGuiManager::Hide() {
	for( int i = 0 ; i < mPanels.size(); i++ ) {
		mPanels[i]->SetVisible(false, true );
	}
}

void NxGuiManager::Update( float Time ) {
	for( int i = 0 ; i < mPanels.size(); i++ ) {
		mPanels[i]->Update( Time );
	}
}

NxGuiColours * NxGuiManager::GetColours() {
	return mColours;
}


void NxGuiManager::getMouseLastCoordinates( int &x, int &y ) {
	x = mX;
	y = mY;
}

void NxGuiManager::mouseMoved( int x, int y ) {
	x = mScreenOriWidth  * x/ mScreenWidth;
	y = mScreenOriHeight * y/ mScreenHeight;
	mX = x; mY = y;
	for( int i = 0 ; i < mPanels.size(); i++ ) {
		mPanels[i]->mouseMoved( x, y );
	}
}

void NxGuiManager::mouseDragged(int x, int y, int button) {
	/*
	x = mScreenOriWidth  * x/ mScreenWidth;
	y = mScreenOriHeight * y/ mScreenHeight;
	for( int i = 0 ; i < mPanels.size(); i++ ) {
		mPanels[i]->mouseDragged( x, y, button );
	}*/
}

bool NxGuiManager::mousePressed( int x, int y, int button ) {

	bool Hit = false;
	x = mScreenOriWidth  * x/ mScreenWidth;
	y = mScreenOriHeight * y/ mScreenHeight;
	mX = x; mY = y;
	for( int i = 0 ; i < mPanels.size(); i++ ) {
		//Hit += mPanels[i]->mousePressed( x, y, button );
		if(mPanels[i]->mousePressed( x, y, button )) { Hit  = true; };//break;  // do not break else others dont get info
	}
	return Hit;
}

void NxGuiManager::mouseReleased(int x, int y, int button) {
	x = mScreenOriWidth  * x/ mScreenWidth;
	y = mScreenOriHeight * y / mScreenHeight;
	mX = x; mY = y;
	for( int i = 0 ; i < mPanels.size(); i++ ) {
		mPanels[i]->mouseReleased( x, y, button );
	}
}

void NxGuiManager::touchPressed( int x, int y, int touchId ) {
	mousePressed( x, y, 0 );
}

void NxGuiManager::touchReleased( int x, int y, int touchId ) {
	mouseReleased( x, y, 0 );
}

void NxGuiManager::touchMoved( int x, int y, int touchId ) {
	mouseMoved( x, y );
}

void NxGuiManager::touchCancelled( int x, int y, int touchId ) {


}

void NxGuiManager::AddPanel( NxPanel * panel ) {
	mPanels.push_back( panel);

}

NxPanel * NxGuiManager::CreatePanel( const std::string & HeaderText, float PosX, float PosY, float Width, float Height ) {
	NxPanel * c = new NxPanel( HeaderText, PosX, PosY, Width, Height, this );
	mPanels.push_back( c );
	return c;
}

}



	 
  