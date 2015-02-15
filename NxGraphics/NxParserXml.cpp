#include "NxGraphicsPreCompiled.h"
#include "../nxdeps/include/OgreMax/tinyxml/tinyxml.h"
#include <iomanip>
#include <fstream>

namespace Nx {

NxGraphicsXml::NxGraphicsXml(): currentElement(0)
{
	 doc = new TiXmlDocument();
}

NxGraphicsXml::~NxGraphicsXml()
{

}

bool NxGraphicsXml::OpenFile( const std::string & pFilename )
{
	std::string fullPath(pFilename);
	std::replace(fullPath.begin(),fullPath.end(),'\\','/');

	bool result = doc->LoadFile(fullPath);  
	if( !result )   {  
		std::ostringstream str;  
		str << "Unable to parse XML file '" << fullPath << ":" << std::endl;  
		str << doc->ErrorDesc() << " in line " << doc->ErrorRow() << ", column " << doc->ErrorCol() << std::endl;  
		LogMsg(  str.str() );  
	}
	mFilename = pFilename;
	return result ;
}

void NxGraphicsXml::CreateFile( const std::string & pFilename )
{
	mFilename = pFilename;
	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
	doc->LinkEndChild( decl ); 
}

void NxGraphicsXml::Parse( const char * data )
{
	doc->Parse( data );
}

bool NxGraphicsXml::Save()
{
	return doc->SaveFile( mFilename );
}

void NxGraphicsXml::GetRootElementName( std::string & RootName )
{
	RootName = doc->RootElement()->ValueStr();
}

void NxGraphicsXml::CreateElement( const std::string& name )  
{  
	TiXmlElement* child = new TiXmlElement( name );  
	if( currentElement == 0 )   {  
		doc->LinkEndChild( child );  
	}  
	else {  
		currentElement->LinkEndChild( child );  
	}
	currentElement = child;  
} 

void NxGraphicsXml::StartElement( const std::string& name )
{
	if( currentElement == 0 ) {
		currentElement = doc->FirstChild( name )->ToElement();  
	}  
	else { 
		 currentElement = currentElement->FirstChild( name )->ToElement(); 
	}  
	 
	assert( currentElement != 0 ); 
}

void NxGraphicsXml::StartElement( int index, std::string & name )
{
	TiXmlNode* child = currentElement->FirstChild();  
	int count = 0;  
	while( child != 0 )  {  
		if( index == count ) {
			currentElement  = child->ToElement();
			name = child->Value();
		}

		if( child->Type() != TiXmlNode::COMMENT ) {  
			count++;  
		}  
		child = child->NextSibling();  
	}
}

void NxGraphicsXml::EndElement()
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	if( currentElement && currentElement->Parent() != 0 ) {  
		currentElement = currentElement->Parent()->ToElement();  // becomes 0 if the document is reached  
	}
} 

unsigned int NxGraphicsXml::GetNumChildNodes()
{
	assert( currentElement != 0 );  
	TiXmlNode* child = currentElement->FirstChild();  
	int count = 0;  
	while( child != 0 )   {  
		if( child->Type() != TiXmlNode::COMMENT ) {  
			count++;  
		}  
		child = child->NextSibling();  
	}  
	return count;  
}


const std::string NxGraphicsXml::getChildValue( const std::string & NodeName )
{
	std::string Result; 
	TiXmlElement * ChildElement = currentElement->FirstChildElement( NodeName );
	if( ChildElement  ) {
		TiXmlNode* child = ChildElement->FirstChild();  
		if( child ) {
			Result = child->ValueStr();
		}
	}
	return Result; 
}

const std::string & NxGraphicsXml::GetAttribute( const std::string& name ) const 
{  
	const std::string* str = currentElement->Attribute( name );  
	if( 0 == str ) {  
		std::ostringstream str;  
		str << "Missing Attribute '" << name << "' for element '" << currentElement->Value() << "' ";  
		str << "in line " << currentElement->Row() << ", column " << currentElement->Column() << " ";   
		LogMsg( str.str() );
	}  
  
	return *str;  
}

bool NxGraphicsXml::HasAttribute( const std::string & AttributeName )
{  
	return currentElement->Attribute( AttributeName ) != 0 ;//return true else return false;
}

void NxGraphicsXml::GetAttribute( const std::string& name, std::string& value )
{
	assert( currentElement != 0 ); 
	value = GetAttribute( name );
}

void NxGraphicsXml::GetAttribute( const std::string& name, bool& value )
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name );
	NxUtils::ToLower( ValueStr );
	if(ValueStr == "true" ){ value = true; return; }
	else if(ValueStr == "false" ){ value = false; return; }
	else if(ValueStr == "1" ){ value = true; return; }
	else if(ValueStr == "0" ){ value = false; return; }
	value = atoi( ValueStr.c_str() );	
}  

void NxGraphicsXml::GetAttribute( const std::string& name, short& value)
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name );  
	value = atoi( ValueStr.c_str() );	
}

void NxGraphicsXml::GetAttribute( const std::string& name, int& value)
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name );  
	value = atoi( ValueStr.c_str() );
}

void NxGraphicsXml::GetAttribute( const std::string& name, long& value)
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name ) ;  
	value = atol( ValueStr.c_str() );
} 

void NxGraphicsXml::GetAttribute( const std::string& name, unsigned long& value  )
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name );  
	value = atol( ValueStr.c_str() );
}  

void NxGraphicsXml::GetAttribute( const std::string& name, float& value)
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name );  
	value = atof( ValueStr.c_str() );
} 

void NxGraphicsXml::GetAttribute( const std::string& name, double& value)
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name );  
	value = atof( ValueStr.c_str() );
}  

void NxGraphicsXml::GetAttribute( const std::string& name, Nx::Vector4& value)
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name );  
	const std::vector<std::string> ValuesSplit = NxUtils::SplitString( ValueStr , " " );
	value.w = atof( ValuesSplit[0].c_str() );
	value.x = atof( ValuesSplit[1].c_str() ); 
	value.y = atof( ValuesSplit[2].c_str() ); 
	value.z = atof( ValuesSplit[3].c_str() ); 
}

void NxGraphicsXml::GetAttribute( const std::string& name, Nx::Vector3& value,const std::string & Split )
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name ) ;  
	const std::vector<std::string> ValuesSplit = NxUtils::SplitString( ValueStr, Split );
	value.x = atof( ValuesSplit[0].c_str() );
	value.y = atof( ValuesSplit[1].c_str() ); 
	value.z = atof( ValuesSplit[2].c_str() );
}

void NxGraphicsXml::GetAttribute( const std::string& name, Nx::Vector2& value)
{
	assert( currentElement != 0 );  
	std::string ValueStr = GetAttribute( name ) ;  
	const std::vector<std::string> ValuesSplit = NxUtils::SplitString( ValueStr , " " );
	value.x = atof( ValuesSplit[0].c_str() );
	value.y = atof( ValuesSplit[1].c_str() ); 
}


void NxGraphicsXml::SetAttribute( const std::string& name, const std::string& value )  
{  
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	currentElement->SetAttribute( name, value.c_str() );  
	//Log("writeParameter( const std::string& name, const std::string& value )  ");
} 

void NxGraphicsXml::SetAttribute( const std::string& name, const bool& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	currentElement->SetAttribute( name, value );  
	//Log("writeParameter( const std::string& name, const bool& value )");
}

void NxGraphicsXml::SetAttribute( const std::string& name, const short& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	currentElement->SetAttribute( name, value );  
	//Log("writeParameter( const std::string& name, const short& value )");
}

void NxGraphicsXml::SetAttribute( const std::string& name, const int& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	currentElement->SetAttribute( name, value ); 
	//Log("writeParameter( const std::string& name, const int& value )");
}

void NxGraphicsXml::SetAttribute( const std::string& name, const long& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	currentElement->SetAttribute( name, value );  
	//Log("writeParameter( const std::string& name, const long& value )");
}  

void NxGraphicsXml::SetAttribute( const std::string& name, const unsigned long& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	std::ostringstream str;
	str << value;
	std::string Res( str.str() );  
	currentElement->SetAttribute( name, Res );  
	//Log("writeParameter( const std::string& name, const unsigned long& value )");
}  

void NxGraphicsXml::SetAttribute( const std::string& name, const float& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	std::ostringstream str;
	str << std::fixed << std::setprecision(4) << value ;
	std::string Res( str.str() );
	currentElement->SetAttribute( name.c_str() , Res ); 
	//Log("writeParameter( const std::string& name, const float& value )");
}

void NxGraphicsXml::SetAttribute( const std::string& name, const Nx::Vector4& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	std::ostringstream str;
	str << std::fixed << std::setprecision(4) << value.w ;
	str << " " ;
	str << std::fixed << std::setprecision(4) << value.x ;
	str << " " ;
	str << std::fixed << std::setprecision(4) << value.y ;
	str << " " ;
	str << std::fixed << std::setprecision(4) << value.z ;
	std::string Res( str.str() );
	currentElement->SetAttribute( name.c_str() , Res ); 
	//Log("writeParameter( const std::string& name, const Ogre::Vector4& value )");
}

void NxGraphicsXml::SetAttribute( const std::string& name, const Nx::Vector3& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	std::ostringstream str;
	str << std::fixed << std::setprecision(4) << value.x ;
	str << " " ;
	str << std::fixed << std::setprecision(4) << value.y ;
	str << " " ;
	str << std::fixed << std::setprecision(4) << value.z ;
	std::string Res( str.str() );
	currentElement->SetAttribute( name.c_str() , Res ); 
	//Log("writeParameter( const std::string& name, const Ogre::Vector3& value )");
}

void NxGraphicsXml::SetAttribute( const std::string& name, const Nx::Vector2& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	std::ostringstream str;
	str << std::fixed << std::setprecision(4) << value.x ;
	str << " " ;
	str << std::fixed << std::setprecision(4) << value.y ;
	std::string Res( str.str() );
	currentElement->SetAttribute( name.c_str() , Res ); 
	//Log("writeParameter( const std::string& name, const Ogre::Vector2& value )");
}

void NxGraphicsXml::SetAttribute( const std::string& name, const double& value )
{
	assert( currentElement != 0 ); // Make sure that beginAttribute has been called!  
	currentElement->SetDoubleAttribute( name.c_str() , value );  
}
 
void NxGraphicsXml::GetElementValue( std::string& value )
{
	assert( currentElement != 0 );  
	value = currentElement->GetText();	
}

//configuration reader
//http://www.adp-gmbh.ch/cpp/config_file.html
NxGraphicsConfiguration::NxGraphicsConfiguration( std::string const& value ) 
{
	value_= value;
}

NxGraphicsConfiguration::NxGraphicsConfiguration( const char* c ) 
{
	value_=c;
}

NxGraphicsConfiguration::NxGraphicsConfiguration( double d ) 
{
	std::stringstream s;
	s<<d;
	value_=s.str();
}

NxGraphicsConfiguration::NxGraphicsConfiguration( NxGraphicsConfiguration const& other )
{
	value_ = other.value_;
}

NxGraphicsConfiguration& NxGraphicsConfiguration::operator=( NxGraphicsConfiguration const& other )
{
	value_=other.value_;
	return *this;
}

NxGraphicsConfiguration& NxGraphicsConfiguration::operator=( double i )
{
	std::stringstream s;
	s << i;
	value_ = s.str();
	return *this;
}

NxGraphicsConfiguration& NxGraphicsConfiguration::operator=( std::string const& s ) 
{
	value_=s;
	return *this;
}

NxGraphicsConfiguration::operator std::string() const 
{
	return value_;
}

NxGraphicsConfiguration::operator double() const 
{
	return atof(value_.c_str());
}

std::string trim( std::string const& source, char const* delims = " \t\r\n" )
{
	std::string result(source);
	std::string::size_type index = result.find_last_not_of(delims);
	if(index != std::string::npos)
	result.erase(++index);

	index = result.find_first_not_of(delims);
	if(index != std::string::npos)
	result.erase(0, index);
	else
	result.erase();
	return result;
}

NxConfigFile::NxConfigFile( std::string const& configFile )
{
	std::ifstream file(configFile.c_str());
	std::string line;
	std::string name;
	std::string value;
	std::string inSection;
	int posEqual;
	while (std::getline(file,line)) 
	{
		if (! line.length()) continue;
		if (line[0] == '#') continue;
		if (line[0] == ';') continue;
		if (line[0] == '[') 
		{
		  inSection=trim(line.substr(1,line.find(']')-1));
		  continue;
		}

		posEqual=line.find('=');
		name  = trim(line.substr(0,posEqual));
		value = trim(line.substr(posEqual+1));
		content_[inSection+'/'+name]= NxGraphicsConfiguration(value);
	}
}

NxGraphicsConfiguration const& NxConfigFile::Value( std::string const& section, std::string const& entry ) const 
{
	std::map<std::string,NxGraphicsConfiguration>::const_iterator ci = content_.find(section + '/' + entry);
	if (ci == content_.end()) throw "does not exist";
	return ci->second;
}

NxGraphicsConfiguration const& NxConfigFile::Value( std::string const& section, std::string const& entry, double value ) 
{
	try 
	{
		return Value(section, entry);
	} 
	catch(const char *) 
	{
		return content_.insert(std::make_pair(section+'/'+entry, NxGraphicsConfiguration(value))).first->second;
	}
}

NxGraphicsConfiguration const& NxConfigFile::Value( std::string const& section, std::string const& entry, std::string const& value ) 
{
	try 
	{
		return Value(section, entry);
	} 
	catch(const char *) 
	{
		return content_.insert(std::make_pair(section+'/'+entry, NxGraphicsConfiguration(value))).first->second;
	}
}


}//namespace