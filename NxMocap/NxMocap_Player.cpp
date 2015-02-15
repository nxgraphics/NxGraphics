#include "NxMocapPreCompiled.h"


#if NXMOCAP_PLATFORM == NXMOCAP_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#else
#include <unistd.h>
#endif


#include "../Nxdeps/include/libmysql/mysql.h"

/* example of use

// NxMocap Player
NxMocap_Player * player = new NxMocap_Player(); 

if( player->MysqlConnect( "127.0.0.1" , "root" , "" ) )// connect to Mysql server
{
	std::list<string> MyDBList = player->MysqlListDatabases();// get actual database names from mysql server
	player->SetConfigType( NxMocapConvertPacketTypes::Mysql_Live_PlayBack   );  
	player->MysqlLoadData( "istperf" );//select mysql database
	player->MysqlSetPlaybackRate( 100  ); // Set the playback rate, 150 frame per second.
	//player->LoadInMemoryMysql();// if NxMocapConvertPacketTypes::Mysql_Memory_PlayBack , load in memory the database.
	player->SetLooping(true);
	player->CreatePlaybackStream( "127.0.0.1" , 50000 );
	player->SetTimeUpdateManual( false ) ;
	player->SetInboundPlayBackTime( 120 * 1000.0f );
	player->SetOutboundPlayBackTime( 121 * 1000.0f );
	player->StartPlayBack();
}

while(true){
//player->SeekTimePosition(       ) ;
	Sleep(10); 
}

//Mocap Recorder
string DatabaseName("blabla");
NxMocap_Recorder * recorder = new NxMocap_Recorder(); 

if( recorder->MysqlConnect( "127.0.0.1" , "root" , "" ) )
{
	if(recorder->MysqlDatabaseExists( DatabaseName) ) // if database exists
	{recorder->MysqlDeleteDatabase( DatabaseName );} // delete it
	if(recorder->MysqlCreateDatabase( DatabaseName ))// create a database with the specified name
	{recorder->StartRecording( DatabaseName , 50000 );} // start recording data with specified database name and port
}
 
*/

// OGRE is typical Right Hand Opengl coordinate system
/*
                | +y
                |
                |       
                ----------- +x
               /
              /
             / +z

*/

namespace NxMocap_Namespace {

	static int StaticNumPlayers = 1 ;// total number of players created; doesnt decrease


NxMocapPlayer::NxMocapPlayer() : NxMocapMysql(), isCurrentlyPlaying(false),isCurrentlyPaused(false),Frame_thread_Handle(0),
Time_thread_Handle(0), frame_rate_stable(0) 
{
	/*
	CreateJoint( "Bip01 L UpperArm", NxSkeletonJoint::NX_SKEL_L_UPPERARM );
	CreateJoint( "Bip01 R UpperArm", NxSkeletonJoint::NX_SKEL_R_UPPERARM );
	CreateJoint( "Bip01 L Forearm", NxSkeletonJoint::NX_SKEL_L_FOREARM );
	CreateJoint( "Bip01 R Forearm", NxSkeletonJoint::NX_SKEL_R_FOREARM );
	CreateJoint( "Bip01 L Thigh", NxSkeletonJoint::NX_SKEL_L_THIGH );
	CreateJoint( "Bip01 R Thigh", NxSkeletonJoint::NX_SKEL_R_THIGH );
	CreateJoint( "Bip01 L Calf", NxSkeletonJoint::NX_SKEL_L_CALF );
	CreateJoint( "Bip01 R Calf", NxSkeletonJoint::NX_SKEL_R_CALF );
	CreateJoint( "Bip01 Head", NxSkeletonJoint::NX_SKEL_HEAD );
	CreateJoint( "Bip01 Spine1", NxSkeletonJoint::NX_SKEL_SPINE1 );
	CreateJoint( "Bip01", NxSkeletonJoint::NX_SKEL_ROOT );
	*/

	DataList.clear();
	for(int i = 0; i< 16; i++ ) 
	{
		NxMocapData * Tmp = new NxMocapData();
		for(int y = 0; y < 7; y++ ) 
		{
			Tmp->push_back(0.0f);
		}
		DataList.insert(make_pair( i , Tmp ));
	}
 
 
	PlayBackFramerate = 25 ; // default playback framerate

	frame_rate_index = 0;

	Sender_Should_Run = false ;
	PlayBackLooping = false ; // playback looping

	mTime = 0.0f ; // set the time index to 0.0 milliseconds
	mTimeInbound = 0.0f ;
	Player_ManualTimeMode = false ; // time update is automatic

 
	MocapPlayerStreamIndex =  StaticNumPlayers ;//NxMocapManager::getSingleton().GetNumPlayers() + 1 ;
	StaticNumPlayers++ ;
}

NxMocapPlayer::~NxMocapPlayer()
{
	if(  IsPlaying() )
	{
		StopPlayBack();
	}
	


	if( IsConnected() )
	{
	 	Disconnect();
	}	 
}
 

int NxMocapPlayer::GetMocapStreamIndex()
{
	return  MocapPlayerStreamIndex ;
}

float NxMocapPlayer::GetTotalTrackTime()
{
	return (float)((float)( NumRows() ) / (float)( PlayBackFramerate/ 1000.0f)  ) ;
}

bool NxMocapPlayer::Player_Is_TimeManual()
{
	return Player_ManualTimeMode ;
}

void NxMocapPlayer::SetTimeUpdateManual( bool manual )
{
	Player_ManualTimeMode = manual ;
}

void  NxMocapPlayer::SeekTimePosition( float mDestTimeMs  )
{
	if( mDestTimeMs <= mTimeInbound ){ mTime = mTimeInbound ;  }
	else if( mDestTimeMs >= mTimeOutbound ){ mTime = mTimeOutbound ;   }
	else{ mTime = mDestTimeMs ;  }		
}

float NxMocapPlayer::GetTimePosition() const
{
	return mTime ;
}

void NxMocapPlayer::SetInboundPlayBackTime( float mDestTimeInboundMs )
{
	mTimeInbound = mDestTimeInboundMs;
	if( Time_Index_Last <= mTimeInbound ){ mTime = mDestTimeInboundMs; }
}

void NxMocapPlayer::SetOutboundPlayBackTime( float mDestTimeOutboundMs )
{
	mTimeOutbound = mDestTimeOutboundMs;
	if( Time_Index_Last >= mTimeOutbound ){ mTime = mTimeInbound; }
}

float NxMocapPlayer::GetInboundPlayBackTime()
{
	return mTimeInbound ;
}

float NxMocapPlayer::GetOutboundPlayBackTime()
{
	return mTimeOutbound;  
}

void NxMocapPlayer::LoadInMemoryMysql()
{
	// convert mysql data rows to Byte pointer array .
	ConvertMocapPacket(  Mysql_Memory_PlayBack  );
}

static int setcol(const std::string& input_record, std::vector<std::string>& colv)
{
	std::istringstream s(input_record);
	std::string temp;
	colv.clear();
	while (std::getline(s, temp, ',')){ colv.push_back(temp); }
	return static_cast<int>(colv.size());
}

void NxMocapPlayer::Convert_Mysql_To_Memory()
{
	Packet_Data.clear();
	cout << "Mysql_To_Memory Started...\r\n";

		for( my_ulonglong i = 0 ; i < NumRows() ; i++)// convert all rows to memory BYTE pointers.
		{
			std::ostringstream os;
			os << "select * from data where id='"<< i+1 <<"';" ; // mysql query
			std::string buffer(os.str());


			Query( buffer );
			MYSQL_ROW Mysql_Row =  FetchRow();
			//MYSQL_ROW Mysql_Row =  QueryDatabaseGet( buffer  );
			//mysql_query( Mysql_Connection , buffer.c_str() );
			//Mysql_Result =   mysql_store_result( Mysql_Connection );
			//Mysql_Row =  mysql_fetch_row( Mysql_Result );


			os.str("");//reset the string
			os << Mysql_Row[1];

			std::string packet_char(os.str());
			//split the string by comas !
			std::vector<std::string> res ;
			int num_bytes = setcol( packet_char , res );
			// create the memory packet
			unsigned char * tmp = new unsigned char[ num_bytes  ];
			for(long o = 0 ; o < num_bytes  ; o++){tmp[o] = atoi ( res[o].c_str()  ) ;}
			tmp[num_bytes] = 0x00 ;
			Packet_Data.insert(  make_pair( i ,  (unsigned char *)tmp   ) );  //insert packet data
			Packet_Length.insert(  make_pair( i ,  num_bytes  ) );   //insert packet size
			
		}

	cout << "Mysql_To_Memory Finished.\r\n";
	cout << "Ready for Playback.\r\n";

}

void NxMocapPlayer::ConvertMocapPacket( NxMocapConvertPacketTypes ConvertType  )
{
	PlayType = ConvertType; // backup the playback type
	switch( ConvertType )
	{
		case Mysql_Memory_PlayBack:
			Convert_Mysql_To_Memory();
		break ;
		case Mysql_Live_PlayBack :
		break ;
		default :
		break;

	}
}

void NxMocapPlayer::MysqlSetPlaybackRate( float fps )
{
	PlayBackFramerate = fps ;
	mTimeOutbound = ( (float)NumRows() * 1000.0f ) / (float)PlayBackFramerate   ;
}

float NxMocapPlayer::MysqlGetPlaybackRate()
{
	return PlayBackFramerate ;
}

void NxMocapPlayer::SetLooping( bool value)
{
	PlayBackLooping = value ;
}

void NxMocapPlayer::SetConfigType( NxMocapConvertPacketTypes Type   )
{
	PlayType = Type ;
}

unsigned char * NxMocapPlayer::GetTimeData( float Time )
{
	float FrameNum;
	float TrackTimeMs = ( (float)NumRows() * 1000.0f ) / PlayBackFramerate;
	if( Time != 0.0f ){ FrameNum = ( ( NumRows() ) / ( TrackTimeMs /  Time ) ); }
	else{ FrameNum = 0.0f; }
	//cout << "Requested frame : " << framerequested << "\r\n" ;
	return GetFrameData( FrameNum ); 

}

unsigned char * NxMocapPlayer::GetFrameData( long frameNumber )
{
	if( frameNumber != 0 ){frameNumber=frameNumber-1;}
	if( frameNumber >= 0 && frameNumber < NumRows() ||  frameNumber != Last_Frame_Num)
	{
		switch( PlayType )
		{
			case Mysql_Memory_PlayBack:
			{
				NxMocapPlayer::Packet_Data_Type::iterator iter_data = Packet_Data.find(frameNumber);//array starts at 0
				Last_Frame_Num = frameNumber ;
				return iter_data->second ;
			}
			break ;

			case Mysql_Live_PlayBack :
			{
				if(IsConnected())
				{
					std::ostringstream os;
					os << "select * from data where id='"<< frameNumber+1 <<"';" ; // mysql query
					std::string buffer(os.str());

					Query( buffer );
					MYSQL_ROW Mysql_Row =  FetchRow();
					unsigned char * tmp = 0;
					if(Mysql_Row != 0){

					os.str("");//reset the string
					os << Mysql_Row[1];
					std::string packet_char(os.str());

					FreeResults();// free the mysql results 

					//split the string by comas !
					std::vector<std::string> res ;
					int num_bytes = setcol( packet_char , res );
					// create the memory packet
					tmp = new unsigned char[ num_bytes ];
					for(long o = 0 ; o < num_bytes  ; o++){tmp[o] = atoi ( res[o].c_str()  ) ;}
					tmp[num_bytes] = 0x00 ;
					Packet_Data.insert(  make_pair( 0, (unsigned char *)tmp ) );  //insert packet data
					Packet_Length.insert(  make_pair( 0, num_bytes ) );   //insert packet size
				}

				Last_Frame_Num = frameNumber ;
				return tmp ;
				}
			}
			break ;
		default :
			break ;
		}
	}

	return 0;
}

void NxMocapPlayer::StopPlayBack()
{
	/* not compatible multi platform
	//stop thread
	Sender_Should_Run = false ; // shuts down thread frame
	FramerateIndexer_Should_Run = false ; // shuts down thread time

	unsigned long dwRet = WaitForSingleObject( Frame_thread_Handle, INFINITE ); 
	if (dwRet == WAIT_TIMEOUT)
	{
		unsigned long dwForcefulExitCode = 1;        
		::TerminateThread( Frame_thread_Handle , dwForcefulExitCode);
	} 
	else
	{

	}
	Frame_thread_Handle = NULL ;

	dwRet = WaitForSingleObject( Time_thread_Handle, INFINITE ); 
	if (dwRet == WAIT_TIMEOUT)
	{
		unsigned long dwForcefulExitCode = 1;        
		::TerminateThread( Time_thread_Handle , dwForcefulExitCode);
	} 
	else
	{

	}
	Time_thread_Handle = NULL ;

	isCurrentlyPlaying = false ;
	isCurrentlyPaused = false ;
	*/

}

bool NxMocapPlayer::IsPlaying()
{
	return isCurrentlyPlaying ;
}

bool NxMocapPlayer::IsStopped()
{
	return isCurrentlyPlaying ? false : true ;
}

bool NxMocapPlayer::IsPaused() const 
{
	return isCurrentlyPaused ;
}

void NxMocapPlayer::PausePlayback()
{
	isCurrentlyPaused = true ;
}

void NxMocapPlayer::ResumePlayback()
{
	isCurrentlyPaused = false ;
}

float NxMocapPlayer::GetCurrentPlaybackFramerate()
{
	return (float)frame_rate_stable ;
}

void NxMocapPlayer::SetReferenceFrame( float Azimuth, float Elevation, float Roll )
{

}

void NxMocapPlayer::GetReferenceFrame( float &Azimuth, float &Elevation, float &Roll )
{

}

NxMocapTypes NxMocapPlayer::GetNxMocapType()
{
	return NxMocapType ;
}

void NxMocapPlayer::SetNxMocapType( NxMocapTypes Type )
{
	NxMocapType = Type ;
}
	
unsigned long time_thread(void * lpParameter)
{
	NxMocapPlayer * pointer = ( NxMocapPlayer * )lpParameter ; 
	char * dbg = new char[255];

	while( pointer->FramerateIndexer_Should_Run )
	{
		//sprintf(dbg , "NxMocap Player framerate : %d \r\n", pointer->frame_rate_index );
		//cout <<  dbg  ;
		pointer->frame_rate_stable = pointer->frame_rate_index ;
		pointer->frame_rate_index = 0;
		//Sleep(1000); // not multiplatform
	}
  
	return 0;
}

void NxMocapPlayer::ConvertToJoints()
{
	//to implement
}


/*
typedef enum NxSkeletonJoint
{
	NX_SKEL_HEAD			= 1,
	NX_SKEL_NECK			= 2,
	NX_SKEL_SPINE3			= 3,
	NX_SKEL_SPINE2			= 4,
	NX_SKEL_SPINE1		    = 5,
	NX_SKEL_SPINE	        = 6,
	NX_SKEL_PELVIS   		= 7,
	NX_SKEL_ROOT    		= 8,
	NX_SKEL_L_THIGH 		= 9,
	NX_SKEL_R_THIGH     	=10,
	NX_SKEL_L_CALF      	=11,
	NX_SKEL_R_CALF      	=12,
	NX_SKEL_L_FOOT  		=13,
	NX_SKEL_R_FOOT   		=14,
	NX_SKEL_L_CLAVICLE 		=15,
	NX_SKEL_R_CLAVICLE  	=16,
	NX_SKEL_L_UPPERARM		=17,
	NX_SKEL_R_UPPERARM		=18,
	NX_SKEL_L_FOREARM 		=19,
	NX_SKEL_R_FOREARM		=20,
	NX_SKEL_L_HAND		    =21,
	NX_SKEL_R_HAND		    =22,

} NxSkeletonJoint;
*/

 
}//namespace