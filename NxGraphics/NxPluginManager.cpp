#include "NxGraphicsPreCompiled.h"

namespace Nx {

template<> NxPluginManager* NxSingleton<NxPluginManager>::ms_Singleton = 0;

NxPluginManager::NxPluginManager()
{

}

NxPluginManager::~NxPluginManager()
{
	for( SystemPluginVector::reverse_iterator It = mSystemPlugins.rbegin(); It != mSystemPlugins.rend(); ++It )// Delete all 
	{
		delete *It;
	}
}

NxPluginManager& NxPluginManager::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

NxPluginManager* NxPluginManager::getSingletonPtr(void)
{
	return ms_Singleton;
}

void NxPluginManager::GetPluginsInfo( std::vector<NxPluginDescription*> & PluginsDescriptions )
{
	std::string NxPluginsDirectory = "..\\..\\NxMedia\\Plugins\\NxPlugins\\";
	std::vector<std::string> PluginPaths;
	int result = NxUtils::SearchDirectory( PluginPaths, NxUtils::GetCurrentWorkingDirectory() + NxPluginsDirectory , "dll", false );
	if(!result)// did find the plugins
	{
	   for(std::vector<std::string>::iterator NxPluginsIter = PluginPaths.begin(); NxPluginsIter != PluginPaths.end(); ++NxPluginsIter )
	   {
			std::string Path = NxUtils::GetFullPath( *NxPluginsIter );
			NxPluginDescription * PlugDesc = new NxPluginDescription();
			PlugDesc->LoadInformation( Path );
			PluginsDescriptions.push_back( PlugDesc );
	   }
	}
}

NxPlugin * NxPluginManager::LoadPlugin( NxPluginDescription * Plugin )
{
	Plugin->Register(*this);
	return ( NxPlugin *  )  true;//mPlugins3D.front(); // to do verify plugin type
	
}

NxPlugin * NxPluginManager::LoadPlugin( const std::string & DllName )
{
	NxPlugin * Plugin = 0;
	std::vector<NxPluginDescription*> PluginsDescriptions;
	GetPluginsInfo( PluginsDescriptions );// all in plugin folder
	for( int i =0; i < PluginsDescriptions.size(); i++ )
	{ 
		std::string FileName = PluginsDescriptions[i]->GetPath();
		NxUtils::SplitFileName( FileName );
		if( FileName.compare(DllName) == 0 )
		{
			LogMsg("loading plugin from path...");
			Plugin = LoadPlugin( PluginsDescriptions[i] );
			LogMsg("loading plugin from path ok");

			break;
		}
	}
	//All the elements of the vector are dropped: their destructors are called, and then they are removed from the vector container, leaving the container with a size of 0.
	LogMsg("Clearing descriptions");
	//PluginsDescriptions.clear();
	LogMsg("Clearing descriptions ok");
	return Plugin;

}

void NxPluginManager::addSystemPlugin( std::auto_ptr<NxSystemPlugin> SysPlug )
{
	mSystemPlugins.push_back(SysPlug.release());
}

size_t NxPluginManager::getSystemPluginsCount() const 
{
	return mSystemPlugins.size();
}

NxPluginManager::NxSystemPlugin & NxPluginManager::getSystemPlugin( size_t Index )
{
	return *mSystemPlugins.at(Index);
}

void NxPluginManager::addPlugin3D( std::auto_ptr<NxPlugin3D> Plug3D )
{
	mPlugins3D.push_back( Plug3D.release() );	 
}

size_t NxPluginManager::getPlugins3DCount() const
{
	return mPlugins3D.size();
}

NxPlugin3D & NxPluginManager::getPlugin3D( size_t Index )
{
	return *mPlugins3D.at(Index);
} 

}