# NXGRAPHICS CMake build file
cmake_minimum_required(VERSION 2.6)

project(NxGraphics)

set(NxGraphics_LIBRARY NxGraphics)

set( NxGraphicsPhysicEngine "" )
set( NxGraphicsPlatform "" )

if( MSVC )

set( NxGraphicsPhysicEngine "Physx_2.8.4" )
set( NxGraphicsPlatform "Win32" )
message( "Windows : Setting physic system to : Physx_2.8.4"  )

elseif( APPLE )

set( NxGraphicsPhysicEngine "empty" )
set( NxGraphicsPlatform "OSX" )
message( "MAC : Setting physic system to : empty"  )

elseif(ANDROID)	
message( "NxGraphics detected Android !"  )
set( NxGraphicsPhysicEngine "empty" )
set( NxGraphicsPlatform "Android" )
message( "Setting physic system to : empty"  )

elseif( UNIX )

set( NxGraphicsPhysicEngine "empty" )
set( NxGraphicsPlatform "Android" )
message( "UNIX : Setting physic system to : empty"  )

else()

set( NxGraphicsPhysicEngine "empty" )
set( NxGraphicsPlatform "OSX" )
message( "Error Unknown operating system ! : Setting physic system to : empty"  )

endif()


set(NxGraphics_src
./NxGraphicsPreCompiled.cpp
./NxContentManager.cpp
./NxColourValue.cpp
./NxGraphics.cpp
./NxFontManager.cpp
./NxInputManager.cpp
./NxLog.cpp
./NxTextureManager.cpp
./NxPlatform.cpp
./NxRecorder.cpp
./NxStateManager.cpp
./NxUtils.cpp
./NxParserXml.cpp
./NxParserJson.cpp
./NxMath.cpp
./NxMatrix3.cpp
./NxMatrix4.cpp
./NxQuaternion.cpp
./NxVector2.cpp
./NxVector3.cpp
./NxVector4.cpp
./NxStringConverter.cpp
./NxSceneManager.cpp
./NxMeshManager.cpp
./NxMeshManagerUtils.cpp
./NxSceneShadows.cpp
./NxObject.cpp
./NxText.cpp
./NxLine.cpp
./NxPatch.cpp
./NxTriangleList.cpp
./NxLight.cpp
./NxCamera.cpp
./NxEntityCharacter.cpp
./NxEntityCharacterController.cpp
./NxEntity.cpp
./NxNode.cpp
./NxTerrainTree.cpp
./NxTerrain.cpp
./NxTerrainPagedEntity.cpp
./NxTerrainGrass.cpp
./NxTerrainGrassLayer.cpp
./NxEntitySound.cpp
./NxParticles.cpp
./NxScene.cpp
./NxSceneEmpty.cpp
./NxSceneOgreMax.cpp
./NxSceneOgreMaxCallback.cpp
./NxSceneOgreMaxParsing.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsScene.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsSceneDebug.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsManager.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsActor.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsClothe.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsClothes.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsController.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsFluid.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsFluidEmitter.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsJointD6.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsJointRevolute.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsMagnet.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShape.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeBox.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeCapsule.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeMesh.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapePlane.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsVehicule.cpp
./NxSceneGizmo.cpp
./NxParticleEmitter.cpp
./NxParticleEmitterJong.cpp
./NxParticles.cpp
./NxParticleSystem.cpp
./NxParticleSystemAffector.cpp
./NxParticleSystemAffectorInterpolator.cpp
./NxRenderables.cpp
./NxRenderablesDelaunay.cpp
./NxRenderablesLines.cpp
./NxRenderablesParticles.cpp
./NxRenderablesText.cpp
./NxRenderablesTriangles.cpp
./${NxGraphicsPlatform}/NxWindowManager.cpp
./${NxGraphicsPlatform}/NxWindowScreen.cpp
./NxWindowMonitor.cpp
./NxAnimation.cpp
./NxKeyFrame.cpp
./NxAnimationManager.cpp
./NxScript.cpp
./NxScriptLua.cpp
./${NxGraphicsPlatform}/NxPluginDescription.cpp
./${NxGraphicsPlatform}/NxPluginEffect.cpp
./${NxGraphicsPlatform}/NxPluginEffectFreeframe.cpp
./NxPluginManager.cpp
./NxPluginParam.cpp
./NxPluginParamButton.cpp
./NxPluginParamColorButton.cpp
./NxPluginParamComboBox.cpp
./NxPluginParamSlider.cpp
./NxFrameBuffer.cpp
./NxFrameBuffer3D.cpp
./NxFrameBufferCamera.cpp
./NxFrameBufferCanvas.cpp
./NxFrameBufferImage.cpp
./NxFrameBufferVideo.cpp
./NxPostEffect.cpp
./NxPostEffectManager.cpp
./NxPostEffectDof.cpp
./NxPostEffectGaussianBlur.cpp
./NxPostEffectHdr.cpp
./NxOverlay.cpp
./NxOverlayManager.cpp
./NxOverlayProgressBar.cpp
./NxImportXfl.cpp
./NxMaterial.cpp
./NxMaterialManager.cpp
./NxPass.cpp
./NxShader.cpp
./NxTechnique.cpp
./NxTextureUnit.cpp
./NxImage.cpp
./NxImageCanvas.cpp
./NxImageCanvasGradient.cpp
./NxImageCanvasPath.cpp
./NxImageCanvasPattern.cpp
./NxImageCanvasText.cpp
./NxImageCanvasTransforms.cpp
./NxLayer3D.cpp
./NxLayerBase.cpp
./NxLayerCamera.cpp
./NxLayerImage.cpp
./NxLayerStream.cpp
./NxLayerVideo.cpp
./NxLayerWeb.cpp
./NxStateIntro.cpp
./NxEngine.cpp
./NxSceneManager.cpp
./NxViewport.cpp
./NxWindow.cpp
./NxRectangle2D.cpp
./NxGui/NxGuiButton.cpp
./NxGui/NxGuiButtonToggle.cpp
./NxGui/NxGuiButtonRadio.cpp
./NxGui/NxGuiColours.cpp
./NxGui/NxGuiElement.cpp
./NxGui/NxGuiLabel.cpp
./NxGui/NxGuiLine.cpp
./NxGui/NxGuiManager.cpp
./NxGui/NxGuiPanel.cpp
./NxGui/NxGuiRectangle.cpp
./NxGui/NxGuiSlider.cpp
./NxGui/NxGuiText.cpp
./NxGui/NxGuiWidget.cpp
./NxGui/NxGuiDragAndDropContainer.cpp
./NxGui/NxGuiButton.h
./NxGui/NxGuiButtonToggle.h
./NxGui/NxGuiButtonRadio.h
./NxGui/NxGuiColours.h
./NxGui/NxGuiElement.h
./NxGui/NxGuiLabel.h
./NxGui/NxGuiLine.h
./NxGui/NxGuiManager.h
./NxGui/NxGuiPanel.h
./NxGui/NxGuiRectangle.h
./NxGui/NxGuiSlider.h
./NxGui/NxGuiText.h
./NxGui/NxGuiWidget.h
./NxGui/NxGuiDragAndDropContainer.h
./NxGui/NxGuiDragAndDropTarget.h
./NxGraphics.h
./NxEnums.h
./NxGraphics_Headers.h
./NxImportXfl.h
./NxListeners.h
./NxPlatform.h
./NxPrerequisites.h
./NxParserXml.h
./NxParserJson.h
./NxSingleton.h
./NxUtils.h
./NxGraphicsPreCompiled.h
./NxSceneShadows.h
./NxFrameBuffer.h
./NxFrameBufferRecorder.h
./NxLog.h
./NxMaterialCubeReflection.h
./NxRenderables.h
./NxPostEffects.h
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsActor.h
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsManager.h
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsDraw.h
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsInclude.h
./Physics/${NxGraphicsPhysicEngine}/NxPhysxCallBack.h
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsScene.h
./NxEngine.h
./NxContentManager.h
./NxViewport.h
./NxWindow.h
./NxAnimation.h
./NxAnimationManager.h
./NxSceneGizmo.h
./NxSceneManager.h
./NxMeshManager.h
./NxObject.h
./NxEntity.h
./NxEntityCharacter.h
./NxEntityCharacterController.h
./NxEntityCloth.h
./NxEntityFluid.h
./NxEntityVehicule.h
./NxTerrainGrass.h
./NxTerrain.h
./NxTerrainTree.h
./NxParticleEmitter.h
./NxParticleSystem.h
./NxParticleSystemAffector.h
./NxLight.h
./NxCamera.h
./NxNode.h
./NxEntitySound.h
./NxScene.h
./NxLine.h
./NxTriangles.h
./NxPostEffectManager.h
./NxPlugin.h
./NxOverlay.h
./NxOverlayContainer.h
./NxOverlayElement.h
./NxOverlayText.h
./NxOverlayManager.h
./NxLayer.h
./NxMaterialManager.h
./NxMaterial.h
./NxTechnique.h
./NxPass.h
./NxTextureUnit.h
./NxShader.h
./NxTextureManager.h
./NxImage.h
./NxTexture.h
./NxInputManager.h
./NxFontManager.h
./NxScreen.h
./NxScreenManager.h
./NxScriptManager.h
./NxStateManager.h
./NxMathAsm.h
./NxMath.h
./NxMatrix3.h
./NxMatrix4.h
./NxQuaternion.h
./NxVector2.h
./NxVector3.h
./NxVector4.h
./NxStringConverter.h
./NxRectangle2D.h
../NxDeps/include/OgreMax/OgreMaxModel.hpp
../NxDeps/include/OgreMax/OgreMaxPlatform.hpp
../NxDeps/include/OgreMax/OgreMaxRenderWindowIterator.hpp
../NxDeps/include/OgreMax/OgreMaxScene.hpp
../NxDeps/include/OgreMax/OgreMaxTypes.hpp
../NxDeps/include/OgreMax/OgreMaxUtilities.hpp
../NxDeps/include/OgreMax/ProgressCalculator.hpp
../NxDeps/include/OgreMax/Version.hpp
../NxDeps/include/OgreMax/OgreMaxModel.cpp
../NxDeps/include/OgreMax/OgreMaxScene.cpp
../NxDeps/include/OgreMax/OgreMaxUtilities.cpp
../NxDeps/include/OgreMax/ProgressCalculator.cpp
../NxDeps/include/OgreMax/Version.cpp
../NxDeps/include/Stereo/StereoManager.cpp
../NxDeps/include/Stereo/StereoManager.h
../NxDeps/include/OgreProcedurals/include/Procedural.h
../NxDeps/include/OgreProcedurals/include/ProceduralBoolean.h
../NxDeps/include/OgreProcedurals/include/ProceduralBoxGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralCapsuleGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralConeGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralCylinderGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralExtruder.h
../NxDeps/include/OgreProcedurals/include/ProceduralGeometryHelpers.h
../NxDeps/include/OgreProcedurals/include/ProceduralIcoSphereGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralLathe.h
../NxDeps/include/OgreProcedurals/include/ProceduralMeshGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralMultiShape.h
../NxDeps/include/OgreProcedurals/include/ProceduralPath.h
../NxDeps/include/OgreProcedurals/include/ProceduralPathGenerators.h
../NxDeps/include/OgreProcedurals/include/ProceduralPlaneGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralPlatform.h
../NxDeps/include/OgreProcedurals/include/ProceduralRoundedBoxGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralShape.h
../NxDeps/include/OgreProcedurals/include/ProceduralShapeGenerators.h
../NxDeps/include/OgreProcedurals/include/ProceduralSphereGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralSplines.h
../NxDeps/include/OgreProcedurals/include/ProceduralStableHeaders.h
../NxDeps/include/OgreProcedurals/include/ProceduralTorusGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTorusKnotGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTrack.h
../NxDeps/include/OgreProcedurals/include/ProceduralTriangleBuffer.h
../NxDeps/include/OgreProcedurals/include/ProceduralTriangulator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTubeGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralUtils.h
../NxDeps/include/OgreProcedurals/src/ProceduralBoolean.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralBoxGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralCapsuleGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralConeGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralCylinderGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralDebugRendering.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralExtruder.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralGeometryHelpers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralIcoSphereGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralLathe.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMeshModifiers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMultiShape.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMultiShapeGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralNoise.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPath.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPathGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPlaneGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPrecompiledHeaders.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralRoundedBoxGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralShape.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralShapeGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSphereGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSpringGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSVG.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureBuffer.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureModifiers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTorusGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTorusKnotGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTrack.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTriangleBuffer.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTriangulator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTubeGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralUtils.cpp
../NxDeps/include/CaduneTree/CTParameters.cpp
../NxDeps/include/CaduneTree/CTSection.cpp
../NxDeps/include/CaduneTree/CTSerializer.cpp
../NxDeps/include/CaduneTree/CTStem.cpp
../NxDeps/include/PagedGeometry/source/BatchedGeometry.cpp
../NxDeps/include/PagedGeometry/source/BatchPage.cpp
../NxDeps/include/PagedGeometry/source/GrassLoader.cpp
../NxDeps/include/PagedGeometry/source/ImpostorPage.cpp
../NxDeps/include/PagedGeometry/source/PagedGeometry.cpp
../NxDeps/include/PagedGeometry/source/PropertyMaps.cpp
../NxDeps/include/PagedGeometry/source/StaticBillboardSet.cpp
../NxDeps/include/PagedGeometry/source/TreeLoader2D.cpp
../NxDeps/include/PagedGeometry/source/TreeLoader3D.cpp
../NxDeps/include/PagedGeometry/source/WindBatchedGeometry.cpp
../NxDeps/include/PagedGeometry/source/WindBatchPage.cpp
../NxDeps/Include/OIS/OIS.h
../NxDeps/Include/OIS/OISConfig.h
../NxDeps/Include/OIS/OISEffect.h
../NxDeps/Include/OIS/OISEvents.h
../NxDeps/Include/OIS/OISException.h
../NxDeps/Include/OIS/OISFactoryCreator.h
../NxDeps/Include/OIS/OISForceFeedback.h
../NxDeps/Include/OIS/OISInputManager.h
../NxDeps/Include/OIS/OISInterface.h
../NxDeps/Include/OIS/OISJoyStick.h
../NxDeps/Include/OIS/OISKeyboard.h
../NxDeps/Include/OIS/OISMouse.h
../NxDeps/Include/OIS/OISObject.h
../NxDeps/Include/OIS/OISPrereqs.h
../NxDeps/Include/TinyXml/tinystr.cpp
../NxDeps/Include/TinyXml/tinyxml.cpp
../NxDeps/Include/TinyXml/tinyxmlerror.cpp
../NxDeps/Include/TinyXml/tinyxmlparser.cpp
../NxDeps/Include/Lua/Src/lapi.c
../NxDeps/Include/Lua/Src/lauxlib.c
../NxDeps/Include/Lua/Src/lbaselib.c
../NxDeps/Include/Lua/Src/lcode.c
../NxDeps/Include/Lua/Src/ldblib.c
../NxDeps/Include/Lua/Src/ldebug.c
../NxDeps/Include/Lua/Src/ldo.c
../NxDeps/Include/Lua/Src/ldump.c
../NxDeps/Include/Lua/Src/lfunc.c
../NxDeps/Include/Lua/Src/lgc.c
../NxDeps/Include/Lua/Src/linit.c
../NxDeps/Include/Lua/Src/liolib.c
../NxDeps/Include/Lua/Src/llex.c
../NxDeps/Include/Lua/Src/lmathlib.c
../NxDeps/Include/Lua/Src/lmem.c
../NxDeps/Include/Lua/Src/loadlib.c
../NxDeps/Include/Lua/Src/lobject.c
../NxDeps/Include/Lua/Src/lopcodes.c
../NxDeps/Include/Lua/Src/loslib.c
../NxDeps/Include/Lua/Src/lparser.c
../NxDeps/Include/Lua/Src/lstate.c
../NxDeps/Include/Lua/Src/lstring.c
../NxDeps/Include/Lua/Src/lstrlib.c
../NxDeps/Include/Lua/Src/ltable.c
../NxDeps/Include/Lua/Src/ltablib.c
../NxDeps/Include/Lua/Src/ltm.c
../NxDeps/Include/Lua/Src/lundump.c
../NxDeps/Include/Lua/Src/lvm.c
../NxDeps/Include/Lua/Src/lzio.c
../NxDeps/include/Skia/include/core/Sk64.h
../NxDeps/include/Skia/include/core/SkAutoKern.h
../NxDeps/include/Skia/include/core/SkBitmap.h
../NxDeps/include/Skia/include/core/SkBlitRow.h
../NxDeps/include/Skia/include/core/SkBlitter.h
../NxDeps/include/Skia/include/core/SkBounder.h
../NxDeps/include/Skia/include/core/SkBuffer.h
../NxDeps/include/Skia/include/core/SkCanvas.h
../NxDeps/include/Skia/include/core/SkChunkAlloc.h
../NxDeps/include/Skia/include/core/SkColor.h
../NxDeps/include/Skia/include/core/SkColorFilter.h
../NxDeps/include/Skia/include/core/SkColorPriv.h
../NxDeps/include/Skia/include/core/SkColorShader.h
../NxDeps/include/Skia/include/core/SkComposeShader.h
../NxDeps/include/Skia/include/core/SkDeque.h
../NxDeps/include/Skia/include/core/SkDescriptor.h
../NxDeps/include/Skia/include/core/SkDevice.h
../NxDeps/include/Skia/include/core/SkDither.h
../NxDeps/include/Skia/include/core/SkDraw.h
../NxDeps/include/Skia/include/core/SkDrawFilter.h
../NxDeps/include/Skia/include/core/SkDrawLooper.h
../NxDeps/include/Skia/include/core/SkEdgeClipper.h
../NxDeps/include/Skia/include/core/SkEndian.h
../NxDeps/include/Skia/include/core/SkFDot6.h
../NxDeps/include/Skia/include/core/SkFixed.h
../NxDeps/include/Skia/include/core/SkFlattenable.h
../NxDeps/include/Skia/include/core/SkFloatBits.h
../NxDeps/include/Skia/include/core/SkFloatingPoint.h
../NxDeps/include/Skia/include/core/SkFontHost.h
../NxDeps/include/Skia/include/core/SkGeometry.h
../NxDeps/include/Skia/include/core/SkGlobals.h
../NxDeps/include/Skia/include/core/SkGraphics.h
../NxDeps/include/Skia/include/core/SkLineClipper.h
../NxDeps/include/Skia/include/core/SkMallocPixelRef.h
../NxDeps/include/Skia/include/core/SkMask.h
../NxDeps/include/Skia/include/core/SkMaskFilter.h
../NxDeps/include/Skia/include/core/SkMath.h
../NxDeps/include/Skia/include/core/SkMatrix.h
../NxDeps/include/Skia/include/core/SkMMapStream.h
../NxDeps/include/Skia/include/core/SkOSFile.h
../NxDeps/include/Skia/include/core/SkPackBits.h
../NxDeps/include/Skia/include/core/SkPaint.h
../NxDeps/include/Skia/include/core/SkPath.h
../NxDeps/include/Skia/include/core/SkPathEffect.h
../NxDeps/include/Skia/include/core/SkPathMeasure.h
../NxDeps/include/Skia/include/core/SkPerspIter.h
../NxDeps/include/Skia/include/core/SkPicture.h
../NxDeps/include/Skia/include/core/SkPixelRef.h
../NxDeps/include/Skia/include/core/SkPoint.h
../NxDeps/include/Skia/include/core/SkPostConfig.h
../NxDeps/include/Skia/include/core/SkPreConfig.h
../NxDeps/include/Skia/include/core/SkPtrRecorder.h
../NxDeps/include/Skia/include/core/SkRandom.h
../NxDeps/include/Skia/include/core/SkRasterizer.h
../NxDeps/include/Skia/include/core/SkReader32.h
../NxDeps/include/Skia/include/core/SkRect.h
../NxDeps/include/Skia/include/core/SkRefCnt.h
../NxDeps/include/Skia/include/core/SkRegion.h
../NxDeps/include/Skia/include/core/SkScalar.h
../NxDeps/include/Skia/include/core/SkScalarCompare.h
../NxDeps/include/Skia/include/core/SkScalerContext.h
../NxDeps/include/Skia/include/core/SkScan.h
../NxDeps/include/Skia/include/core/SkShader.h
../NxDeps/include/Skia/include/core/SkShape.h
../NxDeps/include/Skia/include/core/SkSize.h
../NxDeps/include/Skia/include/core/SkStream.h
../NxDeps/include/Skia/include/core/SkString.h
../NxDeps/include/Skia/include/core/SkStroke.h
../NxDeps/include/Skia/include/core/SkTDArray.h
../NxDeps/include/Skia/include/core/SkTDict.h
../NxDeps/include/Skia/include/core/SkTDStack.h
../NxDeps/include/Skia/include/core/SkTemplates.h
../NxDeps/include/Skia/include/core/SkThread.h
../NxDeps/include/Skia/include/core/SkThread_platform.h
../NxDeps/include/Skia/include/core/SkTime.h
../NxDeps/include/Skia/include/core/SkTRegistry.h
../NxDeps/include/Skia/include/core/SkTSearch.h
../NxDeps/include/Skia/include/core/SkTypeface.h
../NxDeps/include/Skia/include/core/SkTypes.h
../NxDeps/include/Skia/include/core/SkUnitMapper.h
../NxDeps/include/Skia/include/core/SkUnPreMultiply.h
../NxDeps/include/Skia/include/core/SkUtils.h
../NxDeps/include/Skia/include/core/SkWriter32.h
../NxDeps/include/Skia/include/core/SkXfermode.h
../NxDeps/include/Skia/src/animator/SkTime.cpp
../NxDeps/include/Skia/src/core/Sk64.cpp
../NxDeps/include/Skia/src/core/SkAlphaRuns.cpp
../NxDeps/include/Skia/src/core/SkBitmap.cpp
../NxDeps/include/Skia/src/core/SkBitmap_scroll.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcShader.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcState.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcState_matrixProcs.cpp
../NxDeps/include/Skia/src/core/SkBitmapSampler.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D16.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D32.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D4444.cpp
../NxDeps/include/Skia/src/core/SkBlitter.cpp
../NxDeps/include/Skia/src/core/SkBlitter_4444.cpp
../NxDeps/include/Skia/src/core/SkBlitter_A1.cpp
../NxDeps/include/Skia/src/core/SkBlitter_A8.cpp
../NxDeps/include/Skia/src/core/SkBlitter_ARGB32.cpp
../NxDeps/include/Skia/src/core/SkBlitter_ARGB32_Subpixel.cpp
../NxDeps/include/Skia/src/core/SkBlitter_RGB16.cpp
../NxDeps/include/Skia/src/core/SkBlitter_Sprite.cpp
../NxDeps/include/Skia/src/core/SkBuffer.cpp
../NxDeps/include/Skia/src/core/SkCanvas.cpp
../NxDeps/include/Skia/src/core/SkChunkAlloc.cpp
../NxDeps/include/Skia/src/core/SkColor.cpp
../NxDeps/include/Skia/src/core/SkColorFilter.cpp
../NxDeps/include/Skia/src/core/SkColorTable.cpp
../NxDeps/include/Skia/src/core/SkComposeShader.cpp
../NxDeps/include/Skia/src/core/SkConcaveToTriangles.cpp
../NxDeps/include/Skia/src/core/SkCordic.cpp
../NxDeps/include/Skia/src/core/SkCubicClipper.cpp
../NxDeps/include/Skia/src/core/SkDebug.cpp
../NxDeps/include/Skia/src/core/SkDeque.cpp
../NxDeps/include/Skia/src/core/SkDevice.cpp
../NxDeps/include/Skia/src/core/SkDither.cpp
../NxDeps/include/Skia/src/core/SkDraw.cpp
../NxDeps/include/Skia/src/core/SkEdge.cpp
../NxDeps/include/Skia/src/core/SkEdgeBuilder.cpp
../NxDeps/include/Skia/src/core/SkEdgeClipper.cpp
../NxDeps/include/Skia/src/core/SkFilterProc.cpp
../NxDeps/include/Skia/src/core/SkFlattenable.cpp
../NxDeps/include/Skia/src/core/SkFloat.cpp
../NxDeps/include/Skia/src/core/SkFloatBits.cpp
../NxDeps/include/Skia/src/core/SkFontHost.cpp
../NxDeps/include/Skia/src/core/SkGeometry.cpp
../NxDeps/include/Skia/src/core/SkGlobals.cpp
../NxDeps/include/Skia/src/core/SkGlyphCache.cpp
../NxDeps/include/Skia/src/core/SkGraphics.cpp
../NxDeps/include/Skia/src/core/SkLineClipper.cpp
../NxDeps/include/Skia/src/core/SkMask.cpp
../NxDeps/include/Skia/src/core/SkMaskFilter.cpp
../NxDeps/include/Skia/src/core/SkMath.cpp
../NxDeps/include/Skia/src/core/SkMatrix.cpp
../NxDeps/include/Skia/src/core/SkMemory_stdlib.cpp
../NxDeps/include/Skia/src/core/SkPackBits.cpp
../NxDeps/include/Skia/src/core/SkPaint.cpp
../NxDeps/include/Skia/src/core/SkPath.cpp
../NxDeps/include/Skia/src/core/SkPathEffect.cpp
../NxDeps/include/Skia/src/core/SkPathHeap.cpp
../NxDeps/include/Skia/src/core/SkPathMeasure.cpp
../NxDeps/include/Skia/src/core/SkPicture.cpp
../NxDeps/include/Skia/src/core/SkPictureFlat.cpp
../NxDeps/include/Skia/src/core/SkPicturePlayback.cpp
../NxDeps/include/Skia/src/core/SkPictureRecord.cpp
../NxDeps/include/Skia/src/core/SkPixelRef.cpp
../NxDeps/include/Skia/src/core/SkPoint.cpp
../NxDeps/include/Skia/src/core/SkProcSpriteBlitter.cpp
../NxDeps/include/Skia/src/core/SkPtrRecorder.cpp
../NxDeps/include/Skia/src/core/SkQuadClipper.cpp
../NxDeps/include/Skia/src/core/SkRasterizer.cpp
../NxDeps/include/Skia/src/core/SkRect.cpp
../NxDeps/include/Skia/src/core/SkRefCnt.cpp
../NxDeps/include/Skia/src/core/SkRegion.cpp
../NxDeps/include/Skia/src/core/SkRegion_path.cpp
../NxDeps/include/Skia/src/core/SkScalerContext.cpp
../NxDeps/include/Skia/src/core/SkScan.cpp
../NxDeps/include/Skia/src/core/SkScan_Antihair.cpp
../NxDeps/include/Skia/src/core/SkScan_AntiPath.cpp
../NxDeps/include/Skia/src/core/SkScan_Hairline.cpp
../NxDeps/include/Skia/src/core/SkScan_Path.cpp
../NxDeps/include/Skia/src/core/SkShader.cpp
../NxDeps/include/Skia/src/core/SkShape.cpp
../NxDeps/include/Skia/src/core/SkSpriteBlitter_ARGB32.cpp
../NxDeps/include/Skia/src/core/SkSpriteBlitter_RGB16.cpp
../NxDeps/include/Skia/src/core/SkStream.cpp
../NxDeps/include/Skia/src/core/SkString.cpp
../NxDeps/include/Skia/src/core/SkStroke.cpp
../NxDeps/include/Skia/src/core/SkStrokerPriv.cpp
../NxDeps/include/Skia/src/core/SkTSearch.cpp
../NxDeps/include/Skia/src/core/SkTypeface.cpp
../NxDeps/include/Skia/src/core/SkUnPreMultiply.cpp
../NxDeps/include/Skia/src/core/SkUtils.cpp
../NxDeps/include/Skia/src/core/SkWriter32.cpp
../NxDeps/include/Skia/src/core/SkXfermode.cpp
../NxDeps/include/Skia/src/effects/Sk1DPathEffect.cpp
../NxDeps/include/Skia/src/effects/Sk2DPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkAvoidXfermode.cpp
../NxDeps/include/Skia/src/effects/SkBlurDrawLooper.cpp
../NxDeps/include/Skia/src/effects/SkBlurMask.cpp
../NxDeps/include/Skia/src/effects/SkBlurMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkColorFilters.cpp
../NxDeps/include/Skia/src/effects/SkColorMatrixFilter.cpp
../NxDeps/include/Skia/src/effects/SkCornerPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkDashPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkDiscretePathEffect.cpp
../NxDeps/include/Skia/src/effects/SkEmbossMask.cpp
../NxDeps/include/Skia/src/effects/SkEmbossMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkGradientShader.cpp
../NxDeps/include/Skia/src/effects/SkGroupShape.cpp
../NxDeps/include/Skia/src/effects/SkKernel33MaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkLayerDrawLooper.cpp
../NxDeps/include/Skia/src/effects/SkLayerRasterizer.cpp
../NxDeps/include/Skia/src/effects/SkPaintFlagsDrawFilter.cpp
../NxDeps/include/Skia/src/effects/SkPixelXorXfermode.cpp
../NxDeps/include/Skia/src/effects/SkPorterDuff.cpp
../NxDeps/include/Skia/src/effects/SkRectShape.cpp
../NxDeps/include/Skia/src/effects/SkTableMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkTransparentShader.cpp
../NxDeps/include/Fast/fast.c
../NxDeps/include/Fast/fast_10.c
../NxDeps/include/Fast/fast_11.c
../NxDeps/include/Fast/fast_12.c
../NxDeps/include/Fast/fast_9.c
../NxDeps/include/Fast/nonmax.c
)





SET( NOPRECOMPILED
../NxDeps/include/OgreMax/OgreMaxModel.hpp
../NxDeps/include/OgreMax/OgreMaxPlatform.hpp
../NxDeps/include/OgreMax/OgreMaxRenderWindowIterator.hpp
../NxDeps/include/OgreMax/OgreMaxScene.hpp
../NxDeps/include/OgreMax/OgreMaxTypes.hpp
../NxDeps/include/OgreMax/OgreMaxUtilities.hpp
../NxDeps/include/OgreMax/ProgressCalculator.hpp
../NxDeps/include/OgreMax/Version.hpp
../NxDeps/include/OgreMax/OgreMaxModel.cpp
../NxDeps/include/OgreMax/OgreMaxScene.cpp
../NxDeps/include/OgreMax/OgreMaxUtilities.cpp
../NxDeps/include/OgreMax/ProgressCalculator.cpp
../NxDeps/include/OgreMax/Version.cpp
../NxDeps/include/Stereo/StereoManager.cpp
../NxDeps/include/Stereo/StereoManager.h
../NxDeps/include/OgreProcedurals/include/Procedural.h
../NxDeps/include/OgreProcedurals/include/ProceduralBoolean.h
../NxDeps/include/OgreProcedurals/include/ProceduralBoxGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralCapsuleGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralConeGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralCylinderGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralExtruder.h
../NxDeps/include/OgreProcedurals/include/ProceduralGeometryHelpers.h
../NxDeps/include/OgreProcedurals/include/ProceduralIcoSphereGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralLathe.h
../NxDeps/include/OgreProcedurals/include/ProceduralMeshGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralMultiShape.h
../NxDeps/include/OgreProcedurals/include/ProceduralPath.h
../NxDeps/include/OgreProcedurals/include/ProceduralPathGenerators.h
../NxDeps/include/OgreProcedurals/include/ProceduralPlaneGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralPlatform.h
../NxDeps/include/OgreProcedurals/include/ProceduralRoundedBoxGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralShape.h
../NxDeps/include/OgreProcedurals/include/ProceduralShapeGenerators.h
../NxDeps/include/OgreProcedurals/include/ProceduralSphereGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralSplines.h
../NxDeps/include/OgreProcedurals/include/ProceduralStableHeaders.h
../NxDeps/include/OgreProcedurals/include/ProceduralTorusGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTorusKnotGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTrack.h
../NxDeps/include/OgreProcedurals/include/ProceduralTriangleBuffer.h
../NxDeps/include/OgreProcedurals/include/ProceduralTriangulator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTubeGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralUtils.h
../NxDeps/include/OgreProcedurals/src/ProceduralBoolean.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralBoxGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralCapsuleGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralConeGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralCylinderGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralDebugRendering.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralExtruder.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralGeometryHelpers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralIcoSphereGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralLathe.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMeshModifiers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMultiShape.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMultiShapeGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralNoise.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPath.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPathGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPlaneGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPrecompiledHeaders.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralRoundedBoxGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralShape.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralShapeGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSphereGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSpringGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSVG.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureBuffer.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureModifiers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTorusGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTorusKnotGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTrack.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTriangleBuffer.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTriangulator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTubeGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralUtils.cpp
../NxDeps/include/CaduneTree/CTParameters.cpp
../NxDeps/include/CaduneTree/CTSection.cpp
../NxDeps/include/CaduneTree/CTSerializer.cpp
../NxDeps/include/CaduneTree/CTStem.cpp
../NxDeps/include/PagedGeometry/source/BatchedGeometry.cpp
../NxDeps/include/PagedGeometry/source/BatchPage.cpp
../NxDeps/include/PagedGeometry/source/GrassLoader.cpp
../NxDeps/include/PagedGeometry/source/ImpostorPage.cpp
../NxDeps/include/PagedGeometry/source/PagedGeometry.cpp
../NxDeps/include/PagedGeometry/source/PropertyMaps.cpp
../NxDeps/include/PagedGeometry/source/StaticBillboardSet.cpp
../NxDeps/include/PagedGeometry/source/TreeLoader2D.cpp
../NxDeps/include/PagedGeometry/source/TreeLoader3D.cpp
../NxDeps/include/PagedGeometry/source/WindBatchedGeometry.cpp
../NxDeps/include/PagedGeometry/source/WindBatchPage.cpp
../NxDeps/Include/OIS/OIS.h
../NxDeps/Include/OIS/OISConfig.h
../NxDeps/Include/OIS/OISEffect.h
../NxDeps/Include/OIS/OISEvents.h
../NxDeps/Include/OIS/OISException.h
../NxDeps/Include/OIS/OISFactoryCreator.h
../NxDeps/Include/OIS/OISForceFeedback.h
../NxDeps/Include/OIS/OISInputManager.h
../NxDeps/Include/OIS/OISInterface.h
../NxDeps/Include/OIS/OISJoyStick.h
../NxDeps/Include/OIS/OISKeyboard.h
../NxDeps/Include/OIS/OISMouse.h
../NxDeps/Include/OIS/OISObject.h
../NxDeps/Include/OIS/OISPrereqs.h
../NxDeps/Include/TinyXml/tinystr.cpp
../NxDeps/Include/TinyXml/tinyxml.cpp
../NxDeps/Include/TinyXml/tinyxmlerror.cpp
../NxDeps/Include/TinyXml/tinyxmlparser.cpp
../NxDeps/Include/Lua/Src/lapi.c
../NxDeps/Include/Lua/Src/lauxlib.c
../NxDeps/Include/Lua/Src/lbaselib.c
../NxDeps/Include/Lua/Src/lcode.c
../NxDeps/Include/Lua/Src/ldblib.c
../NxDeps/Include/Lua/Src/ldebug.c
../NxDeps/Include/Lua/Src/ldo.c
../NxDeps/Include/Lua/Src/ldump.c
../NxDeps/Include/Lua/Src/lfunc.c
../NxDeps/Include/Lua/Src/lgc.c
../NxDeps/Include/Lua/Src/linit.c
../NxDeps/Include/Lua/Src/liolib.c
../NxDeps/Include/Lua/Src/llex.c
../NxDeps/Include/Lua/Src/lmathlib.c
../NxDeps/Include/Lua/Src/lmem.c
../NxDeps/Include/Lua/Src/loadlib.c
../NxDeps/Include/Lua/Src/lobject.c
../NxDeps/Include/Lua/Src/lopcodes.c
../NxDeps/Include/Lua/Src/loslib.c
../NxDeps/Include/Lua/Src/lparser.c
../NxDeps/Include/Lua/Src/lstate.c
../NxDeps/Include/Lua/Src/lstring.c
../NxDeps/Include/Lua/Src/lstrlib.c
../NxDeps/Include/Lua/Src/ltable.c
../NxDeps/Include/Lua/Src/ltablib.c
../NxDeps/Include/Lua/Src/ltm.c
../NxDeps/Include/Lua/Src/lundump.c
../NxDeps/Include/Lua/Src/lvm.c
../NxDeps/Include/Lua/Src/lzio.c
../NxDeps/include/Skia/include/core/Sk64.h
../NxDeps/include/Skia/include/core/SkAutoKern.h
../NxDeps/include/Skia/include/core/SkBitmap.h
../NxDeps/include/Skia/include/core/SkBlitRow.h
../NxDeps/include/Skia/include/core/SkBlitter.h
../NxDeps/include/Skia/include/core/SkBounder.h
../NxDeps/include/Skia/include/core/SkBuffer.h
../NxDeps/include/Skia/include/core/SkCanvas.h
../NxDeps/include/Skia/include/core/SkChunkAlloc.h
../NxDeps/include/Skia/include/core/SkColor.h
../NxDeps/include/Skia/include/core/SkColorFilter.h
../NxDeps/include/Skia/include/core/SkColorPriv.h
../NxDeps/include/Skia/include/core/SkColorShader.h
../NxDeps/include/Skia/include/core/SkComposeShader.h
../NxDeps/include/Skia/include/core/SkDeque.h
../NxDeps/include/Skia/include/core/SkDescriptor.h
../NxDeps/include/Skia/include/core/SkDevice.h
../NxDeps/include/Skia/include/core/SkDither.h
../NxDeps/include/Skia/include/core/SkDraw.h
../NxDeps/include/Skia/include/core/SkDrawFilter.h
../NxDeps/include/Skia/include/core/SkDrawLooper.h
../NxDeps/include/Skia/include/core/SkEdgeClipper.h
../NxDeps/include/Skia/include/core/SkEndian.h
../NxDeps/include/Skia/include/core/SkFDot6.h
../NxDeps/include/Skia/include/core/SkFixed.h
../NxDeps/include/Skia/include/core/SkFlattenable.h
../NxDeps/include/Skia/include/core/SkFloatBits.h
../NxDeps/include/Skia/include/core/SkFloatingPoint.h
../NxDeps/include/Skia/include/core/SkFontHost.h
../NxDeps/include/Skia/include/core/SkGeometry.h
../NxDeps/include/Skia/include/core/SkGlobals.h
../NxDeps/include/Skia/include/core/SkGraphics.h
../NxDeps/include/Skia/include/core/SkLineClipper.h
../NxDeps/include/Skia/include/core/SkMallocPixelRef.h
../NxDeps/include/Skia/include/core/SkMask.h
../NxDeps/include/Skia/include/core/SkMaskFilter.h
../NxDeps/include/Skia/include/core/SkMath.h
../NxDeps/include/Skia/include/core/SkMatrix.h
../NxDeps/include/Skia/include/core/SkMMapStream.h
../NxDeps/include/Skia/include/core/SkOSFile.h
../NxDeps/include/Skia/include/core/SkPackBits.h
../NxDeps/include/Skia/include/core/SkPaint.h
../NxDeps/include/Skia/include/core/SkPath.h
../NxDeps/include/Skia/include/core/SkPathEffect.h
../NxDeps/include/Skia/include/core/SkPathMeasure.h
../NxDeps/include/Skia/include/core/SkPerspIter.h
../NxDeps/include/Skia/include/core/SkPicture.h
../NxDeps/include/Skia/include/core/SkPixelRef.h
../NxDeps/include/Skia/include/core/SkPoint.h
../NxDeps/include/Skia/include/core/SkPostConfig.h
../NxDeps/include/Skia/include/core/SkPreConfig.h
../NxDeps/include/Skia/include/core/SkPtrRecorder.h
../NxDeps/include/Skia/include/core/SkRandom.h
../NxDeps/include/Skia/include/core/SkRasterizer.h
../NxDeps/include/Skia/include/core/SkReader32.h
../NxDeps/include/Skia/include/core/SkRect.h
../NxDeps/include/Skia/include/core/SkRefCnt.h
../NxDeps/include/Skia/include/core/SkRegion.h
../NxDeps/include/Skia/include/core/SkScalar.h
../NxDeps/include/Skia/include/core/SkScalarCompare.h
../NxDeps/include/Skia/include/core/SkScalerContext.h
../NxDeps/include/Skia/include/core/SkScan.h
../NxDeps/include/Skia/include/core/SkShader.h
../NxDeps/include/Skia/include/core/SkShape.h
../NxDeps/include/Skia/include/core/SkSize.h
../NxDeps/include/Skia/include/core/SkStream.h
../NxDeps/include/Skia/include/core/SkString.h
../NxDeps/include/Skia/include/core/SkStroke.h
../NxDeps/include/Skia/include/core/SkTDArray.h
../NxDeps/include/Skia/include/core/SkTDict.h
../NxDeps/include/Skia/include/core/SkTDStack.h
../NxDeps/include/Skia/include/core/SkTemplates.h
../NxDeps/include/Skia/include/core/SkThread.h
../NxDeps/include/Skia/include/core/SkThread_platform.h
../NxDeps/include/Skia/include/core/SkTime.h
../NxDeps/include/Skia/include/core/SkTRegistry.h
../NxDeps/include/Skia/include/core/SkTSearch.h
../NxDeps/include/Skia/include/core/SkTypeface.h
../NxDeps/include/Skia/include/core/SkTypes.h
../NxDeps/include/Skia/include/core/SkUnitMapper.h
../NxDeps/include/Skia/include/core/SkUnPreMultiply.h
../NxDeps/include/Skia/include/core/SkUtils.h
../NxDeps/include/Skia/include/core/SkWriter32.h
../NxDeps/include/Skia/include/core/SkXfermode.h
../NxDeps/include/Skia/src/animator/SkTime.cpp
../NxDeps/include/Skia/src/core/Sk64.cpp
../NxDeps/include/Skia/src/core/SkAlphaRuns.cpp
../NxDeps/include/Skia/src/core/SkBitmap.cpp
../NxDeps/include/Skia/src/core/SkBitmap_scroll.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcShader.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcState.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcState_matrixProcs.cpp
../NxDeps/include/Skia/src/core/SkBitmapSampler.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D16.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D32.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D4444.cpp
../NxDeps/include/Skia/src/core/SkBlitter.cpp
../NxDeps/include/Skia/src/core/SkBlitter_4444.cpp
../NxDeps/include/Skia/src/core/SkBlitter_A1.cpp
../NxDeps/include/Skia/src/core/SkBlitter_A8.cpp
../NxDeps/include/Skia/src/core/SkBlitter_ARGB32.cpp
../NxDeps/include/Skia/src/core/SkBlitter_ARGB32_Subpixel.cpp
../NxDeps/include/Skia/src/core/SkBlitter_RGB16.cpp
../NxDeps/include/Skia/src/core/SkBlitter_Sprite.cpp
../NxDeps/include/Skia/src/core/SkBuffer.cpp
../NxDeps/include/Skia/src/core/SkCanvas.cpp
../NxDeps/include/Skia/src/core/SkChunkAlloc.cpp
../NxDeps/include/Skia/src/core/SkColor.cpp
../NxDeps/include/Skia/src/core/SkColorFilter.cpp
../NxDeps/include/Skia/src/core/SkColorTable.cpp
../NxDeps/include/Skia/src/core/SkComposeShader.cpp
../NxDeps/include/Skia/src/core/SkConcaveToTriangles.cpp
../NxDeps/include/Skia/src/core/SkCordic.cpp
../NxDeps/include/Skia/src/core/SkCubicClipper.cpp
../NxDeps/include/Skia/src/core/SkDebug.cpp
../NxDeps/include/Skia/src/core/SkDeque.cpp
../NxDeps/include/Skia/src/core/SkDevice.cpp
../NxDeps/include/Skia/src/core/SkDither.cpp
../NxDeps/include/Skia/src/core/SkDraw.cpp
../NxDeps/include/Skia/src/core/SkEdge.cpp
../NxDeps/include/Skia/src/core/SkEdgeBuilder.cpp
../NxDeps/include/Skia/src/core/SkEdgeClipper.cpp
../NxDeps/include/Skia/src/core/SkFilterProc.cpp
../NxDeps/include/Skia/src/core/SkFlattenable.cpp
../NxDeps/include/Skia/src/core/SkFloat.cpp
../NxDeps/include/Skia/src/core/SkFloatBits.cpp
../NxDeps/include/Skia/src/core/SkFontHost.cpp
../NxDeps/include/Skia/src/core/SkGeometry.cpp
../NxDeps/include/Skia/src/core/SkGlobals.cpp
../NxDeps/include/Skia/src/core/SkGlyphCache.cpp
../NxDeps/include/Skia/src/core/SkGraphics.cpp
../NxDeps/include/Skia/src/core/SkLineClipper.cpp
../NxDeps/include/Skia/src/core/SkMask.cpp
../NxDeps/include/Skia/src/core/SkMaskFilter.cpp
../NxDeps/include/Skia/src/core/SkMath.cpp
../NxDeps/include/Skia/src/core/SkMatrix.cpp
../NxDeps/include/Skia/src/core/SkMemory_stdlib.cpp
../NxDeps/include/Skia/src/core/SkPackBits.cpp
../NxDeps/include/Skia/src/core/SkPaint.cpp
../NxDeps/include/Skia/src/core/SkPath.cpp
../NxDeps/include/Skia/src/core/SkPathEffect.cpp
../NxDeps/include/Skia/src/core/SkPathHeap.cpp
../NxDeps/include/Skia/src/core/SkPathMeasure.cpp
../NxDeps/include/Skia/src/core/SkPicture.cpp
../NxDeps/include/Skia/src/core/SkPictureFlat.cpp
../NxDeps/include/Skia/src/core/SkPicturePlayback.cpp
../NxDeps/include/Skia/src/core/SkPictureRecord.cpp
../NxDeps/include/Skia/src/core/SkPixelRef.cpp
../NxDeps/include/Skia/src/core/SkPoint.cpp
../NxDeps/include/Skia/src/core/SkProcSpriteBlitter.cpp
../NxDeps/include/Skia/src/core/SkPtrRecorder.cpp
../NxDeps/include/Skia/src/core/SkQuadClipper.cpp
../NxDeps/include/Skia/src/core/SkRasterizer.cpp
../NxDeps/include/Skia/src/core/SkRect.cpp
../NxDeps/include/Skia/src/core/SkRefCnt.cpp
../NxDeps/include/Skia/src/core/SkRegion.cpp
../NxDeps/include/Skia/src/core/SkRegion_path.cpp
../NxDeps/include/Skia/src/core/SkScalerContext.cpp
../NxDeps/include/Skia/src/core/SkScan.cpp
../NxDeps/include/Skia/src/core/SkScan_Antihair.cpp
../NxDeps/include/Skia/src/core/SkScan_AntiPath.cpp
../NxDeps/include/Skia/src/core/SkScan_Hairline.cpp
../NxDeps/include/Skia/src/core/SkScan_Path.cpp
../NxDeps/include/Skia/src/core/SkShader.cpp
../NxDeps/include/Skia/src/core/SkShape.cpp
../NxDeps/include/Skia/src/core/SkSpriteBlitter_ARGB32.cpp
../NxDeps/include/Skia/src/core/SkSpriteBlitter_RGB16.cpp
../NxDeps/include/Skia/src/core/SkStream.cpp
../NxDeps/include/Skia/src/core/SkString.cpp
../NxDeps/include/Skia/src/core/SkStroke.cpp
../NxDeps/include/Skia/src/core/SkStrokerPriv.cpp
../NxDeps/include/Skia/src/core/SkTSearch.cpp
../NxDeps/include/Skia/src/core/SkTypeface.cpp
../NxDeps/include/Skia/src/core/SkUnPreMultiply.cpp
../NxDeps/include/Skia/src/core/SkUtils.cpp
../NxDeps/include/Skia/src/core/SkWriter32.cpp
../NxDeps/include/Skia/src/core/SkXfermode.cpp
../NxDeps/include/Skia/src/effects/Sk1DPathEffect.cpp
../NxDeps/include/Skia/src/effects/Sk2DPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkAvoidXfermode.cpp
../NxDeps/include/Skia/src/effects/SkBlurDrawLooper.cpp
../NxDeps/include/Skia/src/effects/SkBlurMask.cpp
../NxDeps/include/Skia/src/effects/SkBlurMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkColorFilters.cpp
../NxDeps/include/Skia/src/effects/SkColorMatrixFilter.cpp
../NxDeps/include/Skia/src/effects/SkCornerPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkDashPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkDiscretePathEffect.cpp
../NxDeps/include/Skia/src/effects/SkEmbossMask.cpp
../NxDeps/include/Skia/src/effects/SkEmbossMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkGradientShader.cpp
../NxDeps/include/Skia/src/effects/SkGroupShape.cpp
../NxDeps/include/Skia/src/effects/SkKernel33MaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkLayerDrawLooper.cpp
../NxDeps/include/Skia/src/effects/SkLayerRasterizer.cpp
../NxDeps/include/Skia/src/effects/SkPaintFlagsDrawFilter.cpp
../NxDeps/include/Skia/src/effects/SkPixelXorXfermode.cpp
../NxDeps/include/Skia/src/effects/SkPorterDuff.cpp
../NxDeps/include/Skia/src/effects/SkRectShape.cpp
../NxDeps/include/Skia/src/effects/SkTableMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkTransparentShader.cpp
../NxDeps/include/Fast/fast.c
../NxDeps/include/Fast/fast_10.c
../NxDeps/include/Fast/fast_11.c
../NxDeps/include/Fast/fast_12.c
../NxDeps/include/Fast/fast_9.c
../NxDeps/include/Fast/nonmax.c
)


SET(PRECOMPILED
./NxGraphics.cpp
./NxColourValue.cpp
./NxFontManager.cpp
./NxInputManager.cpp
./NxLog.cpp
./NxTextureManager.cpp
./NxPlatform.cpp
./NxRecorder.cpp
./NxStateManager.cpp
./NxUtils.cpp
./NxSceneManager.cpp
./NxMeshManager.cpp
./NxMeshManagerUtils.cpp
./NxSceneShadows.cpp
./NxObject.cpp
./NxText.cpp
./NxLine.cpp
./NxPatch.cpp
./NxTriangleList.cpp
./NxLight.cpp
./NxCamera.cpp
./NxCharacter.cpp
./NxCharacterController.cpp
./NxEntity.cpp
./NxNode.cpp
./NxTerrainTree.cpp
./NxTerrain.cpp
./NxTerrainEntity.cpp
./NxTerrainGrass.cpp
./NxTerrainGrassLayer.cpp
./NxEntitySound.cpp
./NxParticles.cpp
./NxParticleEmitter.cpp
./NxParticleEmitterJong.cpp
./NxParticleSystem.cpp
./NxParticleSystemAffector.cpp
./NxParticleSystemAffectorInterpolator.cpp
./NxScene.cpp
./NxSceneEmpty.cpp
./NxSceneOgreMax.cpp
./NxSceneOgreMaxCallback.cpp
./NxSceneOgreMaxParsing.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsScene.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsSceneDebug.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsManager.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsActor.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsClothe.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsClothes.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsController.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsFluid.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsFluidEmitter.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsJointD6.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsJointRevolute.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsMagnet.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShape.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeBox.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeCapsule.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeMesh.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapePlane.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsVehicule.cpp
./NxSceneGizmo.cpp
./NxRenderables.cpp
./NxRenderablesDelaunay.cpp
./NxRenderablesLines.cpp
./NxRenderablesParticles.cpp
./NxRenderablesText.cpp
./NxRenderablesTriangles.cpp
./${NxGraphicsPlatform}/NxWindowManager.cpp
./${NxGraphicsPlatform}/NxWindowScreen.cpp
./NxWindowMonitor.cpp
./NxAnimation.cpp
./NxKeyFrame.cpp
./NxAnimationManager.cpp
./NxScript.cpp
./NxScriptLua.cpp
./${NxGraphicsPlatform}/NxPluginDescription.cpp
./${NxGraphicsPlatform}/NxPluginEffect.cpp
./${NxGraphicsPlatform}/NxPluginEffectFreeframe.cpp
./NxPluginManager.cpp
./NxPluginParam.cpp
./NxPluginParamButton.cpp
./NxPluginParamColorButton.cpp
./NxPluginParamComboBox.cpp
./NxPluginParamSlider.cpp
./NxFrameBuffer.cpp
./NxFrameBuffer3D.cpp
./NxFrameBufferCamera.cpp
./NxFrameBufferCanvas.cpp
./NxFrameBufferImage.cpp
./NxFrameBufferVideo.cpp
./NxPostEffect.cpp
./NxPostEffectManager.cpp
./NxPostEffectDof.cpp
./NxPostEffectGaussianBlur.cpp
./NxPostEffectHdr.cpp
./NxOverlayManager.cpp
./NxOverlay.cpp
./NxOverlayProgressBar.cpp
./NxImportXfl.cpp
./NxMaterial.cpp
./NxMaterialManager.cpp
./NxPass.cpp
./NxShader.cpp
./NxTechnique.cpp
./NxTextureUnit.cpp
./NxImage.cpp
./NxImageCanvas.cpp
./NxImageCanvasGradient.cpp
./NxImageCanvasPath.cpp
./NxImageCanvasPattern.cpp
./NxImageCanvasText.cpp
./NxImageCanvasTransforms.cpp
./NxLayer3D.cpp
./NxLayerBase.cpp
./NxLayerCamera.cpp
./NxLayerImage.cpp
./NxLayerStream.cpp
./NxLayerVideo.cpp
./NxLayerWeb.cpp
./NxStateIntro.cpp
./NxEngine.cpp
./NxContentManager.cpp
./NxViewport.cpp
./NxWindow.cpp
./NxParserXml.cpp
./NxParserJson.cpp
./NxMath.cpp
./NxMatrix3.cpp
./NxMatrix4.cpp
./NxQuaternion.cpp
./NxVector2.cpp
./NxVector3.cpp
./NxVector4.cpp
./NxRectangle2D.cpp
./NxGui/NxGuiButton.cpp
./NxGui/NxGuiButtonToggle.cpp
./NxGui/NxGuiButtonRadio.cpp
./NxGui/NxGuiColours.cpp
./NxGui/NxGuiElement.cpp
./NxGui/NxGuiLabel.cpp
./NxGui/NxGuiLine.cpp
./NxGui/NxGuiManager.cpp
./NxGui/NxGuiPanel.cpp
./NxGui/NxGuiRectangle.cpp
./NxGui/NxGuiSlider.cpp
./NxGui/NxGuiText.cpp
./NxGui/NxGuiWidget.cpp
./NxGui/NxGuiDragAndDropContainer.cpp
)
 

SOURCE_GROUP("Source Files\\GUI" FILES  
./NxGui/NxGuiButton.cpp
./NxGui/NxGuiButtonToggle.cpp
./NxGui/NxGuiButtonRadio.cpp
./NxGui/NxGuiColours.cpp
./NxGui/NxGuiElement.cpp
./NxGui/NxGuiLabel.cpp
./NxGui/NxGuiLine.cpp
./NxGui/NxGuiManager.cpp
./NxGui/NxGuiPanel.cpp
./NxGui/NxGuiRectangle.cpp
./NxGui/NxGuiSlider.cpp
./NxGui/NxGuiText.cpp
./NxGui/NxGuiWidget.cpp
./NxGui/NxGuiDragAndDropContainer.cpp
)
 
SOURCE_GROUP("Source Files\\ANIMATION" FILES 
./NxAnimation.cpp
./NxKeyFrame.cpp
./NxAnimationManager.cpp
)

SOURCE_GROUP("Source Files\\CORE" FILES 
./NxEngine.cpp
./NxContentManager.cpp
./NxViewport.cpp
./NxWindow.cpp
./NxCamera.cpp
)

SOURCE_GROUP("Source Files\\PARTICLES" FILES 
./NxParticleEmitter.cpp
./NxParticleEmitterJong.cpp
./NxParticles.cpp
./NxParticleSystem.cpp
./NxParticleSystemAffector.cpp
./NxParticleSystemAffectorInterpolator.cpp
)

SOURCE_GROUP("Source Files\\PARSER" FILES 
./NxParserXml.cpp
./NxParserJson.cpp
)

SOURCE_GROUP("Source Files\\INPUT" FILES 
./NxInputManager.cpp
)

SOURCE_GROUP("Source Files\\MATH" FILES 
./NxMath.cpp
./NxMatrix3.cpp
./NxMatrix4.cpp
./NxQuaternion.cpp
./NxVector2.cpp
./NxVector3.cpp
./NxVector4.cpp
)

SOURCE_GROUP("Source Files\\FONT" FILES 
./NxFontManager.cpp
)





SOURCE_GROUP("Source Files\\FRAMEBUFFER" FILES 
./NxFrameBuffer.cpp
./NxFrameBuffer3D.cpp
./NxFrameBufferCamera.cpp
./NxFrameBufferCanvas.cpp
./NxFrameBufferImage.cpp
./NxFrameBufferVideo.cpp
)

SOURCE_GROUP("Source Files\\GAMESTATES" FILES 
./NxStateIntro.cpp
)

SOURCE_GROUP("Source Files\\IMPORT/EXPORT" FILES 
./NxImportXfl.cpp
)

SOURCE_GROUP("Source Files\\MATERIALS" FILES 
./NxMaterial.cpp
./NxMaterialManager.cpp
./NxTextureManager.cpp
./NxPass.cpp
./NxShader.cpp
./NxTechnique.cpp
./NxTextureUnit.cpp
./NxColourValue.cpp
)

SOURCE_GROUP("Source Files\\MATERIALS\\IMAGE" FILES 
./NxImage.cpp
./NxImageCanvas.cpp
./NxImageCanvasGradient.cpp
./NxImageCanvasPath.cpp
./NxImageCanvasPattern.cpp
./NxImageCanvasText.cpp
./NxImageCanvasTransforms.cpp
)

SOURCE_GROUP("Source Files\\MATERIALS\\TEXTURES" FILES 
./NxLayer3D.cpp
./NxLayerBase.cpp
./NxLayerCamera.cpp
./NxLayerImage.cpp
./NxLayerStream.cpp
./NxLayerVideo.cpp
./NxLayerWeb.cpp
)



SOURCE_GROUP("Source Files\\MEDIA\\2D\\OBJECTS" FILES 
./NxRenderables.cpp
./NxRenderablesDelaunay.cpp
./NxRenderablesLines.cpp
./NxRenderablesParticles.cpp
./NxRenderablesText.cpp
./NxRenderablesTriangles.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D" FILES 
./NxSceneManager.cpp
./NxMeshManager.cpp
./NxMeshManagerUtils.cpp
./NxSceneShadows.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsManager.cpp
)

 

SOURCE_GROUP("Source Files\\MEDIA\\3D\\OBJECTS\\CHARACTER" FILES 
./NxEntityCharacter.cpp
./NxEntityCharacterController.cpp

)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\OBJECTS\\ENTITY" FILES 
./NxEntity.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\OBJECTS\\LIGHTS" FILES 
./NxLight.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\OBJECTS\\NODE" FILES 
./NxNode.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\OBJECTS\\OBJECTS" FILES
./NxObject.cpp
./NxText.cpp
./NxLine.cpp
./NxPatch.cpp
./NxTriangleList.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\OBJECTS\\PARTICLES" FILES
./NxParticles.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsActor.cpp
)


SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS\\CLOTH" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsClothe.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsClothes.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS\\FLUIDS" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsFluid.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsFluidEmitter.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS\\JOINTS" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsJointD6.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsJointRevolute.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS\\MAGNETS" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsMagnet.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS\\CONTROLLER" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsController.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS\\SHAPES" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShape.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeBox.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeCapsule.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapeMesh.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsShapePlane.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS\\VEHICULE" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsVehicule.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\SOUND" FILES
./NxEntitySound.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\TERRAIN" FILES
./NxTerrain.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\TERRAIN\\PAGED" FILES
./NxTerrainPagedEntity.cpp
./NxTerrainGrass.cpp
./NxTerrainGrassLayer.cpp
./NxTerrainTree.cpp
)
 

SOURCE_GROUP("Source Files\\MEDIA\\3D\\SCENE" FILES
./NxScene.cpp
./NxSceneEmpty.cpp
./NxSceneOgreMax.cpp
./NxSceneOgreMaxCallback.cpp
./NxSceneOgreMaxParsing.cpp
./NxSceneGizmo.cpp
)
 

SOURCE_GROUP("Source Files\\MEDIA\\3D\\MESH" FILES
./NMeshManager.cpp
./NxMeshManagerUtils.cpp
)

SOURCE_GROUP("Source Files\\MEDIA\\3D\\PHYSICS" FILES
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsSceneDebug.cpp
./Physics/${NxGraphicsPhysicEngine}/NxPhysicsScene.cpp
)


SOURCE_GROUP("Source Files\\OVERLAY" FILES
./NxOverlay.cpp
./NxOverlayManager.cpp
./NxOverlayProgressBar.cpp
)

SOURCE_GROUP("Source Files\\PLUGIN" FILES
./${NxGraphicsPlatform}/NxPluginDescription.cpp
./${NxGraphicsPlatform}/NxPluginEffect.cpp
./NxPluginManager.cpp
./NxPluginParam.cpp
)

SOURCE_GROUP("Source Files\\PLUGIN\\WIDGETS" FILES
./NxPluginParamButton.cpp
./NxPluginParamColorButton.cpp
./NxPluginParamComboBox.cpp
./NxPluginParamSlider.cpp
)

SOURCE_GROUP("Source Files\\PLUGIN\\VIDEOPLUGINS" FILES
./${NxGraphicsPlatform}/NxPluginEffectFreeframe.cpp
)

SOURCE_GROUP("Source Files\\POSTEFFECTS" FILES
./NxPostEffectManager.cpp
./NxPostEffect.cpp
)

SOURCE_GROUP("Source Files\\POSTEFFECTS\\EFFECTS" FILES
./NxPostEffectDof.cpp
./NxPostEffectGaussianBlur.cpp
./NxPostEffectHdr.cpp
)

SOURCE_GROUP("Source Files\\SCRIPTING" FILES
./NxScriptLua.cpp
./NxScript.cpp
)

SOURCE_GROUP("Source Files\\WINDOW" FILES
./${NxGraphicsPlatform}/NxWindowManager.cpp
./NxWindowMonitor.cpp
./${NxGraphicsPlatform}/NxWindowScreen.cpp
)

if( MSVC )

if( NxGraphicsPhysicEngine MATCHES "Physx_2.8.4" )
set( NxGraphics_src ${NxGraphics_src}
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/win/MediaPath_WIN.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Asc2Bin.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ColladaExport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ColladaImport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_cooking.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_customcopy.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_File.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Geometry.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_GraphicsMesh.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_helper.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_hull.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_PhysicsExport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_PhysicsInstantiator.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ScaledCopy.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_schema.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_SchemaStream.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Streaming.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_string.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinystr.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxml.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxmlerror.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxmlparser.cpp
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/Stream.cpp
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/UserAllocator.cpp
)

set( NOPRECOMPILED ${NOPRECOMPILED} 
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/win/MediaPath_WIN.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Asc2Bin.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ColladaExport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ColladaImport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_cooking.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_customcopy.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_File.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Geometry.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_GraphicsMesh.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_helper.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_hull.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_PhysicsExport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_PhysicsInstantiator.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ScaledCopy.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_schema.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_SchemaStream.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Streaming.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_string.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinystr.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxml.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxmlerror.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxmlparser.cpp
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/Stream.cpp
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/UserAllocator.cpp
)

SOURCE_GROUP("LIBRAIRIES\\3D\\Physics\\Source" FILES 
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/win/MediaPath_WIN.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Asc2Bin.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ColladaExport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ColladaImport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_cooking.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_customcopy.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_File.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Geometry.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_GraphicsMesh.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_helper.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_hull.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_PhysicsExport.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_PhysicsInstantiator.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_ScaledCopy.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_schema.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_SchemaStream.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_Streaming.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_string.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinystr.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxml.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxmlerror.cpp
../NxDeps/Include/Physx/v2.8.4_win/Tools/NxuStream2/NXU_tinyxmlparser.cpp
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/Stream.cpp
../NxDeps/Include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/UserAllocator.cpp
)

endif()

set( NxGraphics_src ${NxGraphics_src}
../NxDeps/include/Skia/src/opts/opts_check_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkBitmapProcState_opts_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkBlitRow_opts_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkUtils_opts_SSE2.cpp
../NxDeps/include/Skia/src/ports/SkDebug_stdio.cpp
../NxDeps/include/Skia/src/ports/SkFontHost_win.cpp
../NxDeps/include/Skia/src/ports/SkGlobals_global.cpp
../NxDeps/include/Skia/src/ports/SkOSFile_stdio.cpp
../NxDeps/include/Skia/src/ports/SkThread_win.cpp
)

set( NOPRECOMPILED ${NOPRECOMPILED} 
../NxDeps/include/Skia/src/opts/opts_check_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkBitmapProcState_opts_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkBlitRow_opts_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkUtils_opts_SSE2.cpp
../NxDeps/include/Skia/src/ports/SkDebug_stdio.cpp
../NxDeps/include/Skia/src/ports/SkFontHost_win.cpp
../NxDeps/include/Skia/src/ports/SkGlobals_global.cpp
../NxDeps/include/Skia/src/ports/SkOSFile_stdio.cpp
../NxDeps/include/Skia/src/ports/SkThread_win.cpp
)

SOURCE_GROUP("LIBRAIRIES\\2D\\SKIA\\Sources\\Opts" FILES 
../NxDeps/include/Skia/src/opts/opts_check_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkBitmapProcState_opts_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkBlitRow_opts_SSE2.cpp
../NxDeps/include/Skia/src/opts/SkUtils_opts_SSE2.cpp
)

SOURCE_GROUP("LIBRAIRIES\\2D\\SKIA\\Sources\\Ports" FILES 
../NxDeps/include/Skia/src/ports/SkDebug_stdio.cpp
../NxDeps/include/Skia/src/ports/SkFontHost_win.cpp
../NxDeps/include/Skia/src/ports/SkGlobals_global.cpp
../NxDeps/include/Skia/src/ports/SkOSFile_stdio.cpp
../NxDeps/include/Skia/src/ports/SkThread_win.cpp
)

message( "Windows : Adding windowing dependencies"  )

elseif(APPLE)

#skia on mac is boken ( carbon deprecated ): todo update to the latest skia.

set( NxGraphics_src ${NxGraphics_src}
../NxDeps/include/Skia/src/opts/SkBitmapProcState_opts_none.cpp
../NxDeps/include/Skia/src/opts/SkBlitRow_opts_none.cpp
../NxDeps/include/Skia/src/opts/SkUtils_opts_none.cpp
../NxDeps/include/Skia/src/ports/SkDebug_stdio.cpp
#../NxDeps/include/Skia/src/ports/SkFontHost_mac.cpp
../NxDeps/include/Skia/src/ports/SkFontHost_none.cpp
../NxDeps/include/Skia/src/ports/SkGlobals_global.cpp
../NxDeps/include/Skia/src/ports/SkOSFile_stdio.cpp
../NxDeps/include/Skia/src/ports/SkThread_pthread.cpp
)

set( NOPRECOMPILED ${NOPRECOMPILED} 
../NxDeps/include/Skia/src/opts/SkBitmapProcState_opts_none.cpp
../NxDeps/include/Skia/src/opts/SkBlitRow_opts_none.cpp
../NxDeps/include/Skia/src/opts/SkUtils_opts_none.cpp
../NxDeps/include/Skia/src/ports/SkDebug_stdio.cpp
#../NxDeps/include/Skia/src/ports/SkFontHost_mac.cpp
../NxDeps/include/Skia/src/ports/SkFontHost_none.cpp
../NxDeps/include/Skia/src/ports/SkGlobals_global.cpp
../NxDeps/include/Skia/src/ports/SkOSFile_stdio.cpp
../NxDeps/include/Skia/src/ports/SkThread_pthread.cpp
)

message( "Osx : Adding windowing dependencies"  )

elseif(UNIX)

set( NxGraphics_src ${NxGraphics_src}
../NxDeps/include/Skia/src/opts/SkBitmapProcState_opts_none.cpp
../NxDeps/include/Skia/src/opts/SkBlitRow_opts_none.cpp
../NxDeps/include/Skia/src/opts/SkUtils_opts_none.cpp
../NxDeps/include/Skia/src/ports/SkDebug_stdio.cpp
#../NxDeps/include/Skia/src/ports/SkFontHost_android.cpp
../NxDeps/include/Skia/src/ports/SkFontHost_none.cpp
../NxDeps/include/Skia/src/ports/SkGlobals_global.cpp
../NxDeps/include/Skia/src/ports/SkOSFile_stdio.cpp
../NxDeps/include/Skia/src/ports/SkThread_pthread.cpp
)

set( NOPRECOMPILED ${NOPRECOMPILED} 
../NxDeps/include/Skia/src/opts/SkBitmapProcState_opts_none.cpp
../NxDeps/include/Skia/src/opts/SkBlitRow_opts_none.cpp
../NxDeps/include/Skia/src/opts/SkUtils_opts_none.cpp
../NxDeps/include/Skia/src/ports/SkDebug_stdio.cpp
#../NxDeps/include/Skia/src/ports/SkFontHost_android.cpp
../NxDeps/include/Skia/src/ports/SkFontHost_none.cpp
../NxDeps/include/Skia/src/ports/SkGlobals_global.cpp
../NxDeps/include/Skia/src/ports/SkOSFile_stdio.cpp
../NxDeps/include/Skia/src/ports/SkThread_pthread.cpp
)
 

message( "Unix : Adding windowing dependencies"  )

else()

message( "NO OS Error : For windowing dependencies"  )

endif()



#DEPENDENCIES

SOURCE_GROUP("LIBRAIRIES\\3D\\OGREMAX\\Headers" FILES 
../NxDeps/include/OgreMax/OgreMaxModel.hpp
../NxDeps/include/OgreMax/OgreMaxPlatform.hpp
../NxDeps/include/OgreMax/OgreMaxRenderWindowIterator.hpp
../NxDeps/include/OgreMax/OgreMaxScene.hpp
../NxDeps/include/OgreMax/OgreMaxTypes.hpp
../NxDeps/include/OgreMax/OgreMaxUtilities.hpp
../NxDeps/include/OgreMax/ProgressCalculator.hpp
../NxDeps/include/OgreMax/Version.hpp
)


SOURCE_GROUP("LIBRAIRIES\\3D\\OGREMAX\\Sources" FILES 
../NxDeps/include/OgreMax/OgreMaxModel.cpp
../NxDeps/include/OgreMax/OgreMaxScene.cpp
../NxDeps/include/OgreMax/OgreMaxUtilities.cpp
../NxDeps/include/OgreMax/ProgressCalculator.cpp
../NxDeps/include/OgreMax/Version.cpp
)

SOURCE_GROUP("LIBRAIRIES\\3D\\OGREPROCEDURALS\\Sources" FILES 
../NxDeps/include/OgreProcedurals/src/ProceduralBoolean.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralBoxGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralCapsuleGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralConeGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralCylinderGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralExtruder.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralGeometryHelpers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralIcoSphereGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralLathe.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMultiShape.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPath.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPathGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPlaneGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralPrecompiledHeaders.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralRoot.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralRoundedBoxGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralShape.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralShapeGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSphereGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTorusGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTorusKnotGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTrack.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTriangulator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTubeGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralUtils.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureBuffer.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralDebugRendering.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMeshModifiers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralMultiShapeGenerators.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralNoise.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSpringGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralSVG.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureGenerator.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTextureModifiers.cpp
../NxDeps/include/OgreProcedurals/src/ProceduralTriangleBuffer.cpp

)

SOURCE_GROUP("LIBRAIRIES\\3D\\OGREPROCEDURALS\\Headers" FILES 
../NxDeps/include/OgreProcedurals/include/Procedural.h
../NxDeps/include/OgreProcedurals/include/ProceduralBoolean.h
../NxDeps/include/OgreProcedurals/include/ProceduralBoxGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralCapsuleGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralConeGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralCylinderGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralExtruder.h
../NxDeps/include/OgreProcedurals/include/ProceduralGeometryHelpers.h
../NxDeps/include/OgreProcedurals/include/ProceduralIcoSphereGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralLathe.h
../NxDeps/include/OgreProcedurals/include/ProceduralMeshGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralMultiShape.h
../NxDeps/include/OgreProcedurals/include/ProceduralPath.h
../NxDeps/include/OgreProcedurals/include/ProceduralPathGenerators.h
../NxDeps/include/OgreProcedurals/include/ProceduralPlaneGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralPlatform.h
../NxDeps/include/OgreProcedurals/include/ProceduralRoot.h
../NxDeps/include/OgreProcedurals/include/ProceduralRoundedBoxGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralShape.h
../NxDeps/include/OgreProcedurals/include/ProceduralShapeGenerators.h
../NxDeps/include/OgreProcedurals/include/ProceduralSphereGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralSplines.h
../NxDeps/include/OgreProcedurals/include/ProceduralStableHeaders.h
../NxDeps/include/OgreProcedurals/include/ProceduralTorusGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTorusKnotGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTrack.h
../NxDeps/include/OgreProcedurals/include/ProceduralTriangleBuffer.h
../NxDeps/include/OgreProcedurals/include/ProceduralTriangulator.h
../NxDeps/include/OgreProcedurals/include/ProceduralTubeGenerator.h
../NxDeps/include/OgreProcedurals/include/ProceduralUtils.h
)


SOURCE_GROUP("LIBRAIRIES\\3D\\CADUNETREE\\Sources" FILES 
../NxDeps/include/CaduneTree/CTParameters.cpp
../NxDeps/include/CaduneTree/CTSection.cpp
../NxDeps/include/CaduneTree/CTSerializer.cpp
../NxDeps/include/CaduneTree/CTStem.cpp
)

SOURCE_GROUP("LIBRAIRIES\\3D\\PagedGeometry\\Sources" FILES 
../NxDeps/include/PagedGeometry/source/BatchedGeometry.cpp
../NxDeps/include/PagedGeometry/source/BatchPage.cpp
../NxDeps/include/PagedGeometry/source/GrassLoader.cpp
../NxDeps/include/PagedGeometry/source/ImpostorPage.cpp
../NxDeps/include/PagedGeometry/source/PagedGeometry.cpp
../NxDeps/include/PagedGeometry/source/PropertyMaps.cpp
../NxDeps/include/PagedGeometry/source/StaticBillboardSet.cpp
../NxDeps/include/PagedGeometry/source/TreeLoader2D.cpp
../NxDeps/include/PagedGeometry/source/TreeLoader3D.cpp
../NxDeps/include/PagedGeometry/source/WindBatchedGeometry.cpp
../NxDeps/include/PagedGeometry/source/WindBatchPage.cpp
)

SOURCE_GROUP("LIBRAIRIES\\3D\\OIS\\Headers" FILES 
../NxDeps/Include/OIS/OIS.h
../NxDeps/Include/OIS/OISConfig.h
../NxDeps/Include/OIS/OISEffect.h
../NxDeps/Include/OIS/OISEvents.h
../NxDeps/Include/OIS/OISException.h
../NxDeps/Include/OIS/OISFactoryCreator.h
../NxDeps/Include/OIS/OISForceFeedback.h
../NxDeps/Include/OIS/OISInputManager.h
../NxDeps/Include/OIS/OISInterface.h
../NxDeps/Include/OIS/OISJoyStick.h
../NxDeps/Include/OIS/OISKeyboard.h
../NxDeps/Include/OIS/OISMouse.h
../NxDeps/Include/OIS/OISObject.h
../NxDeps/Include/OIS/OISPrereqs.h
)

SOURCE_GROUP("LIBRAIRIES\\Utils\\TinyXml\\Source" FILES 
../NxDeps/Include/TinyXml/tinystr.cpp
../NxDeps/Include/TinyXml/tinyxml.cpp
../NxDeps/Include/TinyXml/tinyxmlerror.cpp
../NxDeps/Include/TinyXml/tinyxmlparser.cpp
)

SOURCE_GROUP("LIBRAIRIES\\Utils\\Fast\\Source" FILES 
../NxDeps/include/Fast/fast.c
../NxDeps/include/Fast/fast_10.c
../NxDeps/include/Fast/fast_11.c
../NxDeps/include/Fast/fast_12.c
../NxDeps/include/Fast/fast_9.c
../NxDeps/include/Fast/nonmax.c
)

SOURCE_GROUP("LIBRAIRIES\\3D\\Stereo\\Source" FILES 
../NxDeps/include/Stereo/StereoManager.cpp
)

SOURCE_GROUP("LIBRAIRIES\\3D\\Stereo\\Headers" FILES 
../NxDeps/include/Stereo/StereoManager.h
)


SOURCE_GROUP("LIBRAIRIES\\Scripting\\Lua\\Source" FILES 
../NxDeps/Include/Lua/Src/lapi.c
../NxDeps/Include/Lua/Src/lauxlib.c
../NxDeps/Include/Lua/Src/lbaselib.c
../NxDeps/Include/Lua/Src/lcode.c
../NxDeps/Include/Lua/Src/ldblib.c
../NxDeps/Include/Lua/Src/ldebug.c
../NxDeps/Include/Lua/Src/ldo.c
../NxDeps/Include/Lua/Src/ldump.c
../NxDeps/Include/Lua/Src/lfunc.c
../NxDeps/Include/Lua/Src/lgc.c
../NxDeps/Include/Lua/Src/linit.c
../NxDeps/Include/Lua/Src/liolib.c
../NxDeps/Include/Lua/Src/llex.c
../NxDeps/Include/Lua/Src/lmathlib.c
../NxDeps/Include/Lua/Src/lmem.c
../NxDeps/Include/Lua/Src/loadlib.c
../NxDeps/Include/Lua/Src/lobject.c
../NxDeps/Include/Lua/Src/lopcodes.c
../NxDeps/Include/Lua/Src/loslib.c
../NxDeps/Include/Lua/Src/lparser.c
../NxDeps/Include/Lua/Src/lstate.c
../NxDeps/Include/Lua/Src/lstring.c
../NxDeps/Include/Lua/Src/lstrlib.c
../NxDeps/Include/Lua/Src/ltable.c
../NxDeps/Include/Lua/Src/ltablib.c
../NxDeps/Include/Lua/Src/ltm.c
../NxDeps/Include/Lua/Src/lundump.c
../NxDeps/Include/Lua/Src/lvm.c
../NxDeps/Include/Lua/Src/lzio.c
)
 
SOURCE_GROUP("LIBRAIRIES\\2D\\SKIA\\Headers" FILES 
../NxDeps/include/Skia/include/core/Sk64.h
../NxDeps/include/Skia/include/core/SkAutoKern.h
../NxDeps/include/Skia/include/core/SkBitmap.h
../NxDeps/include/Skia/include/core/SkBlitRow.h
../NxDeps/include/Skia/include/core/SkBlitter.h
../NxDeps/include/Skia/include/core/SkBounder.h
../NxDeps/include/Skia/include/core/SkBuffer.h
../NxDeps/include/Skia/include/core/SkCanvas.h
../NxDeps/include/Skia/include/core/SkChunkAlloc.h
../NxDeps/include/Skia/include/core/SkColor.h
../NxDeps/include/Skia/include/core/SkColorFilter.h
../NxDeps/include/Skia/include/core/SkColorPriv.h
../NxDeps/include/Skia/include/core/SkColorShader.h
../NxDeps/include/Skia/include/core/SkComposeShader.h
../NxDeps/include/Skia/include/core/SkDeque.h
../NxDeps/include/Skia/include/core/SkDescriptor.h
../NxDeps/include/Skia/include/core/SkDevice.h
../NxDeps/include/Skia/include/core/SkDither.h
../NxDeps/include/Skia/include/core/SkDraw.h
../NxDeps/include/Skia/include/core/SkDrawFilter.h
../NxDeps/include/Skia/include/core/SkDrawLooper.h
../NxDeps/include/Skia/include/core/SkEdgeClipper.h
../NxDeps/include/Skia/include/core/SkEndian.h
../NxDeps/include/Skia/include/core/SkFDot6.h
../NxDeps/include/Skia/include/core/SkFixed.h
../NxDeps/include/Skia/include/core/SkFlattenable.h
../NxDeps/include/Skia/include/core/SkFloatBits.h
../NxDeps/include/Skia/include/core/SkFloatingPoint.h
../NxDeps/include/Skia/include/core/SkFontHost.h
../NxDeps/include/Skia/include/core/SkGeometry.h
../NxDeps/include/Skia/include/core/SkGlobals.h
../NxDeps/include/Skia/include/core/SkGraphics.h
../NxDeps/include/Skia/include/core/SkLineClipper.h
../NxDeps/include/Skia/include/core/SkMallocPixelRef.h
../NxDeps/include/Skia/include/core/SkMask.h
../NxDeps/include/Skia/include/core/SkMaskFilter.h
../NxDeps/include/Skia/include/core/SkMath.h
../NxDeps/include/Skia/include/core/SkMatrix.h
../NxDeps/include/Skia/include/core/SkMMapStream.h
../NxDeps/include/Skia/include/core/SkOSFile.h
../NxDeps/include/Skia/include/core/SkPackBits.h
../NxDeps/include/Skia/include/core/SkPaint.h
../NxDeps/include/Skia/include/core/SkPath.h
../NxDeps/include/Skia/include/core/SkPathEffect.h
../NxDeps/include/Skia/include/core/SkPathMeasure.h
../NxDeps/include/Skia/include/core/SkPerspIter.h
../NxDeps/include/Skia/include/core/SkPicture.h
../NxDeps/include/Skia/include/core/SkPixelRef.h
../NxDeps/include/Skia/include/core/SkPoint.h
../NxDeps/include/Skia/include/core/SkPostConfig.h
../NxDeps/include/Skia/include/core/SkPreConfig.h
../NxDeps/include/Skia/include/core/SkPtrRecorder.h
../NxDeps/include/Skia/include/core/SkRandom.h
../NxDeps/include/Skia/include/core/SkRasterizer.h
../NxDeps/include/Skia/include/core/SkReader32.h
../NxDeps/include/Skia/include/core/SkRect.h
../NxDeps/include/Skia/include/core/SkRefCnt.h
../NxDeps/include/Skia/include/core/SkRegion.h
../NxDeps/include/Skia/include/core/SkScalar.h
../NxDeps/include/Skia/include/core/SkScalarCompare.h
../NxDeps/include/Skia/include/core/SkScalerContext.h
../NxDeps/include/Skia/include/core/SkScan.h
../NxDeps/include/Skia/include/core/SkShader.h
../NxDeps/include/Skia/include/core/SkShape.h
../NxDeps/include/Skia/include/core/SkSize.h
../NxDeps/include/Skia/include/core/SkStream.h
../NxDeps/include/Skia/include/core/SkString.h
../NxDeps/include/Skia/include/core/SkStroke.h
../NxDeps/include/Skia/include/core/SkTDArray.h
../NxDeps/include/Skia/include/core/SkTDict.h
../NxDeps/include/Skia/include/core/SkTDStack.h
../NxDeps/include/Skia/include/core/SkTemplates.h
../NxDeps/include/Skia/include/core/SkThread.h
../NxDeps/include/Skia/include/core/SkThread_platform.h
../NxDeps/include/Skia/include/core/SkTime.h
../NxDeps/include/Skia/include/core/SkTRegistry.h
../NxDeps/include/Skia/include/core/SkTSearch.h
../NxDeps/include/Skia/include/core/SkTypeface.h
../NxDeps/include/Skia/include/core/SkTypes.h
../NxDeps/include/Skia/include/core/SkUnitMapper.h
../NxDeps/include/Skia/include/core/SkUnPreMultiply.h
../NxDeps/include/Skia/include/core/SkUtils.h
../NxDeps/include/Skia/include/core/SkWriter32.h
../NxDeps/include/Skia/include/core/SkXfermode.h

)

SOURCE_GROUP("LIBRAIRIES\\2D\\SKIA\\Sources\\Animator" FILES 
../NxDeps/include/Skia/src/animator/SkTime.cpp
)

SOURCE_GROUP("LIBRAIRIES\\2D\\SKIA\\Sources\\Core" FILES 
../NxDeps/include/Skia/src/core/Sk64.cpp
../NxDeps/include/Skia/src/core/SkAlphaRuns.cpp
../NxDeps/include/Skia/src/core/SkBitmap.cpp
../NxDeps/include/Skia/src/core/SkBitmap_scroll.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcShader.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcState.cpp
../NxDeps/include/Skia/src/core/SkBitmapProcState_matrixProcs.cpp
../NxDeps/include/Skia/src/core/SkBitmapSampler.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D16.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D32.cpp
../NxDeps/include/Skia/src/core/SkBlitRow_D4444.cpp
../NxDeps/include/Skia/src/core/SkBlitter.cpp
../NxDeps/include/Skia/src/core/SkBlitter_4444.cpp
../NxDeps/include/Skia/src/core/SkBlitter_A1.cpp
../NxDeps/include/Skia/src/core/SkBlitter_A8.cpp
../NxDeps/include/Skia/src/core/SkBlitter_ARGB32.cpp
../NxDeps/include/Skia/src/core/SkBlitter_ARGB32_Subpixel.cpp
../NxDeps/include/Skia/src/core/SkBlitter_RGB16.cpp
../NxDeps/include/Skia/src/core/SkBlitter_Sprite.cpp
../NxDeps/include/Skia/src/core/SkBuffer.cpp
../NxDeps/include/Skia/src/core/SkCanvas.cpp
../NxDeps/include/Skia/src/core/SkChunkAlloc.cpp
../NxDeps/include/Skia/src/core/SkColor.cpp
../NxDeps/include/Skia/src/core/SkColorFilter.cpp
../NxDeps/include/Skia/src/core/SkColorTable.cpp
../NxDeps/include/Skia/src/core/SkComposeShader.cpp
../NxDeps/include/Skia/src/core/SkConcaveToTriangles.cpp
../NxDeps/include/Skia/src/core/SkCordic.cpp
../NxDeps/include/Skia/src/core/SkCubicClipper.cpp
../NxDeps/include/Skia/src/core/SkDebug.cpp
../NxDeps/include/Skia/src/core/SkDeque.cpp
../NxDeps/include/Skia/src/core/SkDevice.cpp
../NxDeps/include/Skia/src/core/SkDither.cpp
../NxDeps/include/Skia/src/core/SkDraw.cpp
../NxDeps/include/Skia/src/core/SkEdge.cpp
../NxDeps/include/Skia/src/core/SkEdgeBuilder.cpp
../NxDeps/include/Skia/src/core/SkEdgeClipper.cpp
../NxDeps/include/Skia/src/core/SkFilterProc.cpp
../NxDeps/include/Skia/src/core/SkFlattenable.cpp
../NxDeps/include/Skia/src/core/SkFloat.cpp
../NxDeps/include/Skia/src/core/SkFloatBits.cpp
../NxDeps/include/Skia/src/core/SkFontHost.cpp
../NxDeps/include/Skia/src/core/SkGeometry.cpp
../NxDeps/include/Skia/src/core/SkGlobals.cpp
../NxDeps/include/Skia/src/core/SkGlyphCache.cpp
../NxDeps/include/Skia/src/core/SkGraphics.cpp
../NxDeps/include/Skia/src/core/SkLineClipper.cpp
../NxDeps/include/Skia/src/core/SkMask.cpp
../NxDeps/include/Skia/src/core/SkMaskFilter.cpp
../NxDeps/include/Skia/src/core/SkMath.cpp
../NxDeps/include/Skia/src/core/SkMatrix.cpp
../NxDeps/include/Skia/src/core/SkMemory_stdlib.cpp
../NxDeps/include/Skia/src/core/SkPackBits.cpp
../NxDeps/include/Skia/src/core/SkPaint.cpp
../NxDeps/include/Skia/src/core/SkPath.cpp
../NxDeps/include/Skia/src/core/SkPathEffect.cpp
../NxDeps/include/Skia/src/core/SkPathHeap.cpp
../NxDeps/include/Skia/src/core/SkPathMeasure.cpp
../NxDeps/include/Skia/src/core/SkPicture.cpp
../NxDeps/include/Skia/src/core/SkPictureFlat.cpp
../NxDeps/include/Skia/src/core/SkPicturePlayback.cpp
../NxDeps/include/Skia/src/core/SkPictureRecord.cpp
../NxDeps/include/Skia/src/core/SkPixelRef.cpp
../NxDeps/include/Skia/src/core/SkPoint.cpp
../NxDeps/include/Skia/src/core/SkProcSpriteBlitter.cpp
../NxDeps/include/Skia/src/core/SkPtrRecorder.cpp
../NxDeps/include/Skia/src/core/SkQuadClipper.cpp
../NxDeps/include/Skia/src/core/SkRasterizer.cpp
../NxDeps/include/Skia/src/core/SkRect.cpp
../NxDeps/include/Skia/src/core/SkRefCnt.cpp
../NxDeps/include/Skia/src/core/SkRegion.cpp
../NxDeps/include/Skia/src/core/SkRegion_path.cpp
../NxDeps/include/Skia/src/core/SkScalerContext.cpp
../NxDeps/include/Skia/src/core/SkScan.cpp
../NxDeps/include/Skia/src/core/SkScan_Antihair.cpp
../NxDeps/include/Skia/src/core/SkScan_AntiPath.cpp
../NxDeps/include/Skia/src/core/SkScan_Hairline.cpp
../NxDeps/include/Skia/src/core/SkScan_Path.cpp
../NxDeps/include/Skia/src/core/SkShader.cpp
../NxDeps/include/Skia/src/core/SkShape.cpp
../NxDeps/include/Skia/src/core/SkSpriteBlitter_ARGB32.cpp
../NxDeps/include/Skia/src/core/SkSpriteBlitter_RGB16.cpp
../NxDeps/include/Skia/src/core/SkStream.cpp
../NxDeps/include/Skia/src/core/SkString.cpp
../NxDeps/include/Skia/src/core/SkStroke.cpp
../NxDeps/include/Skia/src/core/SkStrokerPriv.cpp
../NxDeps/include/Skia/src/core/SkTSearch.cpp
../NxDeps/include/Skia/src/core/SkTypeface.cpp
../NxDeps/include/Skia/src/core/SkUnPreMultiply.cpp
../NxDeps/include/Skia/src/core/SkUtils.cpp
../NxDeps/include/Skia/src/core/SkWriter32.cpp
../NxDeps/include/Skia/src/core/SkXfermode.cpp
)

SOURCE_GROUP("LIBRAIRIES\\2D\\SKIA\\Sources\\Effects" FILES 
../NxDeps/include/Skia/src/effects/Sk1DPathEffect.cpp
../NxDeps/include/Skia/src/effects/Sk2DPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkAvoidXfermode.cpp
../NxDeps/include/Skia/src/effects/SkBlurDrawLooper.cpp
../NxDeps/include/Skia/src/effects/SkBlurMask.cpp
../NxDeps/include/Skia/src/effects/SkBlurMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkColorFilters.cpp
../NxDeps/include/Skia/src/effects/SkColorMatrixFilter.cpp
../NxDeps/include/Skia/src/effects/SkCornerPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkDashPathEffect.cpp
../NxDeps/include/Skia/src/effects/SkDiscretePathEffect.cpp
../NxDeps/include/Skia/src/effects/SkEmbossMask.cpp
../NxDeps/include/Skia/src/effects/SkEmbossMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkGradientShader.cpp
../NxDeps/include/Skia/src/effects/SkGroupShape.cpp
../NxDeps/include/Skia/src/effects/SkKernel33MaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkLayerDrawLooper.cpp
../NxDeps/include/Skia/src/effects/SkLayerRasterizer.cpp
../NxDeps/include/Skia/src/effects/SkPaintFlagsDrawFilter.cpp
../NxDeps/include/Skia/src/effects/SkPixelXorXfermode.cpp
../NxDeps/include/Skia/src/effects/SkPorterDuff.cpp
../NxDeps/include/Skia/src/effects/SkRectShape.cpp
../NxDeps/include/Skia/src/effects/SkTableMaskFilter.cpp
../NxDeps/include/Skia/src/effects/SkTransparentShader.cpp
)




 
 
 if (MSVC AND NOT NMAKE AND NOT OGRE_UNITY_BUILD)
 

	# add definition to use precompiled headers for all files :
	#add_definitions(/Yu"NxGraphicsPreCompiled.h")	

	# set source file to create precompiled header aka NxGraphicsPreCompiled.h
 	set_source_files_properties( "./NxGraphicsPreCompiled.cpp" PPROPERTIES COMPILE_FLAGS "/YcNxGraphicsPreCompiled.h")
	
	#set files to use precompiled headers
 	set_source_files_properties( ${PRECOMPILED} PPROPERTIES COMPILE_FLAGS "/YuNxGraphicsPreCompiled.h")
	# /FpRelease/NxGraphics.pch
	
	# choose files few files to not use precompiled headers
 	set_source_files_properties( ${NOPRECOMPILED} PPROPERTIES COMPILE_FLAGS "/Y-NxGraphicsPreCompiled.h" )
		
 endif()	


# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

# warning level
IF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
   # Use the lowest warning level for visual studio.
   SET(CMAKE_CXX_WARNING_LEVEL 0)
   IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     STRING(REGEX REPLACE "/W[0-4]" "/W0"
            CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   ELSE(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
   ENDIF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
ENDIF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")



#SOURCE_GROUP(foo_headers REGULAR_EXPRESSION "foo.*\\.h")
#ADD_EXECUTABLE(NxGraphics ${NxGraphics_src})

include_directories(
../nxdeps/include
../nxdeps/include/OIS
../nxdeps/include/OGRE
../nxdeps/include/OGRE/Paging
../nxdeps/include/OGRE/RenderSystems/GL/include
../nxdeps/include/OgreProcedurals/include
../nxdeps/include/PagedGeometry/include
../nxdeps/include/boost_1_49
../nxdeps/include/Lua/src
../nxdeps/include/LuaBind
../nxdeps/include/Physx/v2.8.4_win/SDKs/PhysXLoader/include
../nxdeps/include/Physx/v2.8.4_win/SDKs/Physics/include
../nxdeps/include/Physx/v2.8.4_win/SDKs/Foundation/include
../nxdeps/include/Physx/v2.8.4_win/SDKs/Cooking/include
../nxdeps/include/Physx/v2.8.4_win/SDKs/NxCharacter/include
../nxdeps/include/Physx/v2.8.4_win/Samples/SampleCommonCode/src/win
../nxdeps/include/Physx/v2.8.4_win/Samples/SampleCommonCode/src
../nxdeps/include/Physx/v2.8.4_win/Tools/NxuStream2
../nxdeps/include/Skia/include
../nxdeps/include/Skia/include/config
../nxdeps/include/Skia/include/core
../nxdeps/include/Skia/include/effects
../nxdeps/include/Skia/include/images
../nxdeps/include/Skia/include/views
../nxdeps/include/Skia/include/utils
../nxdeps/include/Skia/include/xml
../nxdeps/include/Skia/src/core
)

 

set(NxGraphics_DEF "NXGRAPHICS_NONCLIENT_BUILD;" )

set(NxGraphics_LINK_FLAGS "" )

if( MSVC )

	include_directories(./) 
	include_directories(./Physics/Physx_2.8.4)

	# from http:\\www.cmake.org/pipermail/cmake/2007-November/017372.html
	SET( NxGraphics_LINK_FLAGS "/NODEFAULTLIB:libcmt.lib")
 
	SET(NxGraphics_DEF
		"${NxGraphics_DEF}"
		TIXML_USE_STL
		OGREMAX_SHARED_LIBRARY_EXPORT
		NXCHARACTER_EXPORTS
		NX_USE_SOFTBODY_API
		NX_USE_FLUID_API
		SK_IGNORE_STDINT_DOT_H
		PROCEDURAL_NONCLIENT_BUILD
		SK_BUILD_FOR_WIN32
	)

	SET(NxGraphics_Deps 
	    ${PROJECT_SOURCE_DIR}/../Lib
		${PROJECT_SOURCE_DIR}/../nxdeps/lib/${CMAKE_BUILD_TYPE}
		${PROJECT_SOURCE_DIR}/../nxdeps/include/Physx/v2.8.4_win/SDKs/lib/win32
		
	)
 
	ADD_DEFINITIONS(/Zm500) 

	LINK_DIRECTORIES( ${NxGraphics_Deps} )

	SET(NxGraphics_EXTRALIBS
		debug NxVideo_d optimized NxVideo
		debug NxSound_d optimized NxSound
		debug NxMocap_d optimized NxMocap
		debug NxDevice_d optimized NxDevice
		winmm.lib
		comctl32.lib
		PhysXLoader.lib
		PhysXCooking.lib
		NxCharacter.lib
		debug OgreMain_d optimized OgreMain
		debug OgreOverlay_d optimized OgreOverlay
		debug OgrePaging_d optimized OgrePaging
		debug OgreRTShaderSystem_d optimized OgreRTShaderSystem
		debug OgreTerrain_d optimized OgreTerrain 
		debug OgreVolume_d optimized OgreVolume
		#OgreProperty.lib
		debug OIS_d optimized OIS
		opengl32.lib
	)
	
elseif(APPLE)	

	set( NxGraphics_DEF "${NxGraphics_DEF}"
		TIXML_USE_STL
		SK_BUILD_FOR_MAC
		OGREMAX_SHARED_LIBRARY_EXPORT
		PROCEDURAL_NONCLIENT_BUILD
	)
	
	#SK_IGNORE_STDINT_DOT_H

	set(NxGraphics_DEF "${NxGraphics_DEF}__MACOSX_CORE__;")
    set(NxGraphics_EXTRALIBS /System/Library/Frameworks/CoreAudio.framework )
    set(NxGraphics_EXTRALIBS /System/Library/Frameworks/AudioUnit.framework ${NxGraphics_EXTRALIBS})
	set(NxGraphics_EXTRALIBS /System/Library/Frameworks/CoreMidi.framework ${NxGraphics_EXTRALIBS})
    set(NxGraphics_EXTRALIBS /System/Library/Frameworks/ApplicationServices.framework ${NxGraphics_EXTRALIBS})

	# Some versions of OSX may need the AudioToolbox framework. Add it if it's found.
    FIND_LIBRARY(AUDIOTOOLBOX_LIBRARY NAMES AudioToolbox PATHS ~/Library/Frameworks /Library/Frameworks /System/Library/Frameworks )
    IF(AUDIOTOOLBOX_LIBRARY)
    	SET(NxGraphics_EXTRALIBS ${AUDIOTOOLBOX_LIBRARY} ${NxGraphics_EXTRALIBS})
    ENDIF()
	include_directories(./) 
	include_directories(./Physics/empty)	
	include_directories(../nxdeps/include/libFlac)

elseif(UNIX)

message( "NxGraphics System is UNIX "  )

#warning messages from NxVideo
add_definitions(-Wno-multichar)

	include_directories(./) 
	include_directories(./Physics/empty)
	
	
	include_directories(
	../nxdeps/include/OGRE/Plugins/OctreeSceneManager
	../nxdeps/include/OGRE/Plugins/ParticleFX
	../nxdeps/include/OGRE/RenderSystems/GLES2
	../nxdeps/include/OGRE/RenderSystems/GLES2/ANDROID	
	../nxdeps/include/OGRE/RTShaderSystem
	)	
	

	set( NxGraphics_DEF "${NxGraphics_DEF}"
		TIXML_USE_STL
		OGREMAX_SHARED_LIBRARY_EXPORT
		SK_IGNORE_STDINT_DOT_H
		PROCEDURAL_NONCLIENT_BUILD
	)
	#SK_BUILD_FOR_ANDROID ??

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=gnu++98 -fno-common -fno-strict-aliasing -fno-rtti -fno-exceptions -Wall -Wundef -Wno-long-long -pedantic -I/usr/local/include")

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_FLAGS} -O3")

#list( APPEND CMAKE_CXX_FLAGS "-std=C++98 ${CMAKE_CXX_FLAGS} -g -ftest-coverage -fprofile-arcs")


 



#if(CMAKE_COMPILER_IS_GNUCXX)
 
#  if(CMAKE_SYSTEM_NAME MATCHES Linux)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COVERAGE_FLAGS} -g2")
#    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${COVERAGE_FLAGS} -O2 -g2")
#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${COVERAGE_FLAGS} -fno-inline-functions")
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${COVERAGE_FLAGS} -O0 -g3")
#  endif(CMAKE_SYSTEM_NAME MATCHES Linux)
 
#endif(CMAKE_COMPILER_IS_GNUCXX)




else()

	
	
endif()


if( MSVC )
add_library(${NxGraphics_LIBRARY} SHARED ${NxGraphics_src})
else()
add_library(${NxGraphics_LIBRARY} STATIC ${NxGraphics_src})
endif()


set_target_properties(${NxGraphics_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxGraphics_DEF}" LINK_FLAGS_RELEASE "${NxGraphics_LINK_FLAGS}")

set_target_properties(${NxGraphics_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxGraphics_DEF}" LINK_FLAGS_DEBUG "${NxGraphics_LINK_FLAGS}")

target_link_libraries(${NxGraphics_LIBRARY} ${NxGraphics_EXTRALIBS})












