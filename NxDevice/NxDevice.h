/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
				_   __       ____               _           
			   / | / /_  __ / __ \ ___  _   __ (_)_____ ___ 
			  /  |/ /| |/_// / / // _ \| | / // // ___// _ \
			 / /|  /_>  < / /_/ //  __/| |/ // // /__ /  __/
			/_/ |_//_/|_|/_____/ \___/ |___//_/ \___/ \___/ 
			                                                
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
#ifndef __NXDEVICE_H__
#define __NXDEVICE_H__

#include "NxDevice_Singleton.h"
#include "NxDevice_Hardware.h"

namespace NxDevice_Namespace {

class NxDevice_Export NxDeviceManagerListener  
{
public :
	virtual void NxDeviceReceiveMidiCallback( unsigned long DeviceID , NxMidiTimestamp timestamp , int type , int channel , int data1 , int data2 ){}
	virtual void NxDeviceReceiveOscMessage( unsigned long DeviceID, const NxDeviceOscMessages& m  ){}
};


class NxDevice_Export NxDeviceHardwareDesc
{
public:
	//! NxDeviceHardwareDesc class Constructor.
	inline NxDeviceHardwareDesc(){};
	//! Device Type.
	NxDevice_Hardware_Types DevType;
	//! Device SubType.
	NxDevice_Hardware_SubTypes DevSubType;
	//! Device Index.
	unsigned long DevIndex;
	//! Device Device Name.
	std::string DevName;
};

class NxDevice_Export NxDeviceManager : public Singleton<NxDeviceManager>
{
public :
	//! NxDeviceManager Class Constructor.
	NxDeviceManager(); 
	//! NxDeviceManager Class Deconstructor.
	~NxDeviceManager(); 
	//! NxDeviceManager Class Singleton.
	static NxDeviceManager& getSingleton();
	//! NxDeviceManager Class Singleton Pointer.
	static NxDeviceManager* getSingletonPtr();
	//! Create a device with specified parameters.
	NxDevice_Hardware * CreateDevice( const NxDeviceHardwareDesc& hardwareDesc );
	//! Delete a device.
	bool DeleteDevice( NxDevice_Hardware * Device );
	//! Delete All Devices.
	bool DeleteDevices();
	//! Add a listener.
	void AddListener( NxDeviceManagerListener * listener );
	//! Remove a listener.
	void RemoveListener( NxDeviceManagerListener * listener );
	//! Get Number of Available Dmx Ouputs.
	int GetNumDmxDevices();
	//! Get Device From Id.
	NxDevice_Hardware * GetDeviceFromID( unsigned long ID );
	//! Get Midi Manager.
	NxMidiManager * GetMidiManager();
	//!Get Device List.
	void GetDeviceList( std::vector<NxDevice_Hardware *> & List );
	//! Get Num Created Devices.
	size_t GetNumDevices();
	//! Generate an Id;
	unsigned long GenerateId();

	std::list<NxDeviceManagerListener *> Listeners_List;
	std::vector<NxDevice_Hardware *> Device_List;


private:
	NxMidiManager * mMidiManager;
	//MTRand * IdGenerator ;



};



} //NxDevice_Namespace


#endif

 

