/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __      _____                           __
			   / | / /_  __/ ___/ ____   __  __ ____   ____/ /
			  /  |/ /| |/_/\__ \ / __ \ / / / // __ \ / __  / 
			 / /|  /_>  < ___/ // /_/ // /_/ // / / // /_/ /  
			/_/ |_//_/|_|/____/ \____/ \__,_//_/ /_/ \__,_/   
			                                                  
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
#ifndef __NXSOUNDPLUGINEFFECTOGGBROADCASTER_H__
#define __NXSOUNDPLUGINEFFECTOGGBROADCASTER_H__

#include "NxSound_Prerequisites.h"

enum TypeSocket
{
	BlockingSocket,
	NonBlockingSocket
};

namespace NxSound_Namespace {

class NxSound_Export NxSoundEffectOggBroadCaster : public NxSoundEffect
{
	struct ogg_page;
public:
	NxSoundEffectOggBroadCaster();
	~NxSoundEffectOggBroadCaster();

	void CreateServer( int Port );
	void SendOggFileHeader( int Sock );
	bool Initialized ;
	int port ;
	//virtuals
	void Process( float ** BufIn , float ** BufOut , int BlockSize );
	int write_page ( ogg_page * page, int sock, bool flush );

	stk::TcpServer * Server ;
	std::vector<int> ServerSocketList ;
 
private :
	//unsigned long SocketID ;
	//void * SocketHandle ; 
	//friend unsigned long StreamUpdater(void * lpParameter);
	//friend unsigned long StreamReceiver(void * lpParameter);
	struct encoder_s * OggEncoder ;
	stk::Thread * mSender;
};

}

#endif