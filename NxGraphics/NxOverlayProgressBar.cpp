#include "NxGraphicsPreCompiled.h"
//#include "..\nxdeps\include\OGRE\OgreTextAreaOverlayElement.h"
//#include "..\nxdeps\include\OGRE\OgreBorderPanelOverlayElement.h"
//#include "..\nxdeps\include\OGRE\OgreFontManager.h"
#include "../nxdeps/include/OGRE/Overlay/OgreOverlayManager.h"
#include "../nxdeps/include/OGRE/Overlay/OgreTextAreaOverlayElement.h"
#include "../nxdeps/include/OGRE/Overlay/OgreBorderPanelOverlayElement.h"
#include "../nxdeps/include/OGRE/Overlay/OgreFontManager.h"

namespace Nx {

class NxLoadingBar : public ResourceGroupListener
{
public:
	RenderWindow* mWindow;
	Overlay* mLoadOverlay;
	Real mInitProportion;
	unsigned short mNumGroupsInit;
	unsigned short mNumGroupsLoad;
	Real mProgressBarMaxSize;
	Real mProgressBarScriptSize;
	Real mProgressBarInc;
	OverlayElement* mLoadingBarElement;
	TextAreaOverlayElement* mLoadingDescriptionElement;
	TextAreaOverlayElement* mLoadingCommentElement;

public:
	NxLoadingBar() {}
	virtual ~NxLoadingBar(){}

	/** Show the loading bar and start listening.
	@param window The window to update 
	@param numGroupsInit The number of groups you're going to be initialising
	@param numGroupsLoad The number of groups you're going to be loading
	@param initProportion The proportion of the progress which will be taken up by initialisation (ie script parsing etc). Defaults to 0.7 since script parsing can often take the majority of the time.
	*/
	virtual void start( RenderWindow* window, unsigned short numGroupsInit = 1, unsigned short numGroupsLoad = 1, Real initProportion = 0.70f )
	{
		mWindow = window;
		mNumGroupsInit = numGroupsInit;
		mNumGroupsLoad = numGroupsLoad;
		mInitProportion = initProportion;
		 
		

		MaterialPtr BackGroundColour = Ogre::MaterialManager::getSingleton().create( "BackgroundLogo", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		BackGroundColour->getTechnique(0)->getPass(0)->setLightingEnabled( false );
		BackGroundColour->getTechnique(0)->getPass(0)->setSceneBlending( SBT_TRANSPARENT_ALPHA );
		BackGroundColour->getTechnique(0)->getPass(0)->setDepthCheckEnabled( false );
		BackGroundColour->getTechnique(0)->getPass(0)->createTextureUnitState( "NxBorderCenter.png" );


		MaterialPtr BackGroundProgressColour = Ogre::MaterialManager::getSingleton().create( "BackgroundProgressLogo", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		BackGroundProgressColour->getTechnique(0)->getPass(0)->setLightingEnabled( false );
		BackGroundProgressColour->getTechnique(0)->getPass(0)->setSceneBlending( SBT_TRANSPARENT_ALPHA );
		BackGroundProgressColour->getTechnique(0)->getPass(0)->setDepthCheckEnabled( false );
		BackGroundProgressColour->getTechnique(0)->getPass(0)->createTextureUnitState( "NxBorderBack.png" );

		mLoadOverlay = Ogre::OverlayManager::getSingleton().create("Overlay");
 

		BorderPanelOverlayElement * Border  = static_cast<BorderPanelOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement( "BorderPanel", "Core/LoadOverlay2" ));
		Border->setMetricsMode( (GuiMetricsMode) GMM_PIXELS);
		Border->setVerticalAlignment( GVA_CENTER);
		Border->setHorizontalAlignment( GHA_CENTER);
		Border->setLeft(-200);
		Border->setTop(100);  
		Border->setWidth(400);
		Border->setHeight( 80 );
		Border->setMaterialName( BackGroundColour->getName() );
		Border->setBorderSize(1,1,1,1);
		Border->setBorderMaterialName(BackGroundColour->getName());

		mLoadingDescriptionElement = static_cast<TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement( "TextArea", "Core/LoadPanel/Description2" ));
		mLoadingDescriptionElement->setMetricsMode( (GuiMetricsMode)GMM_PIXELS);
		mLoadingDescriptionElement->setLeft(5);
		mLoadingDescriptionElement->setTop(5);
		mLoadingDescriptionElement->setWidth(300);
		mLoadingDescriptionElement->setHeight( 20 );
		mLoadingDescriptionElement->setFontName( "BlueHighway" );
		mLoadingDescriptionElement->setParameter("char_height","19");
		mLoadingDescriptionElement->setCaption( "Loading, please wait..." );
		mLoadingDescriptionElement->setColourTop( Ogre::ColourValue(1,1,0.7,1)  );
		mLoadingDescriptionElement->setColourBottom( Ogre::ColourValue(1,1,0.7,1)  );
		Border->addChild( mLoadingDescriptionElement );

		BorderPanelOverlayElement * Border2  = static_cast<BorderPanelOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement( "BorderPanel", "Core/LoadPanel/Bar2" ));
		Border2->setMetricsMode( (GuiMetricsMode)GMM_PIXELS);
		Border2->setVerticalAlignment( GVA_TOP);
		Border2->setLeft(10);
		Border2->setTop(25);
		Border2->setWidth(380);
		Border2->setHeight( 30 );
		Border2->setMaterialName(  BackGroundProgressColour->getName() );
		Border->addChild(  Border2 );

		// barre progress jaune
		mLoadingBarElement  = static_cast<PanelOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement( "Panel", "Core/LoadPanel/Bar/Progress2" ));
		mLoadingBarElement->setMetricsMode( (GuiMetricsMode)GMM_PIXELS);
		mLoadingBarElement->setLeft(0);
		mLoadingBarElement->setTop(2);
		mLoadingBarElement->setWidth(400);
		mLoadingBarElement->setHeight( 26 );
		mLoadingBarElement->setMaterialName( "Core/ProgressBar" );
		Border2->addChild( mLoadingBarElement );

		// parsing script names..
		mLoadingCommentElement = static_cast<TextAreaOverlayElement*>(Ogre::OverlayManager::getSingleton().createOverlayElement( "TextArea", "Core/LoadPanel/Comment2" ));
		mLoadingCommentElement->setMetricsMode( (GuiMetricsMode)GMM_PIXELS);
		mLoadingCommentElement->setLeft(5);
		mLoadingCommentElement->setTop(60);
		mLoadingCommentElement->setWidth(300);
		mLoadingCommentElement->setHeight( 20 );
		mLoadingCommentElement->setFontName( "BlueHighway" );
		mLoadingCommentElement->setParameter("char_height","15");
		mLoadingCommentElement->setCaption( "Initialising..." );
		mLoadingCommentElement->setColourTop( Ogre::ColourValue(0.8,0.8,0.5,1)  );
		mLoadingCommentElement->setColourBottom( Ogre::ColourValue(0.8,0.8,0.5,1)  );
		Border->addChild( mLoadingCommentElement );

		mLoadOverlay->add2D( Border );
		mLoadOverlay->show();

		OverlayElement* barContainer = Border2; 
		mProgressBarMaxSize = barContainer->getWidth();
		mLoadingBarElement->setWidth(0);

		// self is listener
		ResourceGroupManager::getSingleton().addResourceGroupListener(this);

		//Ogre::WindowEventUtilities::messagePump();
	}

	// Hide the loading bar and stop listening. 
	virtual void finish(void) {
		// hide loading screen
		mLoadOverlay->hide();
		// Unregister listener
		ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
	}


	// ResourceGroupListener callbacks
	void resourceGroupScriptingStarted(const String& groupName, size_t scriptCount)
	{
		assert(mNumGroupsInit > 0 && "You stated you were not going to init any groups, but you did! Divide by zero would follow...");
		// Lets assume script loading is 70%
		mProgressBarInc = mProgressBarMaxSize * mInitProportion / (Real)scriptCount;
		mProgressBarInc /= mNumGroupsInit;
		mLoadingDescriptionElement->setCaption("Parsing scripts...");
		mWindow->update();
	 
	}
	void scriptParseStarted(const String& scriptName, bool &skipThisScript)
	{
		mLoadingCommentElement->setCaption(scriptName);
		mWindow->update();
	 
	}
	void scriptParseEnded(const String& scriptName, bool skipped)
	{
		mLoadingBarElement->setWidth( mLoadingBarElement->getWidth() + mProgressBarInc);
		mWindow->update();
	 
	}
	void resourceGroupScriptingEnded(const String& groupName)
	{
		 
	}
	void resourceGroupLoadStarted(const String& groupName, size_t resourceCount)
	{
		assert(mNumGroupsLoad > 0 && "You stated you were not going to load any groups, but you did! Divide by zero would follow...");
		mProgressBarInc = mProgressBarMaxSize * (1-mInitProportion) /  (Real)resourceCount;
		mProgressBarInc /= mNumGroupsLoad;
		mLoadingDescriptionElement->setCaption("Loading resources...");
		mWindow->update();
		 
	}
	void resourceLoadStarted(const ResourcePtr& resource)
	{
		mLoadingCommentElement->setCaption(resource->getName());
		mWindow->update();
		 
	}
	void resourceLoadEnded(void)
	{
	}
	void worldGeometryStageStarted(const String& description)
	{
		mLoadingCommentElement->setCaption(description);
		mWindow->update();
	 
	}
	void worldGeometryStageEnded(void)
	{
		mLoadingBarElement->setWidth( mLoadingBarElement->getWidth() + mProgressBarInc);
		mWindow->update();
	 
	}
	void resourceGroupLoadEnded(const String& groupName)
	{
	}

	Ogre::DataStreamPtr resourceLoading(const String &name, const String &group, Resource *resource)
	{
		return Ogre::DataStreamPtr();
	}

	bool resourceCollision(ResourcePtr &resource, ResourceManager *resourceManager)
	{
		return false;
	}

};



 
ProgressBar::ProgressBar( /*const std::string & name, const std::string& caption, float width, float commentBoxWidth*/ ) : mProgress(0.0f)
{
  
}

void ProgressBar::Start( const Nx::Vector4 & size, const std::string & texturename, unsigned short NumGroups, unsigned short NumGroupsLoad, float Proportion )
{	
	mLoadingBar = new NxLoadingBar();
	mLoadingBar->start( NxEngine::getSingleton().GetNxWindow()->GetWindow(), NumGroups, NumGroupsLoad, Proportion  );

 
	// NxLogo
	MaterialPtr LogoMaterial = Ogre::MaterialManager::getSingleton().create( "LogoMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	LogoMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( texturename /*"NxLogo.jpg"*/);

	Ogre::OverlayContainer * Cont =  static_cast<OverlayContainer*>(OverlayManager::getSingleton().createOverlayElement( "Panel", "LogoNx" ));
	Cont->setMetricsMode(Ogre::GMM_RELATIVE);

	Cont->setPosition(size.x, size.y);
	Cont->setDimensions(size.z, size.w);


	/*
	float RatioX = 1.0f/3.0f;
	float RatioY = 1.0f/2.0f;
	Cont->setPosition(RatioX, 0.1f);
	Cont->setDimensions(RatioX, RatioY);*/
	Cont->show();
	Cont->setMaterialName( LogoMaterial->getName()  );
	mLoadingBar->mLoadOverlay->add2D( Cont );
}

NxOverlayContainer * ProgressBar::GetContainer()
{
	return mElement;
}

void ProgressBar::Finish()
{
	mLoadingBar->finish();
}

void ProgressBar::setProgress( float progress )
{
	mProgress = Ogre::Math::Clamp<Ogre::Real>(progress, 0, 1);
	mFill->SetWidth(std::max<int>((int)mFill->GetHeight(), (int)(mProgress * (mMeter->GetWidth() - 2 * mFill->GetLeft()))));
}

float ProgressBar::getProgress()
{
	return mProgress;
}

const std::string & ProgressBar::getCaption()
{
	return mTextArea->GetCaption();
}

void ProgressBar::setCaption(const std::string & caption)
{
	mTextArea->SetCaption(caption);
}

const std::string & ProgressBar::getComment()
{
	return mCommentTextArea->GetCaption();
}

void ProgressBar::setComment(const  std::string & comment)
{
	mCommentTextArea->SetCaption(comment);
}

 

}// namespace