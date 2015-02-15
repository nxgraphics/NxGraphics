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
#ifndef __NXMOCAPPrerequisites_H__
#define __NXMOCAPPrerequisites_H__

#include <NxMocapConfig.h>
#include "NxMocap_Plateform.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

#include <iostream>
#include <map>
#include <sstream>
#include <fstream>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>


class KinectPlayerOpenNi;
class KinectPlayerMicrosoft;
 
namespace NxMocap_Namespace {
 
	enum NxMocapDeviceTypes {
		NxMocapKinectOpenNi2,
		NxMocapKinectMicrosoft
	};

	enum NxMocapTypes {  
		NxMocapPlayerType,
		NxMocapRecorderType,
		NxMocapReceiverType
	};

	enum NxMocapConvertPacketTypes {  
		Mysql_Memory_PlayBack,
		Mysql_Live_PlayBack,
		Latus_Usb_PlayBack
	};

	enum NxMocapSensorDataTypes {  
		POSX,
		POSY,
		POSZ,
		ORIX,
		ORIY,
		ORIZ
	};

	enum NxMocapSensorStatus{ 
		PhaseCalib,
		PhaseLive 
	};


	using namespace std ;

	class NxMocapManager;
	class NxMocapDeviceKinect;
	class NxMocapDevice;
	class NxMocapMysql;
	class NxSqlHelper;
	class SocketServer;
	class SocketClient;
	class NxMocapNetStreamClient;
	class NxMocapSkeleton;
	class NxMocapPlayer;
	class NxMocapPlayerPolhemus;
	class NxMocap_Recorder ;
	class Socket;

	class NxMocapPlayerBvh;
	class NxMocapUser;

	//typedef __w64 unsigned int SOCKET;

	typedef std::vector<float> NxMocapData;
	typedef std::map<int, NxMocapData *> NxMocapDataList;
 

}// NxMocap_Namespace

 

#endif