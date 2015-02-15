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
#ifndef __NXMOCAPSTREAM_H__
#define __NXMOCAPSTREAM_H__

#include "NxMocap_Prerequisites.h"

namespace NxMocap_Namespace {

class NxMocap_Export NxMocapNetStreamClient
{
public:
	NxMocapNetStreamClient();
	~NxMocapNetStreamClient();
	void CreateStream( const std::string & IP, int port );
	void DeleteStream();
	int SendData( unsigned char * Data, int Size );
	const std::string & GetIp() const;
	int GetPort();
private :
	SocketClient * mSocketClient;
	std::string mDestinationIp;
	int mDestinationPort;
};

class NxMocap_Export NxMocapNetStreamServer
{
public:
	NxMocapNetStreamServer();
	~NxMocapNetStreamServer();
	void CreateStream( int port );
	void DeleteStream();
	int SendData( unsigned char * Data, int Size );
	int GetPort();
private :
	SocketServer * mSocketServer;
	int mDestinationPort;
};



}//namespace

#endif