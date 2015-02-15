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
#ifndef __NXMOCAPDEVICES_H__
#define __NXMOCAPDEVICES_H__

#include "NxMocap_Prerequisites.h"

namespace NxMocap_Namespace {

typedef struct NxMocapImageRGB {
	unsigned char nRed;
	unsigned char nGreen;
	unsigned char nBlue;
} NxMocapImageRGB;

typedef unsigned short NxMocapImageDepth;
typedef unsigned short NxMocapImageIr;

//! Base class for all MocapDevices.
class NxMocap_Export NxMocapDevice
{
protected :
	//! NxMocapDevice Class Constructor.
	NxMocapDevice();
public :
	//! NxMocapDevice Class Destructor.
	virtual ~NxMocapDevice();
	//! Connect MocapDevice.
	virtual bool ConnectDevice() = 0;
	//! Disconnect MocapDevice.
	virtual bool DisconnectDevice() = 0;
	//! update Mocap Device.
	virtual bool Update() = 0;

};

class NxMocap_Export NxMocapDeviceKinect : public NxMocapDevice
{
protected :
	//! NxMocapDevice Class Constructor.
	NxMocapDeviceKinect();
public :
	//! NxMocapDevice Class Destructor.
	virtual ~NxMocapDeviceKinect();
	virtual bool ConnectDevice( bool useColor, bool useDepth, bool useInfrared ) = 0;
	virtual bool HasNewDepth() = 0 ;
	virtual bool HasNewColor() = 0;
	virtual bool HasNewInfraRed() = 0;
	virtual bool IsUsingInfraRedMap() = 0;  
	virtual const NxMocapImageDepth * GetDepthMap() const = 0;
	virtual const NxMocapImageRGB * GetImageMap() const = 0;
	virtual const NxMocapImageIr * GetInfraRedMap() const = 0;
	virtual void ConvertDepthToColor( long depthX, long depthY, NxMocapImageDepth DepthZ, long * ColorX, long * ColorY ) = 0;
	virtual bool GetDepthToColorMap( const NxMocapImageDepth * src, long * dst ) const = 0;
};

#ifdef USE_OPENNI2 
//! Kinect MoCap Device Class.
class NxMocap_Export NxMocapDeviceKinectOpenNi : public NxMocapDeviceKinect
{
protected:
	//! NxMocapDeviceKinect Class Constructor.
	NxMocapDeviceKinectOpenNi();
public :
	//! NxMocapDeviceKinect Class Destructor.
	~NxMocapDeviceKinectOpenNi();
	//! Connect Kinect Device.
	bool ConnectDevice();
	//! Connect Kinect Device.
	bool ConnectDevice( bool useColor, bool useDepth, bool useInfrared );
	//! Disconnect Kinect Device.
	bool DisconnectDevice();
	//! has new depth map.
	bool HasNewDepth();
	//! has new color map.
	bool HasNewColor();
	//! has new infrared map.
	bool HasNewInfraRed();
	//! is InfraRedMap Enabled.
	bool IsUsingInfraRedMap(); 
	//! Update and refresh Kinect Data.
	bool Update();
	//! Get Depth Map.
	const NxMocapImageDepth * GetDepthMap() const;
	//! Get Image Map.
	const NxMocapImageRGB * GetImageMap() const;
	//! Get Infrared map
	const NxMocapImageIr * GetInfraRedMap() const;
	//! convert depth to color map
	void ConvertDepthToColor( long depthX, long depthY, NxMocapImageDepth DepthZ, long * ColorX, long * ColorY );
	//! convert depth to color coordinates
	bool GetDepthToColorMap( const NxMocapImageDepth * src, long * dst ) const;

private:
	//! Kinect Device.
	KinectPlayerOpenNi * mKinect;
	friend class NxMocapManager;
};
#endif

#ifdef USE_MICROSOFTKINECT  
class NxMocap_Export NxMocapDeviceKinectMicrosoft : public NxMocapDeviceKinect
{
protected :
	//! NxMocapDeviceKinect Class Constructor.
	NxMocapDeviceKinectMicrosoft();
public :
	//! NxMocapDeviceKinect Class Destructor.
	~NxMocapDeviceKinectMicrosoft();
	//! Connect Kinect Device.
	bool ConnectDevice();
	//! Connect Kinect Device.
	bool ConnectDevice( bool useColor, bool useDepth, bool useInfrared );
	//! Disconnect Kinect Device.
	bool DisconnectDevice();
	//! has new depth map.
	bool HasNewDepth();
	//! has new color map.
	bool HasNewColor();
	//! has new infrared map.
	bool HasNewInfraRed();
	//! is InfraRedMap Enabled.
	bool IsUsingInfraRedMap(); 
	//! Update and refresh Kinect Data.
	bool Update();
	//! Get Depth Map.
	const NxMocapImageDepth * GetDepthMap() const;
	//! Get Image Map.
	const NxMocapImageRGB * GetImageMap() const;
	//! Get Infrared map
	const NxMocapImageIr * GetInfraRedMap() const;
	//! convert depth to color map
	void ConvertDepthToColor( long depthX, long depthY, NxMocapImageDepth DepthZ, long * ColorX, long * ColorY );
	//! convert depth to color coordinates
	bool GetDepthToColorMap( const NxMocapImageDepth * src, long * dst ) const;

private:
	//! Kinect Device.
	KinectPlayerMicrosoft * mKinect;
	friend class NxMocapManager;
};
#endif



} //namespace

#endif