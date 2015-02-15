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

#ifndef __NXMOCAP_H__
#define __NXMOCAP_H__
 
#include "NxMocap_Singleton.h"

namespace NxMocap_Namespace {

class NxMocap_Export NxMocapManagerListener
{
public :
	virtual ~NxMocapManagerListener(){}
	virtual void PlayerValueChanged( unsigned long ID, const NxMocapDataList & List ) = 0;
	virtual void RecorderValueChanged( unsigned long ID, const NxMocapDataList & List ) = 0;
};

class NxMocap_Export NxMocapDeviceDesc
{
public:
	NxMocapDeviceTypes Type;
	char DevName[255];
	int DevIndex;

	NxMocapDeviceDesc() : DevIndex(-1) {
	
	}

};

class NxMocap_Export NxMocapManager : public Singleton< NxMocapManager >
{
public :
	//! NxMocapManager class Constructor.
	NxMocapManager();
	//! NxMocapManager class Destructor.
	~NxMocapManager();
	//! NxMocapManager Singleton.
	static NxMocapManager& getSingleton();
	//! NxMocapManager Singleton.
	static NxMocapManager* getSingletonPtr();
	//! Create a Mocap device.
	NxMocapDeviceKinect * CreateDeviceKinect( const NxMocapDeviceDesc & Dev );
	//! create polhemus
	NxMocapPlayerPolhemus * CreatePlayerPolhemus();
	//! Delete a Mocap Player.
	void DeletePlayer( NxMocapPlayer * player );
	//! Get a Player by Index.
	NxMocapPlayer * GetPlayer( int Index );
	//! Get total number of players Created.
	size_t GetNumPlayers();
	//! Release a Player By Index.
	void ReleasePlayer( int Index );
	//! Create a Mocap Recorder.
	NxMocap_Recorder * CreateRecorder();
	//! Delete a Mocap Recorder.
	void DeleteRecorder( NxMocap_Recorder * recorder );
	//! Get total number of recroders Created.
	size_t GetNumRecorders();
	//! Get Recorder By Index.
	NxMocap_Recorder * GetRecorder( int Index );
	//! release Recorder By Index.
	void ReleaseRecorder( int index );
	size_t GetNumTotalStreams();// get total num streams running ( recorder + player )
	void GetStreamList( std::list<string> & list );

	void AddListener( NxMocapManagerListener * listener );
	void RemoveListener( NxMocapManagerListener * listener );

	std::list<NxMocapManagerListener *> Listeners_List;	
	std::list<NxMocapPlayer *>& GetList();

private:
	std::list< NxMocapPlayer * > PlayerList;
	std::list< NxMocap_Recorder * > RecorderList;
	//! Full list.
	std::list<NxMocapPlayer *> FullNxMocapList ;
	
 

};



}

#endif
