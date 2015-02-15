#include "NxGraphicsPreCompiled.h"
#include "..\nxdeps\include\FreeFrame\Include\freeframe.h"

namespace Nx {

//========================================================

typedef __declspec(dllimport) void* (__stdcall *NxFF_Main_FuncPtr)(DWORD,LPVOID,DWORD);

class NxGraphics_Export NxGraphics_Plugin_Effect_Freeframe : public NxPlugin 
{
public :
	NxGraphics_Plugin_Effect_Freeframe();
	bool Initialize() ;
	bool Deinitialise();
	void ProcessComboBox( NxPluginParam * Parameter , std::string ParamName ){};
    void ProcessButton( NxPluginParam * Parameter , bool Toggled , void * Data ){};
	void ProcessSlider( NxPluginParam * Parameter , double Value );
	DWORD ProcessFrame( LPVOID  Buffer  );
	const PluginInfoStruct * GetInfo() const ;
	bool GetInfo( PluginInfoStruct& PlugInfo) const ;
	static void CopyFFString( std::string& dst, const BYTE *src, DWORD len );
	bool GetPluginName( std::string& Name ) const ;
	bool GetPluginId( std::string& Id ) const ;
	int GetNumParams() const;
	bool GetParamName(int ParamIdx, std::string& Name ) const;
	float GetParamDefault(int ParamIdx) const ;
	void GetParamDisplay(int ParamIdx , std::string& DisplayName ) const;
	int GetPluginCaps(int CapsIdx) const;
	const PluginExtendedInfoStruct * GetExtendedInfo() const ;
	bool GetExtendedInfo( PluginExtendedInfoStruct& PlugExtInfo ) const;
	int GetParamType(int ParamIdx) const;
	bool GetPluginPath(std::string& Path) const;
	bool Instantiate( bool ResetParms );
	void GetLoadedPluginInfo() ;
	bool Free();
	bool Load( const std::string & Path );
	bool DeInitialize();
	bool Mute( bool mute );
	bool IsCreated() const;
	LPCSTR GetParamDisplay( int ParamIdx ) const;
	float GetParam( int ParamIdx ) const;
	bool SetParam( const SetParameterStruct& sps );
	bool SetParam( int ParamIdx, float Value );
	// Operations
	bool ProcessFrameInter( LPVOID Frame );
	bool ProcessFrameCopy( const ProcessFrameCopyStruct& pfcs );
	bool Create( NxGraphics_Plugin_Effect_Freeframe * Plugin,  VideoInfoStruct * VideoInfo );
	bool Destroy();

protected :
	HINSTANCE m_hInst;	// instance handle of the plugin's DLL
	NxFF_Main_FuncPtr m_pff;	// pointer to the plugin's main function
	//FF_Main_FuncPtr m_pff;
	LPVOID	m_Instance;		// if we're created, plugin's instance ID
	LPVOID CallFF( DWORD Code, LPVOID Arg ) const;
private:
	NxPluginParam * SliceIndex ;
};

NxGraphics_Plugin_Effect_Freeframe::NxGraphics_Plugin_Effect_Freeframe()  
{
	m_hInst = NULL;
	m_pff = NULL;
	m_Instance = NULL;
	
	mPluginType = TYPE_VIDEO ;
	mPluginSubType = TYPE_VIDEO_FREEFRAME ;
}

DWORD NxGraphics_Plugin_Effect_Freeframe::ProcessFrame( LPVOID Buffer )
{
	if( IsCreated() )
	{
		if(!IsMuted())
		{
			ProcessFrameInter( Buffer ) ;
		}
	}

	return 0;
}

bool NxGraphics_Plugin_Effect_Freeframe::Free()
{
	if (m_pff != NULL) 
	{
		(int)m_pff(FF_DEINITIALISE, 0, 0);
		m_pff = NULL;
		LogMsg(" freeframe plugin FF_DEINITIALISE" );
	}
	if (m_hInst != NULL)
	{
		FreeLibrary(m_hInst);
		LogMsg(" freeframe plugin FreeLibrary" );
		m_hInst = NULL;
		return(TRUE);
	}
	return(FALSE);
}

bool NxGraphics_Plugin_Effect_Freeframe::DeInitialize()
{
	std::string Name;
	GetPluginName( Name) ;
	LogMsg("DeInitialize freeframe plugin " + Name);
	Destroy();
	Free() ;
	LogMsg("NxGraphics: DeInitialize Freeframe success." );
	return true ;
}

bool NxGraphics_Plugin_Effect_Freeframe::Mute( bool mute )
{
	Muted = mute ;
	LogMsg("Freeframe Plugin Muted : " + Ogre::StringConverter::toString(Muted)   );
	return true ;
}

bool NxGraphics_Plugin_Effect_Freeframe::Load( const std::string & Path )
{
	Free();
	m_hInst = LoadLibrary(Path.c_str());
	if (m_hInst != NULL) {
		m_pff = (NxFF_Main_FuncPtr)GetProcAddress(m_hInst, "plugMain");
		if (m_pff != NULL)
		{
			DWORD res = ((DWORD)m_pff(FF_INITIALISE, NULL , 0)) ;
			if(res == FF_SUCCESS)
			{
				LogMsg("Freeframe plugin, Load Initialize Success");
				return true ;
			}
			else
			{
				LogMsg("Freeframe plugin, Load Initialize Failed");
				return false ;
			}
		}
		else
		{
			LogMsg("Freeframe plugin, could not find GetProcAddress named plugMain, is it a freeframe plugin ?");
		
		}
	}
	return(FALSE);
}

const PluginInfoStruct * NxGraphics_Plugin_Effect_Freeframe::GetInfo() const
{
	return(( PluginInfoStruct * )m_pff(FF_GETINFO, 0, 0));
}

bool NxGraphics_Plugin_Effect_Freeframe::GetInfo( PluginInfoStruct& PlugInfo ) const
{
	const PluginInfoStruct *pis = ( const PluginInfoStruct *  ) GetInfo();
	if (pis == NULL)
		return(FALSE);
	PlugInfo = *GetInfo();
	return(TRUE);
}

void NxGraphics_Plugin_Effect_Freeframe::CopyFFString( std::string& dst, const BYTE *src, DWORD len)
{
	std::string encode( src, src + len ) ; 
	dst = encode ;
}

bool NxGraphics_Plugin_Effect_Freeframe::GetPluginName( std::string& Name) const
{
	const PluginInfoStruct *pis = ( const PluginInfoStruct *  ) GetInfo();
	if (pis == NULL)
		return(FALSE);
	CopyFFString(Name, GetInfo()->PluginName, 16);
	return(TRUE);
}

bool NxGraphics_Plugin_Effect_Freeframe::GetPluginId(std::string& Id) const
{
	const PluginInfoStruct *pis = ( const PluginInfoStruct *  ) GetInfo();
	if (pis == NULL)
		return(FALSE);
	CopyFFString(Id, GetInfo()->PluginUniqueID, 4);
	return(TRUE);
}

int NxGraphics_Plugin_Effect_Freeframe::GetNumParams() const
{
	return((int)m_pff(FF_GETNUMPARAMETERS, 0, 0));
}

bool NxGraphics_Plugin_Effect_Freeframe::GetParamName(int ParamIdx, std::string& Name) const
{
	const BYTE *p = (const BYTE *)m_pff(FF_GETPARAMETERNAME, LPVOID(ParamIdx), 0);
	if (p == NULL)
		return(FALSE);
	CopyFFString(Name, p, 16);
	return(TRUE);
}

float NxGraphics_Plugin_Effect_Freeframe::GetParamDefault(int ParamIdx) const
{
	plugMainUnionTag pmu;
	pmu.ivalue = (int)m_pff(FF_GETPARAMETERDEFAULT, LPVOID(ParamIdx), 0);  
	return(pmu.fvalue);
}

int NxGraphics_Plugin_Effect_Freeframe::GetParamType(int ParamIdx) const
{
	return((int)m_pff(FF_GETPARAMETERTYPE, LPVOID(ParamIdx), 0));
}

void NxGraphics_Plugin_Effect_Freeframe::GetParamDisplay(int ParamIdx , std::string& DisplayName ) const
{
		const BYTE * pDisplay = ( const BYTE *  )(m_pff)(FF_GETPARAMETERDISPLAY, (LPVOID)ParamIdx, 0); 
		CopyFFString(DisplayName , pDisplay , 16);
}

int NxGraphics_Plugin_Effect_Freeframe::GetPluginCaps(int CapsIdx) const
{
	return((int)m_pff(FF_GETPLUGINCAPS, LPVOID(CapsIdx), 0));
}

const PluginExtendedInfoStruct * NxGraphics_Plugin_Effect_Freeframe::GetExtendedInfo() const
{
	return((PluginExtendedInfoStruct *)m_pff(FF_GETEXTENDEDINFO, 0, 0));
}

bool NxGraphics_Plugin_Effect_Freeframe::GetExtendedInfo( PluginExtendedInfoStruct& PlugExtInfo) const
{
	const PluginExtendedInfoStruct * peis = ( const PluginExtendedInfoStruct * ) GetExtendedInfo();
	if (peis == NULL)
	{ 
		return(FALSE);
	}
	PlugExtInfo = *peis;
	return(TRUE);
}

bool NxGraphics_Plugin_Effect_Freeframe::GetPluginPath(std::string& Path) const
{
	return(TRUE);
}

bool NxGraphics_Plugin_Effect_Freeframe::Instantiate(bool ResetParms)
{
	/*
	LayerWidth  = MixerManager::getSingleton().layer_mixer[GetAssociatedLayerNumber()].Width ;  
	LayerHeight  = MixerManager::getSingleton().layer_mixer[GetAssociatedLayerNumber()].Height; 
	LayerBpp = MixerManager::getSingleton().layer_mixer[GetAssociatedLayerNumber()].Bpp ;

	int	BitDepth;
	switch ( LayerBpp * 8 ) 
	{
		case 16:
			BitDepth = FF_CAP_16BITVIDEO;
			Log(" freeframe plugin,requesting FF_CAP_16BITVIDEO  ");
			break;
		case 24:
			BitDepth = FF_CAP_24BITVIDEO;
			Log(" freeframe plugin,requesting FF_CAP_24BITVIDEO  ");
			break;
		case 32:
			BitDepth = FF_CAP_32BITVIDEO;
			Log(" freeframe plugin,requesting FF_CAP_32BITVIDEO  ");
			break;
		default:
			Log(" Could not load freeframe plugin, IDS_FF_BAD_BIT_DEPTH !");
			return(FALSE);
	}

	int plugincaps = GetPluginCaps(BitDepth) ;
	if ( plugincaps == FF_FAIL ) 
	{
		Ogre::String msg, name;
		GetPluginName(name);
		Log(" Could not load freeframe plugin, GetPluginCaps(BitDepth) Failed ! for " + name );
		return(FALSE);
	}
	else
	{
		if( FF_TRUE == plugincaps )
		{
			Log("Accepted Bitdepth format" );
		}
		else
		{
			Log("Refused Bitdepth format, consider using another bit depth format to use this plugin" );
			return false;
		}
 	
	}

	 plugincaps = GetPluginCaps( FF_CAP_PROCESSFRAMECOPY ) ;
	 if( plugincaps == FF_TRUE )
	 {
		Log("Accepted FF_CAP_PROCESSFRAMECOPY format" );
	 }
	 else
	 {
		Log("Refused FF_CAP_PROCESSFRAMECOPY format" );
	 }

	  plugincaps = GetPluginCaps( FF_CAP_COPYORINPLACE ) ;
	  switch(plugincaps)
	  {
		  case FF_CAP_PREFER_NONE:
			  Log("Preference FF_CAP_PREFER_NONE format" );
		  break;
		  case FF_CAP_PREFER_INPLACE:
			   Log("Preference FF_CAP_PREFER_INPLACE format" );
		  break;
		  case FF_CAP_PREFER_COPY:
			  Log("Preference FF_CAP_PREFER_COPY format" );
		  break;
		  case FF_CAP_PREFER_BOTH:
			  Log("Preference FF_CAP_PREFER_BOTH INPLACE and COPY format" );
		  break;
	  default:
		  break;
	  }

	VideoInfoStruct * vis = new VideoInfoStruct();
	vis->FrameWidth = LayerWidth ;
	vis->FrameHeight = LayerHeight ;
	vis->BitDepth= BitDepth;
	vis->Orientation = FF_ORIENTATION_BL ;
	bool retc ;
	int	Parms = GetNumParams();

	Log("Freeframe GetNumParams : "  + Ogre::StringConverter::toString( Parms )  );
	if (ResetParms)
	{
		 m_Instance= m_pff( FF_INSTANTIATE, (void*)vis, 0);

	} else 
	{
		for ( int i = 0; i < Parms; i++ )
		{
			Ogre::String Name ;
			GetParamName( i, Name) ;
			int ParamType =  GetParamType( i ) ; // FF_ParameterType
			switch(ParamType)
			{
				case FF_TYPE_BOOLEAN : 
					Log( "Freeframe plugin parameter type FF_BOOL : " + Name );
					break ;
				case FF_TYPE_EVENT : 
					Log("Freeframe plugin parameter type FF_EVENT : " + Name );
					break ;
				case FF_TYPE_RED : 
					Log("Freeframe plugin parameter type FF_COL_RED : " + Name );
					break ;
				case FF_TYPE_GREEN : 
					Log("Freeframe plugin parameter type FF_COL_GREEN : " + Name );
					break ;
				case FF_TYPE_BLUE : 
					Log("Freeframe plugin parameter type FF_COL_BLUE : " + Name );
					break ;
				case FF_TYPE_XPOS : 
					Log("Freeframe plugin parameter type FF_POS_X : " + Name );
					break ;
				case FF_TYPE_YPOS : 
					Log("Freeframe plugin parameter type FF_POS_Y : " + Name );
					break ;
				case FF_TYPE_STANDARD : 
					Log("Freeframe plugin parameter type FF_STANDARD : " + Name );
					break ;
				case FF_TYPE_TEXT : 
					Log("Freeframe plugin parameter type FF_TEXT : " + Name );
					break ;
				default :
					Log("Freeframe plugin parameter type Not Defined : " + Name );
					break ;
			}

			float ParamDefaultValue = GetParamDefault( i ) ;
			//Log(" freeframe plugin, Creating parameter " +  Name + "With default value : " + Ogre::StringConverter::toString( ParamDefaultValue ) );

			//NxPluginParam * FreeParam = new NxPluginParam( Name.c_str(), PluginParameterTypes::PluginParameterTypeSlider );
			//FreeParam->SetRange( 0, 1.0, 0.001 );
			//FreeParam->SetDefaultValue( ParamDefaultValue );

			//FreeParam->SetHost( this );
			//FreeParam->SetParameterIndex( i );
			//FreeParam->SetDisplayOnButton( true );
			 
			NxPluginParamSlider * FreeParam = new NxPluginParamSlider( Name.c_str() );
			FreeParam->SetRange( 0, 1.0, 0.001 );
			FreeParam->SetDefaultValue( ParamDefaultValue );
			FreeParam->SetHost( this );
			FreeParam->SetParamIndex( i );
			//FreeParam->SetDisplayOnButton( true );
			ParamList.push_back(  ( NxPluginParam * ) FreeParam );
		}
		 
		m_Instance= m_pff(FF_INSTANTIATE, (void*)vis, 0);
		Ogre::LogManager::getSingleton().logMessage(" freeframe plugin, Created !");

		int ParamIndex = 0;

		for( int i = 0; i < ParamList.size(); i++ )
		{
			NxPluginParam * Parameter = ParamList[i];
			Parameter->SetParamDescription( std::string( GetParamDisplay( ParamIndex ) ) );
			ParamIndex++;
		}	 
	}
	 
	Log(" freeframe plugin, DONE !");
	*/

	return true ; 
}

void NxGraphics_Plugin_Effect_Freeframe::ProcessSlider( NxPluginParam * Parameter, double Value )
{
	int Index = 0 ;
	//for( std::list<NxPluginParam  *>::iterator iter = ParamList.begin(); iter != ParamList.end(); iter++)
	for( int i = 0; i < ParamList.size(); i++ )
	{
		NxPluginParam  * ptr = ParamList[i];
		if( ptr == Parameter )
		{
			SetParam( Index, Value );
			Parameter->SetValue( Value );

		}
		Index++;
	}
}

void NxGraphics_Plugin_Effect_Freeframe::GetLoadedPluginInfo()
{
	PluginInfoStruct pis;
	ZeroMemory(&pis, sizeof( pis ) );
	GetInfo( ( PluginInfoStruct &)  pis );
	PluginExtendedInfoStruct peis;
	ZeroMemory(&peis, sizeof( peis ) );
	GetExtendedInfo( ( PluginExtendedInfoStruct &  ) peis );
	std::string PluginName, PluginId, Loger;
	GetPluginName( PluginName );
	GetPluginId( PluginId );
	Loger += "Plugname : " + PluginName + " ";
	Loger += "PluginId : " + PluginId + " ";
	Loger += "APIVersion  : " + Ogre::StringConverter::toString((Ogre::Real) ( pis.APIMinorVersion / 1000.0 + pis.APIMajorVersion ) )+ " " ;
	Loger += "PluginVersion  : " + Ogre::StringConverter::toString( (Ogre::Real) ( peis.PluginMinorVersion / 1000.0 + peis.PluginMajorVersion  ))+ " " ;
	//Loger += "PluginType  : " + pis.pluginType ? Ogre::String("Source") : Ogre::String("Effect");
	LogMsg( Loger );
}
 
#define CLAMP(x, lo, hi) (min(max((x), (lo)), (hi)))

bool NxGraphics_Plugin_Effect_Freeframe::Initialize() 
{
	std::string Path = NxPlugin::GetPluginPath();
	if( Load( Path) )
	{
		int MAX_INPUTS = 2;
		BOOL m_UseFrameCopy ;
		int m_NumInputs = GetPluginCaps(FF_CAP_MAXIMUMINPUTFRAMES);
		m_NumInputs = CLAMP(m_NumInputs, 1, MAX_INPUTS);	// enforce our range
		if (m_NumInputs > 1)
		{
			m_UseFrameCopy = TRUE;
		}
		else 
		{
			int	CopyPref = GetPluginCaps(FF_CAP_COPYORINPLACE);
			m_UseFrameCopy =  CopyPref == FF_CAP_PREFER_COPY;
		}

		GetLoadedPluginInfo();
		if(!Instantiate( false )){ return false ;}
	}

	return true ;
}

bool NxGraphics_Plugin_Effect_Freeframe::Deinitialise()
{
	return true ;
}

inline bool NxGraphics_Plugin_Effect_Freeframe::IsCreated() const
{
	return(m_Instance != NULL);
}

inline LPVOID NxGraphics_Plugin_Effect_Freeframe::CallFF( DWORD Code, LPVOID Arg ) const
{
	assert(m_Instance != NULL);
	return(m_pff(Code, Arg, UINT(m_Instance)));
}

inline LPCSTR NxGraphics_Plugin_Effect_Freeframe::GetParamDisplay( int ParamIdx ) const
{
	return((LPCSTR)CallFF(FF_GETPARAMETERDISPLAY, LPVOID(ParamIdx)));
}

inline float NxGraphics_Plugin_Effect_Freeframe::GetParam(int ParamIdx) const
{
	plugMainUnionTag	pmu;
	pmu.ivalue = (int)CallFF(FF_GETPARAMETER, LPVOID(ParamIdx));
	return(pmu.fvalue);
}

inline bool NxGraphics_Plugin_Effect_Freeframe::SetParam( const SetParameterStruct& sps )
{
	return((int)CallFF(FF_SETPARAMETER, LPVOID(&sps)) != FF_FAIL);
}

inline bool NxGraphics_Plugin_Effect_Freeframe::SetParam( int ParamIdx, float Value )
{
	SetParameterStruct	sps;
	sps.ParameterNumber = ParamIdx;
	sps.NewParameterValue = Value;
	return(SetParam( (  SetParameterStruct & ) sps));
}

inline bool NxGraphics_Plugin_Effect_Freeframe::ProcessFrameInter( LPVOID Frame )
{
	return((int)CallFF(FF_PROCESSFRAME, Frame));
}

inline bool NxGraphics_Plugin_Effect_Freeframe::ProcessFrameCopy( const ProcessFrameCopyStruct& pfcs)
{
	return((int)CallFF(FF_PROCESSFRAMECOPY, LPVOID(&pfcs)) != FF_FAIL);
}

bool NxGraphics_Plugin_Effect_Freeframe::Create( NxGraphics_Plugin_Effect_Freeframe * Plugin,  VideoInfoStruct * VideoInfo)
{
	if (Plugin == NULL)
	{
		LogMsg(" freeframe plugin, this pointer is false !");
		return(FALSE);
	}

	LogMsg(" freeframe plugin, preparing resource release !");
	Destroy();
	LogMsg(" freeframe plugin, creating instance !");

	m_Instance= m_pff(FF_INSTANTIATE, (void*)VideoInfo, 0);

	LogMsg(" freeframe plugin,   instance created!");
	if (m_Instance == LPVOID(FF_FAIL))
	{
		m_Instance = NULL;
	}

	return(m_Instance != NULL);
}

bool NxGraphics_Plugin_Effect_Freeframe::Destroy()
{
	bool	retc;
	if (m_Instance != NULL) 
	{
		retc = ((int)CallFF(FF_DEINSTANTIATE, 0) != FF_FAIL);
		m_Instance = NULL;
		LogMsg(" freeframe plugin FF_DEINSTANTIATE" );
	} else
	{
		retc = FALSE;
	}
	return(retc);
}

//================================================

NxGraphics_Plugin_Effect_Freeframe * Inst;

NxPluginVideoFreeframe::NxPluginVideoFreeframe() : NxPluginVideo()
{
	Inst = new NxGraphics_Plugin_Effect_Freeframe();
}

NxPluginVideoFreeframe::~NxPluginVideoFreeframe()
{

}

bool NxPluginVideoFreeframe::Initialize()
{
	return true;
}

bool NxPluginVideoFreeframe::DeInitialize()
{
	return true;
}

void NxPluginVideoFreeframe::SetVideoWidth( unsigned int )
{

}

void NxPluginVideoFreeframe::SetVideoHeight( unsigned int )
{

}

void NxPluginVideoFreeframe::SetVideoBpp( unsigned int )
{

}

bool NxPluginVideoFreeframe::Load( const std::string & Filename )
{
	LogMsg("NxPluginVideoFreeframe : Loading freeframe Plugin ");
	if(Inst->Load( Filename ))
	{
		PluginInfoStruct Info;
		ZeroMemory(&Info, sizeof( Info ) );
		Inst->GetInfo( Info );

		std::string encode( Info.PluginName, Info.PluginName + 16 ) ; 
		PluginName = encode ;
		PluginPath = Filename;

		PluginExtendedInfoStruct ExtendedInfo;
		ZeroMemory(&ExtendedInfo, sizeof( ExtendedInfo ) );
		Inst->GetExtendedInfo(ExtendedInfo);
		return true;
 
	}

	return false;
}

unsigned long NxPluginVideoFreeframe::ProcessFrame( void * Buffer )
{
	return 0;
}

}//namespace