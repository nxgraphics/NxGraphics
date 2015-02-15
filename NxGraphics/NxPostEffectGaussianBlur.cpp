#include "NxGraphicsPreCompiled.h"
#include "NxPostEffects.h"

namespace Nx {

	class GaussianListener : public Ogre::CompositorInstance::Listener
	{
	protected:
		int mVpWidth, mVpHeight;
		// Array params - have to pack in groups of 4 since this is how Cg generates them
		// also prevents dependent texture read problems if ops don't require swizzle
		float mBloomTexWeights[15][4];
		float mBloomTexOffsetsHorz[15][4];
		float mBloomTexOffsetsVert[15][4];
	public:
		GaussianListener();
		virtual ~GaussianListener();
		void notifyViewportSize(int width, int height);
		virtual void notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
		virtual void notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat);
	};

	Ogre::CompositorInstance::Listener* GaussianBlurLogic::createListener(Ogre::CompositorInstance* instance)
	{
		GaussianListener* listener = new GaussianListener;
		Ogre::Viewport* vp = instance->getChain()->getViewport();
		listener->notifyViewportSize(vp->getActualWidth(), vp->getActualHeight());
		return listener;
	}

	GaussianListener::GaussianListener()
	{
	}
	//---------------------------------------------------------------------------
	GaussianListener::~GaussianListener()
	{
	}
	//---------------------------------------------------------------------------
	void GaussianListener::notifyViewportSize(int width, int height)
	{
		mVpWidth = width;
		mVpHeight = height;
		// Calculate gaussian texture offsets & weights
		float deviation = 3.0f;
		float texelSize = 1.0f / (float)std::min(mVpWidth, mVpHeight);

		// central sample, no offset
		mBloomTexOffsetsHorz[0][0] = 0.0f;
		mBloomTexOffsetsHorz[0][1] = 0.0f;
		mBloomTexOffsetsVert[0][0] = 0.0f;
		mBloomTexOffsetsVert[0][1] = 0.0f;
		mBloomTexWeights[0][0] = mBloomTexWeights[0][1] =
			mBloomTexWeights[0][2] = Ogre::Math::gaussianDistribution(0, 0, deviation);
		mBloomTexWeights[0][3] = 1.0f;

		// 'pre' samples
		for(int i = 1; i < 8; ++i)
		{
			mBloomTexWeights[i][0] = mBloomTexWeights[i][1] =
				mBloomTexWeights[i][2] = Ogre::Math::gaussianDistribution(i, 0, deviation);
			mBloomTexWeights[i][3] = 1.0f;
			mBloomTexOffsetsHorz[i][0] = i * texelSize;
			mBloomTexOffsetsHorz[i][1] = 0.0f;
			mBloomTexOffsetsVert[i][0] = 0.0f;
			mBloomTexOffsetsVert[i][1] = i * texelSize;
		}
		// 'post' samples
		for(int i = 8; i < 15; ++i)
		{
			mBloomTexWeights[i][0] = mBloomTexWeights[i][1] =
				mBloomTexWeights[i][2] = mBloomTexWeights[i - 7][0];
			mBloomTexWeights[i][3] = 1.0f;

			mBloomTexOffsetsHorz[i][0] = -mBloomTexOffsetsHorz[i - 7][0];
			mBloomTexOffsetsHorz[i][1] = 0.0f;
			mBloomTexOffsetsVert[i][0] = 0.0f;
			mBloomTexOffsetsVert[i][1] = -mBloomTexOffsetsVert[i - 7][1];
		}
	}
	//---------------------------------------------------------------------------
	void GaussianListener::notifyMaterialSetup(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
	{
		// Prepare the fragment params offsets
		switch(pass_id)
		{
		case 701: // blur horz
			{
				// horizontal bloom
				mat->load();
				Ogre::GpuProgramParametersSharedPtr fparams =
					mat->getBestTechnique()->getPass(0)->getFragmentProgramParameters();
				fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsHorz[0], 15);
				fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);

				break;
			}
		case 700: // blur vert
			{
				// vertical bloom
				mat->load();
				Ogre::GpuProgramParametersSharedPtr fparams =
					mat->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
				fparams->setNamedConstant("sampleOffsets", mBloomTexOffsetsVert[0], 15);
				fparams->setNamedConstant("sampleWeights", mBloomTexWeights[0], 15);

				break;
			}
		}
	}
	//---------------------------------------------------------------------------
	void GaussianListener::notifyMaterialRender(Ogre::uint32 pass_id, Ogre::MaterialPtr &mat)
	{
	}


}