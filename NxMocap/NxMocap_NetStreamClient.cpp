#include "NxMocapPreCompiled.h"
 

namespace NxMocap_Namespace {

NxMocapNetStreamClient::NxMocapNetStreamClient() : mSocketClient(0)
{

}

NxMocapNetStreamClient::~NxMocapNetStreamClient()
{

}

void NxMocapNetStreamClient::CreateStream( const std::string & IP, int port ) {
 
}
 
void NxMocapNetStreamClient::DeleteStream() {
 
}

int NxMocapNetStreamClient::SendData( unsigned char * Data, int Size ) {
	return 0;
	 
}
 
const std::string & NxMocapNetStreamClient::GetIp() const {
	 return "";
}

int NxMocapNetStreamClient::GetPort() {
	return 0  ;
}


}