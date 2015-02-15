#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxShader::NxShader() : mProgram(0)
{

}

NxShader::~NxShader()
{
	const std::string & name = mProgram->getName();
	Ogre::HighLevelGpuProgramManager::getSingleton().unload(name);
	Ogre::HighLevelGpuProgramManager::getSingleton().remove(name);
 
}

const std::string & NxShader::GetName()
{
	return mProgram->getName();
}

void NxShader::SetParameter( const std::string & ParamName, const std::string & ParamDef )
{
	mProgram->setParameter( ParamName, ParamDef ); 
}

void NxShader::SetSource( const std::string & shaderDef )
{
	mProgram->setSource( shaderDef );
}

void NxShader::SetSourceFile( const std::string & shaderFile )
{
	mProgram->setSourceFile( shaderFile );
}

void NxShader::Load( bool backgroundThread )
{
	mProgram->load( backgroundThread );
}

void NxShader::Reload()
{
	mProgram->reload();
}

void NxShader::Unload()
{
	mProgram->unload();
}



//=============================================

NxVertexShader::NxVertexShader( const std::string & VertexProgramName, const std::string & ShaderType ) : NxShader()
{

 

	HighLevelGpuProgramPtr vertexShader = HighLevelGpuProgramManager::getSingleton().createProgram( 
	VertexProgramName, 
	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	ShaderType,
	GPT_VERTEX_PROGRAM );
	//vertexShader->setSourceFile( FileName );
	//vertexShader->load();

	mProgram = vertexShader.get();
 
}

NxVertexShader::~NxVertexShader()
{

}
 
NxPixelShader::NxPixelShader( const std::string & PixelProgramName, const std::string & ShaderType ) : NxShader()
{

	HighLevelGpuProgramPtr PixelShader = HighLevelGpuProgramManager::getSingleton().createProgram( PixelProgramName,
	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	ShaderType,
	GPT_FRAGMENT_PROGRAM );
	//PixelShader->setSourceFile( FileName );
	//PixelShader->load();

	mProgram = PixelShader.get();
 
}

NxPixelShader::~NxPixelShader()
{

}

NxGeometryShader::NxGeometryShader( const std::string & GeometryProgramName, const std::string & ShaderType ) : NxShader()
{

	HighLevelGpuProgramPtr GeometryShader = HighLevelGpuProgramManager::getSingleton().createProgram( GeometryProgramName,
	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	ShaderType,
	GPT_GEOMETRY_PROGRAM );
	//GeometryShader->setSourceFile( FileName );
	//GeometryShader->load();

	mProgram = GeometryShader.get();
 
}

NxGeometryShader::~NxGeometryShader()
{

}



}