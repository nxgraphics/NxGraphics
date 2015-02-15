#include "NxGraphicsPreCompiled.h"

namespace Nx {

	class DepthOfFieldEffect;
	class Lens;
	class DOFManager
	{
	public:
		DOFManager( Ogre::Camera * Cam );
		~DOFManager();
		DepthOfFieldEffect * GetEffect();
		void setEnabled(bool enabled);
		bool getEnabled();
		void setFocusMode(int mode){ mFocusMode = (FocusMode)mode;}
		void setAutoSpeed(float f);
		void zoomView(float delta);
		void Aperture(float delta);
		void moveFocus(float delta);
		void setZoom(float f);
		void setAperture(float f);
		void setFocus(float f);
		Lens* mLens;
	protected:
		void cleanup();
		DepthOfFieldEffect* mDepthOfFieldEffect;
		enum FocusMode {Auto, Manual, Pinhole};
		FocusMode mFocusMode;
 
		Ogre::Real targetFocalDistance;
		Ogre::Camera * mCamera;
	};


 
	class Lens {
	public:
		Lens(void) {};
		Lens(const Ogre::Real& _focalLength, const Ogre::Real& _fStop, const Ogre::Real& _frameSize = 3.5, const Ogre::Real& _circleOfConfusion = 0.003);
		Lens(const Ogre::Radian& _fieldOfView, const Ogre::Real& _fStop, const Ogre::Real& _frameSize = 3.5, const Ogre::Real& _circleOfConfusion = 0.003);
		~Lens(void) {};

		void init(const Ogre::Real& _focalLength, const Ogre::Real& _fStop, const Ogre::Real& _frameSize = 3.5, const Ogre::Real& _circleOfConfusion = 0.003);
		void init(const Ogre::Radian& _fieldOfView, const Ogre::Real& _fStop, const Ogre::Real& _frameSize = 3.5, const Ogre::Real& _circleOfConfusion = 0.003);

		void setFrameSize(const Ogre::Real& _frameSize);
		void setFocalDistance(const Ogre::Real& _focalDistance);
		void setFocalLength(const Ogre::Real& _focalLength);
		void setFieldOfView(const Ogre::Radian& _fieldOfView);
		void setFStop(const Ogre::Real& _fStop);

		const Ogre::Real& getFrameSize(void)            { return mFrameSize; }
		const Ogre::Real& getFocalDistance(void)        { return mFocalDistance; }
		const Ogre::Real& getFocalLength(void)          { return mFocalLength; }
		const Ogre::Radian& getFieldOfView(void)        { return mFieldOfView; }
		const Ogre::Real& getFStop(void)                { return mFStop; }
		const Ogre::Real& getHyperfocalLength(void)     { return mHyperfocalLength; }

		void recalculateDepthOfField(Ogre::Real& _nearDepth, Ogre::Real& _focalDepth, Ogre::Real& _farDepth);


	protected:
		// NOTE: All units must be the same, eg mm, cm or m etc
		// Primary attributes
		Ogre::Real mFrameSize;                // Film stock/sensor size, arbitrarily selected to help mimic the properties of film, eg 35mm, 3.5cm, 0.035m etc
		Ogre::Real mCircleOfConfusion;        // The area within which the depth of field is clear, it's tied to frame size, eg 0.03mm, 0.003cm, 0.0003m etc
		Ogre::Real mFocalDistance;            // The distance to the object we are focusing on
		Ogre::Real mFocalLength;              // Focal length of the lens, this directly effects field of view, we can do anything from wide angle to telephoto as we don't have the limitations of physical lenses.  Focal length is the distance from the optical centre of the lens to the film stock/sensor etc
		Ogre::Real mFStop;                    // FStop number, ie aperture, changing the aperture of a lens has an effect of depth of field, the narrower the aperture/higher the fstop number, the greater the depth of field/clearer the picture is.

		// Secondary attributes
		Ogre::Real mHyperfocalLength;         // The hyperfocal length is the point at which far depth of field is infinite, ie if mFocalDistance is >= to this value everythig will be clear to infinity
		Ogre::Radian mFieldOfView;            // Field of view of the lens, directly related to focal length


	private:
		void recalculateHyperfocalLength(void);
		void recalculateFieldOfView(void);
		void recalculateFocalLength(void);
	};


	using namespace Ogre;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#undef min
#undef max
#endif

	template<class T>
	const T& min(const T& a, const T& b) {
		return (a < b) ? a : b;
	};


	template<class T>
	const T& max(const T& a, const T& b) {
		return (a > b) ? a : b;
	};


	Lens::Lens(const Real& _focalLength, const Real& _fStop, const Real& _frameSize, const Real& _circleOfConfusion) {
		init(_focalLength, _fStop, _frameSize, _circleOfConfusion);
	}


	Lens::Lens(const Ogre::Radian& _fieldOfView, const Real& _fStop, const Real& _frameSize, const Real& _circleOfConfusion) {
		init(_fieldOfView, _fStop, _frameSize, _circleOfConfusion);
	}


	void Lens::init(const Real& _focalLength, const Real& _fStop, const Real& _frameSize, const Real& _circleOfConfusion) {
		mFocalLength = _focalLength;
		mFStop = _fStop;
		mFrameSize = _frameSize;
		mCircleOfConfusion = _circleOfConfusion;
		recalculateFieldOfView();
	}


	void Lens::init(const Ogre::Radian& _fieldOfView, const Real& _fStop, const Real& _frameSize, const Real& _circleOfConfusion) {
		mFieldOfView = _fieldOfView;
		mFStop = _fStop;
		mFrameSize = _frameSize;
		mCircleOfConfusion = _circleOfConfusion;
		recalculateFocalLength();
	}


	void Lens::setFrameSize(const Real& _frameSize) {
		mFrameSize = _frameSize;
		recalculateFieldOfView();
	}


	void Lens::setFocalDistance(const Real& _focalDistance) {
		mFocalDistance = max(_focalDistance, Real(0));
	}


	void Lens::setFocalLength(const Real& _focalLength) {
		mFocalLength = max(_focalLength, Real(0.3));
		recalculateFieldOfView();
	}


	void Lens::setFieldOfView(const Ogre::Radian& _fieldOfView) {
		mFieldOfView = min(_fieldOfView, Ogre::Radian(2.8));
		recalculateFocalLength();
	}


	void Lens::setFStop(const Real& _fStop) {
		mFStop = max(_fStop, Real(0));
	}


	void Lens::recalculateFocalLength(void) {
		mFocalLength = (mFrameSize / Ogre::Math::Tan(mFieldOfView / 2.0)) / 2.0;
	}


	void Lens::recalculateFieldOfView(void) {
		mFieldOfView = 2.0 * Ogre::Math::ATan(mFrameSize / (2.0 * mFocalLength));
	}


	void Lens::recalculateHyperfocalLength(void) {
		mHyperfocalLength = (mFocalLength * mFocalLength) / (mFStop * mCircleOfConfusion) + mFocalLength;
	}


	void Lens::recalculateDepthOfField(Real& _nearDepth, Real& _focalDepth, Real& _farDepth) {
		// Set focalDepth to the current focalDistance
		_focalDepth = mFocalDistance;

		// Recalculate the Hyperfocal length
		recalculateHyperfocalLength();

		// Calculate the numerator of the optics equations
		Real numerator = (mFocalDistance * (mHyperfocalLength - mFocalLength));

		Real nearClear = numerator / (mHyperfocalLength + mFocalDistance - (2.0 * mFocalLength));

		// Adjust the nearDepth relative to the aperture. This is an approximation.
		_nearDepth = min(nearClear - nearClear * mFStop, (Real)0);

		if(mFocalDistance < mHyperfocalLength)
		{
			// Calculate the far clear plane
			Real farClear = numerator / (mHyperfocalLength - mFocalDistance);

			// Adjust the farDepth relative to the aperture. This is an approximation.
			_farDepth = farClear + farClear * mFStop;
		}

		// Far depth of field should be infinite
		else
			_farDepth = Math::POS_INFINITY;
	}

	class DepthOfFieldEffect : public Ogre::CompositorInstance::Listener, public Ogre::RenderTargetListener, public Ogre::RenderQueue::RenderableListener
	{
	public:
		DepthOfFieldEffect( Ogre::Viewport * View);
		~DepthOfFieldEffect();

		float getNearDepth() const {return mNearDepth; }
		float getFocalDepth() const {return mFocalDepth; }
		float getFarDepth() const {return mFarDepth; }
		void setFocalDepths(float nearDepth, float focalDepth, float farDepth);
		float getFarBlurCutoff() const {return mFarBlurCutoff; }
		void setFarBlurCutoff(float cutoff);
		bool getEnabled() const;
		void setEnabled(bool value);

	private:
		// Implementation of Ogre::CompositorInstance::Listener
		void notifyMaterialSetup(Ogre::uint32 passId, Ogre::MaterialPtr& material);
		// Implementation of Ogre::RenderTargetListener
		void preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt);
		void postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt);
		// Implementation of Ogre::RenderQueue::RenderableListener
		bool renderableQueued(Ogre::Renderable* rend, Ogre::uint8 groupID, 
			Ogre::ushort priority, Ogre::Technique** ppTech, Ogre::RenderQueue* pQueue);

		int mWidth;
		int mHeight;

		static const int BLUR_DIVISOR;

		enum PassId
		{
			BlurPass,
			OutputPass
		};

		Ogre::Viewport* mDepthViewport;
		Ogre::RenderTexture* mDepthTarget;
		Ogre::TexturePtr mDepthTexture;
		Ogre::MaterialPtr mDepthMaterial;
		Ogre::Technique* mDepthTechnique;
		Ogre::CompositorInstance* mCompositor;
		float mNearDepth;
		float mFocalDepth;
		float mFarDepth;
		float mFarBlurCutoff;
		Ogre::Viewport * mViewport;

		void createDepthRenderTexture();
		void destroyDepthRenderTexture();
		//	void createCompositor();
		//	void destroyCompositor();
		void addCompositor();
		void removeCompositor();
	};

	const int DepthOfFieldEffect::BLUR_DIVISOR = 4;


	DepthOfFieldEffect::DepthOfFieldEffect( Ogre::Viewport * View  ) : mViewport( View ),
     mNearDepth(10.0)//0.1
 	, mFocalDepth(100.0)//2.0
 	, mFarDepth(190.0)//10.0
 	, mFarBlurCutoff(1.0)//1.0
	{
		LogMsg("DepthOfFieldEffect : constructor");
		mWidth = mViewport->getActualWidth();
		mHeight = mViewport->getActualHeight();
		LogMsg("DepthOfFieldEffect : Height OK");

		mCompositor = NULL;
		mDepthTechnique = NULL;
		mDepthTarget = NULL;
		mDepthViewport = NULL;
		mDepthTexture.setNull();
		mDepthMaterial.setNull();

		createDepthRenderTexture();
				
		addCompositor();
	}

	DepthOfFieldEffect::~DepthOfFieldEffect()
	{
		removeCompositor();
		destroyDepthRenderTexture();
	}

	void DepthOfFieldEffect::setFocalDepths(float nearDepth, float focalDepth, float farDepth )
	{
		mNearDepth = nearDepth;
		mFocalDepth = focalDepth;
		mFarDepth = farDepth;

		float dofParams[4] = { nearDepth , focalDepth, farDepth, mFarBlurCutoff };
		Ogre::MaterialPtr DepthMat = MaterialManager::getSingleton().getByName("DoF_Depth");
		GpuProgramParametersSharedPtr fragParams = DepthMat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
		fragParams->setNamedConstant("dofParams", dofParams,1,4);

	 

	}

	void DepthOfFieldEffect::setFarBlurCutoff(float cutoff)
	{
		mFarBlurCutoff = cutoff;
	}

	bool DepthOfFieldEffect::getEnabled() const
	{
		return mCompositor->getEnabled();
	}

	void DepthOfFieldEffect::setEnabled(bool value)
	{
		mCompositor->setEnabled(value);
	}

	void DepthOfFieldEffect::createDepthRenderTexture()
	{
		// Create the depth render texture
		mDepthTexture = TextureManager::getSingleton().createManual(
			"DoF_Depth",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			TEX_TYPE_2D, mWidth, mHeight,
			0, Ogre::PF_L8, TU_RENDERTARGET);

		mDepthTexture->load();
		//Ogre::TexturePtr DOFTEX = TextureManager::getSingleton().getByName("DoF_Depth");

		// Get its render target and add a viewport to it
		mDepthTarget = mDepthTexture->getBuffer()->getRenderTarget();
		mDepthTarget->setAutoUpdated(true);

		//Log("DepthOfFieldEffect::createDepthRenderTexture : add viewport..");
		mDepthViewport = mDepthTarget->addViewport( mViewport->getCamera() );
		mDepthViewport->setAutoUpdated(true);

		// Register 'this' as a render target listener
		mDepthTarget->addListener(this);

		// Get the technique to use when rendering the depth render texture
		mDepthMaterial = MaterialManager::getSingleton().getByName("DoF_Depth");
		mDepthMaterial->load(); // needs to be loaded manually
		mDepthTechnique = mDepthMaterial->getBestTechnique();

		// Create a custom render queue invocation sequence for the depth render texture
		RenderQueueInvocationSequence* invocationSequence = Root::getSingleton().createRenderQueueInvocationSequence("DoF_Depth");

		// Add a render queue invocation to the sequence, and disable shadows for it
		RenderQueueInvocation* invocation = invocationSequence->add(RENDER_QUEUE_MAIN, "main");
		invocation->setSuppressShadows(true);

		// Set the render queue invocation sequence for the depth render texture viewport
		mDepthViewport->setRenderQueueInvocationSequenceName("DoF_Depth");

		//re-set texture "DoF_Depth"
		MaterialPtr p = MaterialManager::getSingleton().getByName("DoF_DepthOfField");
		
		Ogre::TextureUnitState * unit =  p->getTechnique(0)->getPass(0)->getTextureUnitState("depth");	
		unit->setTextureName("DoF_Depth");

		LogMsg("DepthOfFieldEffect::createDepthRenderTexture :load ");
		p->load();
		//p->unload();

	}

	void DepthOfFieldEffect::destroyDepthRenderTexture()
	{
		mDepthViewport->setRenderQueueInvocationSequenceName("");
		Root::getSingleton().destroyRenderQueueInvocationSequence("DoF_Depth");

		mDepthMaterial->unload();

		mDepthTarget->removeAllListeners();
		mDepthTarget->removeAllViewports();
		//TextureManager::getSingleton().unload("DoF_Depth");
		TextureManager::getSingleton().remove("DoF_Depth");
	}

	void DepthOfFieldEffect::addCompositor()
	{
		LogMsg("DepthOfFieldEffect::addCompositor : add Compositor...");
		mCompositor = CompositorManager::getSingleton().addCompositor(mViewport, "DoF_Compositor_test");
		mCompositor->addListener(this);
		mCompositor->setEnabled(true);
	}

	void DepthOfFieldEffect::removeCompositor()
	{
		mCompositor->setEnabled(false);

		mCompositor->removeListener(this);
		CompositorManager::getSingleton().removeCompositor(mViewport, "DoF_Compositor_test");
	}

	void DepthOfFieldEffect::notifyMaterialSetup( Ogre::uint32 passId, MaterialPtr& material)
	{
		switch (passId)
		{
		case BlurPass:
			{
				//float pixelSize[2] = {
				//	1.0f / (mViewport->getActualWidth() / BLUR_DIVISOR),
				//	1.0f / (mViewport->getActualHeight() / BLUR_DIVISOR)};

				// Adjust fragment program parameters
				Ogre::Vector3 ps = Ogre::Vector3(1.0f / (mWidth / BLUR_DIVISOR),1.0f / (mHeight / BLUR_DIVISOR), 1.0f);
				float pixelSize[3] = { ps.x, ps.y, ps.z };
				GpuProgramParametersSharedPtr fragParams = material->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
				if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("pixelSize")))
					fragParams->setNamedConstant("pixelSize", pixelSize, 1, 3);

				break;
			}

		case OutputPass:
			{
				float pixelSizeScene[3] = {
					1.0f / mWidth,
					1.0f / mHeight,
					0};

					float pixelSizeBlur[3] = {
						1.0f / (mWidth / BLUR_DIVISOR),
						1.0f / (mHeight / BLUR_DIVISOR),
						0};

						// Adjust fragment program parameters
						GpuProgramParametersSharedPtr fragParams =
							material->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
						if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("pixelSizeScene")))
							fragParams->setNamedConstant("pixelSizeScene", pixelSizeScene,1,3);
						if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("pixelSizeBlur")))
							fragParams->setNamedConstant("pixelSizeBlur", pixelSizeBlur,1,3);

						break;
			}
		}
	}

	void DepthOfFieldEffect::preViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{

		float dofParams[4] = {mNearDepth, mFocalDepth, mFarDepth, mFarBlurCutoff};

		

		//float dofParams[4] = {mNearDepth, mFocalDepth, mFarDepth, mFarBlurCutoff};

		// Adjust fragment program parameters for depth pass
		//GpuProgramParametersSharedPtr fragParams = mDepthTechnique->getPass(0)->getFragmentProgramParameters();

		//fragParams->setNamedConstant("pixelSizeScene", pixelSizeScene,
		//	sizeof(pixelSizeScene)/sizeof(pixelSizeScene[0]), 1.0f);
		//fragParams->setNamedConstant("pixelSizeBlur", pixelSizeBlur,
		//	sizeof(pixelSizeBlur)/sizeof(pixelSizeBlur[0]), 1.0f);
		//fragParams->setNamedConstant("dofParams", dofParams,
		//	sizeof(dofParams)/sizeof(dofParams[0]), 1.0f);
		//fragParams->setNamedConstant("pixelSize", pixelSize,
		//	sizeof(pixelSize)/sizeof(pixelSize[0]), 1.0f);

		//GpuProgramParametersSharedPtr fragParams = mDepthTechnique->getPass(0)->getFragmentProgramParameters();
		//fragParams->setNamedConstant("dofParams", dofParams, sizeof(dofParams)/sizeof(dofParams[0]));

		//GpuProgramParametersSharedPtr fragParams = mDepthTechnique->getPass(0)->getFragmentProgramParameters();
		//if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("dofParams")))
		//	fragParams->setNamedConstant("dofParams", dofParams,1,4);


		// Log("Passss");
		//if((!fragParams.isNull())&&(fragParams->_findNamedConstantDefinition("dofParams")))
		//	fragParams->setNamedConstant("dofParams", dofParams,1,4);

		//fragParams->setNamedConstant("dofParams", dofParams, sizeof(dofParams)/sizeof(dofParams[0]), 1.0f); //





		// Add 'this' as a RenderableListener to replace the technique for all renderables
		RenderQueue* queue = evt.source->getCamera()->getSceneManager()->getRenderQueue();
		queue->setRenderableListener(this);
	}

	void DepthOfFieldEffect::postViewportUpdate(const Ogre::RenderTargetViewportEvent& evt)
	{
		// Reset the RenderableListener
		RenderQueue* queue = evt.source->getCamera()->getSceneManager()->getRenderQueue();
		queue->setRenderableListener(0);
	}

	bool DepthOfFieldEffect::renderableQueued(Ogre::Renderable* rend, Ogre::uint8 groupID, Ogre::ushort priority, Ogre::Technique** ppTech, Ogre::RenderQueue* pQueue)
	{
		// Replace the technique of all renderables
		*ppTech = mDepthTechnique;
		return true;
	}

///////////////
DOFManager::DOFManager( Ogre::Camera * Cam ) : mCamera( Cam )
{
	mFocusMode = Manual;
 
	targetFocalDistance = 5;

	LogMsg("Creating Depth Of Field...");
	mDepthOfFieldEffect = new DepthOfFieldEffect( Cam->getViewport() );
	LogMsg("Creating Depth Of Field : done ");
	mLens = new Lens(mCamera->getFOVy(), 1);
	LogMsg("Creating Lens : Done");
	mLens->setFocalDistance(5);
	//mLens->setFStop(10);
//	mDepthOfFieldEffect->setEnabled(false);

}

DOFManager::~DOFManager()
{
	cleanup();
}

DepthOfFieldEffect * DOFManager::GetEffect()
{
	return mDepthOfFieldEffect;
}

void DOFManager::cleanup()
{
	delete mLens;
	mLens = NULL;

	delete mDepthOfFieldEffect;
	mDepthOfFieldEffect = NULL;
}

void DOFManager::setEnabled(bool enabled)
{
	mDepthOfFieldEffect->setEnabled(enabled);
}

bool DOFManager::getEnabled()
{
	return mDepthOfFieldEffect->getEnabled();
}

void DOFManager::zoomView(float delta)
{
	Real fieldOfView = mLens->getFieldOfView().valueRadians();
	fieldOfView += delta;
	fieldOfView = std::max<Real>(0.1, std::min<Real>(fieldOfView, 2.0));
	mLens->setFieldOfView(Ogre::Radian(fieldOfView));
	mCamera->setFOVy(Ogre::Radian(fieldOfView));
}

void DOFManager::Aperture(float delta)
{
	if (mFocusMode == Pinhole)
		return;
	Real fStop = mLens->getFStop();
	fStop += delta;
	fStop = std::max<Real>(0.5, std::min<Real>(fStop, 12.0));
	mLens->setFStop(fStop);
}

void  DOFManager::setAperture(float f)
{
	if (mFocusMode == Pinhole)
		return;
	Real fStop = f;
	fStop = std::max<Real>(0.5, std::min<Real>(fStop, 12.0));
	mLens->setFStop(fStop);
}

void DOFManager::moveFocus(float delta)
{
	mLens->setFocalDistance(mLens->getFocalDistance() + delta);
}

void DOFManager::setZoom(float f)
{
	Real fieldOfView = Degree(Real(f)).valueRadians();
	fieldOfView = std::max<Real>(0.1, std::min<Real>(fieldOfView, 2.0));
	mLens->setFieldOfView(Ogre::Radian(fieldOfView));
	mCamera->setFOVy(Ogre::Radian(fieldOfView));
}

void  DOFManager::setFocus(float f)
{
	mLens->setFocalDistance(f);
}

/*
bool DOFManager::frameStarted(const FrameEvent& evt)
{
	// Update Depth of Field effect
	if (mFocusMode != Pinhole)
	{
		mDepthOfFieldEffect->setEnabled(true);

		// Calculate and set depth of field using lens
		float nearDepth, focalDepth, farDepth;
		mLens->recalculateDepthOfField(nearDepth, focalDepth, farDepth);
		mDepthOfFieldEffect->setFocalDepths(nearDepth, focalDepth, farDepth);
	}
	else
	{
		mDepthOfFieldEffect->setEnabled(false);
	}
	

	return true;
}
*/
///////////

NxPostEffectDof::NxPostEffectDof( Ogre::Viewport * View )
{
	mDofManager = new DOFManager( View->getCamera() );
}
NxPostEffectDof::~NxPostEffectDof()
{
	delete mDofManager;
}

void NxPostEffectDof::SetFocalDepths(float nearDepth, float focalDepth, float farDepth )
{
	//mDofManager->mLens->recalculateDepthOfField(nearDepth, focalDepth, farDepth);


	 

	mDofManager->GetEffect()->setFocalDepths( nearDepth, focalDepth, farDepth );
}

float NxPostEffectDof::GetNearDepth()
{
	return mDofManager->GetEffect()->getNearDepth();
}

float NxPostEffectDof::GetFocalDepth()
{
	return mDofManager->GetEffect()->getFocalDepth();
}

float NxPostEffectDof::GetFarDepth()
{
	return mDofManager->GetEffect()->getFarDepth();
}

void NxPostEffectDof::SetActive( bool Active )
{
	mDofManager->setEnabled( Active );
}

void NxPostEffectDof::ZoomView(float delta)
{
	mDofManager->zoomView(  delta);
}

void NxPostEffectDof::Aperture(float delta)
{
	mDofManager->Aperture(  delta);
}

void NxPostEffectDof::MoveFocus(float delta)
{
	mDofManager->moveFocus(  delta);
}

void NxPostEffectDof::SetZoom(float f)
{
	mDofManager->setZoom( f );
}

void NxPostEffectDof::SetAperture(float f)
{
	mDofManager->setAperture(  f);
}

void NxPostEffectDof::SetFocus(float f)
{
	mDofManager->setFocus(  f);
}

}