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
#ifndef __NXPLUGIN_H__
#define __NXPLUGIN_H__

#include "NxPrerequisites.h"

namespace Nx {

enum PluginParameterTypes
{ 
	PluginParameterTypeNone,
	PluginParameterTypeSlider,
	PluginParameterTypeButton,
	PluginParameterTypeComboBox,
	PluginParameterTypeColour  
};


//!  Base Class For All Plugin Parameters.
/*!
	this is the base class for all plugin parameters and shouldnt be used directly.
*/
class NxGraphics_Export NxPluginParam
{
protected:
	//! NxPluginParam class Constructor
	NxPluginParam( std::string PluginParamName, PluginParameterTypes ParameterType );
public :
	//! NxPluginParam class Destructor.
	virtual ~NxPluginParam();
	//! Get Parameter Name.
	const std::string GetParamName() const;
	//! Set Default Value.
	virtual void SetDefaultValue( float val );
	//! Get Default Value.
	float GetDefaultValue();
	//! Get Value.
	float GetValue();
	//! Set Value.
	void SetValue( float val );
	//! Get Minimum Value;
	float GetMin();
	//! Get Maximum Value.
	float GetMax();
	//! Set Param Description.
	void SetParamDescription( std::string Description );
	//! Get Param Description.
	std::string GetParamDescription();
	//! Set Parameter Index.
	void SetParamIndex( int Index );
	//! Get Parameter index.
	int GetParamIndex();
	//! Set Parent Plugin.
	void SetHost( NxPlugin * Host );
	//! Get Parent Plugin.
	NxPlugin * GetHost();
	//! Get Parameter Type. see PluginParameterTypes.
	PluginParameterTypes GetParameterType();
 
protected:
	//! Plugin Host.
	NxPlugin * mPluginHost;
	//! Plugin Type
	NxGraphics_Plugin_Effect_Types mPluginType;
	//! Plugin Sub Type.
	NxGraphics_Plugin_Effect_SubTypes mPluginSubType;
	//! Parameter Type.
	PluginParameterTypes mParamType;
	//! Minimum Value.
	double mValueMin;
	//! Mximum Value.
	double mValueMax;
	//! Parameter Index.
	int mParamIndex ;
	
private :
	float ValueIndex;
	//! Default Value.
	float mValueDefault;
	//! Parameter Name.
	std::string mParamName;
	//! Display Name.
	std::string mParamDescription;
};

//!  NxPluginParamButton Class.
/*!
	This class Is managing Button Parameters.
*/
class NxGraphics_Export NxPluginParamButton : public NxPluginParam
{
public :
	//! NxPluginParamButton class Constructor.
	NxPluginParamButton( std::string PluginParamName );
	//! NxPluginParamButton class Destructor.
	~NxPluginParamButton();

};

//!  NxPluginParamColorButton Class.
/*!
	This class Is managing Color Button Parameters.
*/
class NxGraphics_Export NxPluginParamColorButton : public NxPluginParam
{
public :
	//! NxPluginParamColorButton class Constructor.
	NxPluginParamColorButton( std::string PluginParamName );
	//! NxPluginParamColorButton class Destructor.
	~NxPluginParamColorButton();
	//! Set Button Default Color.
	void SetDefaultColor( NxColourValue Col );
	//! Get Button Default Color.
	NxColourValue GetDefaultColor();
private:
	//! Button Default Color.
	NxColourValue mDefaultColourParam;
};

//!  NxPluginParamSlider Class.
/*!
	This class is managing Slider Parameters.
*/
class NxGraphics_Export NxPluginParamSlider : public NxPluginParam
{
public :
	//! NxPluginParamSlider class Constructor.
	NxPluginParamSlider( std::string PluginParamName );
	//! NxPluginParamSlider class Destructor.
	~NxPluginParamSlider();
	//! Set Slider Range.
	void SetRange( double MinVal, double MaxVal, double Increment );
	//! Get Increment Value.
	double GetIncrementValue();
	//! Get Inbound Value Limit;
	double GetInboundValue();
	//! Get Outbound Value Limit;
	double GetOutboundValue();

private :
	//! Increment Value for a slider.
	double mValueIncrement;
	//! Inbound Value for a slider with 2 limiters.
	double mValueInbound;
	//! Outbound Value for a slider with 2 limiters.
	double mValueOutbound;
};

//!  NxPluginParamList Class.
/*!
	This class is managing ComboBox Parameters.
*/
class NxGraphics_Export NxPluginParamList : public NxPluginParam
{
public :
	//! NxPluginParamList class Constructor.
	NxPluginParamList( std::string PluginParamName );
	//! NxPluginParamList class Destructor.
	~NxPluginParamList();
	void InitializeParameter();
	std::vector< std::string > GetList();
	void UpdateList( std::vector<  std::string > List );
	void SetComboBoxDefaultParameter( std::string str );
	std::string GetComboBoxDefaultParameter();
	std::vector< std::string > StringList;
private:
	std::string mDefaultComboBoxParam ;

};

//!  NxPluginDescription Class.
/*!
	This class provides description for a plugin.
*/
class NxPluginDescriptionStruct;
class NxGraphics_Export NxPluginDescription
{
public : 
	//! NxPluginDescription Constructor.
	NxPluginDescription();
	//! NxPluginDescription Constructor.
	NxPluginDescription( std::string PluginName, std::string PluginPath, NxGraphics_Plugin_Effect_Types Type, NxGraphics_Plugin_Effect_SubTypes SubType );
	//! NxPluginDescription DeConstructor.
	~NxPluginDescription();
	//! Load Base Information from path.
	void LoadInformation( std::string Path );
	//! Load plugin in main plugin pool.
	void Register( NxPluginManager & PlugManager );
	//! Get Plugin Path.
	const std::string GetPath() const;
	//! Get Plugin Name.
	const std::string GetName() const;
	//! Get Plugin Type.
	const NxGraphics_Plugin_Effect_Types GetType() const ;
	//! Get Plugin Sub Type.
	const NxGraphics_Plugin_Effect_SubTypes GetSubType() const ;

private:
	//! Plugin Name.
	std::string mPluginName;
	//! Plugin Path.
	std::string mPluginPath;
	//! Plugin Type.
	NxGraphics_Plugin_Effect_Types mPluginType;
	//! Plugin SubType.
	NxGraphics_Plugin_Effect_SubTypes mPluginSubType;
	//! Plugin Version.
	float mVersion;

    typedef const char * GetPluginName(); // Plugin Name.
    typedef int GetPluginType(); // Plugin Type.
    typedef int GetPluginSubType(); // Plugin Sub Type.
    typedef float GetPluginVersion();
    typedef void RegisterPlugin( NxPluginManager & );

	NxPluginDescriptionStruct * mHelper; 

	//! Dll Instance.
    //HMODULE m_hDLL;
	//! Number of references to the DLL.
    size_t * m_pDLLRefCount;
	//! Version query function.
    GetPluginVersion * m_pfnGetPluginVersion;
	//! Plugin Name.
    GetPluginName * m_pfnGetPluginName;
	//! Plugin Type.
    GetPluginType * m_pfnGetPluginType; 
	//! Plugin Sub Type.
    GetPluginSubType * m_pfnGetPluginSubType;
	//! Plugin registration function.
    RegisterPlugin * m_pfnRegisterPlugin;
};

//!  NxPlugin Class.
/*!
	Dont use directly this class.
*/
class NxPluginStruct;
class NxGraphics_Export NxPlugin
{
protected:

public :
	//! NxPlugin Constructor.
	NxPlugin();
	//! NxPlugin Copy Constructor;
    NxPlugin(const NxPlugin &Other);
	//! NxPlugin Destructor.
	virtual ~NxPlugin();
	//! Load Plugin and default Information;
	virtual bool Load( const std::string & Filename );
	//! Unload.
	void Unload();
	//! Get Plugin Name.
	std::string GetPluginName();
	//! Get Plugin Version.
	float GetPluginVersion() const;
	//! Get Plugin Path.
	std::string GetPluginPath();
	//! Get Plugin Type.
	NxGraphics_Plugin_Effect_Types GetPluginType();	
	//! Get Plugin Sub Type.
	NxGraphics_Plugin_Effect_SubTypes GetPluginSubType();
	//! Is Muted
	bool IsMuted();
	//! Set Muted
	bool SetMute( bool mMute );
	//! initialize Plugin.
	virtual bool Initialize();
	//! Deinitialize Plugin.
	virtual bool DeInitialize();
	//! Is Plugin Initialized ?
	bool IsInitialized();
	//Get Number Parameters
	size_t GetNumParams();
	//! Get Parameter.
	const NxPluginParam & GetParam( int ParamIndex ) const ;
	//! Get Parameter Index.
	int GetIndexParam( NxPluginParam * parameter );
	//! Get Parameter List.
	void GetParameterList( 	std::vector< NxPluginParam * > & ParamList );
	//! ComboBox Callback.
	virtual void ProcessComboBox( NxPluginParam * Parameter, std::string ParamName ); 
	//! Button Callback.
	virtual void ProcessButton( NxPluginParam * Parameter, bool Toggled, void * Data ); 
	//! Slider Callback.
	virtual void ProcessSlider( NxPluginParam * Parameter, double Value );

	void SetAssociatedLayerNumber( int layer );
	int GetAssociatedLayerNumber();
 
protected :
	bool Muted ;
	int AssociatedLayerNumber ;
	int LayerWidth;
	int LayerHeight;
	int LayerBpp;
	bool Initialized;
	bool Instantiated;
	std::string PluginName;
	std::string PluginPath;
	std::vector< NxPluginParam * > ParamList;
	NxGraphics_Plugin_Effect_SubTypes mPluginSubType;
	NxGraphics_Plugin_Effect_Types mPluginType;

private:

    typedef const char * fnGetPluginName(); // Plugin Name.
    typedef int fnGetPluginType(); // Plugin Type.
    typedef int fnGetPluginSubType(); // Plugin Sub Type.
    typedef float fnGetPluginVersion();
    typedef void fnRegisterPlugin( NxPluginManager & );

	//! Dll Instance.
    //HMODULE m_hDLL;
	NxPluginStruct * mHelper;
	//! Number of references to the DLL.
    size_t * m_pDLLRefCount;
	//! Version query function.
    fnGetPluginVersion * m_pfnGetPluginVersion;
	//! Plugin Name.
    fnGetPluginName * m_pfnGetPluginName;
	//! Plugin Type.
    fnGetPluginType * m_pfnGetPluginType; 
	//! Plugin Sub Type.
    fnGetPluginSubType * m_pfnGetPluginSubType;
	//! Plugin registration function.
    fnRegisterPlugin * m_pfnRegisterPlugin;
};

class NxGraphics_Export NxPluginVideo : public NxPlugin
{
protected:
	//! NxPlugin3D Constructor.
	NxPluginVideo(){};
public:
	//! NxPlugin3D Destructor.
	virtual ~NxPluginVideo(){};
	//virtuals
	bool Initialize() = 0 ;
	bool DeInitialize() = 0;
	virtual void SetVideoWidth( unsigned int ) = 0;
	virtual void SetVideoHeight( unsigned int ) = 0;
	virtual void SetVideoBpp( unsigned int ) = 0;
	virtual unsigned long ProcessFrame( void * Buffer ) = 0;
 
};

class NxGraphics_Export NxPluginVideoFreeframe : public NxPluginVideo
{
public:
	NxPluginVideoFreeframe();
	~NxPluginVideoFreeframe();
	bool Initialize();
	bool DeInitialize();
	void SetVideoWidth( unsigned int );
	void SetVideoHeight( unsigned int );
	void SetVideoBpp( unsigned int );
	bool Load( const std::string & Filename );
	unsigned long ProcessFrame( void * Buffer );
};

class NxGraphics_Export NxPlugin3D : public NxPlugin
{
public:
	//! NxPlugin3D Constructor.
	NxPlugin3D(){};
	//! NxPlugin3D Destructor.
	virtual ~NxPlugin3D(){};

	bool Initialize() = 0 ;
	bool DeInitialize() = 0;
	//virtual bool frameStarted( const Ogre::FrameEvent& evt ) = 0;
	//virtual bool frameEnded( const Ogre::FrameEvent &evt ) = 0;
	virtual void SetSceneManager( Ogre::SceneManager * mManager ) = 0;
	virtual void SetCamera( Ogre::Camera * mCamera ) = 0 ;
};

class NxGraphics_Export NxPluginManager : public NxSingleton<NxPluginManager>
{
public:
	//! NxPluginManager Constructor.
	NxPluginManager();
	//! NxPluginManager Destructor.
	~NxPluginManager();
	//! NxPluginManager Singleton.
	static NxPluginManager& getSingleton();
	//! NxPluginManager Singleton Pointer.
	static NxPluginManager* getSingletonPtr();
	//! Get Plugin List Info.
	void GetPluginsInfo( std::vector<NxPluginDescription*> & PluginsDescriptions );
	//! Load A Plugin By Description.
	NxPlugin * LoadPlugin( NxPluginDescription * Plugin );
	//! Load A Plugin By Dll Name.
	NxPlugin * LoadPlugin( const std::string & DllName );

	class NxSystemPlugin //: public NxPlugin
	{
		public:
		virtual ~NxSystemPlugin(){} // Destructor
		virtual const std::string &getName() const = 0;// Get name of the graphics driver
		virtual std::auto_ptr<NxPlugin> CreateSystem() = 0;// Create a renderer
		std::vector< NxPluginParam * > ParamList;
	};

	void addSystemPlugin( std::auto_ptr<NxSystemPlugin> SysPlug );// Allows plugins to add new graphics drivers
	size_t getSystemPluginsCount() const; // Get the total number of registered graphics drivers
	NxSystemPlugin &getSystemPlugin( size_t Index ); // Access a driver by its index

	void addPlugin3D( std::auto_ptr<NxPlugin3D> Plug3D ); 
	size_t getPlugins3DCount() const; 
	NxPlugin3D &getPlugin3D( size_t Index ); 
 

private:
    typedef std::vector<NxSystemPlugin *> SystemPluginVector; // All available System Plugins.
    SystemPluginVector mSystemPlugins; 

    typedef std::vector<NxPlugin3D *> PluginVector3D; // All available 3D Plugins.
    PluginVector3D mPlugins3D;
};

} // namespace


#endif