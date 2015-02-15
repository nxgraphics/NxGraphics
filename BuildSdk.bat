@echo This will clean up all intermediate/user-specific files for NxGraphics. 
@echo ** You must close Visual C++ before running this file! **
COPY lib\Release\*.lib NxGraphicsSdk\lib\
COPY lib\Release\*.dll NxGraphicsSdk\bin\Release
COPY lib\Debug\*.dll NxGraphicsSdk\bin\Debug
COPY Bin\Release\NxGraphics.pdb NxGraphicsSdk\bin\Release
COPY Bin\Release\NxVideo.pdb NxGraphicsSdk\bin\Release
COPY Bin\Release\NxDevice.pdb NxGraphicsSdk\bin\Release
COPY Bin\Release\NxSound.pdb NxGraphicsSdk\bin\Release
COPY Bin\Release\NxMocap.pdb NxGraphicsSdk\bin\Release
COPY Bin\Release\NxNetwork.pdb NxGraphicsSdk\bin\Release
COPY Bin\Debug\NxGraphics_d.pdb NxGraphicsSdk\bin\Debug
COPY Bin\Debug\NxVideo_d.pdb NxGraphicsSdk\bin\Debug
COPY Bin\Debug\NxDevice_d.pdb NxGraphicsSdk\bin\Debug
COPY Bin\Debug\NxSound_d.pdb NxGraphicsSdk\bin\Debug
COPY Bin\Debug\NxMocap_d.pdb NxGraphicsSdk\bin\Debug
COPY Bin\Debug\NxNetwork_d.pdb NxGraphicsSdk\bin\Debug
COPY NxGraphics\*.h NxGraphicsSdk\NxGraphics
COPY NxGraphics\Physics\Physx_2.8.4\*.h NxGraphicsSdk\NxGraphics\Physics\Physx_2.8.4
COPY NxGraphics\NxGui\*.h NxGraphicsSdk\NxGraphics\NxGui
COPY NxDevice\*.h NxGraphicsSdk\NxDevice
COPY NxSound\*.h NxGraphicsSdk\NxSound
COPY NxVideo\*.h NxGraphicsSdk\NxVideo


COPY NxMocap\*.h NxGraphicsSdk\NxMocap

COPY NxNetwork\*.h NxGraphicsSdk\NxNetwork
@echo ============== Done! ===============