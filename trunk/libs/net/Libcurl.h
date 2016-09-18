#pragma once
#include <vector>
#include <functional>
#include <curl.h>
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
			//responseFun, ����ֵΪ�����ģ������������-1�������Ĳ����� libcurl �� CURLOPT_WRITEFUNCTION
			bool HttpGet(const wchar_t * url, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& responseFun);
			//responseFun, ����ֵΪ�����ģ������������-1�������Ĳ����� libcurl �� CURLOPT_WRITEFUNCTION
			bool HttpGet(const char * url, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& responseFun);

			bool HttpPost(const wchar_t * url, const wchar_t * responseFile, const wchar_t * requestFile);
			bool HttpPost(const char * url, const char * responseFile, const wchar_t * requestFile);
			bool HttpPost(const wchar_t * url, char * responseBuffer, size_t responseSize, char * requestBuffer, size_t requestSize);
			bool HttpPost(const char * url, char * responseBuffer, size_t bufferSize, char * requestBuffer, size_t requestSize);
			bool HttpPost(const wchar_t * url, std::vector<char>& responseBuffer, std::vector<char>& requestBuffer);
			bool HttpPost(const char * url, std::vector<char>& responseBuffer, std::vector<char>& requestBuffer);

			//responseFun, ����ֵΪ�����ģ������������-1�������Ĳ����� libcurl �� CURLOPT_WRITEFUNCTION
			bool HttpPost(const char * url, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& responseFun, std::function<size_t(char *ptr, size_t size, size_t nmemb)>& requestFun);



		private:
			static size_t WriteBufferCallback(char *ptr, size_t size, size_t nmemb, void *userdata);
		private:
			CURL * curl = nullptr;
		};
	}
}