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
#ifndef __NXPASS_H__
#define __NXPASS_H__

#include "NxPrerequisites.h"

namespace Nx {

class NxGraphics_Export NxPass
{
private :
	typedef vector<NxTextureUnit*>::type Units;
	/// List of primary units
	Units mUnits;
public :
	//! NxPass Class Constructor.
	NxPass( NxTechnique * parent, const std::string & PassName );
	//! NxPass Class Destructor.
	~NxPass();
	//! Create a Texture Unit.
	NxTextureUnit * CreateTextureUnit( const std::string & FileName );
	//! Get Texture Unit.
	NxTextureUnit * GetTextureUnit( const std::string & name );
	//! Get technique Name.
	const std::string & GetName() const;
	//! Get number of Texture Units.
	unsigned short GetNumTextureUnit() const;
	//! Set Vertex Program.
	void SetVertexProgram( const std::string & VertexProgramName );
	//! set vertex parameter value.
	void SetVertexParameterValue( const std::string & Name, int Value );
	//! set vertex parameter value.
	void SetVertexParameterValue( const std::string & Name, float Value );
	//! set vertex parameter value.
	void SetVertexParameterValue( const std::string & Name, const Nx::Vector3 & Value );
	//! set vertex parameter value.
	void SetVertexParameterValue( const std::string & Name, const Nx::Vector4 & Value );
	//! set vertex parameter value.
	void SetVertexParameterValue( const std::string & Name, const Nx::Matrix4 & Value );
	//! set vertex Auto parameter value.
	void SetVertexAutoParameterValue( const std::string & Name, NxAutoConstantType Type );
	//! set vertex Auto parameter Float value( Time, Sin, etc ).
	void SetVertexAutoParameterFloatValue( const std::string & Name, NxAutoConstantType Type, float Value );
	//! Set Fragment Program.
	void SetFragmentProgram( const std::string & FragmentProgramName );
	//! set Fragment parameter value.
	void SetFragmentParameterValue( const std::string & Name, int Value );
	//! set Fragment  parameter value.
	void SetFragmentParameterValue( const std::string & Name, float Value );
	//! set Fragment parameter value.
	void SetFragmentParameterValue( const std::string & Name, const Nx::Vector2 & Value );
	//! set Fragment parameter value.
	void SetFragmentParameterValue( const std::string & Name, const Nx::Vector3 & Value );
	//! set Fragment parameter value.
	void SetFragmentParameterValue( const std::string & Name, const Nx::Vector4 & Value );
	//! set Fragment parameter value.
	void SetFragmentParameterValue( const std::string & Name, const Nx::Matrix4 & Value );
	//! set Fragment Auto parameter value.
	void SetFragmentAutoParameterValue( const std::string & Name, NxAutoConstantType Type );

	//! Set Geometry Program.
	void SetGeometryProgram( const std::string & GeometryProgramName );
	//! set Geometry parameter value.
	void SetGeometryParameterValue( const std::string & Name, float Value );
	//! set Geometry parameter value.
	void SetGeometryParameterValue( const std::string & Name, const Nx::Vector2 & Value );
	//! set Geometry parameter value.
	void SetGeometryParameterValue( const std::string & Name, const Nx::Vector3 & Value );
	//! set Geometry parameter value.
	void SetGeometryParameterValue( const std::string & Name, const Nx::Vector4 & Value );
	//! set Geometry array parameter value.
	void SetGeometryParameterValue( const std::string & Name, const float * dataArray, size_t Arraysize, size_t multiple );
	//! set Geometry Auto parameter value.
	void SetGeometryAutoParameterValue( const std::string & Name, NxAutoConstantType Type );
	//! set vertex tracking color if enabled.
	void SetVertexColourTracking( NxTrackVertexColourEnum track );
	//! get vertex tracking color
	NxTrackVertexColourType GetVertexColourTracking() const;
	//! Set Diffuse.
	void SetDiffuse( const NxColourValue& diffuse );
	//! Set Ambient.
	void SetAmbient( const NxColourValue& ambient );
	//! Set Specular.
	void SetSpecular(const NxColourValue& specular);
	//! Set Emissive.
	void SetEmissive( const NxColourValue& emissive );
	//! Set Shininess
	void SetShininess( float val);
	//! Set Scene Blending
	void SetSceneBlending( const NxSceneBlendType sbt );
	//! Set Separate Scene Blending
	void SetSeparateSceneBlending( const NxSceneBlendType sbt, const NxSceneBlendType sbta );
	void SetSceneBlending( const NxSceneBlendFactor sourceFactor, const NxSceneBlendFactor destFactor);
	void SetSeparateSceneBlending( const NxSceneBlendFactor sourceFactor, const NxSceneBlendFactor destFactor, const NxSceneBlendFactor sourceFactorAlpha, const NxSceneBlendFactor destFactorAlpha );
	void SetSceneBlendingOperation( NxSceneBlendOperation op);
	void SetSeparateSceneBlendingOperation( NxSceneBlendOperation op, NxSceneBlendOperation alphaOp );

	void SetDepthBias(float constantBias, float slopeScaleBias = 0.0f);

	void SetDepthCheckEnabled( bool Enabled );
	void SetDepthWriteEnabled( bool Enabled );
	void SetLightingEnabled( bool Enabled );

	void SetAlphaRejectSettings(NxCompareFunction func, unsigned char value, bool alphaToCoverage = false);
	void SetAlphaRejectFunction(NxCompareFunction func);

	void SetAlphaToCoverageEnabled(bool enabled);
	void SetTransparentSortingEnabled(bool enabled);
	bool GetTransparentSortingEnabled() const;
	void SetTransparentSortingForced(bool enabled);
	bool GetTransparentSortingForced() const;


	void SetManualCullingMode( NxManualCullingMode mode );

	void SetCullingMode( NxCullingMode Mode );
	//! Set point sprites enabled.
	void SetPointSpritesEnabled( bool Enabled );
	//! Set point sprites size.
	void SetPointSize( float size );
	//! Set point sprites attenuation.
	void SetPointAttenuation( bool enabled, float constant = 0.0f, float linear = 1.0f, float quadratic = 0.0f );
	//! Set min point size.
	void SetPointMinSize( float Min );
	//! Set max point size.
	void SetPointMaxSize( float Max );
	//! set polygon mode
	void SetPolygonMode( NxPolygonMode mode );
	//! Get Pass
	Ogre::Pass * GetPass();
private :
	Ogre::Pass * mPass;
};


}//namespace

#endif