#include "NxGraphicsPreCompiled.h"
#include "NxPostEffects.h"

namespace Nx {

template<> NxPostEffectManager* NxSingleton<NxPostEffectManager>::ms_Singleton = 0;
 
void CreateMotionBlurCompositor()
{
	Ogre::CompositorPtr comp3 = Ogre::CompositorManager::getSingleton().create("Motion Blur", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		{
			Ogre::CompositionTechnique *t = comp3->createTechnique();
			{
				Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("scene");
				def->width = 0;
				def->height = 0;
				def->formatList.push_back(Ogre::PF_R8G8B8);
			}
			{
				Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("sum");
				def->width = 0;
				def->height = 0;
				def->formatList.push_back(Ogre::PF_R8G8B8);
			}
			{
				Ogre::CompositionTechnique::TextureDefinition *def = t->createTextureDefinition("temp");
				def->width = 0;
				def->height = 0;
				def->formatList.push_back(Ogre::PF_R8G8B8);
			}
			/// Render scene
			{
				Ogre::CompositionTargetPass *tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
				tp->setOutputName("scene"); 
			}
			/// Initialisation pass for sum texture
			{
				Ogre::CompositionTargetPass *tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_PREVIOUS);
				tp->setOutputName("sum");
				tp->setOnlyInitial(true);
			}
			/// Do the motion blur
			{
				Ogre::CompositionTargetPass *tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				tp->setOutputName("temp");
				{ 
					Ogre::CompositionPass *pass = tp->createPass();
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Ogre/Compositor/Combine");
					pass->setInput(0, "scene");
					pass->setInput(1, "sum");
				}
			}
			/// Copy back sum texture
			{
				Ogre::CompositionTargetPass *tp = t->createTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				tp->setOutputName("sum");
				{ 
					Ogre::CompositionPass *pass = tp->createPass();
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Ogre/Compositor/Copyback");
					pass->setInput(0, "temp");
				}
			}
			/// Display result
			{
				Ogre::CompositionTargetPass *tp = t->getOutputTargetPass();
				tp->setInputMode(Ogre::CompositionTargetPass::IM_NONE);
				{ 
					Ogre::CompositionPass *pass = tp->createPass();
					pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
					pass->setMaterialName("Ogre/Compositor/MotionBlur");
					pass->setInput(0, "sum");
				}
			}
		}
		 
}// end create motion blur


NxPostEffectManager::NxPostEffectManager()
{

	/*
	HighLevelGpuProgramPtr VertexShader = HighLevelGpuProgramManager::getSingleton().createProgram( "mynamevert", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "glsl", GPT_VERTEX_PROGRAM );
	VertexShader->setSourceFile( "yuvtorgb.vert" );
	VertexShader->load();

	HighLevelGpuProgramPtr PixelShader = HighLevelGpuProgramManager::getSingleton().createProgram( "mynamefrag", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, "glsl", GPT_FRAGMENT_PROGRAM );
	PixelShader->setSourceFile( "fxaa_pass.frag" );
	PixelShader->load();
 

	MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("FullscreenMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	 Ogre::Pass * pass = mat->getTechnique(0)->getPass(0);
	 pass->setVertexProgram("mynamevert");
	 pass->setFragmentProgram("mynamefrag");

	  NxRenderWindow * win =  NxEngine::getSingleton().GetNxWindow();
	 TexturePtr Global_Video_Texture = TextureManager::getSingleton().createManual( 
		 "RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
win->GetWidth(), win->GetHeight(), 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);

	 TextureUnitState * state =  mat->getTechnique(0)->getPass(0)->createTextureUnitState( "test" , 0  );
	 state->setTexture( Global_Video_Texture );

	 GpuProgramParametersSharedPtr params = pass->getFragmentProgramParameters();
	 int in = 0;
	 params->setNamedConstant( "inRTT", in);

 

	mat->setCullingMode( CullingMode::CULL_NONE ); // 2 sided material

	CompositorPtr comp4 = Ogre::CompositorManager::getSingleton().create( "FXAA", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	{	
		Ogre::CompositionTechnique *t = comp4->createTechnique();
		{  // scene render
			Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("scene");
			def->width = 0;
			def->height = 0;
			def->fsaa = false ;// FSAA enabled; true = determine from main target (if render_scene), false = disable
			//def->pooled = true ;		// whether to use pooled textures for this one
			//def->scope = Ogre::CompositionTechnique::TextureScope::TS_LOCAL ;
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
				pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				pass->setMaterialName( mat->getName());
				pass->setInput(0, "scene");	
			}	
		}   
	}
	*/



/*
	MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("FullscreenMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	mat->setCullingMode( CullingMode::CULL_NONE ); // 2 sided material
	//mat->setDepthFunction( CompareFunction::CMPF_ALWAYS_PASS );

	Log("PostEffectManager");

	TexturePtr Global_Video_Texture = TextureManager::getSingleton().createManual( "dummytest" ,
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
		320, 240, 0, PF_BYTE_BGR  , TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );// TU_RENDERTARGETset the size to the output windowPF_BYTE_BGRA

	TextureUnitState * state =  mat->getTechnique(0)->getPass(0)->createTextureUnitState( "dummytest" , 0  );

	CompositorPtr comp4 = Ogre::CompositorManager::getSingleton().create( "NxCompositorOutput", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	{	
		Ogre::CompositionTechnique *t = comp4->createTechnique();
		{  // scene render
			Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("scene");
			def->width = 1024 ;//1024;//800;//1600;//width;
			def->height = 768;//1024;// 600;//600;//height;
			def->fsaa = false ;// FSAA enabled; true = determine from main target (if render_scene), false = disable
			def->pooled = true ;		// whether to use pooled textures for this one
			def->scope = Ogre::CompositionTechnique::TextureScope::TS_LOCAL ;
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
				pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				pass->setMaterialName( mat->getName());
				pass->setInput(0, "scene");	
			}	
		}   
	}

	Viewport * mainviewport = NxEngine::getSingleton().GetNxViewport(); 	//MixerManager::getSingleton().Get_Preview_RenderWindow()->getViewport(0); 
	Ogre::CompositorInstance * inst  = Ogre::CompositorManager::getSingleton().addCompositor(  mainviewport  , "NxCompositorOutput" );
	Ogre::CompositorManager::getSingleton().setCompositorEnabled( mainviewport , "NxCompositorOutput" , true );

	*/


	//Log("Initializing Post Effect Manager...");

	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();
	compMgr.registerCompositorLogic("GaussianBlur", new GaussianBlurLogic);
	compMgr.registerCompositorLogic("HDR", new HDRLogic);

 


	//Ogre::CompositorManager::getSingleton().addCompositor( NxEngine::getSingleton().GetNxViewport(), "HDR" );
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled( NxEngine::getSingleton().GetNxViewport(), "HDR", true );

	/*

	//Viewport * mainviewport = MixerManager::getSingleton().Get_Preview_RenderWindow()->getViewport(0); 
	//Ogre::CompositorManager::getSingleton().setCompositorEnabled( mainviewport , "NxCompositorOutput" , true );

	Ogre::CompositorManager::getSingleton().setVerbose(true);
	//Ogre::CompositorManager::getSingleton().setMemoryBudget(0);

	Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();

	compMgr.registerCompositorLogic("HDR", new HDRLogic);

	CreateMotionBlurCompositor();	
	//CreateOutputCompositor( 1600 , 600 ); 

 
	//Ogre::CompositorManager& compMgr = Ogre::CompositorManager::getSingleton();
	//compMgr.registerCompositorLogic("GaussianBlur", new GaussianBlurLogic);
	//compMgr.registerCompositorLogic("HDR", new HDRLogic);
	//compMgr.registerCompositorLogic("HeatVision", new HeatVisionLogic);
 

 

	std::vector<std::string> Effects;
	GetAvailableEffects( Effects ) ;
	for( int i = 0; i < Effects.size(); i++ ) {
		Ogre::CompositorManager::getSingleton().addCompositor( NxEngine::getSingleton().GetNxViewport(), Ogre::String(  Effects[i] ) );
	}


	MaterialPtr mat = Ogre::MaterialManager::getSingleton().create("FullscreenMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	mat->setCullingMode( CullingMode::CULL_NONE ); // 2 sided material
	//mat->setDepthFunction( CompareFunction::CMPF_ALWAYS_PASS );

	Log("PostEffectManager");

	TexturePtr Global_Video_Texture = TextureManager::getSingleton().createManual( "dummytest" ,
	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
	320, 240, 0, PF_BYTE_BGR  , TU_DYNAMIC );// TU_RENDERTARGETset the size to the output windowPF_BYTE_BGRA

	TextureUnitState * state =  mat->getTechnique(0)->getPass(0)->createTextureUnitState( "dummytest" , 0  );
 
	if( state->isBlank() )
	{
		Log("TextureUnitState is blank!");
	}


	CompositorPtr comp4 = Ogre::CompositorManager::getSingleton().create( "NxCompositorOutput", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
	{	
		Ogre::CompositionTechnique *t = comp4->createTechnique();
		{  // scene render
			Ogre::CompositionTechnique::TextureDefinition * def = t->createTextureDefinition("scene");
			def->width = 1024 ;//1024;//800;//1600;//width;
			def->height = 768;//1024;// 600;//600;//height;
			def->fsaa = false ;// FSAA enabled; true = determine from main target (if render_scene), false = disable
			def->pooled = true ;		// whether to use pooled textures for this one
			def->scope = Ogre::CompositionTechnique::TextureScope::TS_LOCAL ;
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
				pass->setType(Ogre::CompositionPass::PT_RENDERQUAD);
				pass->setMaterialName( mat->getName());
				pass->setInput(0, "scene");	
			}	
		}   
	}

	Viewport * mainviewport = NxEngine::getSingleton().GetNxViewport(); 	//MixerManager::getSingleton().Get_Preview_RenderWindow()->getViewport(0); 
	Ogre::CompositorInstance * inst  = Ogre::CompositorManager::getSingleton().addCompositor(  mainviewport  , "NxCompositorOutput" );
	Ogre::CompositorManager::getSingleton().setCompositorEnabled( mainviewport , "NxCompositorOutput" , false );
	Ogre::LogManager::getSingleton().logMessage("Compositor OUTPUT enabled");
	Ogre::LogManager::getSingleton().logMessage("NxGraphics : Output Compositor Created.");

	*/
}

NxPostEffectManager::~NxPostEffectManager()
{

}

NxPostEffectManager* NxPostEffectManager::getSingletonPtr(void)
{
	return ms_Singleton;
}

NxPostEffectManager& NxPostEffectManager::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

void NxPostEffectManager::AddCompositor( NxViewport * view, const std::string& PostEffectName )
{
	Ogre::CompositorManager::getSingleton().addCompositor( view->GetViewport(), PostEffectName );
}

void NxPostEffectManager::SetCompositorEnabled( NxViewport * view, const std::string& PostEffectName, bool enabled )
{
	Ogre::CompositorManager::getSingleton().setCompositorEnabled( view->GetViewport(), PostEffectName, enabled );
}

NxPostEffect * NxPostEffectManager::CreateEffect( const std::string& PostEffectName, const std::string & MaterialName )
{
	return new NxPostEffect( PostEffectName, MaterialName );
}


void NxPostEffectManager::GetAvailableEffects( std::vector<std::string> & PostEffectList )
{
	PostEffectList.push_back("Bloom");
	//PostEffectList.push_back("Radial Blur"); // -> not available
	PostEffectList.push_back("Motion Blur");
	PostEffectList.push_back("Old TV");
	PostEffectList.push_back("B&W");
	PostEffectList.push_back("Embossed");
	PostEffectList.push_back("Sharpen Edges");
	PostEffectList.push_back("Posterize");
	PostEffectList.push_back("Laplace");
	PostEffectList.push_back("Tiling");
	PostEffectList.push_back("Old Movie");
	PostEffectList.push_back("HDR");
	 //PostEffectList.push_back("Gaussian Blur"); // -> not available
	PostEffectList.push_back("Glass");
	PostEffectList.push_back("ASCII");
	// PostEffectList.push_back("Halftone");
	PostEffectList.push_back("Night Vision");
	PostEffectList.push_back("SSAA");
	PostEffectList.push_back("Invert");
	//PostEffectList.push_back("NxCompositorOutput");

}

void NxPostEffectManager::setUniform( std::string compositor, std::string material, std::string uniform, float value, bool setVisible, int position)
{
	//setUniform("SSAO/CreaseShading", "SSAO/CreaseShading", "cMinimumCrease", slider->getValue(), false, 1);
	(static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName(material)))->getTechnique(0)->
	getPass(0)->getFragmentProgramParameters()->setNamedConstant(uniform, value);
	

}

}//namespace
