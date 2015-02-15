#include <NxGraphicsPreCompiled.h>
#include <NxConversions.h>
#include "../../nxsound/NxSound_Headers.h"
#include "../../nxdeps/include/OIS/OIS.h"

namespace Nx {

NxCharacterController::NxCharacterController( NxNode * ParentNode, const std::string & CameraName ) : NxEntity(), NxCharacterNode(0), mActive(false)
{

	mType = NxTypeCamera;
	mNavigationMode = ModeFly;
	m_CameraSpeed = 1.0f;
	m_NewCamPosition = Nx::Vector3::ZERO;
	mMoveSpeed = 200;
	mRotateSpeed = 36 ;
	PxJump = false;
	G = -9.81f;
	WalkStandUpHeight = 1.85f ; // 1 meter 85
	WalkCrouchHeight  = 1.00f;
	NxCharacterSpeed = 7;

	 
 
	//! Fps Camera
	NxCamera = ParentNode->GetNxScene().GetNxSceneManager()->createCamera(  CameraName + "_MainCamera"   );
	//! Camera is relative 1.85 meters in Y from the scene Node ( eye level )
	NxCamera->setPosition( 0, 1.85f , 0 );
	NxCamera->setNearClipDistance( 0.1f );
	NxCamera->setFarClipDistance ( 4096.0f ); 
	//if (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
    //    NxCamera->setFarClipDistance(0);
 
	mNode = ParentNode->GetNxScene().GetNxSceneManager()->createSceneNode( CameraName );
	mNode->attachObject( NxCamera );

	mNxNodeParent = ParentNode;
	mNxNodeParent->GetNxSceneNode()->addChild( mNode );

	 
	mController = new NxPhysicsController( ParentNode );
	 
	//! other view camera
	NxCameraOtherView = ParentNode->GetNxScene().GetNxSceneManager()->createCamera( CameraName + "_OtherView");
	// create a pivot at roughly the character's shoulder
	mCameraPivot = ParentNode->GetNxScene().GetNxSceneManager()->getRootSceneNode()->createChildSceneNode();
	// this is where the camera should be soon, and it spins around the pivot
	mCameraGoal = mCameraPivot->createChildSceneNode(Ogre::Vector3(0, 0, 15 ));
	// this is where the camera actually is
	mCameraNode = ParentNode->GetNxScene().GetNxSceneManager()->getRootSceneNode()->createChildSceneNode();
	mCameraNode->setPosition( mCameraPivot->getPosition() + mCameraGoal->getPosition() );

	mCameraPivot->setFixedYawAxis(true);
	mCameraGoal->setFixedYawAxis(true);
	mCameraNode->setFixedYawAxis(true);

	// our model is quite small, so reduce the clipping planes
	NxCameraOtherView->setNearClipDistance( NxCamera->getNearClipDistance() );
	NxCameraOtherView->setFarClipDistance( NxCamera->getFarClipDistance() );
	mCameraNode->attachObject( NxCameraOtherView );
 
	mPivotPitch = 0;

}
	
NxCharacterController::~NxCharacterController()
{
	if( mController ) delete mController; 
}

void NxCharacterController::LookAt( const Nx::Vector3 & Target )
{
	Ogre::Vector3 vec1;
	NxVec3ToOgre( vec1, Target );
	NxCamera->lookAt( vec1 );
}


void NxCharacterController::SetCustomProjectionMatrix( bool enable, const Nx::Matrix4 &projectionMatrix  ) {

	Ogre::Matrix4 mat1;
	NxMat4toOgre( mat1, projectionMatrix );
	NxCamera->setCustomProjectionMatrix( enable, mat1 );

}
 
void NxCharacterController::SetCustomViewMatrix( bool enable, const Nx::Matrix4 &viewMatrix  ) {
	Ogre::Matrix4 mat1;
	NxMat4toOgre( mat1, viewMatrix );
	NxCamera->setCustomViewMatrix( enable, mat1 );
}

void NxCharacterController::SetFocalLength( float focalLength ) {
	NxCamera->setFocalLength( focalLength );
}
	 
void NxCharacterController::SetFrustumExtents( float left, float right, float top, float bottom ) {
	NxCamera->setFrustumExtents( left, right, top, bottom  );
}

void NxCharacterController::SetFrustumOffset (float hori, float vert ) {
	NxCamera->setFrustumOffset( hori, vert  );
}


void NxCharacterController::Rotate(const Nx::Quaternion &q) {
	Ogre::Quaternion quat1;
	NxQuatToOgre( quat1, q );
	NxCamera->rotate( quat1 );
}

void NxCharacterController::Roll( const Nx::Degree & rolldegree ) {
	Ogre::Degree deg1;
	NxDegreeToOgre( deg1, rolldegree );
	NxCamera->roll( deg1 );
}

void NxCharacterController::Yaw( const Nx::Degree & yawdegree ) {
	Ogre::Degree deg1;
	NxDegreeToOgre( deg1, yawdegree );
	NxCamera->yaw( deg1 );
}

void NxCharacterController::Pitch( const Nx::Degree & pitchdegree ) {
	Ogre::Degree deg1;
	NxDegreeToOgre( deg1, pitchdegree );
	NxCamera->pitch( deg1 );
}

void NxCharacterController::SetOrientation (const Nx::Quaternion &q){
	Ogre::Quaternion quat1;
	NxQuatToOgre( quat1, q );
	NxCamera->setOrientation( quat1 );
}

void NxCharacterController::SetDirection( const Nx::Vector3 & vec2 ) {
	Ogre::Vector3 vec1;
	NxVec3ToOgre( vec1, vec2 );
	NxCamera->setDirection( vec1 );
}

void NxCharacterController::SetFixedYawAxis( bool useFixed, const Nx::Vector3 & fixedAxis ) {
	Ogre::Vector3 vec1;
	NxVec3ToOgre( vec1, fixedAxis );
	NxCamera->setFixedYawAxis( useFixed, vec1 );
} 		


void NxCharacterController::SetPosition( const Nx::Vector3 & pos ) {
	Ogre::Vector3 vec1;
	NxVec3ToOgre( vec1, pos );
	NxCamera->setPosition(  vec1 );
} 	

const Nx::Vector3 & NxCharacterController::GetPosition() const
{
	Nx::Vector3 Pos(0,0,0);

	if( mNavigationMode == ModeFly ){
		//Pos = NxCamera->getPosition();
		Pos = Nx::Vector3( mController->GetPosition().x, mController->GetPosition().y, mController->GetPosition().z );
	}
	else if( mNavigationMode == ModeWalk ){
		Pos = Nx::Vector3( mController->GetPosition().x, mController->GetPosition().y, mController->GetPosition().z );
	}
	else if( mNavigationMode == ModeCharacter ){
		Pos = Nx::Vector3( mController->GetPosition().x, mController->GetPosition().y, mController->GetPosition().z );
	
	}
	return Pos;
}

void NxCharacterController::SetAspectRatio( float AspectRatio ){
	NxCamera->setAspectRatio( AspectRatio );
}

float NxCharacterController::GetAspectRatio(){
	return NxCamera->getAspectRatio();
}

void NxCharacterController::SetNearPlane( float Distance ){
	NxCamera->setNearClipDistance( Distance );
}

float NxCharacterController::GetNearPlane()
{
	return NxCamera->getNearClipDistance();
}

void NxCharacterController::SetFarPlane( float Distance )
{
	NxCamera->setFarClipDistance( Distance );
}

float NxCharacterController::GetFarPlane()
{
	return NxCamera->getFarClipDistance();
}



void NxCharacterController::SetProjectionType( bool Orthogonal )
{
	NxCamera->setProjectionType( Orthogonal ? PT_ORTHOGRAPHIC : PT_PERSPECTIVE );
}

void NxCharacterController::SetPolygonView( NxPolygonMode mode )
{
	NxCamera->setPolygonMode( (Ogre::PolygonMode)mode ); 
}

NxPolygonMode NxCharacterController::GetPolygonView()
{
	return ( NxPolygonMode ) NxCamera->getPolygonMode();
}

void NxCharacterController::SetFieldOfView( float AngleDegree )
{
	NxCamera->setFOVy( Ogre::Degree( AngleDegree ));
}

float NxCharacterController::GetFieldOfView()
{
	return NxCamera->getFOVy().valueDegrees();
}

void NxCharacterController::SetOrientationSpeed( const Nx::Degree & Rotspeed )
{
	mRotateSpeed = Rotspeed ;
}

const Nx::Degree & NxCharacterController::GetOrientationSpeed()
{
	return mRotateSpeed ;
}

void NxCharacterController::SetNxCameraSpeed( float speed  )
{
	//fps speed
	m_CameraSpeed = speed ;
	// Character Speed
	NxCharacterSpeed = speed;
}

void NxCharacterController::SetCharacterHeight( float Height )
{
	mController->SetHeight( Height  );
}

void NxCharacterController::SetCharacterRadius( float Radius )
{
	mController->SetRadius( Radius );
}
 
//Caspsule
void NxCharacterController::CreateCharacterController( float radius, float height)
{
	LogMsg("Creating a Camera Controller...");
 
	Ogre::Vector3 Position = mNode->_getDerivedPosition();


	mController->CreateCharacterController ( OgreToNxVec3( Position) , radius,  height  );

	SetCollisionDetection( false );
	CameraTranslate( TranslateNone );
	mActive = true;
	LogMsg("Creating a Camera Controller : Done.");
}

void NxCharacterController::DeleteCharacterController(){
	mController->DeleteCharacterController();
}

void NxCharacterController::SetStepOffset( float Offset )
{
	mController->SetStepOffset( Offset );
}

void NxCharacterController::SetWalkMode( WalkMode Mode )
{
	UpdateCharacterExtents( Mode );
	CurrentWalkMode = Mode;
}

NxCharacterController::WalkMode NxCharacterController::GetWalkMode()
{
	return CurrentWalkMode;
}

void NxCharacterController::Update( float deltaTime )
{
 
	if(!mActive) return;
	mDeltaTime = deltaTime;

	if( mNavigationMode == ModeFly )
	{
		ProcessKeyActions( deltaTime );

		Ogre::Vector3 position;
		NxVec3ToOgre( position, m_NewCamPosition );
		mNode->setPosition( position );

		mController->SetPosition( Nx::Vector3( m_NewCamPosition.x, m_NewCamPosition.y + (mController->GetHeight()/2.0f)+0.5f, m_NewCamPosition.z ) );
		UpdateSoundListener();
		//LogMsg("NxCharacterController::Update : mNavigationMode == ModeFly");
		return ;
	}
	else if( mNavigationMode == ModeCharacter && NxCharacterNode )
	{
		NxCharacterNode->setPosition(  NxVec3ToOgre( GetPosition() ) - Ogre::Vector3(0,(mController->GetHeight()/2.0f)+0.5f,0)  ); 
		// place the camera pivot roughly at the character's shoulder
		float CamHeight = 0.5f;
		mCameraPivot->setPosition( NxCharacterNode->getPosition() + ( Ogre::Vector3::UNIT_Y * CamHeight ) );
		// move the camera smoothly to the goal
		Ogre::Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
		mCameraNode->translate( goalOffset * deltaTime * 9.0f );
		// always look at the pivot
		mCameraNode->lookAt( mCameraPivot->_getDerivedPosition(), Node::TS_WORLD );

		mGoalDirection = Nx::Vector3::ZERO;// we will calculate this

		//if (mDisplacement != Vector3::ZERO  )
		//{
			Ogre::Vector3 zAxis = mCameraNode->getOrientation().zAxis();
			Ogre::Vector3 xAxis = mCameraNode->getOrientation().xAxis();
			mGoalDirection += mDisplacement.z * Nx::Vector3(zAxis.x, zAxis.y, zAxis.z); 
			mGoalDirection += mDisplacement.x * Nx::Vector3(xAxis.x, xAxis.y, xAxis.z); 
			mGoalDirection.y = 0;
			mGoalDirection.normalise();

			Ogre::Quaternion toGoal = NxCharacterNode->getOrientation().zAxis().getRotationTo( NxVec3ToOgre( mGoalDirection ) );

			// calculate how much the character has to turn to face goal direction
			Real yawToGoal = toGoal.getYaw().valueDegrees()  ;
			// this is how much the character CAN turn this frame
			Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * 1000.0f;//1000.0f;//TURN_SPEED;
			// reduce "turnability" if we're in midair
			//if (mBaseAnimID == ANIM_JUMP_LOOP) yawAtSpeed *= 0.2f;

			// turn as much as we can, but not more than we need to
			if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
			else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);
			
			// set characters yaw
			NxCharacterNode->yaw(Ogre::Degree(yawToGoal));
			// set fps camera orientation depending on charatcers orientation
			NxCamera->setOrientation( mCameraNode->getOrientation() );
			
			// move in current body direction (not the goal direction)
			// NxCharacterNode->translate(0, 0, deltaTime * RUN_SPEED * 1.0f, Node::TS_LOCAL);
		 //}
	}

	Nx::Vector3 disp = Nx::Vector3( 0, -9.81f, 0 );
	Nx::Vector3 PxCharacterVecTmp = mDisplacement ;

	//error here
	Nx::Vector3 horizontalDisp = ( mNavigationMode == ModeCharacter ) ? mGoalDirection : PxCharacterVecTmp ;

	horizontalDisp.y = 0.0f;
	horizontalDisp.normalise();
	disp += horizontalDisp * NxCharacterSpeed;
	disp *= deltaTime;

	float height = GetHeight( deltaTime ); // compute height(Y) in jumping
	if (height != 0.0f) disp.y += height;

	unsigned int collisionFlags;
	Move( disp, collisionFlags );
	if( collisionFlags & COLLISION_DOWN_FLAG ) StopJump();
	if( collisionFlags & COLLISION_UP_FLAG ) LogMsg("Collision Ceiling");
	if( collisionFlags & COLLISION_SIDES_FLAG ) LogMsg("Collision Walls");	

	mNode->setPosition( NxVec3ToOgre(  GetPosition() ) - Ogre::Vector3(0,( mController->GetHeight()/2.0f)+0.5,0) );

	UpdateSoundListener();

	

}

void NxCharacterController::Move( const Nx::Vector3  &disp, unsigned int &flag )
{
	const unsigned int COLLIDABLE_MASK = (GROUP_COLLIDABLE_NON_PUSHABLE << 1) | (GROUP_COLLIDABLE_PUSHABLE << 1 )  ;
	mController->Move(disp,COLLIDABLE_MASK,0.000001f,flag,1.0f);
}

void NxCharacterController::OnMouseMoved( const OIS::MouseEvent &evt ) 
{
	if(!mActive) return;

	if( mNavigationMode == ModeFly ) {
		if( NxInputManager::getSingleton().IsMouseButtonDown(   NxButtonRight ) ) {
			CameraRotate( evt.state.X.rel, evt.state.Y.rel );
		}
	}
	else if( mNavigationMode == ModeWalk ) {
		if( NxInputManager::getSingleton().IsMouseButtonDown(   NxButtonRight ) ) {
			CameraRotate( evt.state.X.rel, evt.state.Y.rel );
		}		
		CameraTranslate( GetTranslateMode() );
	}
	else if( mNavigationMode == ModeCharacter ) {
		if( mCameraPivot && mCameraGoal ) {
			Real deltaYaw =  -0.1f * evt.state.X.rel;
			Real deltaPitch = -0.05f * evt.state.Y.rel;
			Real deltaZoom = -0.0005f * evt.state.Z.rel;

			mCameraPivot->yaw(Ogre::Degree(deltaYaw), Node::TS_WORLD);

			// bound the pitch
			Ogre::Real MinPitch = -1;
			Ogre::Real MaxPitch = -60;

			if (!(mPivotPitch + deltaPitch > MinPitch && deltaPitch > 0) && !(mPivotPitch + deltaPitch <  MaxPitch && deltaPitch < 0)){
				mCameraPivot->pitch(Ogre::Degree(deltaPitch), Node::TS_LOCAL);
				mPivotPitch += deltaPitch;
			}

			//Zoom
			Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
			Real distChange = deltaZoom * dist;

			// bound the zoom
			Ogre::Real MinZoom = 2;
			Ogre::Real MaxZoom = 25;

			if (!(dist + distChange < MinZoom && distChange < 0) && !(dist + distChange > MaxZoom && distChange > 0)){
				mCameraGoal->translate(0, 0, distChange, Node::TS_LOCAL);
			}
		}
	}
	
}

void NxCharacterController::OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id )
{

}

void NxCharacterController::OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id )
{

}

void NxCharacterController::OnKeyPressed( const OIS::KeyEvent &evt )
{
	if(!mActive) return;
	if( evt.key ==  OIS::KC_SPACE ){ StartJump( 15.0f ); }
	ProcessKeyActions( mDeltaTime );
}

void NxCharacterController::OnKeyReleased( const OIS::KeyEvent &e )
{
	if(!mActive) return;
	CameraTranslate( TranslateNone );
	ProcessKeyActions( mDeltaTime );
}

NxCharacterController::TranslateMode NxCharacterController::GetTranslateMode()
{
	return mTranslateMode;
}

void NxCharacterController::CameraRotate( float Yaw, float Pitch  ){

	mRotScale = mRotateSpeed * mDeltaTime;
	mRotX = Nx::Degree( -Yaw * mRotScale);
	mRotY = Nx::Degree( -Pitch * mRotScale);

	Ogre::Degree mRotX2;
	NxDegreeToOgre(mRotX2,mRotX);
	NxCamera->yaw(mRotX2 );

	Ogre::Degree mRotY2;
	NxDegreeToOgre(mRotY2,mRotY);
	NxCamera->pitch(mRotY2);

 
}

void NxCharacterController::CameraTranslate( NxCharacterController::TranslateMode Mode )
{
	mTranslateMode = Mode;
	double timeMod = mDeltaTime * m_CameraSpeed;

	Ogre::Vector3 Direction = NxCamera->getDirection();
	Nx::Vector3 DirectionNx = Nx::Vector3(Direction.x,Direction.y,Direction.z);
	Ogre::Vector3 Position = NxCamera->getPosition();
	Ogre::Vector3 Right = NxCamera->getRight();
	Nx::Vector3 RightNx = Nx::Vector3(Right.x,Right.y,Right.z);

	if( m_NewCamPosition == Nx::Vector3::ZERO ) m_NewCamPosition = Nx::Vector3(Position.x,Position.y, Position.z);
	switch( Mode )
	{
		case TranslateForward : 
			m_NewCamPosition += DirectionNx * timeMod ;
			mDisplacement = mNavigationMode == ModeCharacter ? -Nx::Vector3(0,0,1) * timeMod : DirectionNx * timeMod;
			break;
		case TranslateBackWard :
			m_NewCamPosition -= DirectionNx * timeMod;
			mDisplacement = mNavigationMode == ModeCharacter ? Nx::Vector3(0,0,1) * timeMod : -DirectionNx * timeMod;
			break;
		case TranslateLeft :
			m_NewCamPosition -= RightNx * timeMod;
			mDisplacement =  mNavigationMode == ModeCharacter ? -Nx::Vector3(1,0,0) * timeMod : -RightNx * timeMod;
			break;	
		case TranslateRight : 
			m_NewCamPosition += RightNx* timeMod;
			mDisplacement = ( mNavigationMode == ModeCharacter ) ?  Nx::Vector3(1,0,0)* timeMod : RightNx * timeMod;
			break;
		case TranslateUp :
			m_NewCamPosition += Nx::Vector3(0,1,0) * timeMod;
			break;
		case TranslateDown : 
			m_NewCamPosition -= Nx::Vector3(0,1,0) * timeMod;
			break;
		case TranslateNone:
			mDisplacement = Nx::Vector3(0,0,0);
			break;
		default:
			break;
	}
}

void NxCharacterController::ProcessKeyActions( float deltaTime )
{
	mDisplacement = Nx::Vector3::ZERO;
/*
	if( NxCamera ){
		NxInputManager * InputMgr = NxInputManager::getSingletonPtr();
		if (InputMgr->IsKeyDown( OIS::KeyCode::KC_UP ) || InputMgr->IsKeyDown(OIS::KeyCode::KC_W) || InputMgr->IsKeyDown(OIS::KeyCode::KC_Z) ){
			CameraTranslate( TranslateForward );
		}
		if (InputMgr->IsKeyDown(OIS::KeyCode::KC_DOWN) || InputMgr->IsKeyDown(OIS::KeyCode::KC_S)){
			CameraTranslate( TranslateBackWard );
		}
		if (InputMgr->IsKeyDown(OIS::KeyCode::KC_RIGHT) || InputMgr->IsKeyDown(OIS::KeyCode::KC_D)){
			CameraTranslate( TranslateRight );
		}
		if (InputMgr->IsKeyDown(OIS::KeyCode::KC_LEFT) || InputMgr->IsKeyDown(OIS::KeyCode::KC_A)){
			CameraTranslate( TranslateLeft );
		}
		if (InputMgr->IsKeyDown(OIS::KeyCode::KC_PGUP) || InputMgr->IsKeyDown(OIS::KeyCode::KC_E)){
			CameraTranslate( TranslateUp );
		}
		if (InputMgr->IsKeyDown(OIS::KeyCode::KC_PGDOWN) || InputMgr->IsKeyDown(OIS::KeyCode::KC_Q)){
			CameraTranslate( TranslateDown );
		}
	}
	*/
}

NavigationMode NxCharacterController::GetNavigationMode()
{
	return mNavigationMode;
}

void NxCharacterController::SetActiveCamera( const std::string & CameraName )
{
	Nx3D_Scene * Scene = &mNxNodeParent->GetNxScene()   ;//NxScene3DPhysicsManager::getSingleton().GetNx3DSceneFromPhysxScene( CurrentNxScene );
	Scene->GetNxViewport()->setCamera( Scene->GetNxSceneManager()->getCamera(CameraName) );
}

void NxCharacterController::SetNavigationMode( NavigationMode Mode )
{
	mNavigationMode = Mode;
	bool Collision;

	if( mNavigationMode == ModeCharacter ){
		SetActiveCamera( NxCameraOtherView->getName() );
		Collision = true;
	}
	else if( mNavigationMode == ModeWalk ){
		SetActiveCamera( NxCamera->getName() );
		Collision = true;
	}
	else if( mNavigationMode == ModeFly ){
		m_NewCamPosition =  Nx::Vector3( mNode->getPosition().x , mNode->getPosition().y ,mNode->getPosition().z  );
		SetActiveCamera( NxCamera->getName() );
		Collision = false;
	}

	SetCollisionDetection( Collision );
}

void NxCharacterController::SetActive( bool Active )
{
	mActive = Active;
}

bool NxCharacterController::IsActive()
{
	return mActive;
}

void NxCharacterController::SetNxCharacter( NxScene3DObjectCharacter * Character )
{
	NxCharacterNode = Character->GetSceneNode();	 
}
	 
Ogre::Camera * NxCharacterController::GetCameraFps()
{
	return NxCamera ;
}	

const std::string & NxCharacterController::GetCameraFpsName() const {

	return NxCamera->getName().c_str();
}



Ogre::Camera * NxCharacterController::GetCameraCharacter()
{
	return NxCameraOtherView;
}



void NxCharacterController::SetCollisionDetection( bool Collision )
{
	mController->SetCollision( Collision );
}

/*
NxActor * NxCharacterController::GetCharacterActor()
{
	return mController->getActor();
}

NxController * NxCharacterController::GetCharacterController()
{
return mController;
} 

*/

void NxCharacterController::UpdateSoundListener()
{
	Ogre::Vector3 curpos = NxCamera->getDerivedPosition();
	NxSoundManager::getSingleton().SetListenerPosition( curpos.x, curpos.y, curpos.z );
	// Orientation of the listener : look at then look up
	Vector3 axis = Vector3::ZERO;
	axis.x = NxCamera->getDerivedOrientation().getYaw().valueRadians();
	axis.y = NxCamera->getDerivedOrientation().getPitch().valueRadians();
	axis.z = NxCamera->getDerivedOrientation().getRoll().valueRadians();
	NxSoundManager::getSingleton().SetListenerOrientation( axis.x, axis.y, axis.z );

}

float NxCharacterController::GetHeight( float elapsedTime )
{
	if( !PxJump )  return 0.0f; 
	float Vt = PxV0 + G*PxJumpTime; // Vt = Vo + GT
	PxJumpTime += elapsedTime;
	return Vt*elapsedTime + 1/2*G*elapsedTime*elapsedTime; // S = VtT + 1/2GT^2
}

void NxCharacterController::StartJump( float v0)
{
	if (PxJump) return;
	PxJumpTime = 0.0f;
	PxV0 = v0;
	PxJump = true;
}

void NxCharacterController::StopJump()
{
	PxJump = false;
}

void NxCharacterController::UpdateCharacterExtents( WalkMode Mode )
{
	if( !mController ) return ;

	if( Mode == ModeStandUp ){
		NxCharacterSpeed = 7;
	}
	else if( Mode == ModeCrouchDown ){
		NxCharacterSpeed = 1;
	}

	mController->SetExtends( GetPosition() );
 
}



}//namespace