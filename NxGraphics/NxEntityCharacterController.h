/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
									/_/                             
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#ifndef __NXENTITYCHARACTERCONTROLLER_H__
#define __NXENTITYCHARACTERCONTROLLER_H__

#include "NxPrerequisites.h"
#include "NxEntity.h"

namespace Nx {

//!  A class to manage camera controllers in the 3d scene
class NxScene3DObjectCameraCallback;
class NxGraphics_Export NxCharacterController : public NxEntity 
{
public :
	//! NxScene3DObjectCamera Class Constructor.
	NxCharacterController( NxNode * ParentNode, const std::string & CameraName );
	//! NxScene3DObjectCamera Class Destructor.
	~NxCharacterController();
	//! Create a Capsule Based Character Controller
	void CreateCharacterController( float radius, float height);
	//! Deletes the Character controller
	void DeleteCharacterController();
	//! Set Character height
	void SetCharacterHeight( float Height );
	//! Set Character Radius 
	void SetCharacterRadius( float Radius );
	//! Set Step Offset.
	void SetStepOffset( float Offset );
	//! Set Controller Position
	void SetPosition( const Nx::Vector3 & Pos );
	//! Get Controller Position
	const Nx::Vector3 & GetPosition() const;


	void SetCustomProjectionMatrix( bool enable, const Nx::Matrix4 &projectionMatrix = Nx::Matrix4::IDENTITY );
	void SetCustomViewMatrix( bool enable, const Nx::Matrix4 &viewMatrix = Nx::Matrix4::IDENTITY );
	void Rotate(const Nx::Quaternion &q);
	void Roll( const Nx::Degree & rolldegree );
	void Yaw( const Nx::Degree & yawdegree );
	void Pitch( const Nx::Degree & pitchdegree );
	void SetOrientation( const Nx::Quaternion &q);
	void SetDirection( const Nx::Vector3 & vec	);
	void SetFixedYawAxis( bool useFixed, const Nx::Vector3 & fixedAxis = Nx::Vector3::UNIT_Y );
	void SetFocalLength( float focalLength );
	void SetFrustumExtents( float left, float right, float top, float bottom );
	void SetFrustumOffset (float hori, float vert );
	 


	//! Sets aspect ratio for active camera.
    /*!
	  \param AspectRatio the aspect ration to set .
      \sa GetNxCameraOrientation
    */
	void SetAspectRatio( float AspectRatio );
	//! Gets aspect ratio value for active camera.
	float GetAspectRatio();
	//! Sets near plane for active camera.
    /*!
	  \param NearDistance the near distance to set .
      \sa GetNxCameraNearPlane
    */
	void SetNearPlane( float NearDistance );
	//! Gets near plane value for active camera.
	float GetNearPlane();
	//! Sets far plane for active camera.
    /*!
	  \param FarDistance the far distance to set .
      \sa GetNxCameraFarPlane
    */
	void SetFarPlane( float FarDistance );
	//! Gets far plane value for active camera.
	float GetFarPlane();
	//! Look At
	void LookAt( const Nx::Vector3 & Target );
	//! Set the camera to orthogonal view
	void SetProjectionType( bool Orthogonal );
	//! Set the polygon view for active camera
    /*!
	  \param PolyMode the view mode to set
      \sa GetNxCameraRotationSpeed
    */
	void SetPolygonView( NxPolygonMode PolyMode );
	//! get the polygon view for active camera
	NxPolygonMode GetPolygonView();
	//! Set Field Of View, In Degrees.
	void SetFieldOfView( float AngleDegree );
	//! get Field Of View Angle, In Degrees.
	float GetFieldOfView();
	//! Updates the Character controller
	void Update( float time );
	 
	//! Gets the Character Controller Physx Actor.
	//NxActor * GetCharacterActor();
	//! Gets the Character Controller.
	//NxController * GetCharacterController(); 
	 
	//! Gets the Camera Associated with the Character Controller Fps
	Ogre::Camera * GetCameraFps();

	const std::string & GetCameraFpsName() const ;

	//! Gets the Camera Associated with the Character Controller External View
	Ogre::Camera * GetCameraCharacter();
	//! Sets the Character Controller Collision
	void SetCollisionDetection( bool Collision );
	NavigationMode GetNavigationMode();
	void SetNavigationMode( NavigationMode Mode);
	
	void SetOrientationSpeed( const Nx::Degree & Rotspeed );
	const Nx::Degree &  GetOrientationSpeed();
	void SetNxCameraSpeed( float TransSpeed );
	void ProcessKeyActions( float Delta );

	void SetNxCharacter( NxScene3DObjectCharacter * Character );
	//! set the bundle set active.
	void SetActive( bool Active );
	//! Is Bundle set active.
	bool IsActive();
	
	//virtuals
	void OnMouseMoved( const OIS::MouseEvent &e );
	void OnMousePressed( const OIS::MouseEvent &e, NxMouseButtonID id );
	void OnMouseReleased( const OIS::MouseEvent &e, NxMouseButtonID id );

	void OnKeyPressed( const OIS::KeyEvent &e );
	void OnKeyReleased( const OIS::KeyEvent &e );

	void UpdateSoundListener();

	enum WalkMode
	{ 
		ModeStandUp,
		ModeCrouchDown
	};
	void SetWalkMode( WalkMode Mode ); // StandUp, CrouchDown, RunForward , WalkForward, 
	WalkMode GetWalkMode(); 

	void StartJump( float Amplitude );
	void StopJump();

	enum TranslateMode
	{
		TranslateForward,
		TranslateBackWard,
		TranslateLeft,
		TranslateRight,
		TranslateUp,
		TranslateDown,
		TranslateNone

	};

	void CameraRotate( float Yaw, float Pitch );
	void CameraTranslate( NxCharacterController::TranslateMode Mode );
	TranslateMode GetTranslateMode();
	TranslateMode mTranslateMode;

	

private:
	NxPhysicsController * mController;
	Ogre::Camera * NxCameraOtherView;
	Ogre::Camera * NxCamera ;
	Ogre::SceneNode* mCameraPivot;
	Ogre::SceneNode* mCameraGoal;
	Ogre::SceneNode* mCameraNode;
	float mPivotPitch;   
	Nx::Vector3 mGoalDirection;   
	Ogre::SceneNode * NxCharacterNode;

	Nx::Vector3 mDisplacement; 

	Nx::Vector3 m_NewCamPosition;

	float m_CameraSpeed;
	float mDeltaTime;
	Nx::Radian mRotX, mRotY;
	Nx::Vector3 mTranslateVector;
	float mMoveSpeed;
	Nx::Degree mRotateSpeed;
	float mMoveScale;
	Nx::Degree mRotScale;

	//! Moves the Character controller
 

	void Move( const Nx::Vector3  &disp, unsigned int &flag );
           
	void SetActiveCamera( const std::string & CameraName );

	bool mActive;


	NavigationMode mNavigationMode;

	float WalkStandUpHeight;
	float WalkCrouchHeight;
	
	

	bool PxJump;  
	float PxV0;
	float PxJumpTime;
	float G;  
	float NxCharacterSpeed; 
	float GetHeight( float elapsedTime );

	WalkMode CurrentWalkMode ;
	void UpdateCharacterExtents( WalkMode Mode );

};

}
#endif