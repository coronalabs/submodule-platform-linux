//////////////////////////////////////////////////////////////////////////////
//
// This file is part of the Corona game engine.
// For overview and more information on licensing please refer to README.md
// Home page: https://github.com/coronalabs/corona
// Contact: support@coronalabs.com
//
//////////////////////////////////////////////////////////////////////////////

#include "Rtt_HTTPClient.h"
#include "Rtt_Lua.h"
#include <curl/curl.h>

namespace Rtt
{
	bool HTTPClient::HttpDownload(const char *url, Rtt::String &resultData, String &errorMesg, const std::map<std::string, std::string> &headers)
	{
		errorMesg.Set("Not Implemented");
		return false;
	}

	bool HTTPClient::HttpDownloadFile(const char *url, const char *filename, String &errorMesg, const std::map<std::string, std::string> &headers)
	{
		errorMesg.Set("Not Implemented");
		return false;
	}

	static size_t WriteResponse(void *contents, size_t size, size_t nmemb, void *userp)
	{
		((std::string *)userp)->append((char *)contents, size * nmemb);
		return size * nmemb;
	}

	int HTTPClient::download(lua_State *L)
	{
		const char *url = NULL;
		const char *outFilename = NULL;

		if (lua_isstring(L, 1))
		{
			url = lua_tostring(L, 1);
		}
		else
		{
			luaL_error(L, "HttpClient::download(url, filename) url (string) expected, got %s", lua_typename(L, 1));
		}

		if (lua_isstring(L, 2))
		{
			outFilename = lua_tostring(L, 2);
		}
		else
		{
			luaL_error(L, "HttpClient::download(url, filename) filename (string) expected, got %s", lua_typename(L, 2));
		}

		// init curl
		curl_global_init(CURL_GLOBAL_ALL);

		// init the curl session
		CURL *curlHandle = curl_easy_init();
		// set URL
		curl_easy_setopt(curlHandle, CURLOPT_URL, url);
		// turn off full protocol/debug output (set to 1L to enable it)
		curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 0L);
		// disable progress meter (set to 0L to enable it)
		curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 1L);
		// follow redirection
		curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
		// open the file
		FILE *responseFile = fopen(outFilename, "w");

		if (responseFile)
		{
			// write the page body to this file handle
			curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, responseFile);
			curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, NULL);
			// execute the request
			CURLcode res = curl_easy_perform(curlHandle);
			// push the result
			lua_pushnumber(L, res);
			// close the response file
			fclose(responseFile);
		}

		// cleanup
		curl_easy_cleanup(curlHandle);
		curl_global_cleanup();

		return 1;
	}

	int HTTPClient::fetch(lua_State *L)
	{
		std::string response;
		const char *url = NULL;

		if (lua_isstring(L, 1))
		{
			url = lua_tostring(L, 1);
		}
		else
		{
			luaL_error(L, "HttpClient::fetch(url) url (string) expected, got %s", lua_typename(L, 1));
		}

		// init curl
		curl_global_init(CURL_GLOBAL_ALL);

		// init the curl session
		CURL *curlHandle = curl_easy_init();
		// set URL
		curl_easy_setopt(curlHandle, CURLOPT_URL, url);
		// turn off full protocol/debug output (set to 1L to enable it)
		curl_easy_setopt(curlHandle, CURLOPT_VERBOSE, 0L);
		// disable progress meter (set to 0L to enable it)
		curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 1L);
		// follow redirection
		curl_easy_setopt(curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
		// send all data to this function
		curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteResponse);
		// write the response to the handle
		curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, &response);
		// execute the request
		CURLcode res = curl_easy_perform(curlHandle);

		// push the result & response
		lua_pushnumber(L, res);
		lua_pushstring(L, response.c_str());

		// cleanup
		curl_easy_cleanup(curlHandle);
		curl_global_cleanup();

		return 2;
	}

	void HTTPClient::registerFetcherModuleLoaders(lua_State *L)
	{
		lua_pushcfunction(L, &HTTPClient::fetch);
		lua_setglobal ( L, "pluginCollector_fetch");

		lua_pushcfunction(L, &HTTPClient::download);
		lua_setglobal ( L, "pluginCollector_download");
	}
} // namespace Rtt
