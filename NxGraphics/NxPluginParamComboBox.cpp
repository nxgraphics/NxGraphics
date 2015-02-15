#include "NxGraphicsPreCompiled.h"

namespace Nx {

NxPluginParamList::NxPluginParamList( std::string PluginParamName ) : NxPluginParam( PluginParamName, PluginParameterTypeComboBox ), mDefaultComboBoxParam("")
{
	
}

NxPluginParamList::~NxPluginParamList()
{

}

std::vector<std::string> NxPluginParamList::GetList()
{
	return StringList ;
}

void NxPluginParamList::UpdateList( std::vector<std::string> List )
{
	StringList = List ;
}

void NxPluginParamList::SetComboBoxDefaultParameter( std::string str )
{
	mDefaultComboBoxParam = str ;
}

std::string NxPluginParamList::GetComboBoxDefaultParameter()
{
	return mDefaultComboBoxParam ;
}

void NxPluginParamList::InitializeParameter()
{
	switch( mParamType )
	{
		case PluginParameterTypeComboBox:
			this->GetHost()->ProcessComboBox( this , GetComboBoxDefaultParameter() );
		break;
	default :
		break ;
	}
}


}