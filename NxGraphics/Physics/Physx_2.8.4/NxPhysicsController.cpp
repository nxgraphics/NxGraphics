#include <NxGraphicsPreCompiled.h>
#include <NxPhysicsInclude.h>
#include <NxConversions.h>
 

namespace Nx {

class NxPhysicsControllerCallback : public NxUserControllerHitReport
{
public :
	NxPhysicsControllerCallback( NxPhysicsController * cam ) : CurrentCamera( cam ), IsElevatorHit( false )
	{ 
	
	}
	//Called when current controller hits a shape. //param[in] hit Provides information about the contact with the touched shape.
	//return Specifies if to automatically apply forces to the touched actor.
	NxControllerAction onShapeHit( const NxControllerShapeHit& hit )
	{
		/*
		NxController * controller = hit.controller ;
		if( hit.shape )
		{
			NxCollisionGroup group = hit.shape->getGroup();

			if( group != GROUP_COLLIDABLE_NON_PUSHABLE ) // PUSHABLES
			{
				NxActor& actor = hit.shape->getActor();
				if(actor.isDynamic())
				{
					//Log(" Chararacter Hit Dynamic Object : " + Ogre::String(  actor.getName() ) + " With Coeff : " + Ogre::StringConverter::toString( coeff ) );
					Nx3D_Scene * CurrentScene = NxScene3DManager::getSingleton().GetNx3DSceneFromPhysxScene( &actor.getScene() );

					if( CurrentScene )
					{
						NxScene3DObjectEntity * CurrentNxEntity = CurrentScene->GetNxEntityByNxActor( &actor );

						if( CurrentNxEntity )
						{
							NxF32 coeff = actor.getMass() * hit.length * 10.0f;
							//CurrentNxEntity->OnRaisedEvent( NxEntityEvents::NxEntityTouchStart, coeff  );
						}
					}

					if(hit.dir.y==0.0f)
					{
						NxF32 coeff = actor.getMass() * hit.length * 10.0f;
						actor.addForceAtLocalPos(hit.dir*coeff, NxVec3(0,0,0), NX_IMPULSE); // Move Dynamic Object from controller.
					}
				}
				else // not dynamic object
				{

				}
			}
			else if( group == GROUP_COLLIDABLE_NON_PUSHABLE ) // KINEMATIC OR STATIC
			{

				NxActor& actor = hit.shape->getActor();
				if( actor.readBodyFlag( NxBodyFlag::NX_BF_KINEMATIC ))// kinematic has
				{
					//hit.shape->getGlobalPosition();
					//activePlatform = hit.collider.transform;               
					 CurrentHit = hit;
					//CurrentCamera->CollideKinematicObject = true ;
					//NxF32 coeff = 1000.0f * hit.length * 10;
					NxF32 coeff = 10000.0f;//controller->getActor()->getMass() * 100.0f;
					IsElevatorHit = true;
					//Log("Hit by kinematic with coeff : " + Ogre::StringConverter::toString( coeff ) + " Dir : " + Ogre::StringConverter::toString( VecToOgre( hit.dir ) ) );
				}	 
			}
		}
		*/

		return NX_ACTION_PUSH; //Specifies if to automatically apply forces to the touched actor.
	}

	NxControllerAction onControllerHit( const NxControllersHit& hit )
	{
		return NX_ACTION_PUSH; 
	}

	bool IsElevatorHit;
	NxControllerShapeHit CurrentHit ;
	NxPhysicsController * CurrentCamera;

};


	NxPhysicsController::NxPhysicsController( NxNode * Node ) : mController(0), ExtendCharacter(false){

		CurrentNxScene = &Node->GetNxScene().GetPhysicScene()->GetPhysicsScene3D();


	}

	NxPhysicsController::~NxPhysicsController()
	{
		 
	}

	void NxPhysicsController::CreateCharacterController( const Nx::Vector3 & position, float radius, float height) {
		// Defines the maximum height of an obstacle which the character can climb.
		float stepOffset = 0.1f;
		float SkinWidth = 0.1f;//0.01f; // important for elevator and kinematic object
		NxCapsuleControllerDesc desc;
		desc.position = NxExtendedVec3( position.x, position.y, position.z );
		//Ogre::Vector3 size(0.5,1.80,0.5);
		desc.radius = radius;// you could use sqrt(size.x*size.z) * 0.5 - desc.skinWidth;
		desc.height = height;// you could use (size.y - desc.radius - desc.skinWidth) / 1.5;
		// The maximum slope which the character can walk up.
		desc.slopeLimit = cosf( NxMath::degToRad( 60.0f ) ); 
		desc.stepOffset = stepOffset; // see CLIMB_CONSTRAINED
		desc.skinWidth = SkinWidth;
		desc.upDirection = NX_Y;
		desc.climbingMode = CLIMB_EASY;
		mCallback = new NxPhysicsControllerCallback( this );
		desc.callback = mCallback;
		mController = NxScene3DPhysicsManager::getSingleton().GetPhysicsControllerManager()->createController( CurrentNxScene, desc );

		// Access the base Shape
		NxCapsuleController * capsuleCtrl = ((NxCapsuleController*) mController);
		// Set the Capsule Height
		capsuleCtrl->setHeight( height );
		// Set the way the controller interacts with other controllers.
		mController->setInteraction( NXIF_INTERACTION_USE_FILTER );
		// Enable collisions.
		mController->setCollision( true );
		// Set Controller Group
		capsuleCtrl->getActor()->setGroup( GROUP_CHARACTER );

		static int Increment = 0; Increment++;
		Ogre::String Tmp = Ogre::String( "Character_"+ Ogre::StringConverter::toString(  Increment )  );
		capsuleCtrl->getActor()->setName( (const char *)Tmp.c_str() );
		capsuleCtrl->getActor()->setContactReportFlags(  NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH );
		capsuleCtrl->getActor()->setMass(100);
		//capsuleCtrl->getActor()->setContactReportThreshold( 100 );
	}

	void NxPhysicsController::DeleteCharacterController(){
		NxScene3DPhysicsManager::getSingleton().GetPhysicsControllerManager()->releaseController( *mController ); 
		mController = 0;
	}

	void NxPhysicsController::SetPosition( const Nx::Vector3 & position ) {
 
		mController->setPosition( NxExtendedVec3(position.x,position.y,position.z) );
	}

	const Nx::Vector3 & NxPhysicsController::GetPosition() const {
 
		NxExtendedVec3 pos = mController->getPosition();
		return Nx::Vector3(pos.x,pos.y,pos.z);
	}

	void NxPhysicsController::SetCollision( bool Collision )
	{
		mController->setCollision( Collision );
	}

	void NxPhysicsController::Move(const Nx::Vector3 & disp, unsigned int activeGroups, float minDist, unsigned int& collisionFlags, float sharpness ) {

		mController->move( NxVec3ToPhysx(  disp ) , activeGroups, minDist, collisionFlags, sharpness );
	}

	void NxPhysicsController::SetStepOffset( float Offset )
	{
		((NxCapsuleController*) mController )->setStepOffset( Offset );
	}

	void NxPhysicsController::SetHeight( float Height )
	{
		NxCapsuleController* c = ( ( NxCapsuleController * ) mController );
		c->setHeight( Height  );
	}

	float NxPhysicsController::GetHeight()
	{
		return (( NxCapsuleController * ) mController )->getHeight();
	}
 
	void NxPhysicsController::SetRadius( float Radius )
	{
		NxCapsuleController* c = ( ( NxCapsuleController * ) mController );
		c->setRadius( Radius );
	}

	void NxPhysicsController::SetExtends( const Nx::Vector3 & position ){

		NxCapsuleController* c = ( ( NxCapsuleController * ) mController );
		NxF32 height = c->getHeight();
		NxF32 radius = c->getRadius();
		NxF32 inc = 1.0f;
		NxVec3 pos = NxVec3ToPhysx(position);

		if ( ExtendCharacter ){
			height += inc;
			pos.y += inc*0.5f;
		} 
		else { 
			height -= inc;
			//pos.y -= inc*0.5f;//original
			pos.y += inc*0.5f;
		}

		if(1)
		{
			NxCapsule worldCapsule;
			worldCapsule.p0.x = worldCapsule.p1.x = pos.x;
			worldCapsule.p0.y = worldCapsule.p1.y = pos.y;
			worldCapsule.p0.z = worldCapsule.p1.z = pos.z;
			worldCapsule.p0.y -= height*0.5f;
			worldCapsule.p1.y += height*0.5f;
			worldCapsule.radius = radius;
			c->setCollision(false);	// Avoid checking overlap with ourself
			bool Status = CurrentNxScene->checkOverlapCapsule(worldCapsule);
			c->setCollision(true);
			if(Status) {
				LogMsg("Can not resize capsule, ceiling !\n");
				return ;
			}
		}

		ExtendCharacter = !ExtendCharacter;	// Increase or decrease height each time we're called

		// WARNING: the SDK currently doesn't check for collisions when changing height, so if you're close
		// to a wall you might end up penetrating it. In some cases you might also fall through the level.
		// A more advanced implementation will take care of that later.
		c->setPosition(NxExtendedVec3(pos.x, pos.y, pos.z));
		c->setHeight(height);

	}

}