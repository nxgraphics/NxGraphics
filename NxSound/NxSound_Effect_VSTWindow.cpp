#include "NxSoundPreCompiled.h"
#define __cdecl//needed for nmake
#include "VST/public.sdk/source/vst2.x/audioeffectx.h"
 
//from
// http://www.dith.it/listing/vst_stuff/vstsdk2.4/public.sdk/samples/vst2.x/minihost/source/minieditor.cpp

namespace NxSound_Namespace {

 
	struct MyDLGTEMPLATE: DLGTEMPLATE
	{
		WORD ext[3];
		MyDLGTEMPLATE ()
		{
			memset (this, 0, sizeof (*this));
		};
	};

	static INT_PTR CALLBACK EditorProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static AEffect* theEffect = 0;
 

	NxSoundEffectVSTWindow::NxSoundEffectVSTWindow( NxSoundEffectVSTPlugin * VstPlugin ) : mPlugin( VstPlugin )
	{

	}

	NxSoundEffectVSTWindow::~NxSoundEffectVSTWindow()
	{

	}
 
	void NxSoundEffectVSTWindow::ShowDialog()
	{
		theEffect = mPlugin->GetEffect();
		MyDLGTEMPLATE t;	
		t.style = WS_POPUPWINDOW|WS_DLGFRAME|DS_MODALFRAME|DS_CENTER;
		t.cx = 100;
		t.cy = 100;
		DialogBoxIndirectParam (GetModuleHandle (0), &t, 0, (DLGPROC)EditorProc, (LPARAM) mPlugin->GetEffect());
		theEffect = 0;
	}

	INT_PTR CALLBACK EditorProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

		AEffect* effect = theEffect;

		switch(msg)
		{
			//-----------------------
		case WM_INITDIALOG :
			{
				SetWindowText (hwnd, "VST Editor");
				SetTimer (hwnd, 1, 20, 0);

				if (effect)
				{
					printf ("HOST> Open editor...\n");
					effect->dispatcher (effect, effEditOpen, 0, 0, hwnd, 0);

					printf ("HOST> Get editor rect..\n");
					ERect* eRect = 0;
					effect->dispatcher (effect, effEditGetRect, 0, 0, &eRect, 0);

					printf ("HOST> Get editor rect DONE \n");


					if (eRect)
					{
						int width = eRect->right - eRect->left;
						int height = eRect->bottom - eRect->top;
						if (width < 100)
							width = 100;
						if (height < 100)
							height = 100;

						RECT wRect;
						SetRect (&wRect, 0, 0, width, height);
						AdjustWindowRectEx (&wRect, GetWindowLong (hwnd, GWL_STYLE), FALSE, GetWindowLong (hwnd, GWL_EXSTYLE));
						width = wRect.right - wRect.left;
						height = wRect.bottom - wRect.top;

						printf ("HOST> Set Window POS \n");
						SetWindowPos (hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
					}
				}
			}	break;

			//-----------------------
		case WM_TIMER :

			//printf ("HOST> time \n");
			if (effect)
				effect->dispatcher (effect, effEditIdle, 0, 0, 0, 0);
			break;

			//-----------------------
		case WM_CLOSE :
			{
				KillTimer (hwnd, 1);

				printf ("HOST> Close editor..\n");
				if (effect)
					effect->dispatcher (effect, effEditClose, 0, 0, 0, 0);

				EndDialog (hwnd, IDOK);
			}	break;
		}

		return 0;
	}

}