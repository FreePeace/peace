#pragma once
#include <vector>
#include <functional>
#include "curl\curl.h"
namespace peace
{
	namespace net {
		class Libcurl
		{
		public:
			Libcurl();
			~Libcurl();
		public:
			CURL * Curl();
			bool HttpGet(const wchar_t * url, const wchar_t * responseFile);
			bool HttpGet(const char * url, const char * responseFile);
			bool HttpGet(const wchar_t * url, char * responseBuffer, size_t bufferSize);
			bool HttpGet(const char * url, char * responseBuffer, size_t bufferSize);
			bool HttpGet(const wchar_t * url, std::vector<char>& responseBuffer);
			bool HttpGet(const char * url, std::vector<char>& responseBuffer);
			//responseFun, 返回值为处理的，如果出错返回-1。其它的参数见 libcurl 的 CURLOPT_WRITEFUNCTION
			bool HttpGet(const wchar_t * url, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& responseFun);
			//responseFun, 返回值为处理的，如果出错返回-1。其它的参数见 libcurl 的 CURLOPT_WRITEFUNCTION
			bool HttpGet(const char * url, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& responseFun);

			bool HttpPost(const wchar_t * url, const wchar_t * responseFile, const wchar_t * requestFile);
			bool HttpPost(const char * url, const char * responseFile, const char * requestFile);
			bool HttpPost(const wchar_t * url, char * responseBuffer, size_t responseSize, const char * requestBuffer, size_t requestSize);
			bool HttpPost(const char * url, char * responseBuffer, size_t bufferSize, const char * requestBuffer, size_t requestSize);
			bool HttpPost(const wchar_t * url, std::vector<char>& responseBuffer, std::vector<char>& requestBuffer);
			bool HttpPost(const char * url, std::vector<char>& responseBuffer, std::vector<char>& requestBuffer);

			//responseFun, 返回值为处理的，如果出错返回-1。其它的参数见 libcurl 的 CURLOPT_WRITEFUNCTION
			//requestFun 的参数参见 libcurl CURLOPT_READFUNCTION
			bool HttpPost(const wchar_t * url, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& responseFun, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& requestFun);
			//responseFun, 返回值为处理的，如果出错返回-1。其它的参数见 libcurl 的 CURLOPT_WRITEFUNCTION
			//requestFun 的参数参见 libcurl CURLOPT_READFUNCTION
			bool HttpPost(const char * url, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& responseFun, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& requestFun);

		private:
			static size_t WriteCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
			static size_t ReadCallback(char *ptr, size_t size, size_t nmemb, void *userdata);

			void AddHttpsParameters(const char * url);
		private:
			CURL * curl = nullptr;
		};
	}
}