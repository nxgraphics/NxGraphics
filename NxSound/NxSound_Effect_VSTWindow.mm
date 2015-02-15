#include "NxSoundPreCompiled.h"
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"

// If you get an error here, you might need to make sure that
// your build config files don't specify "C++" as one of the
// flags in OTHER_CFLAGS, because that stops the compiler building
// obj-c files correctly.
@class dummyclassname;

// Horrible carbon-based fix for a cocoa bug, where an NSWindow that wraps a carbon
// window fails to keep its position updated when the user drags the window around..
#define WINDOWPOSITON_BODGE 1

#if WINDOWPOSITON_BODGE
#include <Carbon/Carbon.h>
#endif

#include <Cocoa/Cocoa.h>
static pascal OSStatus windowHandler (EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void* inUserData);
static pascal void idleTimerProc (EventLoopTimerRef inTimer, void* inUserData);


namespace NxSound_Namespace {

	NxSoundEffectVSTWindow::NxSoundEffectVSTWindow( NxSoundEffectVSTPlugin * VstPlugin ) : mPlugin( VstPlugin )
	{




	}

	NxSoundEffectVSTWindow::~NxSoundEffectVSTWindow()
	{


	}
	
	void NxSoundEffectVSTWindow::ShowDialog()
	{

		NSApplicationLoad(); 

/*
NSWindow* window = [self window];

NSRect window_frame = [window frame];

NSView* cv = [[[NSView alloc] initWithFrame:window_frame] autorelease];
[window setContentView:cv];

mPlugin->GetEffect()->dispatcher (mPlugin->GetEffect(), effEditOpen, 0, 0, reinterpret_cast<void*>(cv), 0);

*/
/*
		NSRect windowRect = NSMakeRect(10.0f, 10.0f, 800.0f, 600.0f);
		NSWindow *window = [[NSWindow alloc]
		initWithContentRect:windowRect styleMask:
		( NSResizableWindowMask | NSClosableWindowMask |NSTitledWindowMask)
		backing:NSBackingStoreBuffered defer:NO];


VstIntPtr vstDispatchReturn = dispatch(effEditOpen, 0, 0, reinterpret_cast<void*>(inNSView), 0);
*/


	}

	//-------------------------------------------------------------------------------------------------------
	pascal void idleTimerProc (EventLoopTimerRef inTimer, void *inUserData)
	{
		AEffect* effect = (AEffect*)inUserData;
		effect->dispatcher (effect, effEditIdle, 0, 0, 0, 0);
	}

	//-------------------------------------------------------------------------------------------------------
	pascal OSStatus windowHandler (EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
	{
		OSStatus result = eventNotHandledErr;
		WindowRef window = (WindowRef) inUserData;
		UInt32 eventClass = GetEventClass (inEvent);
		UInt32 eventKind = GetEventKind (inEvent);

		switch (eventClass)
		{
		case kEventClassWindow:
			{
				switch (eventKind)
				{
				case kEventWindowClose:
					{
						//QuitAppModalLoopForWindow (window);
						break;
					}
				}
				break;
			}
		}

		return result;
	}	


}