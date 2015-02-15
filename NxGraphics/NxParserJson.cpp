#include "NxGraphicsPreCompiled.h"
#include <sstream>
#include <fstream>


namespace Nx {

	// Settings
	enum Settings {
		// constants
		Enabled = true,
		Disabled = false,
		Permissive = true,
		Strict = false,
		// values
		Parser = Permissive,  // permissive or strict parsing
		Assertions = Enabled  // enabled or disabled assertions (these asserts work both in DEBUG and RELEASE builds)
	};

	bool match(const char* pattern, std::istream& input);
	bool parse_array(std::istream& input, NxJsonArray& array);
	bool parse_bool(std::istream& input, NxJsonBoolean& value);
	bool parse_comment(std::istream &input);
	bool parse_null(std::istream& input);
	bool parse_number(std::istream& input, NxJsonNumber& value);
	bool parse_object(std::istream& input, NxJsonObject& object);
	bool parse_string(std::istream& input, NxJsonString& value);
	bool parse_value(std::istream& input, NxJsonValue& value);


// Try to consume characters from the input stream and match the pattern string.
bool match(const char* pattern, std::istream& input) {
	input >> std::ws;
	const char* cur(pattern);
	char ch(0);
	while(input && !input.eof() && *cur != 0) {
		input.get(ch);
		if (ch != *cur) {
			input.putback(ch);
			if( parse_comment(input) )
				continue;
			while (cur > pattern) {
				cur--;
				input.putback(*cur);
			}
			return false;
		} else {
			cur++;
		}
	}
	return *cur == 0;
}

bool parse_string(std::istream& input, NxJsonString& value) {
	char ch = '\0', delimiter = '"';
	if (!match("\"", input))  {
		if (Parser == Strict) {
			return false;
		}
		delimiter = '\'';
		if (input.peek() != delimiter) {
			return false;
		}
		input.get(ch);
	}
	while(!input.eof() && input.good()) {
		input.get(ch);
		if (ch == delimiter) {
			break;
		}
		if (ch == '\\') {
			input.get(ch);
			switch(ch) {
			case '\\':
			case '/':
				value.push_back(ch);
				break;
			case 'b':
				value.push_back('\b');
				break;
			case 'f':
				value.push_back('\f');
				break;
			case 'n':
				value.push_back('\n');
				break;
			case 'r':
				value.push_back('\r');
				break;
			case 't':
				value.push_back('\t');
				break;
			case 'u': {
				int i;
				std::stringstream ss;
				for( i = 0; (!input.eof() && input.good()) && i < 4; ++i ) {
					input.get(ch);
					ss << ch;
				}
				if( input.good() && (ss >> i) )
					value.push_back(i);
						}
						break;
			default:
				if (ch != delimiter) {
					value.push_back('\\');
					value.push_back(ch);
				} else value.push_back(ch);
				break;
			}
		} else {
			value.push_back(ch);
		}
	}
	if (input && ch == delimiter) {
		return true;
	} else {
		return false;
	}
}

bool parse_number(std::istream& input, NxJsonNumber& value) {
	input >> std::ws;
	input >> value;
	if (input.fail()) {
		input.clear();
		return false;
	}
	return true;
}

bool parse_bool(std::istream& input, NxJsonBoolean& value) {
	if (match("true", input))  {
		value = true;
		return true;
	}
	if (match("false", input)) {
		value = false;
		return true;
	}
	return false;
}

bool parse_null(std::istream& input) {
	if (match("null", input))  {
		return true;
	}
	if (Parser == Strict) {
		return false;
	}
	return (input.peek()==',');
}

bool parse_array(std::istream& input, NxJsonArray & array) {
	return array.parse(input);
}

bool parse_object(std::istream& input, NxJsonObject & object) {
	return object.parse(input);
}

bool parse_comment(std::istream &input) {
	if( Parser == Permissive )
		if( !input.eof() ) {
			char ch0(0);
			input.get(ch0);

			if( !input.eof() ) {
				char ch1(0);
				input.get(ch1);

				if( ch0 == '/' && ch1 == '/' ) {
					// trim chars till \r or \n
					for( char ch(0); !input.eof() && (input.peek() != '\r' && input.peek() != '\n'); )
						input.get(ch);

					// consume spaces, tabs, \r or \n, in case no eof is found
					if( !input.eof() )
						input >> std::ws;
					return true;
				}

				input.unget();
				input.clear();
			}

			input.unget();
			input.clear();
		}

		return false;
}

bool parse_value(std::istream& input, NxJsonValue& value) {
	return value.parse(input);
}


////////////

void NxJsonValue::reset() {
	if (mType == JSONSTRING) {
		delete mValueString;
		mValueString = 0;
	}
	else if (mType == JSONOBJECT) {
		delete mValueObject;
		mValueObject = 0;
	}
	else if (mType == JSONARRAY) {
		delete mValueArray;
		mValueArray = 0;
	}
}

bool NxJsonValue::parse(std::istream &input) {
	return parse(input,*this);
}
bool NxJsonValue::parse(const std::string &input) {
	std::istringstream is( input );
	return parse(is,*this);
}

bool NxJsonValue::parse(std::istream& input, NxJsonValue & value) {
	value.reset();

	std::string string_value;
	if (parse_string(input, string_value)) {
		value.mValueString = new std::string();
		value.mValueString->swap(string_value);
		value.mType = JSONSTRING;
		return true;
	}
	if (parse_number(input, value.mValueNumber)) {
		value.mType = JSONNUMBER;
		return true;
	}

	if (parse_bool(input, value.mValueBool)) {
		value.mType = JSONBOOL;
		return true;
	}
	if (parse_null(input)) {
		value.mType = JSONNULL;
		return true;
	}
	if (input.peek() == '[') {
		value.mValueArray = new NxJsonArray();
		if (parse_array(input, *value.mValueArray)) {
			value.mType = JSONARRAY;
			return true;
		}
		delete value.mValueArray;
	}
	value.mValueObject = new NxJsonObject();
	if (parse_object(input, *value.mValueObject)) {
		value.mType = JSONOBJECT;
		return true;
	}
	delete value.mValueObject;
	return false;
}



//////////////////

NxJsonObject::NxJsonObject() :  value_map_() {

}

NxJsonObject::~NxJsonObject() {

}

void NxJsonObject::reset() {
	NxJsonContainer::iterator i;
	for (i = value_map_.begin(); i != value_map_.end(); ++i) {
		delete i->second;
	}
	value_map_.clear();
}

bool NxJsonObject::parse(std::istream &input) {
	return parse(input,*this);
}

bool NxJsonObject::parse(const std::string &input) {
	std::istringstream is( input );
	return parse(is,*this);
}

bool NxJsonObject::parse(std::istream& input, NxJsonObject& object) {
	object.reset();

	if (!match("{", input)) {
		return false;
	}
	if (match("}", input)) {
		return true;
	}

	do {
		std::string key;
		if (!parse_string(input, key)) {
			if (Parser == Permissive) {
				if (input.peek() == '}')
					break;
			}
			return false;
		}
		if (!match(":", input)) {
			return false;
		}
		NxJsonValue* v = new NxJsonValue();
		if (!parse_value(input, *v)) {
			delete v;
			break;
		}
		object.value_map_[key] = v;
	} while (match(",", input));


	if (!match("}", input)) {
		return false;
	}

	return true;
}


const std::map<std::string, NxJsonValue*> & NxJsonObject::GetItems() const {
	return value_map_;
}

////////////////////////////


NxJsonArray::NxJsonArray() {

}

NxJsonArray::~NxJsonArray() {

}

void NxJsonArray::reset() {
	for (NxJsonContainer::iterator i = values_.begin(); i != values_.end(); ++i) {
		delete *i;
	}
	values_.clear();
}

bool NxJsonArray::parse(std::istream &input) {
	return parse(input,*this);
}
bool NxJsonArray::parse(const std::string &input) {
	std::istringstream is(input);
	return parse(is,*this);
}

bool NxJsonArray::parse(std::istream& input, NxJsonArray& array) {
	array.reset();

	if (!match("[", input)) {
		return false;
	}

	do {
		NxJsonValue* v = new NxJsonValue();
		if (!parse_value(input, *v)) {
			delete v;
			break;
		}
		array.values_.push_back(v);
	} while (match(",", input));

	if (!match("]", input)) {
		return false;
	}
	return true;
}

void NxJsonArray::import(const NxJsonArray &other) {
	if (this != &other) {
		// default
		NxJsonContainer::const_iterator
			it = other.values_.begin(),
			end = other.values_.end();
		for (/**/ ; it != end; ++it) {
			values_.push_back( new NxJsonValue(**it) );
		}
	} else {
		// recursion is supported here
		import( NxJsonArray(*this) );
	}
}
void NxJsonArray::import(const NxJsonValue &value) {
	values_.push_back( new NxJsonValue(value) );
}

const std::vector<NxJsonValue*>& NxJsonArray::GetItems() const {
	return values_;
}




/////////////////////////////

NxGraphicsJson::NxGraphicsJson() : mJsonParser(NULL) {
	
}
 
NxGraphicsJson::~NxGraphicsJson() {
 
}
 

bool NxGraphicsJson::OpenFile( const std::string & pFilename ) {
	mJsonParser = new NxJsonObject();
	std::ifstream t(pFilename.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();
	bool ret =  mJsonParser->parse(buffer.str());
	return ret;
}

const NxJsonObject & NxGraphicsJson::GetRootObject() const {
	return *mJsonParser;
}
 

 
 
}