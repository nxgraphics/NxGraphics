#include "NxMocapPreCompiled.h"
 

namespace NxMocap_Namespace {

NxMocapPlayerPolhemus::NxMocapPlayerPolhemus() : NxMocapPlayer()//, hanim(0)
{
	/*
	SetCurrentPhase( PhaseLive );
	hanim = new CHAnim();
	hanim->setReferenceFrameE( -90 , 180, -180); // hanim->setReferenceFrameE( -90 , 180 , 0);

	int layout[] = {
	-1, // right wrist
	11, //head
	-1, //left hankle
	5, // left shoulder
	6, // left elbow
	3, // right shoulder
	4, // right ezlbow
	2, // left hip
	10, // left knee
	-1, // left wrist
	-1, // right hankle
	0, // sacrollac
	8, // right knee
	7, // right hip
	1 // chest
	};

  // set the defined layout
  hanim->setLayout(layout);
  */

}

NxMocapPlayerPolhemus::~NxMocapPlayerPolhemus()
{


}

/*
void NxMocapPlayerPolhemus::SetReferenceFrame( float Azimuth, float Elevation, float Roll )
{
	hanim->setReferenceFrameE( Azimuth, Elevation, Roll );  
}

void NxMocapPlayerPolhemus::GetReferenceFrame( float &Azimuth, float &Elevation, float &Roll )
{
	float * Data = hanim->getReferenceFrameE();
	Azimuth = Data[0];
	Elevation = Data[1];
	Roll = Data[2];
}

void NxMocapPlayerPolhemus::SetLayout( int newLayout[] )
{
	hanim->setLayout( newLayout );
}

int * NxMocapPlayerPolhemus::GetLayout()
{
	return hanim->getLayout();
}

void NxMocapPlayerPolhemus::ConvertToJoints()
{
 
	for( int SensorID = 0; SensorID < DataList.size(); SensorID++  )
	{
		CHAnim::EJoints ChanimType;
		NxSkeletonJoint NxJointType;
		CQuat quater  ; 
		CVector3D Poser ;
		bool UseValue = true;

		switch(SensorID+1)
		{
			case 1: // SACROILIAC
				quater = hanim->getJointRotationQ( CHAnim::EJoints::SACROILIAC ); 
				Poser = hanim->getJointPosition( CHAnim::EJoints::SACROILIAC ) ; 
				NxJointType = NxSkeletonJoint::NX_SKEL_ROOT;
			break;
			case 2: // head
				quater = hanim->getJointRotationQ( CHAnim::EJoints::VC7 ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::VC7 ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_HEAD;
			break ;
			case 3 : // left upper leg
				quater = hanim->getJointRotationQ( CHAnim::EJoints::L_HIP ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::L_HIP ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_L_THIGH;
			break;
			case 4 : // right upper arm
				quater = hanim->getJointRotationQ(  CHAnim::EJoints::R_SHOULDER ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::R_SHOULDER ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_R_UPPERARM;
			break;
			case 5 : // right down arm
				quater = hanim->getJointRotationQ( CHAnim::EJoints::R_ELBOW ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::R_ELBOW ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_R_FOREARM;
			break;
			case 6 : // left upper arm
				quater = hanim->getJointRotationQ( CHAnim::EJoints::L_SHOULDER ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::L_SHOULDER ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_L_UPPERARM;
			break;
			case 7 : // left down arm
				quater = hanim->getJointRotationQ( CHAnim::EJoints::L_ELBOW ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::L_ELBOW ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_L_FOREARM;
			break;
			case 8 : // right upper leg
				quater = hanim->getJointRotationQ( CHAnim::EJoints::R_HIP ) ;
				Poser =  hanim->getJointPosition( CHAnim::EJoints::R_HIP ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_R_THIGH;
			break;
			case 9 : // right down leg
				quater = hanim->getJointRotationQ( CHAnim::EJoints::R_KNEE ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::R_KNEE ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_R_CALF;
			break;
			case 10 :
			UseValue = false;

			break;
			case 11 : // left down leg
				quater = hanim->getJointRotationQ( CHAnim::EJoints::L_KNEE ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::L_KNEE ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_L_CALF;
			break;
			case 12 : // spine 1
				quater = hanim->getJointRotationQ( CHAnim::EJoints::VL5 ) ;
				Poser = hanim->getJointPosition( CHAnim::EJoints::VL5 ) ;
				NxJointType = NxSkeletonJoint::NX_SKEL_SPINE1;
			break;
			default :
			UseValue = false;	
			break ;
		}

			float Position[3];
			float Orientation[4];
			Position[0] = Poser.v[0]; Position[1] = Poser.v[1]; Position[2] = Poser.v[2];
			Orientation[0] = quater.w; Orientation[1] = quater.x; Orientation[2] = quater.y; Orientation[3] = quater.z;
			if( UseValue ) SetJointPosition( NxJointType, Position, 1.0f );
			if( UseValue ) SetJointOrientation( NxJointType, Orientation );
	}
}
*/

void NxMocapPlayerPolhemus::StartPlayBack( const string & Ip, int port )
{
	/*
	// start thread sender
	Sender_Should_Run = true ;
	FramerateIndexer_Should_Run = true ;

	isCurrentlyPlaying = true ;
	isCurrentlyPaused = false ;

	DWORD socket_thread_id;
	Frame_thread_Handle = CreateThread(NULL, 0,   (LPTHREAD_START_ROUTINE)socket_thread_sender , this , 0, &socket_thread_id); 

	// framerate handler
	DWORD	time_thread_id;
	Time_thread_Handle =   CreateThread(NULL, 0,   (LPTHREAD_START_ROUTINE)time_thread  , this , 0, &time_thread_id); 
	*/
}

/*
DWORD socket_thread_sender(LPVOID lpParameter)
{
	NxMocapPlayerPolhemus * pointer = ( NxMocapPlayerPolhemus * ) lpParameter ; 
	precision_timer  * chrono_final_prec = new precision_timer(); 
	precision_timer  * chrono_prec = new precision_timer(); 
	chrono_prec->start();
 
	float time_elapsed = 0.0f ; // our playbck time pointer
	float Chrono_Last_Time = 0.0f ;
	chrono_final_prec->start();

	while( pointer->Sender_Should_Run  )//while true
	{
		chrono_final_prec->operator =(  ( double )0.0f );
		chrono_final_prec->restart();

		if( pointer->IsPaused() )
		{
			time_elapsed = pointer->mTime ;
			chrono_prec->operator =( ( double )pointer->mTime / 1000.0f );
			chrono_prec->restart();
		}
		else
		{
			chrono_prec->stop();
			chrono_prec->operator =(  fmod(  chrono_prec->elapsed(),  (double) ( (double)pointer->GetTotalQueryRows() * 1000.0f ) / (float)pointer->PlayBackFramerate ));
			time_elapsed = (float)( chrono_prec->elapsed() * 1000.0f   );
			chrono_prec->restart();
		}
 
		if( !pointer->Player_Is_TimeManual() ) // if automatic
		{ 
			pointer->mTime = time_elapsed;
		}

		float Total_Track_Time_Milliseconds = pointer->GetTotalTrackTime() ;

		if(  (pointer->mTime  >  Total_Track_Time_Milliseconds) || ( pointer->mTime  >  pointer->GetOutboundPlayBackTime() ) )
		{
			if(pointer->PlayBackLooping)//should loop ?
			{  
				chrono_prec->stop();
				chrono_prec->operator =( pointer->GetInboundPlayBackTime() / 1000.0f );
		        chrono_prec->restart(); 
			}	
		}

		//inbound limits
		if( pointer->mTime <=  pointer->GetInboundPlayBackTime() )
		{ 
			pointer->SetInboundPlayBackTime( pointer->mTime  ); 
		}
		 
		BYTE * test = pointer->GetTimeData( pointer->mTime );
		if(test != NULL)
		{	
			if(pointer->UsePlayBackStream)
			{
				int sent_packets ;
				NxMocapPlayer::Packet_Length_Type::iterator iter_length = pointer->Packet_Length.find(0);
				sent_packets = pointer->mSocketClient->SendBytes( test, iter_length->second );
				//cout << "sent : "<<  sent_packets <<"\r\n" ;
				 pointer->frame_rate_index++;// framerate indexer
			}


			NxMocapPlayer::Packet_Length_Type::iterator iter_length = pointer->Packet_Length.find(0);

		//	computes hanim to sensor data
			pointer->ParseByteToFloatArray( test , iter_length->second );
			pointer->ConvertToJoints();
					 
		}else
		{
			//cout << "Test is Null !\r\n" ; // forgot to choose config type ?? mysql live or memory
			//cout << pointer->Time_Index ;
		}

		chrono_final_prec->stop();
		float time_to_wait = (DWORD) ((1000.0f / pointer->PlayBackFramerate ) - (chrono_final_prec->elapsed() * 1000.0f  )) ;
		if( time_to_wait > 0.0f && time_to_wait < 1000.0f )
		{
			Sleep( ceil( time_to_wait  ) );
		}
		chrono_final_prec->restart();

		pointer->Time_Index_Last = pointer->mTime ;
 
		   
	} // while true


return 0;
}

void NxMocapPlayerPolhemus::ParseByteToFloatArray( BYTE * Bytes, long count)
{
	hanim->setFrame( ( char * ) Bytes , count );
	if( GetCurrentPhase() ==  PhaseCalib )
	{
		hanim->calibrate();		 
	}

	hanim->computeJoints();
		 
	BYTE * buff  =  (BYTE * ) Bytes ;
	float m_Data[96];
	BYTE * buf;
	buf = (BYTE *) Bytes ;//buff;
	 
	char* eol = (char*)memchr(buf, 10, count);//search line feed
	if (eol == NULL) {     return ;} 
		
	*eol = '\0';
	int frame, frameSize, headerSize, recordSize;
	sscanf((char*)buff, "Frame %d FrameSize %d HeaderSize %d RecordSize %d", &frame, &frameSize, &headerSize, &recordSize);
	int records = (frameSize - headerSize) / recordSize;
	char leader[6];
	recordSize = sizeof(float)*6+5; //
	leader[5] = '\0';

	for (int i=0; i<records; i++) //Sensor total Number
	{
		char* record = (char*)(buf + i*recordSize + headerSize);
		memcpy(leader, record, 5);
		int board = leader[1] - '0'-1;
		int station = leader[3] - '0' - 1;
		float* numsf = (float*)(record+5);
		u_long* nums = (u_long*)(record+5);

		int sensornumber = (station*8 + board) ;

		for (int j=0; j<6; j++) 
		{
			if( GetCurrentPhase() ==  PhaseLive )
			{
				SensorData[sensornumber][j] = numsf[j];	
			}
			else
			{
				CalibSensorData[sensornumber][j] = numsf[j];
			}
		}//j
	}//i
}

float NxMocapPlayerPolhemus::GetSensorData ( int sensor , NxMocapSensorDataTypes Type ) 
{
	 //assert( sensor ) ;

	 switch(Type)
	 {
		 case  POSX :
		 return  SensorData[ sensor - 1 ][ 0 ];
		 break ;
		 case  POSY :
		 return  SensorData[ sensor - 1][ 1 ];
		 break ;
		 case  POSZ :
		 return  SensorData[ sensor - 1][ 2 ];
		 break ;
		 case  ORIX :
		 return  SensorData[ sensor - 1 ][ 3 ];
		 break ;
		 case  ORIY :
		 return  SensorData[ sensor - 1][ 4 ];
		 break ;
		 case  ORIZ :
		 return  SensorData[ sensor - 1 ][ 5 ];
		 break ;
	 
	 default:
		 return 0.0f ;
		 break ;
	 }
 
 }

float NxMocapPlayerPolhemus::GetCalibSensorData ( int sensor , NxMocapSensorDataTypes Type ) 
 {
	 assert( sensor ) ;
	 switch(Type)
	 {
		 case  POSX :
		 return  CalibSensorData[ sensor - 1 ][ 0 ];
		 break ;
		 case  POSY :
		 return  CalibSensorData[ sensor - 1 ][ 1 ];
		 break ;
		 case  POSZ :
		 return  CalibSensorData[ sensor - 1 ][ 2 ];
		 break ;
		 case  ORIX :
		 return  CalibSensorData[ sensor - 1 ][ 3 ];
		 break ;
		 case  ORIY :
		 return  CalibSensorData[ sensor - 1 ][ 4 ];
		 break ;
		 case  ORIZ :
		 return  CalibSensorData[ sensor - 1 ][ 5 ];
		 break ;
	 default:
		 return 0.0f ;
		 break ;
	 }
 }

NxMocapSensorStatus NxMocapPlayerPolhemus::GetCurrentPhase()
{
	return CurrentPhase ;	
}

void NxMocapPlayerPolhemus::SetCurrentPhase( NxMocapSensorStatus Phase )
{
	CurrentPhase = Phase ;
}
*/

}//end namespace

