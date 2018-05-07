@echo This will clean up all intermediate/user-specific files for NxGraphics. 
@echo ** You must close Visual C++ before running this file! **

robocopy Lib NxGraphicsSdk/Lib /E
robocopy nxdeps NxGraphicsSdk/nxdeps /E
robocopy NxGraphics NxGraphicsSdk\NxGraphics *.h
robocopy NxGraphics\NxGui NxGraphicsSdk\NxGraphics\NxGui *.h
robocopy NxGraphics\Physics\empty NxGraphicsSdk\NxGraphics\Physics\empty *.h
robocopy NxDevice NxGraphicsSdk\NxDevice *.h
robocopy NxSound NxGraphicsSdk\NxSound *.h
robocopy NxVideo NxGraphicsSdk\NxVideo *.h
robocopy NxMocap NxGraphicsSdk\NxMocap *.h
robocopy NxNetwork NxGraphicsSdk\NxNetwork *.h
@echo ============== Done! ===============
pause