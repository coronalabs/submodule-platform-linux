//////////////////////////////////////////////////////////////////////////////
//
// This file is part of the Corona game engine.
// For overview and more information on licensing please refer to README.md 
// Home page: https://github.com/coronalabs/corona
// Contact: support@coronalabs.com
//
//////////////////////////////////////////////////////////////////////////////

#include "Rtt_HTTPClient.h"


namespace Rtt {

	bool HTTPClient::HttpDownload(const char* url, Rtt::String& resultData, String& errorMesg, const std::map<std::string, std::string>& headers)
	{
		errorMesg.Set("Not Implemented");
		return false;
	}

	bool HTTPClient::HttpDownloadFile(const char* url, const char* filename, String& errorMesg, const std::map<std::string, std::string>& headers)
	{
		errorMesg.Set("Not Implemented");
		return false;
	}

	int HTTPClient::download( lua_State *L )
	{
		return 0;
	}

	int HTTPClient::fetch( lua_State *L )
	{
	}

	void HTTPClient::registerFetcherModuleLoaders( lua_State *L )
	{

	}

}


