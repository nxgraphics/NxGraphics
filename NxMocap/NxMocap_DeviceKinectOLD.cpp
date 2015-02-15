#include "NxMocapPreCompiled.h"

#pragma once

#include <math.h>
#include <XnCppWrapper.h>

#include <XnVDeviceGenerator.h>
#include <XnVNite.h>
#include <XnTypes.h>
#include <XnV3DVector.h>

#include <XnModuleCppInterface.h>
#include <XnEvent.h>
#include <XnPropNames.h>

#include "NxMocap_vector3.h"
using namespace NxMocap_Namespace;



static XnSkeletonJoint GetOpenNiTypeFromNxSkeletonType( NxSkeletonJoint Type ){
	switch( Type ){
		case NX_SKEL_HEAD :
			return XN_SKEL_HEAD;
			break;
		case NX_SKEL_NECK :
			return XN_SKEL_NECK;
			break;
		case NX_SKEL_SPINE3	:
			return XN_SKEL_TORSO;
			break;
		case NX_SKEL_SPINE2 :
			return XN_SKEL_TORSO;
			break;
		case NX_SKEL_SPINE1	:
			return XN_SKEL_TORSO;
			break;
		case NX_SKEL_SPINE :
			return XN_SKEL_WAIST;
			break;
		case NX_SKEL_PELVIS :
			return XN_SKEL_WAIST;
			break;
		case NX_SKEL_ROOT :
			return XN_SKEL_WAIST;
			break;
		case NX_SKEL_L_THIGH :
			return XN_SKEL_LEFT_HIP;
			break;
		case NX_SKEL_R_THIGH :
			return XN_SKEL_RIGHT_HIP;
			break;
		case NX_SKEL_L_CALF :
			return XN_SKEL_LEFT_KNEE;
			break;
		case NX_SKEL_R_CALF :
			return XN_SKEL_RIGHT_KNEE;
			break;
		case NX_SKEL_L_FOOT :
			return XN_SKEL_LEFT_FOOT;
			break;
		case NX_SKEL_R_FOOT :
			return XN_SKEL_RIGHT_FOOT;
			break;
		case NX_SKEL_L_CLAVICLE :
			return XN_SKEL_LEFT_COLLAR;
			break;
		case NX_SKEL_R_CLAVICLE :
			return XN_SKEL_RIGHT_COLLAR;
			break;
		case NX_SKEL_L_UPPERARM	:
			return XN_SKEL_LEFT_SHOULDER;
			break;
		case NX_SKEL_R_UPPERARM :
			return XN_SKEL_RIGHT_SHOULDER;
			break;
		case NX_SKEL_L_FOREARM :
			return XN_SKEL_LEFT_ELBOW;
			break;
		case NX_SKEL_R_FOREARM :
			return XN_SKEL_RIGHT_ELBOW;
			break;
		case NX_SKEL_L_HAND :
			return XN_SKEL_LEFT_HAND;
			break;
		case NX_SKEL_R_HAND :
			return XN_SKEL_RIGHT_HAND;
			break;

		default:
			break;
	}
}

enum PoseDetectionResult
{
	NOT_IN_POSE,
	IN_POSE_FOR_LITTLE_TIME,
	IN_POSE_FOR_LONG_TIME,
	NOT_ENOUGH_CONFIDENCE,
};

static double GetCurrentTimeInSeconds()
{
    LONGLONG time,freq;
    QueryPerformanceCounter((LARGE_INTEGER *)&time);
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq);
    double d_time = ((double)time) / ((double)freq);
	return d_time;
}

class PoseDetectorBase
{
protected:
	double m_beginTimeOfPose;
	double m_durationOfPoseForDetection;
	double m_detectionPercent;

public:

	double GetDetectionPercent() const{
		return m_detectionPercent;
	}

	PoseDetectorBase(double durationForDetection = 1){
		m_durationOfPoseForDetection = durationForDetection;
		Reset();
	}

	virtual void SetDurationForDetection(double time){
		m_durationOfPoseForDetection = time;
		Reset();
	}

	virtual PoseDetectionResult checkPose() = 0;
	
	virtual void Reset(){
		m_beginTimeOfPose = -1;
		m_detectionPercent = 0;
	}
	virtual PoseDetectionResult checkPoseDuration()
	{
		double curTime = GetCurrentTimeInSeconds();
		switch(checkPose())
		{
		case IN_POSE_FOR_LITTLE_TIME: //falling through
		case IN_POSE_FOR_LONG_TIME:
			if(m_beginTimeOfPose != -1)
			{
				if(m_durationOfPoseForDetection!=0){
					m_detectionPercent = (curTime - m_beginTimeOfPose) / m_durationOfPoseForDetection;
				} 
				else{
					m_detectionPercent = 1;
				}

				if( m_detectionPercent >= 1){
					m_detectionPercent = 1;
					return IN_POSE_FOR_LONG_TIME;
				} 
			} 
			else{
				m_beginTimeOfPose = curTime;
			}
			return IN_POSE_FOR_LITTLE_TIME;
		case NOT_ENOUGH_CONFIDENCE:
			if(m_beginTimeOfPose != -1)
			{
				if((curTime - m_beginTimeOfPose) > m_durationOfPoseForDetection){
					//restart waiting
					Reset();
					return IN_POSE_FOR_LITTLE_TIME;
				}
				return IN_POSE_FOR_LITTLE_TIME;
			}
			break;
		case NOT_IN_POSE:
			Reset();
			break;
		}
		return NOT_IN_POSE;
	}
};


class EndPoseDetector: public PoseDetectorBase
{
public:
	XnSkeletonJointTransformation m_prevLeftHand;
	XnSkeletonJointTransformation m_prevRightHand;
	xn::UserGenerator m_userGenerator;
	XnUserID m_nUserId;

	EndPoseDetector( xn::UserGenerator ug, double duration ) : PoseDetectorBase( duration ){
		m_userGenerator = ug;
	}

	void Reset(){
		PoseDetectorBase::Reset();
		m_prevLeftHand.position.fConfidence = 0;
		m_prevRightHand.position.fConfidence = 0;
	}

	void SetUserId(XnUserID nUserId){
		m_nUserId = nUserId;
	}

	PoseDetectionResult checkPose(){	
		XnSkeletonJointTransformation leftHand;
		XnSkeletonJointTransformation rightHand;
		xn::SkeletonCapability skeletonCap = m_userGenerator.GetSkeletonCap();
		
		if (!skeletonCap.IsTracking(m_nUserId)){
			return NOT_IN_POSE;
		}

		skeletonCap.GetSkeletonJoint(m_nUserId, XnSkeletonJoint::XN_SKEL_LEFT_HAND, leftHand);
		skeletonCap.GetSkeletonJoint(m_nUserId, XnSkeletonJoint::XN_SKEL_RIGHT_HAND, rightHand);

		bool bHaveLeftHand = leftHand.position.fConfidence  >= 0.5;
		bool bHaveRightHand = rightHand.position.fConfidence >= 0.5;
		if(!bHaveLeftHand && !bHaveRightHand ){
			return NOT_IN_POSE;
		}

		if(bHaveLeftHand) m_prevLeftHand  = leftHand;
		if(bHaveRightHand) m_prevRightHand = rightHand;

		//check for X (left hand is "righter" than right (more than 10 cm)
		float xDist = leftHand.position.position.X - rightHand.position.position.X ;

		if(xDist < 60 ) return NOT_IN_POSE;

		//check hands to be at same height
		float yDist = fabs(leftHand.position.position.Y - rightHand.position.position.Y);

		if(yDist > 300 ) return NOT_IN_POSE;

		//printf("in end pose!");
		return IN_POSE_FOR_LITTLE_TIME;
	}
};

class StartPoseDetector: public PoseDetectorBase
{
public:
	
	xn::UserGenerator* m_pUserGenerator;
	bool _isInStartPose;

	StartPoseDetector(double duration):PoseDetectorBase(duration){
		_isInStartPose = false;
	}

	void Reset(){

		PoseDetectorBase::Reset();
		_isInStartPose = false;
	}

	PoseDetectionResult checkPose(){	
		if(IsInStartPose()){

			return IN_POSE_FOR_LITTLE_TIME;
		}
		return NOT_IN_POSE;
	}

	bool IsInStartPose() {
		return _isInStartPose;
	}

	void SetStartPoseState(bool isInStartPose){
		_isInStartPose = isInStartPose;
	}
};




using namespace NxMocap_Namespace;

class KinectPlayer
{

private:


		//NxVector3 m_origTorsoPos;

        xn::Context m_Context;
		xn::DepthGenerator m_DepthGenerator;
		xn::ImageGenerator m_ImageGenerator;
		xn::IRGenerator    m_IrGenerator;
        xn::UserGenerator m_UserGenerator;
        xn::SceneAnalyzer m_SceneAnalyzer;

        XnVSessionManager* m_pSessionManager;
        XnVFlowRouter* m_pQuitFlow;
        XnVSelectableSlider1D* m_pQuitSSlider;

       // XnUserID m_candidateID;

        XnCallbackHandle m_hPoseCallbacks;
        XnCallbackHandle m_hUserCallbacks;
        XnCallbackHandle m_hCalibrationCallbacks;

        StartPoseDetector * m_pStartPoseDetector;
        EndPoseDetector * m_pEndPoseDetector;

		bool mNewDepthFrame;
		XN_THREAD_HANDLE m_hScheduler;

		XN_CRITICAL_SECTION_HANDLE pCriticalSectionHandle;

		//! user List
		std::vector<NxMocapUser*> mUsers;

public:

		bool PrimeSensorInitialized  ;

		static XN_THREAD_PROC SchedulerThread( void* pCookie )
		{
			KinectPlayer* pThis = (KinectPlayer*)pCookie;

			while (true)
			{
				int FPS = 25; 
				// wait 33 ms (to produce 30 FPS)

				pThis->mNewDepthFrame = true;

				 xnOSSleep( 1000000 / FPS / 1000 );

				//pThis->m_Context.WaitNoneUpdateAll();//(pThis->m_DepthGenerator);////
				//xn::SceneMetaData sceneMD;
				//pThis->m_UserGenerator.GetUserPixels(0, sceneMD);

				// pThis->m_Context.WaitAnyUpdateAll();
				 //pThis->m_DepthGenerator.WaitAndUpdateData();
				//pThis->mNewDepthFrame = false;
				
			}

			XN_THREAD_PROC_RETURN(0);
		}



		KinectPlayer() : mNewDepthFrame(false), PrimeSensorInitialized( false )
		{
		
		}

		void Disconnect()
		{
			m_pSessionManager->EndSession();
			m_Context.Shutdown();
		}

		XnStatus initPrimeSensor( bool mInfraRedImage )
		{
			m_hUserCallbacks = NULL;
			m_hPoseCallbacks = NULL;
			m_hCalibrationCallbacks = NULL;

			// Init OpenNI from XML
			XnStatus rc = XN_STATUS_OK;    

			 
			Log("OpenNi Initializing context...");
			rc = m_Context.Init();
			if( rc != XN_STATUS_OK ){ Log("OpenNi Initialization Failed !"); }
			Log("OpenNi Initializing context: Done");
			 
			
		 
			//Log("OpenNi Initializing context...");
			//rc = m_Context.InitFromXmlFile("d:/openni.xml");
			//if( rc != XN_STATUS_OK ){ Log("OpenNi Initialization Failed !"); return rc; }
			//Log("OpenNi Initializing context: Done");
			 


			// Make sure we have all OpenNI nodes we will be needing for this sample
			xn::NodeInfoList nodes;

			// Init NITE Controls (UI stuff)
			m_pSessionManager = new XnVSessionManager;
			rc = m_pSessionManager->Initialize(&m_Context, "Click", "RaiseHand");
			assert(rc == XN_STATUS_OK);
			m_pSessionManager->SetQuickRefocusTimeout(0);
			// set image mirror
			m_Context.SetGlobalMirror(true);

			// Create Depth
			Log("Creating Depth Generator");
			rc = m_DepthGenerator.Create( m_Context );
			assert(rc == XN_STATUS_OK);


			
			XnMapOutputMode mapMode;
			mapMode.nXRes = XN_VGA_X_RES;
			mapMode.nYRes = XN_VGA_Y_RES;
			mapMode.nFPS = 30;

			Log("Creating Depth Generator SetMapOutputMode");
			rc = m_DepthGenerator.SetMapOutputMode(mapMode); 
			if (rc != XN_STATUS_OK){ Log("m_DepthGenerator.SetMapOutputMode Failed !"); return rc; }
			Log("m_DepthGenerator.SetMapOutputMode Succeed!");
			
 
			// set raw output ( set off to match colours)
			//m_DepthGenerator.SetIntProperty( "OutputFormat", 0);
		 


			if( !mInfraRedImage ) // we can use only Image or IR, but not both
			{
				m_ImageGenerator.Create(  m_Context );
				XnMapOutputMode ColourmapMode;
				ColourmapMode.nXRes = XN_VGA_X_RES;
				ColourmapMode.nYRes = XN_VGA_Y_RES;
				ColourmapMode.nFPS = 30;
				rc = m_ImageGenerator.SetMapOutputMode(ColourmapMode); 
				if (rc != XN_STATUS_OK){ Log("m_ImageGenerator.SetMapOutputMode Failed !"); return rc;}

				//m_ImageGenerator.SetIntProperty( "OutputFormat", 1); 
				m_ImageGenerator.SetPixelFormat(XN_PIXEL_FORMAT_RGB24);
				int BitsperPixel = xnGetBytesPerPixelForPixelFormat( m_ImageGenerator.GetPixelFormat() );
				Log("ImageGenerator Pixel Format : " + NxMocapUtils::ToString(  m_ImageGenerator.GetPixelFormat() ));
				Log("ImageGenerator Bits Per pixel : " + NxMocapUtils::ToString(  BitsperPixel ));

				

			}else // infrared
			{
				m_IrGenerator.Create(  m_Context );
				XnMapOutputMode IrmapMode;
				IrmapMode.nXRes = XN_VGA_X_RES;
				IrmapMode.nYRes = XN_VGA_Y_RES;
				IrmapMode.nFPS = 30;
				rc = m_IrGenerator.SetMapOutputMode(IrmapMode); 
				if (rc != XN_STATUS_OK){ Log("m_IrGenerator.SetMapOutputMode Failed !"); return rc; }
				m_IrGenerator.SetIntProperty( "OutputFormat", 0); 
				Log("IrGenerator Bits Per pixel : " + NxMocapUtils::ToString(  m_IrGenerator.GetBytesPerPixel() ));

 
			}

/*
case XN_PIXEL_FORMAT_RGB24:
return sizeof(XnRGB24Pixel);
case XN_PIXEL_FORMAT_YUV422:
return sizeof(XnYUV422DoublePixel)/2;
case XN_PIXEL_FORMAT_GRAYSCALE_8_BIT:
return sizeof(XnGrayscale8Pixel);
case XN_PIXEL_FORMAT_GRAYSCALE_16_BIT:
return sizeof(XnGrayscale16Pixel);
case XN_PIXEL_FORMAT_MJPEG:
*/


  
			if( !mInfraRedImage ) {
				// register the depth image with the rgb image
				if(m_DepthGenerator.IsCapabilitySupported(XN_CAPABILITY_ALTERNATIVE_VIEW_POINT)){
					rc = m_DepthGenerator.GetAlternativeViewPointCap().SetViewPoint(m_ImageGenerator);
					if( rc != XN_STATUS_OK ){ Log("Failed to set View Point Cap"); return rc; }
					Log("Depth & Image Generator Aligned.");
				}
			}


			/*
			// Frame Sync
			if (m_DepthGenerator.IsCapabilitySupported(XN_CAPABILITY_FRAME_SYNC))
			{
				if (m_DepthGenerator.GetFrameSyncCap().CanFrameSyncWith(m_ImageGenerator))
				{
					rc = m_DepthGenerator.GetFrameSyncCap().FrameSyncWith(m_ImageGenerator);
					if( rc != XN_STATUS_OK ){
						Log("Failed to set FrameSyncWith");
						return rc;
					}
				}
			}
			*/

	 

			m_UserGenerator.Create(  m_Context );

			// Init OpenNI nodes
			m_UserGenerator.RegisterUserCallbacks( KinectPlayer::NewUser, KinectPlayer::LostUser, this, m_hUserCallbacks);
			m_UserGenerator.GetPoseDetectionCap().RegisterToPoseCallbacks( KinectPlayer::PoseDetected, KinectPlayer::PoseLost, this, m_hPoseCallbacks);

			// Skeleton stuff
			double m_SmoothingFactor = 0.6;
			double m_SmoothingDelta = 0;
			xn::SkeletonCapability skel = m_UserGenerator.GetSkeletonCap();
			skel.SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
			skel.SetSmoothing(m_SmoothingFactor);
			skel.RegisterCalibrationCallbacks( KinectPlayer::CalibrationStart, KinectPlayer::CalibrationEnd, this, m_hCalibrationCallbacks);

			// Make sure OpenNI nodes start generating
			rc = m_Context.StartGeneratingAll();
			assert(rc == XN_STATUS_OK);

			//m_candidateID = 0;
			m_pStartPoseDetector = new StartPoseDetector(3.0);
			m_pEndPoseDetector = new EndPoseDetector(m_UserGenerator, 2.0);

			//m_pEndPoseDetector->SetUserId(m_candidateID);

			rc = xnOSCreateThread(SchedulerThread, this, &m_hScheduler);
			xnOSCreateCriticalSection( &pCriticalSectionHandle);
			return XN_STATUS_OK;
		}

		/*
        //UPDATE USER ID
        void setUserID(XnUserID nUserId)
        {
                m_pEndPoseDetector->SetUserId(nUserId);
                //FrameListener->setUserID(nUserId);
				CurrentUserId = nUserId;
        }
		*/


		bool UserIdExists( unsigned int nUserId )
		{
			std::vector<NxMocapUser *>::iterator it;
			for(it= mUsers.end(); it!= mUsers.begin(); ){
				--it;
				if( (*it)->GetUserId() == nUserId ) return true;
			}
			return false;
		
		}

		void DeleteUser( unsigned int nUserId )
		{
			std::vector<NxMocapUser *>::iterator it;
			for(it=mUsers.end(); it!=mUsers.begin(); ){
				--it;
				if( (*it)->GetUserId() == nUserId ){
					m_UserGenerator.GetPoseDetectionCap().StopPoseDetection( nUserId );
					//NxMocapUser * User = GetUserFromId( nUserId );
					//if(User) User->SetSkeletonTracked( false );
					Log("DELETING USER : " + NxMocapUtils::ToString( (*it)->GetUserId() ) );
					delete *it;
					it=mUsers.erase(it);
				}
			}
		
		}

        static void XN_CALLBACK_TYPE NewUser(xn::UserGenerator& generator, const XnUserID nUserId, void* pCookie)
        {
			KinectPlayer* This = (KinectPlayer*)pCookie;

			xnOSEnterCriticalSection( &This->pCriticalSectionHandle);

			Log("NEW USER : " + NxMocapUtils::ToString( nUserId ) );
			generator.GetPoseDetectionCap().StartPoseDetection("Psi", nUserId);	// start looking for calibration pose for new users
			//if(!This->UserIdExists( nUserId )){
				Log("ADDED USER : " + NxMocapUtils::ToString( nUserId ) );
				This->mUsers.push_back( new NxMocapUser( nUserId ) );
			//}

				


			xnOSLeaveCriticalSection(  &This->pCriticalSectionHandle);

        }

        //LOST USER
        static void XN_CALLBACK_TYPE LostUser(xn::UserGenerator& generator, const XnUserID nUserId, void* pCookie)
        {
			KinectPlayer* This = (KinectPlayer*)pCookie;

			xnOSEnterCriticalSection( &This->pCriticalSectionHandle); 	

			if(This->UserIdExists( nUserId )){
				NxMocapUser * User = This->GetUserFromId( nUserId );
				//if(User){
					User->SetValid(false);
				//}
			}

			xnOSLeaveCriticalSection(  &This->pCriticalSectionHandle); 

	


			Log("LOST USER : " + NxMocapUtils::ToString( nUserId ) );


			//This->DeleteUser( nUserId );
			//Log(" deleted user success..... " );

			//Log("LOST USER..");



			//Log("PASSE LOST USER..");

			/*
			if(This->m_candidateID == nUserId )
			{
				This->m_candidateID = 0;
				//This->resetBonesToInitialState();
				This->m_pEndPoseDetector->SetUserId(0);
				This->m_pStartPoseDetector->Reset();



				
			}
			*/
        }

        //CALIBRATION START
        static  void XN_CALLBACK_TYPE CalibrationStart(xn::SkeletonCapability& skeleton, const XnUserID nUserId, void* pCookie)
        {
			Log("CALIBRATION START");
        }

        //CALIBRATION END (START TRACKING)
        static  void XN_CALLBACK_TYPE CalibrationEnd(xn::SkeletonCapability& skeleton, const XnUserID nUserId, XnBool bSuccess, void* pCookie)
        {
            KinectPlayer* This = (KinectPlayer*)pCookie;

			xnOSEnterCriticalSection( &This->pCriticalSectionHandle); 		

            if (bSuccess)
            {
                // start tracking
                skeleton.StartTracking(nUserId);
                This->m_pStartPoseDetector->SetStartPoseState(true);
                //This->setUserID(nUserId);

				//This->m_pEndPoseDetector->SetUserId( nUserId );

				

				NxMocapUser * User = This->GetUserFromId( nUserId );
				if(User) User->SetSkeletonTracked( true );

				//This->GetUserFromId( nUserId ).SetSkeletonTracked( true );
 

                // save torso position
				/*
                XnSkeletonJointPosition torsoPos;
                skeleton.GetSkeletonJointPosition(nUserId, XN_SKEL_TORSO, torsoPos);
                This->m_origTorsoPos.x = -torsoPos.position.X;
                This->m_origTorsoPos.y = torsoPos.position.Y;
                This->m_origTorsoPos.z = -torsoPos.position.Z; 

				This->m_candidateID = nUserId;
				*/
				Log("CALIBRATION END SUCCESS");		
            }
            else
            {
				//This->m_candidateID = 0;
				This->m_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", nUserId);
				Log("CALIBRATION END FAILED");	
				//This->GetUserFromId( nUserId ).SetSkeletonTracked( false );
				NxMocapUser * User = This->GetUserFromId( nUserId );
				if(User) User->SetSkeletonTracked( false );
            }
			xnOSLeaveCriticalSection(  &This->pCriticalSectionHandle);
 
        }

        //DETECTION POSE
        static void XN_CALLBACK_TYPE PoseDetected(xn::PoseDetectionCapability& poseDetection, const XnChar* strPose, XnUserID nId, void* pCookie)
        {
			KinectPlayer* This = (KinectPlayer*)pCookie;

			This->m_UserGenerator.GetSkeletonCap().RequestCalibration(nId, FALSE);
			This->m_UserGenerator.GetPoseDetectionCap().StopPoseDetection(nId);

			Log("POSE DETECTED");	
			//This->m_candidateID = nId;

			//// If we dont have an active candidate
			///if(This->m_candidateID == 0) // detects here ....
			//{
			//	This->m_candidateID = nId;
			//	This->m_UserGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
			//	This->m_pStartPoseDetector->SetStartPoseState(true);
			//	Log("POSE DETECTED");
			//}
        }

        //LOST POSE
        static void XN_CALLBACK_TYPE PoseLost(xn::PoseDetectionCapability& poseDetection, const XnChar* strPose, XnUserID nId, void* pCookie)
        {
			KinectPlayer* This = (KinectPlayer*)pCookie;
			This->m_pStartPoseDetector->Reset();
			Log("POSE LOST");
        }

		NxMocapUser * GetUser( unsigned int Index )
		{
			std::vector<NxMocapUser*>::iterator i = mUsers.begin();
			std::advance( i, Index );// now index points to the nth member of the list (counting from zero)
			return *i ;
		}

		NxMocapUser * GetUserFromId( unsigned int UserId ){

			NxMocapUser * User = 0;
			for(unsigned int i=0; i< mUsers.size(); i++ ){
				if( mUsers[i]->GetUserId() == UserId ){
					User = mUsers[i]; break;
				}
			}

			assert( User );
			return User;
		}

		unsigned short GetNumUsers(){
			return mUsers.size();// m_UserGenerator.GetNumberOfUsers();
		}

		void UpdateUserPositions()
		{
			xnOSEnterCriticalSection( &pCriticalSectionHandle); 		

			std::vector<NxMocapUser *>::iterator it;
			for(it=mUsers.end(); it!=mUsers.begin(); ){
				--it;
				if( (*it)->IsValid() == false ){
						delete *it;
						it=mUsers.erase(it);
					}	
				
			}

			//XnUserID aUsers[15];
			//XnUInt16 nUsers = 15;
			//m_UserGenerator.GetUsers(aUsers, nUsers);
			for (int i = 0; i < GetNumUsers() /*nUsers*/; ++i){

				NxMocapUser * User = GetUser(i);// GetUserFromId( aUsers[i] );
				//if(!User){ User->SetValid(false); xnOSLeaveCriticalSection(  &pCriticalSectionHandle); return;   }

				XnPoint3D com;
				m_UserGenerator.GetCoM( User->GetUserId() , com);
				m_DepthGenerator.ConvertRealWorldToProjective(1, &com, &com);

				float Position[3];
				Position[0] = com.X;
				Position[1] = com.Y;
				Position[2] = com.Z;
				User->SetUserPosition( Position );		

				//if(Position[2]==0.0f){ User->SetValid(false); xnOSLeaveCriticalSection(  &pCriticalSectionHandle); return; }
	
				xn::SkeletonCapability pUserSkel = m_UserGenerator.GetSkeletonCap(); 

				if (pUserSkel.IsTracking( User->GetUserId() /*aUsers[i]*/)  ){

					std::vector<NxMocapJoint> JointList;
					User->GetSkeletonJoints( JointList );

					for( int y = 0 ; y < JointList.size(); y++ ){

						XnSkeletonJointPosition jointPos;
						pUserSkel.GetSkeletonJointPosition(  User->GetUserId() /*aUsers[i]*/, GetOpenNiTypeFromNxSkeletonType( JointList[y].mType ), jointPos );

						float Positions[3];
						Positions[0] = jointPos.position.X;
						Positions[1] = jointPos.position.Y;
						Positions[2] = jointPos.position.Z;
						User->SetJointPosition( JointList[y].mType, Positions, jointPos.fConfidence );
					}
				}

				//Get Bounding box
				//UserPositionCapability Cap = m_UserGenerator.GetUserPositionCap();
				//XnBoundingBox3D Position;
				//XnStatus Res = Cap.GetUserPosition(i, Position);
			}
			
			xnOSLeaveCriticalSection(  &pCriticalSectionHandle);
		
		}

		/*
		void Reset()
		{
			xn::SkeletonCapability pUserSkel = m_UserGenerator.GetSkeletonCap();
			m_UserGenerator.GetSkeletonCap().Reset( m_candidateID );
			pUserSkel.StopTracking(m_candidateID);
			m_UserGenerator.GetPoseDetectionCap().StartPoseDetection("Psi", m_candidateID);
		}*/

		void UpdateData()
		{

			m_Context.WaitNoneUpdateAll();

			UpdateUserPositions();

		 
				//XnStatus eResult = m_Context.WaitAndUpdateAll();//WaitNoneUpdateAll();//WaitOneUpdateAll(m_DepthGenerator);////
				//xn::SceneMetaData sceneMD;
				//m_UserGenerator.GetUserPixels(0, sceneMD);
		}

		/*
		XnVector3D GetRootPosition()
		{
			static bool bNewUser = true;
			static NxVector3 origTorsoPos;
			static XnVector3D DestTorsoPos;
			XnSkeletonJointPosition torsoPos;
			xn::SkeletonCapability pUserSkel = m_UserGenerator.GetSkeletonCap(); 
			if (pUserSkel.IsTracking(m_candidateID))
			{
				if(bNewUser)
				{                       
					pUserSkel.GetSkeletonJointPosition(m_candidateID, XN_SKEL_TORSO, torsoPos);
					if(torsoPos.fConfidence > 0.5)
					{
						origTorsoPos.x = -torsoPos.position.X;
						origTorsoPos.y  = torsoPos.position.Y;
						origTorsoPos.z  = -torsoPos.position.Z;
						bNewUser = false;
					}
				}

				pUserSkel.GetSkeletonJointPosition(m_candidateID, XN_SKEL_TORSO, torsoPos);
				NxVector3 newPos;
				newPos.x = -torsoPos.position.X;
				newPos.y = torsoPos.position.Y;
				newPos.z = -torsoPos.position.Z;

				NxVector3 newPos2 = (newPos - origTorsoPos)/100;

				newPos2.y -= 0.3;

				if (newPos2.y  < 0)
				{
						newPos2.y  /= 2.5;

						if (newPos2.y  < -1.5)
						{
								newPos2.y  = -1.5;
						}
				}
				if(torsoPos.fConfidence > 0.5)
				{
					NxVector3 Divider( newPos2 / 10.0f );
					DestTorsoPos.X  = Divider.x;
					DestTorsoPos.Y = Divider.y;
					DestTorsoPos.Z = Divider.z;
				}
			}

			return DestTorsoPos;
		
		}
		*/

		bool IsTracking( const unsigned int UserID )
		{
			xn::SkeletonCapability pUserSkel = m_UserGenerator.GetSkeletonCap(); 
			return pUserSkel.IsTracking( UserID ); 
		}

		/*
		XnStatus GetJointOrientation( XnSkeletonJoint skelJoint, float OriValue[4] )
		{
			xn::SkeletonCapability pUserSkel = m_UserGenerator.GetSkeletonCap();  
			XnSkeletonJointOrientation jointOri;
			XnStatus Res = pUserSkel.GetSkeletonJointOrientation(m_candidateID, skelJoint, jointOri);

            XnVector3D col1 = xnCreatePoint3D(jointOri.orientation.elements[0], jointOri.orientation.elements[3], jointOri.orientation.elements[6]);
            XnVector3D col2 = xnCreatePoint3D(jointOri.orientation.elements[1], jointOri.orientation.elements[4], jointOri.orientation.elements[7]);
            XnVector3D col3 = xnCreatePoint3D(jointOri.orientation.elements[2], jointOri.orientation.elements[5], jointOri.orientation.elements[8]);


              NxMatrix3<float> matOri(jointOri.orientation.elements[0],-jointOri.orientation.elements[1],jointOri.orientation.elements[2],
                        -jointOri.orientation.elements[3],jointOri.orientation.elements[4],-jointOri.orientation.elements[5],
                        jointOri.orientation.elements[6],-jointOri.orientation.elements[7],jointOri.orientation.elements[8]);
			 
			NxQuaternion qa;
			qa.FromRotationMatrix( matOri );

			OriValue[0] = qa._w ;
			OriValue[1] = qa._x;
			OriValue[2] = qa._y;
			OriValue[3] = qa._z;

			return Res;
 
		}
		*/

		/*
		XnStatus GetJointPosition( XnSkeletonJoint skelJoint, float PosValue[3] )
		{
			xn::SkeletonCapability pUserSkel = m_UserGenerator.GetSkeletonCap();  
			XnSkeletonJointPosition jointPos;
			XnStatus Res = pUserSkel.GetSkeletonJointPosition(m_candidateID, skelJoint, jointPos);
			PosValue[0] = jointPos.position.X;
			PosValue[1] = jointPos.position.Y;
			PosValue[2] = jointPos.position.Z;

			return Res;
		}
		*/


		inline const NxMocapImageDepth * GetDepthMap() const
		{
			return m_DepthGenerator.GetDepthMap();
		}

		inline XnDepthPixel GetDeviceMaxDepth() const
		{
			return m_DepthGenerator.GetDeviceMaxDepth();
		}

		inline const NxMocapImageRGB * GetImageMap() const
		{
			return (NxMocapImageRGB*)m_ImageGenerator.GetRGB24ImageMap(); 
		}

		inline const XnIRPixel * GetInfraRedMap() const
		{
			return m_IrGenerator.GetIRMap();
		}

	 

				
		inline bool GetPoints( float * PointsPosition, unsigned int * Colors, unsigned long & NumParticles, float Scale ,float MinRange, float MaxRange /* meters */ )
		{
			if(!mNewDepthFrame) return false;
			NumParticles = 0;
			const XnDepthPixel * pDepthMap = m_DepthGenerator.GetDepthMap (); 
			const XnRGB24Pixel * pImageMap = m_ImageGenerator.GetRGB24ImageMap (); 
			XnMapOutputMode Mode;
			m_DepthGenerator.GetMapOutputMode(Mode);
			int WidthX = Mode.nXRes;
			int WidthY = Mode.nYRes;
			const static float FovH= 1.0144686707507438;
			const static float FovV=0.78980943449644714;

			const static float XtoZ=tan(FovH/2)*2;
			const static float YtoZ=tan(FovV/2)*2;
			unsigned int i, j, idxShift, idx; 

			unsigned long MissingPoints = 0;


			//for (int y = 0, i = 0; y < WidthY; y++){
			//	for (int x = 0; x < WidthX; x++, i++){
			//		float Z = pDepthMap[(int)(y * WidthX + x)];
			//		if (Z >= 0){
			//			float X_rw=((float)x/WidthX-.5)*Z*XtoZ;
			//			float Y_rw=(0.5-(float)y/WidthY)*Z*YtoZ;



			//			fprintf(_file,"%f %f %f %u %u %u\n", X_rw / 1000.0, Y_rw / 1000.0 , -Z / 1000.0, xImage[i].nRed, xImage[i].nGreen, xImage[i].nBlue);
			//		}
			//	}

			for (j = 0; j < WidthY; ++j ){ 
				idxShift = j * WidthX; 

				for (i = 0; i <WidthX; ++i ){ 
					idx = idxShift + i; 

					float X = i;
					float Y = j;
					float Z = pDepthMap[idx];

					 if( ( pDepthMap[idx] <= MaxRange ) && (pDepthMap[idx] >= MinRange ) && pDepthMap[idx] !=0  ) {

						unsigned long Shift = idxShift + ( i - MissingPoints ); //

						PointsPosition[Shift*3] = ((  X/WidthX-.5 )* Z*XtoZ)/Scale;
						PointsPosition[Shift*3+1] = ((0.5- Y/WidthY)* Z*YtoZ)/Scale;
						PointsPosition[Shift*3+2] = -Z/Scale;

						unsigned char val8;
						unsigned int val32 = 0;
						// Alpha
						val8 = static_cast<unsigned char>( 255 );
						val32 = val8 << 24;

						// Blue
						val8 = static_cast<unsigned char>( pImageMap[idx ].nBlue);
						val32 += val8 << 16;

						// Green
						val8 = static_cast<unsigned char>( pImageMap[idx ].nGreen);
						val32 += val8 << 8;

						// Red
						val8 = static_cast<unsigned char>( pImageMap[idx ].nRed);
						val32 += val8;

						*Colors++ = val32;
						NumParticles++;

					 }
					 else
					 {
					 	MissingPoints++;
					 }	
				} 
			} 
				mNewDepthFrame = false;
			return true;

		}

	/*	
		inline bool GetPoints(  std::vector<float> & mPoints , std::vector<unsigned int> & mColours, float Scale ,float MinRange, float MaxRange  )
		{
 
			if(!mNewDepthFrame) return false;

			 
			const XnDepthPixel * pDepthMap = m_DepthGenerator.GetDepthMap (); 
			const XnRGB24Pixel * pImageMap = m_ImageGenerator.GetRGB24ImageMap (); 
			XnMapOutputMode Mode;
			m_DepthGenerator.GetMapOutputMode(Mode);
			int WidthX = Mode.nXRes;
			int WidthY = Mode.nYRes;
			const static float FovH= 1.0144686707507438;
			const static float FovV=0.78980943449644714;

			const static float XtoZ=tan(FovH/2)*2;
			const static float YtoZ=tan(FovV/2)*2;
			unsigned int i, j, idxShift, idx; 

			unsigned long MissingPoints = 0;


			//for (int y = 0, i = 0; y < WidthY; y++){
			//	for (int x = 0; x < WidthX; x++, i++){
			//		float Z = pDepthMap[(int)(y * WidthX + x)];
			//		if (Z >= 0){
			//			float X_rw=((float)x/WidthX-.5)*Z*XtoZ;
			//			float Y_rw=(0.5-(float)y/WidthY)*Z*YtoZ;



			//			fprintf(_file,"%f %f %f %u %u %u\n", X_rw / 1000.0, Y_rw / 1000.0 , -Z / 1000.0, xImage[i].nRed, xImage[i].nGreen, xImage[i].nBlue);
			//		}
			//	}

			for (j = 0; j < WidthY; ++j ){ 
				idxShift = j * WidthX; 

				for (i = 0; i <WidthX; ++i ){ 
					idx = idxShift + i; 

					float X = i;
					float Y = j;
					float Z = pDepthMap[idx];

					if( ( pDepthMap[idx] <= MaxRange ) && (pDepthMap[idx] >= MinRange ) && pDepthMap[idx] !=0  ) {

						unsigned long Shift = idxShift + ( i - MissingPoints ); //

						//PointsPosition[Shift*3] = ((  X/WidthX-.5 )* Z*XtoZ)/Scale;
						//PointsPosition[Shift*3+1] = ((0.5- Y/WidthY)* Z*YtoZ)/Scale;
						//PointsPosition[Shift*3+2] = -Z/Scale;
 

						 
						float PointX = ((  X/WidthX-.5 )* Z*XtoZ)/Scale;
						float PointY = ((0.5- Y/WidthY)* Z*YtoZ)/Scale;
						float PointZ = -Z/Scale;

						mPoints.push_back( PointX );
						mPoints.push_back( PointY );
						mPoints.push_back( PointZ );

						unsigned char val8;
						unsigned int val32 = 0;
						// Alpha
						val8 = static_cast<unsigned char>( 255 );
						val32 = val8 << 24;

						// Blue
						val8 = static_cast<unsigned char>( pImageMap[idx ].nBlue);
						val32 += val8 << 16;

						// Green
						val8 = static_cast<unsigned char>( pImageMap[idx ].nGreen);
						val32 += val8 << 8;

						// Red
						val8 = static_cast<unsigned char>( pImageMap[idx ].nRed);
						val32 += val8;

						//*Colors++ = val32;

						mColours.push_back( val32 );
						//NumParticles++;

					}
					else
					{
						MissingPoints++;
					}	
				} 
			} 
			mNewDepthFrame = false;
			return true;


		}// end function

		*/

}; // class


namespace NxMocap_Namespace {

NxMocapDeviceKinect::NxMocapDeviceKinect() : NxMocapDevice()  {

}

NxMocapDeviceKinect::~NxMocapDeviceKinect(){

}

bool NxMocapDeviceKinect::ConnectDevice()
{
	return ConnectDevice( false );
}

bool NxMocapDeviceKinect::ConnectDevice( bool UseInfrared ) {

	mUseInfraRed = UseInfrared;
	Log("Connecting Kinect..");
	mKinect = new KinectPlayer();
	if( !mKinect->PrimeSensorInitialized ){
		if( mKinect->initPrimeSensor( UseInfrared ) != XN_STATUS_OK ){ Log(" Connecting Kinect Failed !"); return false; }
	}

	Log("Connecting Kinect Success");
	mKinect->PrimeSensorInitialized = true;
	return true;
}

bool NxMocapDeviceKinect::DisconnectDevice(){
	mKinect->Disconnect();
	mKinect->PrimeSensorInitialized = false;
	return true;
}

void NxMocapDeviceKinect::Reset(){
	//mKinect->Reset();
}

bool NxMocapDeviceKinect::IsUsingInfraRedMap()
{
	return mUseInfraRed;
}

bool NxMocapDeviceKinect::IsTracking( unsigned int UserID ){
	return mKinect->IsTracking( UserID );
}

unsigned int NxMocapDeviceKinect::GetNumUsers(){
	return mKinect->GetNumUsers();
}

NxMocapUser * NxMocapDeviceKinect::GetUser( unsigned int Index ){
	return mKinect->GetUser( Index );
}

unsigned short NxMocapDeviceKinect::GetDeviceMaxDepth() const
{
	return (unsigned short)mKinect->GetDeviceMaxDepth();
}

const NxMocapImageDepth * NxMocapDeviceKinect::GetDepthMap() const
{
	return mKinect->GetDepthMap();
}

const NxMocapImageRGB * NxMocapDeviceKinect::GetImageMap() const
{
	return mKinect->GetImageMap();
}

const NxMocapImageIr * NxMocapDeviceKinect::GetInfraRedMap() const
{
	return mKinect->GetInfraRedMap();
}

bool NxMocapDeviceKinect::GetPointsInfo( float * mPoints, unsigned int * mColours, unsigned long & NumParticles, float Scale, float MinRange, float MaxRange ){
//bool NxMocapDeviceKinect::GetPointsInfo( std::vector<float> & mPoints, std::vector<unsigned int> & mColours, unsigned long & NumParticles, float Scale, float MinRange, float MaxRange ){
 
	return mKinect->GetPoints( mPoints, mColours, NumParticles, Scale, MinRange, MaxRange );
}
 
bool NxMocapDeviceKinect::Update(){

	mKinect->UpdateData();

	/*

	if( !IsTracking() ) return false;
	std::vector<NxMocapJoint> JointList;
	GetSkeletonJoints( JointList );

	for( int i= 0 ; i < JointList.size(); i++ )
	{
		float OrientationValue[4];
		//if( mKinect->GetJointOrientation( GetOpenNiTypeFromNxSkeletonType( JointList[i].mType ), OrientationValue ) != XN_STATUS_OK ){ return false; }
		mKinect->GetJointOrientation( GetOpenNiTypeFromNxSkeletonType( JointList[i].mType ), OrientationValue );
		SetJointOrientation( JointList[i].mType ,  OrientationValue  );

		float PositionValue[3];
 
		if( IsRoot( JointList[i]) )
		{
			XnVector3D RootPos = mKinect->GetRootPosition();
			PositionValue[0] = RootPos.X;
			PositionValue[1] = RootPos.Y;
			PositionValue[2] = RootPos.Z;
		}
		else
		{
			//if( mKinect->GetJointPosition( GetOpenNiTypeFromNxSkeletonType( JointList[i].mType ), PositionValue ) != XN_STATUS_OK){ return false; }
			 mKinect->GetJointPosition( GetOpenNiTypeFromNxSkeletonType( JointList[i].mType ), PositionValue ) ;
		}
		
		SetJointPosition( JointList[i].mType, PositionValue );
	}

	*/

	return true;
}



}