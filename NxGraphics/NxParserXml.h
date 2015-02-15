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
#ifndef __NXPARSERXML_H__
#define __NXPARSERXML_H__

#include "NxPrerequisites.h"

class TiXmlDocument;
class TiXmlDocument; 
class TiXmlElement;

namespace Nx {
 
//! NxGraphicsXml Class.
class NxGraphics_Export NxGraphicsXml 
{
public :
	//! NxGraphicsDeSerializer Class Constructor.
	NxGraphicsXml();
	//! NxGraphicsDeSerializer Class Deconstructor.
	~NxGraphicsXml();
	//! Open Xml File;
	bool OpenFile( const std::string & pFilename );
	//! create an XML file.
	void CreateFile( const std::string & pFilename );
	//! Parse Xml string Nodes.
	void Parse( const char * data );
	//! save file.
	bool Save();
	//! Get Root Element Name.
	void GetRootElementName( std::string & RootName );
	//! create element.
	void CreateElement( const std::string& name );
	//! Start Parsing an Attribute.
	void StartElement( const std::string& name );
	//! Start Parsing an Attribute with given index.
	void StartElement( int index, std::string & name );
	//! Read node value.
	void GetElementValue( std::string& value );
	//! End Parsing an Attribute.
	void EndElement();
	//! Get number elements.
	unsigned int GetNumChildNodes();
	//! Current element has attribute ?
	bool HasAttribute( const std::string & AttributeName );
	//! get attribute.
	void GetAttribute( const std::string& name, std::string& value );
	void GetAttribute( const std::string& name, bool& value );  
	void GetAttribute( const std::string& name, short& value  );  	
	void GetAttribute( const std::string& name, int& value );  	
	void GetAttribute( const std::string& name, long& value );	
	void GetAttribute( const std::string& name, unsigned long& value );  	
	void GetAttribute( const std::string& name, float& value );  	
	void GetAttribute( const std::string& name, double& value  );  
	void GetAttribute( const std::string& name, Nx::Vector4& value  );	
	void GetAttribute( const std::string& name, Nx::Vector3& value, const std::string & Split = " " );
	void GetAttribute( const std::string& name, Nx::Vector2& value );
	//! set Attribute
	void SetAttribute( const std::string& name, const std::string& value );  
	void SetAttribute( const std::string& name, const bool& value );  
	void SetAttribute( const std::string& name, const short& value );  
	void SetAttribute( const std::string& name, const int& value );  
	void SetAttribute( const std::string& name, const long& value );  
	void SetAttribute( const std::string& name, const unsigned long& value );  
	void SetAttribute( const std::string& name, const float& value );  
	void SetAttribute( const std::string& name, const double& value ); 
	void SetAttribute( const std::string& name, const Nx::Vector4& value );
	void SetAttribute( const std::string& name, const Nx::Vector3& value );
	void SetAttribute( const std::string& name, const Nx::Vector2& value );
 
	private:  
	//! get Node Value
	const std::string getChildValue( const std::string & NodeName );
	//! Get Attribute Value;
	const std::string & GetAttribute( const std::string& name ) const ;
	//! The actual XML document created by the serializer.
	TiXmlDocument * doc; 
	//! The current node we're working on.
	TiXmlElement* currentElement; 
	//! filename if any;
	std::string mFilename;

};

class NxGraphics_Export NxGraphicsConfiguration 
{
public:
	NxGraphicsConfiguration(){};
	explicit NxGraphicsConfiguration( const std::string& );
	explicit NxGraphicsConfiguration( double );
	explicit NxGraphicsConfiguration( const char* );
	NxGraphicsConfiguration( const NxGraphicsConfiguration& );
	NxGraphicsConfiguration& operator=( NxGraphicsConfiguration const& );
	NxGraphicsConfiguration& operator=( double );
	NxGraphicsConfiguration& operator=( std::string const& );
public:
	operator std::string() const;
	operator double     () const;
private:
	std::string value_;
};

class NxGraphics_Export NxConfigFile 
{
	std::map<std::string,NxGraphicsConfiguration> content_;
public:
	NxConfigFile(std::string const& configFile);
	NxGraphicsConfiguration const& Value(std::string const& section, std::string const& entry) const;
	NxGraphicsConfiguration const& Value(std::string const& section, std::string const& entry, double value);
	NxGraphicsConfiguration const& Value(std::string const& section, std::string const& entry, std::string const& value);
};

}  
#endif