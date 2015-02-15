#include "NxMocapPreCompiled.h"
 

namespace NxMocap_Namespace {

//static int StaticNumRecorders = 1 ;// total number of Recorders created;

NxMocap_Recorder::NxMocap_Recorder() //: NxMocapPlayerPolhemus(), IsRecording(false), Totalframeslost(0), Totalinsertedframes(0),RecordingTimer(0) //
{

	//SockServer = NULL ; // socket handle  


	/*
	mConnection = NULL;
	IsReceiving = false ;
	frame_rate_index = 0;
	Port_Receiver = 50000; // default port
	Receiver_Should_Run = false ;
	RecordingDatabaseName = "";
	SetCurrentPhase( NxMocapSensorStatus::PhaseLive );
	MocapPlayerStreamID =  NxMocapManager::getSingleton().GenerateId();
	MocapPlayerStreamIndex =  StaticNumRecorders ; 
	StaticNumRecorders++ ;
	//NxMocapManager::getSingleton().GetNumRecorders() + 1 ;

	for(int i = 0; i < 16 ; i++)
	{
		for (int j=0; j<6; j++) 
		{
			SensorData[i][j] = 0.0f ;
			CalibSensorData[i][j] = 0.0f ;
		}//j
	}//i

	DataList.clear();
	for(int i = 0; i< 16; i++ ) 
	{
		NxMocapData * Tmp = new NxMocapData();
		for(int y = 0; y < 7; y++ ) 
		{
			Tmp->push_back(0.0f);
		}
		DataList.insert(make_pair( i , Tmp ));
	}*/
}

NxMocap_Recorder::~NxMocap_Recorder()
{
	//if( IsReceiving  )
	//{
	//	StopReceiving();
	//}

	//if(  IsRecording )
	//{ 
	//	StopRecording();
	//}
}

/*
int NxMocap_Recorder::GetTotalFrameLost()
{
	return Totalframeslost ;
}

int NxMocap_Recorder::GetTotalInsertedFrames()
{
	return Totalinsertedframes ;
}

double NxMocap_Recorder::GetElapsedTime()
{
	if( IsRecording && RecordingTimer  )
	{
		RecordingTimer->stop() ;
		double time = RecordingTimer->elapsed();
		RecordingTimer->restart() ;
		return time ;
	}

	return 0;
}
*/

//for mysql recorder
void NxMocap_Recorder::StopRecording()
{
	//IsRecording = false ;
	//Receiver_Should_Run = false ;
	/*
	SockServer->Close();
	delete SockServer ;
	SockServer = NULL ;

	
	DWORD dwRet = WaitForSingleObject( socket_thread_Handle, INFINITE ); 
	if (dwRet == WAIT_TIMEOUT)
	{
		DWORD dwForcefulExitCode = 1;        
		::TerminateThread( socket_thread_Handle , dwForcefulExitCode);
	} 
	else
	{
		int couldnot = 0;
	}
	socket_thread_Handle = NULL ;

	dwRet = WaitForSingleObject( time_thread_Handle, INFINITE ); 
	if (dwRet == WAIT_TIMEOUT)
	{
		DWORD dwForcefulExitCode = 1;        
		::TerminateThread( time_thread_Handle , dwForcefulExitCode);
	} 
	else
	{
		int couldnot = 0;
	}
	time_thread_Handle = NULL ;

	RecordingTimer->stop();
	delete RecordingTimer ;
	RecordingTimer = NULL ;
	*/
}

/*
int NxMocap_Recorder::GetReceivePort()
{
	return Port_Receiver;
}
*/

bool NxMocap_Recorder::StartReceiving( int port )
{

	//SockServer = new SocketServer( port, 255, BlockingSocket ) ;


	/*
	Port_Receiver = port;  

	// start thread receiver
	Receiver_Should_Run = true ;
	DWORD	socket_thread_id;
	socket_thread_Handle = CreateThread(NULL, 0,   (LPTHREAD_START_ROUTINE)socket_thread_receiver , this , 0, &socket_thread_id); 

	// framerate handler
	DWORD	time_thread_id;
	time_thread_Handle =   CreateThread(NULL, 0,   (LPTHREAD_START_ROUTINE)time_thread_recorder  , this , 0, &time_thread_id);

	IsReceiving = true ;
	*/

	return true ;
}

void NxMocap_Recorder::StopReceiving( )
{
	/*
	//IsRecording = false ;
	Receiver_Should_Run = false ;

	DWORD dwRet = WaitForSingleObject( socket_thread_Handle, 1000 ); // wait 1 second for the thread to terminate
	if (dwRet == WAIT_TIMEOUT)
	{
		DWORD dwForcefulExitCode = 1;        
		::TerminateThread( socket_thread_Handle , dwForcefulExitCode);
	} 
	else
	{

	}
	socket_thread_Handle = NULL ;

	dwRet = WaitForSingleObject( time_thread_Handle, INFINITE ); 
	if (dwRet == WAIT_TIMEOUT)
	{
		DWORD dwForcefulExitCode = 1;        
		::TerminateThread( time_thread_Handle , dwForcefulExitCode);
	} 
	else
	{

	}
	time_thread_Handle = NULL ;
	*/
 

	//IsReceiving = false;
}

//mysql recorder
bool NxMocap_Recorder::StartRecording( const string & DatabaseName, int MysqlPort )
{
	/*
	if(!SelectDatabase( DatabaseName )){ return false ;  }
	Port_Receiver = port;  
	SockServer = new SocketServer( port, 255, BlockingSocket ) ;
	IsRecording = true ;
	RecordingDatabaseName = DatabaseName ;
	// start thread receiver
	Receiver_Should_Run = true ;
	RecordingTimer  = new precision_timer() ;
	RecordingTimer->start();
	DWORD	socket_thread_id;
	socket_thread_Handle = CreateThread(NULL, 0,   (LPTHREAD_START_ROUTINE)socket_thread_receiver , this , 0, &socket_thread_id); 
	// framerate handler
	DWORD	time_thread_id;
	time_thread_Handle =   CreateThread(NULL, 0,   (LPTHREAD_START_ROUTINE)time_thread_recorder  , this , 0, &time_thread_id);
	*/

	return true ;
}

/*
DWORD socket_thread_receiver(LPVOID lpParameter)
{
	NxMocap_Recorder * pointer =  ( NxMocap_Recorder * ) lpParameter ;
	BYTE * buff = new BYTE[10000];// buffer for receiving
	long rec ;// packet received bytes indicator
	while(pointer->Receiver_Should_Run)
	{
		while( rec = recv(pointer->SockServer->s_ , (char * ) buff , 10000, 0 ), rec  > 0 && pointer->Receiver_Should_Run )
		{ 
			pointer->frame_rate_index++; //increase the recording frame rate index
			buff[rec] = 0x00; //cut data
			if(pointer->IsRecording)
			{
				pointer->InsertMysqlMocapData( buff , rec  ) ;// insert buffer in mysql server
			}

			pointer->ParseMocapPacketData( buff , rec  ) ;
			pointer->ListenersUpdate();
		}
	}

	return true ;
}

int NxMocap_Recorder::GetRecordingFrameRate()
{
	return frame_rate_stable ;
}

DWORD time_thread_recorder(LPVOID lpParameter)
{
	NxMocap_Recorder * pointer = ( NxMocap_Recorder * )lpParameter ; 
	char * dbg = new char[255];

	while(pointer->Receiver_Should_Run)
	{
		//sprintf(dbg , "NxMocap Recorder framerate : %d \r\n", pointer->frame_rate_index );
		//cout <<  dbg  ;
		pointer->frame_rate_stable = pointer->frame_rate_index ;
		pointer->frame_rate_index = 0;
		Sleep(1000);
	}

	return true ;
}
*/

void NxMocap_Recorder::InsertData( unsigned char * data, int Size )
{
	 

	//convert the string to shit protocol
	char  tmp[1]  ;
	std::ostringstream os;
			
	for( long i = 0 ; i < Size ; i++){ // crypt the hash / should be avoided in future
		sprintf(  tmp , "%d," , data[i] );
		os << tmp  ;  
	}
	std::string StrResult(os.str());//data value

	double time_record = 0.0f;//GetElapsedTime();

	//TimeStamp
	std::ostringstream Timestamp;
	Timestamp.setf(ios::fixed, ios::floatfield);//set fixed double value
	Timestamp << time_record;
	std::string buffer_timestamp(Timestamp.str());

	string query_create("INSERT INTO data VALUES ('");
	query_create.append("");
	query_create.append("', '"); 
	query_create.append( StrResult.c_str() ) ; 
	query_create.append( "',");
	query_create.append( buffer_timestamp.c_str()  );
	query_create.append(");" ) ; 

	/*
	 
	if( QueryDatabaseInsert( query_create ) != 0 )
	{
		cout << "Recorder lost frame\r\n";
		Totalframeslost++ ;
	}
	else
	{
		Totalinsertedframes++ ;
	}
	*/
}

/* 
void NxMocap_Recorder::ParseMocapPacketData( BYTE * Bytes , long count)
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
	//char head[200];
 // mWindow->setDebugText( StringConverter::toString(frame)+ " " + StringConverter::toString(frameSize)+ " " + StringConverter::toString(headerSize)+ " " + StringConverter::toString(recordSize));
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

	string res = ConvertMocapDataToString( SensorData );
	return ;
	
}
*/

//string NxMocap_Recorder::ConvertMocapDataToString( float SensorData[16][6] )
//{
//	// depreciated.
//	string MoCapString ;
//	return MoCapString ;
//}
//
//void NxMocap_Recorder::ListenersUpdate()
//{
//	//update listeners if necessary.
//
//}

}//namespace