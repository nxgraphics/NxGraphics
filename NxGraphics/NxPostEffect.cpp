#include "NxGraphicsPreCompiled.h"

namespace Nx {

 
NxPostEffect::NxPostEffect( const std::string & Name, const std::string & MaterialName )
{
	//NxVertexShader * VertexShader = NxMaterialManager::getSingleton().CreateVertexProgram( "NOISEVS", "NOISEVS.glsl" );
	//NxPixelShader * PixelShader = NxMaterialManager::getSingleton().CreateFragmentProgram( "NOISEPS", "NOISEPS.glsl"); 

	//mMaterial = NxMaterialManager::getSingleton().CreateMaterial( Name + "_CompositorMaterial" );

	//NxPass * pass = mMaterial->CreateTechnique("")->CreatePass("");
	//NxTextureUnit * Base =  pass->CreateTextureUnit( "MainRT" );
	//pass->SetDepthCheckEnabled(false);
	//pass->SetVertexProgram( VertexShader->GetName() );
	//pass->SetFragmentProgram( PixelShader->GetName() ); 
	//mMaterial->Load();
	 
	Ogre::CompositorPtr ptr = Ogre::CompositorManager::getSingleton().create( Name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME ) ;

		 mEffect = ptr.get();

		 Ogre::CompositionTechnique *t = mEffect->createTechnique();
		 {  // scene render
			 Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("scene");
			 def->width = 1280 ;//1024;//800;//1600;//width;
			 def->height = 1024;//1024;// 600;//600;//height;
			 def->fsaa = false ;// FSAA enabled; true = determine from main target (if render_scene), false = disable
			 def->pooled = true ;		// whether to use pooled textures for this one
			 def->scope = Ogre::CompositionTechnique::TS_LOCAL ;
			 def->formatList.push_back(Ogre::PF_R8G8B8);

			 
		 }
		 // Render scene
		 {
			 Ogre::CompositionTargetPass * tp = t->createTargetPass();
			 tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
			 tp->setOutputName("scene");
		 }
		 /// Display result
		 {
			 Ogre::CompositionTargetPass * tp = t->getOutputTargetPass();
			 tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
			 {
				 Ogre::CompositionPass * pass = tp->createPass();
				 pass->setType(Ogre::CompositionPass::PT_RENDERQUAD );
				 pass->setMaterialName( MaterialName );//mMaterial->GetName());
				 pass->setInput(0, "scene");	 
			 }	
		 }   
 
		 Ogre::CompositorManager::getSingleton().addCompositor( NxEngine::getSingleton().GetNxViewport()->GetViewport(), mEffect->getName() );
		 SetActive( true );
}

NxPostEffect::~NxPostEffect()
{

}

void NxPostEffect::SetActive( bool Active )
{
	Ogre::CompositorManager::getSingleton().setCompositorEnabled( NxEngine::getSingleton().GetNxViewport()->GetViewport(), mEffect->getName(), Active );
}


}