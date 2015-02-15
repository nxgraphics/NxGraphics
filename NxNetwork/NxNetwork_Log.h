#ifndef __NXNETWORKLOG_H__
#define __NXNETWORKLOG_H__

#include "NxNetwork_Prerequisites.h"

namespace NxNetwork {

	class NxNetwork_Export NxNetworkLog : public Singleton<NxNetworkLog>  
	{
	public:
		//! NxNetworkLog constructor.
		NxNetworkLog();
		//! NxNetworkLog destructor.
		~NxNetworkLog();
		//! Log message.
		void LogMessage(std::string);
		// NxVideoLog Singleton.
		static NxNetworkLog& getSingleton();
	private:
		std::string text;
		std::ofstream logger;
	};

}//namespace

#endif
