#include "NxVideoPreCompiled.h"

namespace NxVideo_Namespace {

struct NxVideoDeviceQTM
{
public:

	NxVideoDeviceQTM()
	{

	}

	~NxVideoDeviceQTM()
	{

	}

} ; 

NxVideoGrabberQuicktime::NxVideoGrabberQuicktime() : NxVideo_Grabber()
{
	mType = NxVideoCaptureQuicktime ;
	mVideo = new NxVideoDeviceQTM();
}

NxVideoGrabberQuicktime::~NxVideoGrabberQuicktime()
{

}

bool NxVideoGrabberQuicktime::OpenVideoDevice( unsigned long Index, int Width, int Height, int BPP )
{

	return true;
}

bool NxVideoGrabberQuicktime::CloseVideoDevice()
{

	return true;
}

unsigned char * NxVideoGrabberQuicktime::GetBuffer()
{
	return 0;
}

bool NxVideoGrabberQuicktime::NewFrame()
{
	return true;
}


}
 