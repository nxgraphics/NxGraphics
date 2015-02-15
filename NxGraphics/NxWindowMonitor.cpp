#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxMonitor::NxMonitor( NxMonitorInfo * Monitor ) : mMonitorInfo( Monitor )
{

}

NxMonitor::~NxMonitor()
{

}

const std::string NxMonitor::GetMonitorName() const
{
	return std::string( mMonitorInfo->Monitor_Name );
}

const std::string NxMonitor::GetAdapterName() const
{
	return std::string( mMonitorInfo->Adapter_Name );
}

const std::string NxMonitor::GetDeviceName() const
{
	return std::string( mMonitorInfo->Device_Name );
}

long NxMonitor::GetPositionX()
{
	return mMonitorInfo->Pos_X;
}

long NxMonitor::GetPositionY()
{
	return mMonitorInfo->Pos_Y;
}

unsigned long NxMonitor::GetWidth()
{
	return mMonitorInfo->Width;
}

unsigned long NxMonitor::GetHeight()
{
	return mMonitorInfo->Height;
}

unsigned int NxMonitor::GetBpp()
{
	return mMonitorInfo->Bits_Per_Pixel;
}

unsigned int NxMonitor::GetRefreshRate()
{
	return mMonitorInfo->Refresh_Rate;
}

bool NxMonitor::IsPrimary()
{
	return mMonitorInfo->Is_Primary;
}

bool NxMonitor::IsDisabled()
{
	return mMonitorInfo->Is_Disabled;
}

bool NxMonitor::IsRemovable()
{
	return mMonitorInfo->Is_Removable;
}

int NxMonitor::GetId()
{
	return mMonitorInfo->id;
}

}//namespace
