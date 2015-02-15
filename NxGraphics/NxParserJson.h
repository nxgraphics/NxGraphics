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
#ifndef __NXPARSERJSON_H__
#define __NXPARSERJSON_H__

#include "NxPrerequisites.h"

namespace Nx {

	enum JSONTYPE 
	{
		JSONNUMBER,
		JSONSTRING,
		JSONBOOL,
		JSONNULL,
		JSONARRAY,
		JSONOBJECT,
		JSONINVALID
	} ;

	typedef long double NxJsonNumber;
	typedef bool NxJsonBoolean;
	typedef std::string NxJsonString;
	struct NxJsonNull {};
	class NxJsonValue;
	class NxJsonObject;
	class NxJsonArray;


	class NxGraphics_Export NxJsonObject {
	public :
		typedef std::map<std::string, NxJsonValue*> NxJsonContainer;
		NxJsonObject() ;
		~NxJsonObject();
		void reset();
		bool parse(std::istream &input);
		bool parse(const std::string &input);
		bool parse(std::istream& input, NxJsonObject& object);
		const std::map<std::string, NxJsonValue*> & GetItems() const;
	private :
		NxJsonContainer value_map_;
	};

	class NxGraphics_Export NxJsonArray {
	public :
		typedef std::vector<NxJsonValue*> NxJsonContainer;
		NxJsonArray() ;
		~NxJsonArray();
		void reset();
		bool parse(std::istream &input);
		bool parse(const std::string &input);
		void import(const NxJsonArray &other) ;
		void import(const NxJsonValue &value) ;
		const std::vector<NxJsonValue*>& GetItems() const;
	 
	private :
		static bool parse(std::istream& input, NxJsonArray& array);
		NxJsonContainer values_;
	 
	};
 

 
class NxGraphics_Export NxJsonValue {
public :
	NxJsonValue() : mType( JSONINVALID ) {

	}

	~NxJsonValue() {

	}

	bool parse(std::istream &input);
	bool parse(const std::string &input);


	bool IsType( JSONTYPE ref ) {
		return mType == ref;
	}

	bool GetBoolean() { return mValueBool;  }
	long double GetNumber() { return mValueNumber;  }
	const std::string & GetString() const { return *mValueString; }
	const NxJsonArray & GetArray() const { return *mValueArray; }
	const NxJsonObject & GetObject() const { return *mValueObject; }

	void reset() ;

	/*
	void FromType( const bool &b ) {
		reset();
		mType = JSONBOOL;
		mValueBool = b;
	}
#define $number(TYPE) \
	void FromType( const TYPE &n ) { \
	reset(); \
	mType = JSONNUMBER; \
	mValueNumber = n; \
	}
	$number( char )
		$number( int )
		$number( long )
		$number( long long )
		$number( unsigned char )
		$number( unsigned int )
		$number( unsigned long )
		$number( unsigned long long )
		$number( float )
		$number( double )
		$number( long double )
#undef $number
#if JSONXX_COMPILER_HAS_CXX11 > 0
		void FromType( const std::nullptr_t &t ) {
			reset();
			mType = JSONNULL;
	}
#endif
	void FromType( const NxJsonNull &t ) {
		reset();
		mType = JSONNULL;
	}
	void FromType( const NxJsonString &s ) {
		reset();
		mType = JSONSTRING;
		*( mValueString = new NxJsonString() ) = s;
	}
	*/


protected:
	static bool parse(std::istream& input, NxJsonValue& value);
	JSONTYPE mType;

	union {
		NxJsonNumber mValueNumber;
		NxJsonString * mValueString;
		NxJsonBoolean mValueBool;
		NxJsonArray* mValueArray;
		NxJsonObject* mValueObject;
	};
	 
};

 



 
//! NxGraphicsXml Class.
class NxGraphics_Export NxGraphicsJson
{
public :
 
	//! NxGraphicsDeSerializer Class Constructor.
	NxGraphicsJson();
	//! NxGraphicsDeSerializer Class Deconstructor.
	~NxGraphicsJson();
	//! Open Xml File;
	bool OpenFile( const std::string & pFilename );

	const NxJsonObject & GetRootObject() const;
	
   
private :
	NxJsonObject * mJsonParser;
	
	

};
 

 
 

}  
#endif