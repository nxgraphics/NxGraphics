#include "NxNetworkPreCompiled.h"
#include "curl.h"

namespace NxNetwork {

	static const int CURL_MAX_REDIRECTS = 10;


	class NxNetworkUrl
	{
	public:
 
		curl_slist*	m_requestHeaders;
		std::string	m_responseData;
		std::vector< std::string > m_responseHeaders;
		char m_errorBuffer[CURL_ERROR_SIZE];
 
		NxNetworkUrl() : m_requestHeaders(NULL), m_responseHeaders() {  

			m_responseHeaders.reserve(20);
 
		}

		~NxNetworkUrl() {
			if (m_requestHeaders)
				curl_slist_free_all(m_requestHeaders);
		}

		bool mystrnicmp(char const* s1, char const* s2, int n){
			for(int i=0; i < n; ++i){
				unsigned char c1 = static_cast<unsigned char>(s1[i]);
				unsigned char c2 = static_cast<unsigned char>(s2[i]);
				if(tolower(c1) != tolower(c2))
					return false;
				if(c1 == '\0' || c2 == '\0')
					break;
			}
			return true;
		}


		const std::vector<std::string>& GetResponseHeaders() const
		{
			return m_responseHeaders;
		}

		const std::string& GetResponseData() const
		{
			return m_responseData;
		}

		void HandleWriteData(const char* data, const size_t dataSize)
		{
			m_responseData.append(data, dataSize);
		}

		static size_t WriteFunction(void* data, size_t mult, size_t size, void* state)
		{
			const size_t dataSize = size * mult;
			if (dataSize && state)
				reinterpret_cast<NxNetworkUrl*>(state)->HandleWriteData(reinterpret_cast<char*>(data), dataSize);
			return dataSize;
		}
 
		static size_t HeaderFunction(void* data, size_t mult, size_t size, void* state)
		{
			const size_t dataSize = size * mult;
			if (dataSize && state)
				reinterpret_cast<NxNetworkUrl*>(state)->HandleHeaderData(reinterpret_cast<char*>(data), dataSize);
			return dataSize;
		}

		void HandleHeaderData(const char* data, const size_t dataSize)
		{
			m_responseHeaders.push_back(std::string(data, dataSize));

			data = m_responseHeaders.back().c_str();
			if (!mystrnicmp(data, "content-length:", strlen(data))) {
				Log("content-length: not found in header");
				return;
			}

			int contentLength = std::atoi(data + 16);
			if (contentLength)
				m_responseData.reserve(contentLength);
		}

	} ; 

 
	NxNetworkHttpClient::NxNetworkHttpClient()
	{


	}

	NxNetworkHttpClient::~NxNetworkHttpClient()
	{

	}

	/* Converts a hex character to its integer value */
	char from_hex(char ch) {
		return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
	}

	/* Converts an integer value to its hex character*/
	char to_hex(char code) {
		static char hex[] = "0123456789abcdef";
		return hex[code & 15];
	}

	/* Returns a url-encoded version of str */
	/* IMPORTANT: be sure to free() the returned string after use */
	char *url_encode(char *str) {
		char *pstr = str, *buf = (char*)malloc(strlen(str) * 3 + 1), *pbuf = buf;
		while (*pstr) {
			if (isalnum(*pstr) || *pstr == '-' || *pstr == '_' || *pstr == '.' || *pstr == '~') 
				*pbuf++ = *pstr;
			else if (*pstr == ' ') 
				*pbuf++ = '+';
			else 
				*pbuf++ = '%', *pbuf++ = to_hex(*pstr >> 4), *pbuf++ = to_hex(*pstr & 15);
			pstr++;
		}
		*pbuf = '\0';
		return buf;
	}

	/* Returns a url-decoded version of str */
	/* IMPORTANT: be sure to free() the returned string after use */
	char *url_decode(char *str) {
		char *pstr = str, *buf = (char*)malloc(strlen(str) + 1), *pbuf = buf;
		while (*pstr) {
			if (*pstr == '%') {
				if (pstr[1] && pstr[2]) {
					*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
					pstr += 2;
				}
			} else if (*pstr == '+') { 
				*pbuf++ = ' ';
			} else {
				*pbuf++ = *pstr;
			}
			pstr++;
		}
		*pbuf = '\0';
		return buf;
	}

	void NxNetworkHttpClient::UrlEncode( std::string & encode ) {
		char * enc = url_encode( (char*)encode.c_str()  );
		encode = enc;
		free( enc );
	}
	
	void NxNetworkHttpClient::UrlDecode( std::string & decode ) {
		char * dec = url_decode( (char*)decode.c_str()  );
		decode = dec;
		free( dec );
	}


	struct MemoryStruct {
		char *memory;
		size_t size;
	};
 
	static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		size_t realsize = size * nmemb;
		struct MemoryStruct *mem = (struct MemoryStruct *)userp;

		mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
		if (mem->memory == NULL) {
			/* out of memory! */ 
			Log("not enough memory (realloc returned NULL)");
			exit(EXIT_FAILURE);
		}

		memcpy(&(mem->memory[mem->size]), contents, realsize);
		mem->size += realsize;
		mem->memory[mem->size] = 0;

		return realsize;
	}
 
	bool NxNetworkHttpClient::GetResourceBuffer( const std::string & Url, char * Buffer, std::size_t * Size, long * ReturnCode )
	{
		CURL *curl_handle = NULL;

		mNetwork = new NxNetworkUrl();

		/* init the curl session */ 
		curl_handle = curl_easy_init();

		if(!curl_handle){ Log("Curl Failed !"); return false; }

		MemoryStruct chunk;
		chunk.memory = NULL;
		chunk.size = 0;

		/* specify URL to get */ 
		curl_easy_setopt(curl_handle, CURLOPT_URL, Url.c_str());
		/* some servers don't like requests that are made without a user-agent field, so we provide one */ 
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/535.6 (KHTML, like Gecko) Chrome/16.0.897.0 Safari/535.6");

		//curl_easy_setopt(curl_handle, CURLOPT_HEADER, true);

		curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, true);

		/* send all data to this function  */ 
		curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

		/* we pass our 'chunk' struct to the callback function */ 
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

		/* get it! */ 
		CURLcode code = curl_easy_perform(curl_handle);
		if( code != CURLE_OK) { Log("curl_easy_perform Failed !"); return false;  } 

		if( Buffer ) memcpy( &Buffer[0], chunk.memory, chunk.size );
		*Size = chunk.size;

		curl_easy_getinfo (curl_handle, CURLINFO_RESPONSE_CODE,  ReturnCode);
 
		curl_easy_cleanup(curl_handle);
 
		free( chunk.memory );

		return true;
	}

	bool NxNetworkHttpClient::ResourceExists( const std::string & url, long * ReturnCode, std::size_t * Size )
	{

		mNetwork = new NxNetworkUrl();

		CURL *m_curlHandle = NULL;

		//init the curl session  
		m_curlHandle = curl_easy_init();

		if(!m_curlHandle){ Log("Curl Failed !"); return false; }

		curl_easy_setopt(m_curlHandle, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(m_curlHandle, CURLOPT_NOPROGRESS, 1);
		curl_easy_setopt(m_curlHandle, CURLOPT_URL, url.c_str());
		curl_easy_setopt(m_curlHandle, CURLOPT_SSL_VERIFYPEER, 0);						// disable certificates
		curl_easy_setopt(m_curlHandle, CURLOPT_ERRORBUFFER, mNetwork->m_errorBuffer);	// error buffer  
		curl_easy_setopt(m_curlHandle, CURLOPT_FOLLOWLOCATION, 1);						// follow redirects
		curl_easy_setopt(m_curlHandle, CURLOPT_MAXREDIRS, CURL_MAX_REDIRECTS);			// number of redirects to follow
		//curl_easy_setopt(m_curlHandle, CURLOPT_VERBOSE, 1);							// VERBROSE

		//if (compress)
		//{
		//	curl_easy_setopt(m_curlHandle, CURLOPT_ENCODING, "deflate");  // request zlib  compression
		//}

		// set up a minimum transfer rate - less then a 100 bytes in 5 minutes is a failure
		curl_easy_setopt(m_curlHandle, CURLOPT_LOW_SPEED_LIMIT, 100);
		curl_easy_setopt(m_curlHandle, CURLOPT_LOW_SPEED_TIME , 300);

		/*
		if (!etag.empty())
		{
			std::string etagHeader("If-None-Match: ");
			etagHeader += etag;
			m_requestHeaders = curl_slist_append(m_requestHeaders, etagHeader.c_str());
			curl_easy_setopt(m_curlHandle, CURLOPT_HTTPHEADER, m_requestHeaders);
		}
		*/

		curl_easy_setopt(m_curlHandle, CURLOPT_HEADERFUNCTION, &NxNetworkUrl::HeaderFunction);
		curl_easy_setopt(m_curlHandle, CURLOPT_HEADERDATA, mNetwork);

		curl_easy_setopt(m_curlHandle, CURLOPT_WRITEFUNCTION, &NxNetworkUrl::WriteFunction);
		curl_easy_setopt(m_curlHandle, CURLOPT_WRITEDATA, mNetwork);

		if( curl_easy_perform(m_curlHandle) != CURLE_OK ) return false;

		long code = 0;
		*ReturnCode =  curl_easy_getinfo(m_curlHandle, CURLINFO_RESPONSE_CODE, &code) == CURLE_OK ? code : 0;
		*Size = mNetwork->m_responseData.size();

		Log("Url : "  + url );
		//Log("Return Code : " +   NxNetworkUtils::ToString(  ReturnCode  )  ); // error on android
		//Log("Header size in Bytes : " + NxNetworkUtils::ToString( *Size  )  );

		delete mNetwork;
		mNetwork = NULL;

		curl_easy_cleanup(m_curlHandle);

		return true;
	}


}