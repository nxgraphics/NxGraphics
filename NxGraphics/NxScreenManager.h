/*
-----------------------------------------------------------------------------
This file is part of NxGraphics, Multimedia Engine.
Author : Stephane Kyles. Developed in Prague, Czech Republic.
		_   __       ______                     __     _            
	   / | / /_  __ / ____/_____ ____ _ ____   / /_   (_)_____ _____
	  /  |/ /| |/_// / __ / ___// __ `// __ \ / __ \ / // ___// ___/
	 / /|  /_>  < / /_/ // /   / /_/ // /_/ // / / // // /__ (__  ) 
	/_/ |_//_/|_| \____//_/    \__,_// .___//_/ /_//_/ \___//____/  
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
#ifndef __NXSCREENMANAGER_H__
#define __NXSCREENMANAGER_H__

#include "NxPrerequisites.h"

namespace Nx {

enum NxGraphics_Display_Types 
{ 
	WINDOW_STYLE_DEFAULT,
	WINDOW_STYLE_PANORAMIC,
	WINDOW_STYLE_VPANORAMIC,
	WINDOW_STYLE_STRETCHED
};


/*
typedef struct NxtagPOINT
{
	long  x;
	long  y;
} NXMONITORPOINT;
*/


struct NxMonitorInfo
{
	int id;
	char Monitor_Name[255];
	char Adapter_Name[255];
	char Device_Name[255];
	long Pos_X;
	long Pos_Y;
	unsigned long Width;
	unsigned long Height;
	unsigned long Refresh_Rate;
	unsigned long Bits_Per_Pixel ;
	bool Is_Primary;
	bool Is_Disabled;
	bool Is_Removable;
	long Workspace_X[2];
	long  Workspace_Y[2];
	void * Monitor_ID ;

};

class NxGraphics_Export NxMonitor
{
public:
	//! NxMonitor class Constructor.
	NxMonitor( NxMonitorInfo * );
	//! NxMonitor class Destructor.
	~NxMonitor();
	//! Monitor Id.
	int GetId();
	//! Get Monitor Name.
	const std::string GetMonitorName() const;
	//! Get Adapter Name.
	const std::string GetAdapterName() const;
	//! Get Device Name.
	const std::string GetDeviceName() const;
	//! Get Monitor Position X.
	long GetPositionX();
	//! Get Monitor Position Y.
	long GetPositionY();
	//! Get Monitor Width in Pixels.
	unsigned long GetWidth();
	//! Get Monitor Height in Pixels.
	unsigned long GetHeight();
	//! Get Monitor Bits Per Pixels.
	unsigned int GetBpp();
	//! Get Monitor Refresh Rate.
	unsigned int GetRefreshRate();
	//! Monitor Is Primary.
	bool IsPrimary();
	//! Monitor Is Disabled.
	bool IsDisabled();
	//! Monitor Is Removable.
	bool IsRemovable();

private:
	long Workspace_X[2] ;
	long Workspace_Y[2];
	void * Monitor_ID ;
	NxMonitorInfo * mMonitorInfo;

};



class NxGraphics_Export NxScreenManager : public NxSingleton<NxScreenManager>
{
public :
	//! NxScreenManager class Constructor.
	NxScreenManager();
	//! NxScreenManager class Destructor.
	~NxScreenManager();
	//! NxScreenManager Singleton.
	static NxScreenManager& getSingleton();
	//! NxScreenManager Singleton pointer
	static NxScreenManager* getSingletonPtr();
	//! Get Device Name From Index.
	void GetDeviceNameFromIndex( std::string & DevName, unsigned int Index );	
	//! Create Screen on Monitor;
	NxScreen * CreateExternalWindow( int MonitorID, bool FullScreen, unsigned int Width = 800, unsigned int Height = 600 );
	//! Create a Window Holder.
	void * CreateWindowHolder( NxMonitor * Monitor, bool Fullscreen );
	//! Delete screen.
	void DeleteExternalWindow( NxScreen * Output );
	//! Get Number of created NxScreens.
	unsigned int GetNumScreens();
	//! Get Available Monitor List.
	void GetMonitorList( std::vector< NxMonitor * > & MonitorList ); 
	//! Get Created NxScreens List.
	void GetActiveWindowsList( std::list< NxScreen *> & ScreenList ); 
	//! Update texture for all windows.
	void Update();
	//! Delete All windows.
	void DeleteWindows();
	//! Get Monitor.
	NxMonitor * GetMonitor( int index );
	//! Get Screen.
	NxScreen & GetScreen( int Index );
	//! Set Multi Display Mode.
	void SetMultiDisplayMode( NxGraphics_Display_Types DisplayType );
	//! Get Multi Display Mode.
	NxGraphics_Display_Types GetMultiDisplayMode();
	//! Build Monitor List
	void BuildMonitorsInfo();
	//! Build Monitor Screen Order;
	void RebuiltScreenOrder();
	//! Effects Scroll.
	void SetAnimationScroll( float U, float V);
	//! Effects Rotate.
	void SetAnimationRotate( float Speed );
	//! Build scaled camera matrix
	Nx::Matrix4 BuildScaledOrthoMatrix( float left, float right, float bottom, float top, float near_plane, float far_plane );

private :
	//! Available Monitor List.
	std::vector< NxMonitor * > MonitorList;
	//! Active Screen List.
	std::list< NxScreen * > MonitorListActive; 
	//! Current Multi Display Mode.
	NxGraphics_Display_Types mMultiDisplayMode;
	//! Main Screen Rtt.
	Ogre::RenderTexture * mRenderTexture;
};

} // NxGraphics_Namespace 
#endif