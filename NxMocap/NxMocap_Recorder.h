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
#ifndef __NXMOCAPRECORDER_H__
#define __NXMOCAPRECORDER_H__

#include "NxMocap_Prerequisites.h"
#include "NxMocap_Player.h"

namespace NxMocap_Namespace {

class NxMocap_Export NxMocap_Recorder_Listener
{
public :
	virtual ~NxMocap_Recorder_Listener() {}
    virtual void RecorderValueChanged ( NxMocap_Recorder * PlayerCallback ) = 0;
};

class NxMocap_Export NxMocap_Recorder
{
public :

	NxMocap_Recorder();
	~NxMocap_Recorder();
	// start receiving data on socket.
	bool StartReceiving( int port );
	// stop receiving data on socket.
	void StopReceiving();
	// start recording data on database.
	bool StartRecording( const string & DatabaseName, int MysqlPort );
	// stop recording data on database.
	void StopRecording();


protected:
	// insert data in database
	void InsertData( unsigned char * data, int Size );

	/*
	int GetTotalFrameLost();
	int GetTotalInsertedFrames();
	double GetElapsedTime();
	int GetReceivePort();

	void ListenersUpdate();
	int  GetRecordingFrameRate() ;

	friend DWORD socket_thread_receiver(LPVOID lpParameter);
	friend DWORD time_thread_recorder(LPVOID lpParameter);

	void ParseMocapPacketData( BYTE * Bytes, long count);
	//mysql recorder
	string ConvertMocapDataToString( float SensorData[16][6] );
	*/


private :

//SocketServer * SockServer;

/*
		bool IsReceiving ;
		
		string RecordingDatabaseName ;
		int Port_Receiver ; // default port receiver
		bool Receiver_Should_Run  ;
		bool IsRecording ;
		HANDLE socket_thread_Handle  ;
		HANDLE time_thread_Handle  ;
		int Totalframeslost ;
		int Totalinsertedframes ;
		precision_timer * RecordingTimer ;
		*/

};

/*
class NxMocap_Export NxMocapRecorderPolhemus : public NxMocap_Recorder
{
public :

	NxMocap_Recorder();
	~NxMocap_Recorder();
 
	int Port_Receiver ; // default port receiver
	bool Receiver_Should_Run  ;
	bool IsRecording ;
	HANDLE socket_thread_Handle  ;
	HANDLE time_thread_Handle  ;
	int Totalframeslost ;
	int Totalinsertedframes ;
	precision_timer * RecordingTimer ;
	void ListenersUpdate();
	int  GetRecordingFrameRate() ;
	SocketServer * SockServer ;
	string RecordingDatabaseName ;
	friend DWORD socket_thread_receiver(LPVOID lpParameter);
	friend DWORD time_thread_recorder(LPVOID lpParameter);
	void ParseMocapPacketData( BYTE * Bytes , long count);
	//mysql recorder
	bool StartRecording( string DatabaseName, int port );
	void StopRecording();
	void InsertMysqlMocapData( BYTE *  data, int count  );
	string ConvertMocapDataToString( float SensorData[16][6] );
	//reveicer
	bool StartReceiving( int port );
	void StopReceiving();

	bool IsReceiving ;
private :


};
*/


}


#endif