/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __       __  ___                         
			   / | / /_  __ /  |/  /____   _____ ____ _ ____ 
			  /  |/ /| |/_// /|_/ // __ \ / ___// __ `// __ \
			 / /|  /_>  < / /  / // /_/ // /__ / /_/ // /_/ /
			/_/ |_//_/|_|/_/  /_/ \____/ \___/ \__,_// .___/ 
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
#ifndef __NXMOCAPPLAYER_H__
#define __NXMOCAPPLAYER_H__

#include "NxMocap_Prerequisites.h"

namespace NxMocap_Namespace {

class NxMocap_Export NxMocapPlayer : public NxMocapMysql, public NxMocapNetStreamClient// public NxMocapSkeleton, 
{
public :
	NxMocapPlayer();
	~NxMocapPlayer();
	virtual void ConvertToJoints();
	virtual void SetReferenceFrame( float Azimuth, float Elevation, float Roll );
	virtual void GetReferenceFrame( float &Azimuth, float &Elevation, float &Roll );
	//mysql
	void LoadInMemoryMysql();
	void MysqlSetPlaybackRate( float fps );
	float MysqlGetPlaybackRate();
	void SetLooping( bool value );
	//player
	virtual void StartPlayBack( const string & Ip, int port ) = 0;
	void StopPlayBack();
	void PausePlayback();
	void ResumePlayback();
	bool IsPlaying();
	bool IsStopped();
	bool IsPaused() const  ;
	void SeekTimePosition( float milliseconds );  
	float GetTimePosition() const ;
	void SetInboundPlayBackTime( float milliseconds );
	void SetOutboundPlayBackTime( float milliseconds );
	float GetInboundPlayBackTime();
	float GetOutboundPlayBackTime();
	float GetTotalTrackTime();
	float GetCurrentPlaybackFramerate();
 
	int GetMocapStreamIndex(); 
	void SetConfigType( NxMocapConvertPacketTypes Type   ); 
	//MYSQL
	//! convert mysql to binary
	void ConvertMocapPacket( NxMocapConvertPacketTypes ConvertType  );
	//! convert mysql to binary
	void Convert_Mysql_To_Memory();
	unsigned char * GetTimeData( float milisecond_time );
	unsigned char * GetFrameData( long frameNumber );
	bool Player_Is_TimeManual();
	void SetTimeUpdateManual( bool manual );
	NxMocapTypes GetNxMocapType();
	void SetNxMocapType( NxMocapTypes Type );
	protected :
 
	int MocapPlayerStreamIndex ;
	//private :
	NxMocapConvertPacketTypes PlayType ;//playback type
	friend unsigned long socket_thread_sender( void * lpParameter );
	friend unsigned long time_thread(void * lpParameter);
	void * Frame_thread_Handle  ;
	void * Time_thread_Handle  ;
	float PlayBackFramerate ;
	bool PlayBackLooping ;
	long Last_Frame_Num ;

	NxMocapTypes NxMocapType;

	bool Sender_Should_Run ;
	bool FramerateIndexer_Should_Run ;
	bool isCurrentlyPlaying ;
	bool isCurrentlyPaused ;

	float mTimeInbound ;
	float mTimeOutbound ;
	float mTime ;
	float Time_Index_Last ; // last rendered time index

	typedef std::map< unsigned long long, unsigned char * > Packet_Data_Type ;
	typedef std::map< unsigned long long, int > Packet_Length_Type ;
	Packet_Data_Type Packet_Data ;
	Packet_Length_Type Packet_Length ;
	bool Player_ManualTimeMode ;

protected:

	int frame_rate_index ; 
	int frame_rate_stable ;
	NxMocapDataList DataList;
 
};

//plays polhemus mocap from database
class NxMocap_Export NxMocapPlayerPolhemus : public NxMocapPlayer
{
public :
	NxMocapPlayerPolhemus();
	~NxMocapPlayerPolhemus();
	void StartPlayBack( const string & Ip, int port );

	/*
	//! Virutals
	void SetReferenceFrame( float Azimuth, float Elevation, float Roll );
	void GetReferenceFrame( float &Azimuth, float &Elevation, float &Roll );
	void ConvertToJoints();
	//!Streamer Virtuals
	
	void SetLayout( int newLayout[] );
	int * GetLayout();
	void ParseByteToFloatArray( BYTE * Bytes, long count);
	float GetSensorData ( int sensor, NxMocapSensorDataTypes Type ); 
	float GetCalibSensorData ( int sensor, NxMocapSensorDataTypes Type );
	void SetCurrentPhase( NxMocapSensorStatus Phase );
	NxMocapSensorStatus GetCurrentPhase();
	//! Hanim standard
	CHAnim * hanim;
	float SensorData[16][6];
	float CalibSensorData[16][6];
	NxMocapSensorStatus CurrentPhase ; 
	*/

};

class NxMocap_Export NxMocapPlayerKinect : public NxMocapPlayer
{
public :
	NxMocapPlayerKinect();
	~NxMocapPlayerKinect();
};

//play kinect data from database
class NxMocap_Export NxMocapPlayerBvh : public NxMocapPlayer
{
public :
	NxMocapPlayerBvh();
	~NxMocapPlayerBvh();
};






}

#endif