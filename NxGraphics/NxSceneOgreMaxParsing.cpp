#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"
#include "../nxdeps/include/OgreMax/OgreMaxScene.hpp"
#include "../NxVideo/NxVideo_Utilities.h"
#include "../NxSound/NxSound_Headers.h"



using namespace OgreMax;

namespace Nx {

NxEntity * ParseEntity( Ogre::SceneNode * Node, NxGraphicsXml * Deserial )
{
	/*
	std::string VideoFile, SoundFile;
	int VideoOpacity, VideoSpeed;
	int SoundRange;
 
	Deserial->ReadParameter(  "SoundFile", SoundFile,  false);
	Deserial->ReadParameter(  "SoundRange" ,SoundRange, false);
	Deserial->ReadParameter(  "VideoTexture", VideoFile, false );
	Deserial->ReadParameter(  "VideoOpacity" ,VideoOpacity, false);
	Deserial->ReadParameter(  "VideoSpeed" ,VideoSpeed, false);

	Nx3DSceneOgreMax * Scene = (Nx3DSceneOgreMax *)NxScene3DManager::getSingleton().GetNx3DSceneFromOgreSceneManager(Node->getCreator());
	
	NxScene3DObjectEntity * NxEntityObject = 0;//Scene->GetNxEntityByName( Node->getName().c_str() );
 
	
	
	std::string PathCopy = Scene->GetNxSceneFileFullPath();
	NxGraphicsUtilities::SplitFilepath( PathCopy );

 

	if( !VideoFile.empty() )
	{
		std::string FileName = PathCopy + "/" + VideoFile;
		NxVideoUtils::SplitFileName( FileName );

		//Log(" Looking for video in pool : " + FileName ); 
		if( !MixerManager::getSingleton().IsVideoInTexturePool( FileName )) // not in pool
		{
			Log("adding video " + FileName );
			NxTextureVideo * Video = MixerManager::getSingleton().CreateVideoTexture( MixerManager::getSingleton().GetNumActiveLayers(), PathCopy + "/" +VideoFile );

			Ogre::MaterialPtr Mat =  Ogre::MaterialManager::getSingleton().getByName(Video->GetMaterialName());
			TextureUnitState* NxUnitState = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
			NxUnitState->setTextureAddressingMode(TextureUnitState::TAM_WRAP);//TAM_BORDER); //dont see the borders


			NxEntityObject->SetMaterialName( Video->GetMaterialName() );	
			Video->SetOpacity( VideoOpacity );
			Video->GetVideo()->SetFramerate( VideoSpeed );
			
		}else //is in pool
		{
			static int MaterialIndex = 0;
			Ogre::MaterialPtr mat = MixerManager::getSingleton().GetVideoTextureMaterial( FileName );
			Ogre::MaterialPtr newmat = mat->clone( mat->getName() + "Copy_" + Ogre::StringConverter::toString( MaterialIndex ));
			VideoOpacity = VideoOpacity / 100.0f;
			Ogre::Pass * mPass = newmat->getTechnique(0)->getPass(0);
			TextureUnitState * mTUnit = mPass->getTextureUnitState(0);
			mTUnit->setAlphaOperation( LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, VideoOpacity );
			NxEntityObject->SetMaterialName( newmat->getName().c_str() );
			MaterialIndex++;   
		}
	}

	if( !SoundFile.empty() )
	{
 

	}

	*/
 
	return 0;
}

Nx3DObjectPhysicFluid * CreateFluid( Ogre::SceneNode * Node, NxGraphicsXml * Deserial )
{
	/*
	int ParticlesNum, ParticlesLifeTime, ParticlesRate; 
	float ParticlesSize, ParticlesViscosity, MaxEmitterAngle; 
	bool Gravity;
	Ogre::Vector3 FluidDirection;
	std::string FluidMaterial;

	Deserial->beginAttribute( "NxFluid" );

	Deserial->ReadParameter( "ParticlesNum", ParticlesNum, false );
	Deserial->ReadParameter( "ParticlesLifeTime", ParticlesLifeTime, false );
	Deserial->ReadParameter( "ParticlesRate", ParticlesRate, false ); 
	Deserial->ReadParameter( "ParticlesSize", ParticlesSize, false ); 
	Deserial->ReadParameter( "Gravity", Gravity, false );
	Deserial->ReadParameter( "ParticlesViscosity", ParticlesViscosity, false ); 
	Deserial->ReadParameter( "MaxEmitterAngle", MaxEmitterAngle, false ); 
	Deserial->ReadParameter( "FluidDirection", FluidDirection, ",", false );
	Deserial->ReadParameter( "FluidMaterial", FluidMaterial, false );


	

	Nx3DObjectPhysicFluid * Fluid = new Nx3DObjectPhysicFluid( Node, ParticlesNum );
	Fluid->SetParticlesLifeTime( ParticlesLifeTime );
	Fluid->SetParticlesRate( ParticlesRate );
	Fluid->SetVelocityMagnitude( 0.4f );
	Fluid->SetParticlesSize( ParticlesSize, ParticlesSize );
	Fluid->SetEmitterMaxAngle( MaxEmitterAngle );
	Fluid->SetViscosity( ParticlesViscosity );

 
	Fluid->SetExternalAcceleration( FluidDirection );
	//Fluid->SetRepulsionCoefficient( 0.01f );
	Fluid->SetGravity( Gravity );  
	Fluid->SetEnabled( true );
	Fluid->SetVisible( true );
	Fluid->SetPosition( Node->getPosition() );
	Fluid->SetMaterialName(FluidMaterial);
	ParseEntity( Node, Deserial );
	//Fluid->SetMaterialName( Fluid->GetMaterialName() );

	Deserial->endAttribute();  

	return Fluid;
	*/

	return 0;
}
/*
Nx3DObjectPhysicMagnet * CreateMagnet( Ogre::SceneNode * Node, NxGraphicsDeSerializer * Deserial )
{
	Deserial->beginAttribute( "NxMagnet" );  

	Ogre::Vector3 MagnetForce, MagnetInfluenceSize;
	Deserial->ReadParameter( "MagnetForce", MagnetForce, ",", false );
	Deserial->ReadParameter( "MagnetInfluenceSize", MagnetInfluenceSize, ",", false );
	Nx3DObjectPhysicMagnet * Magnet = new Nx3DObjectPhysicMagnet( Node );
	Magnet->SetMagnetCoordinatesType( NxEntityMagnetCoordinates::NxSpherical );
	Magnet->SetMagnetForce( MagnetForce );
	Magnet->SetMagnetConstant( Ogre::Vector3( 10, 0, 0 ) );
	Magnet->SetPosition(Node->getPosition());

	Log("MagnetInfluenceSize : " + Ogre::StringConverter::toString( MagnetInfluenceSize ));
	Magnet->SetMagnetInfluenceSize( MagnetInfluenceSize );

	ParseEntity( Node, Deserial );

	Deserial->endAttribute();  

	return Magnet;
}
*/

NxNode * CreateElement( Nx3DSceneOgreMax * Scene, NxNode * Obj, Types::ObjectExtraData * Data )
{
	NxGraphicsXml * Deserial = new NxGraphicsXml();
	Deserial->Parse( Data->userData.c_str() );

	std::string mRootName;
	Deserial->GetRootElementName( mRootName );

	LogMsg("Parsing Root Element : " + mRootName );

	/*
	if( mRootName == "NxMagnet" )
	{
		return CreateMagnet( Obj->GetNxSceneNode(), Deserial );
	}
	else if( mRootName == "NxFluid" )
	{
		return CreateFluid( Obj->GetNxSceneNode(), Deserial );
	}
	else if( mRootName == "NxEntity" )
	{
		Deserial->beginAttribute( "NxEntity" );  
		ParseEntity( Obj->GetNxSceneNode(), Deserial );
		Deserial->endAttribute(); 
		return 0;
	}
	*/

	delete Deserial;

	return 0;
}


void Nx3DSceneOgreMax::ParseUserData()
{
	/*
	std::vector<NxScene3DObject *>::iterator it;
	for( it=NxPhysxEntitiesList.end(); it!=NxPhysxEntitiesList.begin(); )
	{
		--it;

		Types::ObjectExtraData* Data = 0; 

		if((*it)->GetNxType() == NxTypeEntity )
		{
			Data = mSceneOgreMax->GetObjectExtraData( (( NxScene3DObjectEntity *) (*it) )->GetEntity() );
		}
		else if((*it)->GetNxType() == NxTypeSceneNode )
		{
			Data = mSceneOgreMax->GetObjectExtraData( (*it)->GetNxSceneNode()  ); 
		}
		else
		{

		}

		if( Data && Data->HasUserData() )
		{
			NxScene3DObject * NewObj = CreateElement( this, (*it), Data );
			if(NewObj)
			{
				NxPhysxEntitiesList.erase(it);
				NxPhysxEntitiesList.push_back( NewObj );
			}
		}
		
		//if( (*it)->IsValid() == false )
		//{
		//	delete *it;
		//	it=NxPhysxEntitiesList.erase(it);
		//}	
	}
	*/
	return ;
}

void Nx3DSceneOgreMax::CreateActors()//serialize actors
{
   Ogre::Root::MovableObjectFactoryIterator iterFactory = Ogre::Root::getSingleton().getMovableObjectFactoryIterator();

   /*
	BillboardChain
	BillboardSet
	Entity
	Light
	ManualObject
	ParticleSystem
	RibbonTrail
	*/



   /*
   while(iterFactory.hasMoreElements())
   {
      Ogre::MovableObjectFactory* factory = iterFactory.getNext();
	  Log( "Ogre types : " + factory->getType() );
     
   }

	std::vector< std::string > AddNodeNames;
	Ogre::Node::ChildNodeIterator iter = mSceneManager->getRootSceneNode()->getChildIterator();
	while( iter.hasMoreElements() )
	{
		 SceneNode* tmp = static_cast<SceneNode*>(iter.getNext());

		 Log("Found SceneNode : " + tmp->getName() );

		 if( tmp->numAttachedObjects() )
		 {
			 Log("----------------------");
			 Log("SceneNode : " + tmp->getName() + " Got Attached Objects." );
			 Ogre::SceneNode::ObjectIterator objiter = tmp->getAttachedObjectIterator();

			 while( objiter.hasMoreElements() )
			 {
				 Ogre::MovableObject * Obj = objiter.getNext();
				 if( Obj->getMovableType() == "Entity" )
				 {
					NxScene3DObject * Noder = new NxScene3DObject( tmp );
					NxScene3DObjectEntity * PhysxEnt = new NxScene3DObjectEntity( dynamic_cast<Entity *>( Obj ) );
					PhysxEnt->SetPhysicProperties( PhysicGroup::GROUP_NONE );
					Noder->AttachObject( PhysxEnt );

					
					//object dynamic kinematic : Need SceneNode.
					//object static :
					//object dynamic : sceneNode = pos 0000



					NxPhysxEntitiesList.push_back( Noder );
				 }
				 else if( Obj->getMovableType() == "Light")
				 {
					 Log("--- Creating NxLight for : " + tmp->getName() );
				 
				 }
				 else if( Obj->getMovableType() == "Camera")
				 {
					  Log("--- Creating NxCamera for : " + tmp->getName() );
				 
				 }
				 else
				 {
					 Log("--- Type not defined : " + Obj->getMovableType() );
				 
				 }

				 // avoid camera and rest
			 
			 }
			 
		 
		
		 }
		 else // scenenode only
		 {
			 Log("--- Creating NxSceneNode for : " + tmp->getName() );
			NxScene3DObject * Object = new NxScene3DObject( tmp );
			NxPhysxEntitiesList.push_back( Object );	
			
		 
		 }	
	}

	*/
}



}