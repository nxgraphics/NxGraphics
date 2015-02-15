#ifndef __NXVIDEOLOG_H__
#define __NXVIDEOLOG_H__

#include "NxVideo_Prerequisites.h"

namespace NxVideo_Namespace {

class NxVideo_Export NxVideoLog : public Singleton<NxVideoLog>  
{
public:
	//! NxVideoLog constructor.
	NxVideoLog();
	//! NxVideoLog destructor.
	~NxVideoLog();
	//! Log message.
	void LogMessage(std::string);
	// NxVideoLog Singleton.
	static NxVideoLog& getSingleton();
private:
	std::string text;
	std::ofstream logger;
};

}//namespace

#endif
