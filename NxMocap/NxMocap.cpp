#include "NxMocapPreCompiled.h"

namespace NxMocap_Namespace {

template<> NxMocapManager* Singleton<NxMocapManager>::ms_Singleton = 0;

NxMocapManager::NxMocapManager()
{
	new NxMocapLog();

	Log("Started NxMocap.");
	 
}

NxMocapManager::~NxMocapManager()
{
	
}

NxMocapManager& NxMocapManager::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

NxMocapManager* NxMocapManager::getSingletonPtr(void)
{
	return ms_Singleton;
}

NxMocapDeviceKinect * NxMocapManager::CreateDeviceKinect( const NxMocapDeviceDesc & Dev )
{
	switch (Dev.Type) {
#ifdef USE_OPENNI2 
	case  NxMocapKinectOpenNi2 :
		return new NxMocapDeviceKinectOpenNi();
		break;
#endif
#ifdef USE_MICROSOFTKINECT  
	case  NxMocapKinectMicrosoft :
		return new NxMocapDeviceKinectMicrosoft();
		break;
#endif
	default:
		break;
	}

}


NxMocapPlayerPolhemus * NxMocapManager::CreatePlayerPolhemus()
{
	NxMocapPlayerPolhemus * player = new NxMocapPlayerPolhemus(); 
	player->SetNxMocapType(  NxMocapPlayerType );
	PlayerList.push_back( player );
	FullNxMocapList.push_back( player );//full list
	return player ;
}

void NxMocapManager::DeletePlayer( NxMocapPlayer * player )
{
	//player->RemoveListener( this );
	PlayerList.remove( player );
	delete player ;
}

NxMocap_Recorder * NxMocapManager::CreateRecorder()
{
	Log( "Creating Recorder ..." );
	NxMocap_Recorder * recorder = new NxMocap_Recorder(); 
//	recorder->SetNxMocapType( NxMocapTypes::NxMocapReceiverType );
	RecorderList.push_back( recorder );
//	FullNxMocapList.push_back( recorder );//full list
	Log( "Creating Recorder Done." );
	return recorder ;

}

void NxMocapManager::DeleteRecorder( NxMocap_Recorder * recorder )
{
	RecorderList.remove( recorder );
	delete recorder ;
}

size_t NxMocapManager::GetNumRecorders()
{
	return RecorderList.size() ;
}

size_t NxMocapManager::GetNumPlayers()
{
	return PlayerList.size() ;
}
 
 

void NxMocapManager::GetStreamList( std::list<string>& StreamList )
{
	//for( int i = 0 ; i < GetNumPlayers() ; i++ )
	//{
	//	NxMocapPlayer * Player = GetPlayer( i );
	//	std::ostringstream os;
	//	os << "Player Stream "<< Player->GetMocapStreamIndex()  ;  
	//	std::string buffer(os.str());
	//	StreamList.push_back( buffer );
	//}

	//for( int i = 0 ; i < GetNumRecorders() ; i++ )
	//{
	//	NxMocap_Recorder * Recorder = GetRecorder( i );
	//	std::ostringstream os;
	//	os << "Recorder Stream "<< Recorder->GetMocapStreamIndex()  ;  
	//	std::string buffer(os.str());
	//	StreamList.push_back( buffer );
	//}
	return;
}

NxMocapPlayer * NxMocapManager::GetPlayer( int index )
{
	list<NxMocapPlayer *>::iterator i = PlayerList.begin();
	std::advance(i, index );
	// now index points to the nth member of the list (counting from zero)
	return *i ;
}

NxMocap_Recorder * NxMocapManager::GetRecorder( int index )
{
	list<NxMocap_Recorder *>::iterator i = RecorderList.begin();
	std::advance(i, index );
	// now index points to the nth member of the list (counting from zero)
	return *i ;
}

size_t NxMocapManager::GetNumTotalStreams()
{
	return GetNumPlayers() + GetNumRecorders() ;
}

std::list<NxMocapPlayer *>& NxMocapManager::GetList()
{
	return FullNxMocapList;
}

void NxMocapManager::AddListener( NxMocapManagerListener * listener )
{
	 Listeners_List.push_back(listener); 
}

void NxMocapManager::RemoveListener( NxMocapManagerListener * listener )
{
	 Listeners_List.remove(listener); 
}
 


} // namespace
