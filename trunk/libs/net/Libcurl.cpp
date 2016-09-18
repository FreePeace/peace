#include "Libcurl.h"
#include <cwchar>
#include <vector>
#include <experimental\filesystem>
#include <memory>
namespace peace
{
	namespace net
	{
		Libcurl::Libcurl()
		{
			curl = curl_easy_init();
			//curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
			//curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
			//curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
#ifdef _DEBUG
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif // _DEBUG

		}

		Libcurl::~Libcurl()
		{
			if (nullptr != curl)
			{
				curl_easy_cleanup(curl);
				curl = nullptr;
			}
		}

		CURL * Libcurl::Curl()
		{
			return curl;
		}

		bool Libcurl::HttpGet(const wchar_t * url, const wchar_t * responseFile)
		{
			int len = std::wcslen(responseFile);
			std::vector<char> fn(2 * len + 1, '\0');
			{
				mbstate_t mbs;
				std::mbrlen(NULL, 0, &mbs);
				std::wcsrtombs(&fn[0], &responseFile, fn.size() - 1, &mbs);
			}

			len = std::wcslen(url);
			std::vector<char> u(2 * len + 1, '\0');
			{
				mbstate_t mbs;
				std::mbrlen(NULL, 0, &mbs);
				std::wcsrtombs(&u[0], &url, u.size() - 1, &mbs);
			}
			return HttpGet(&u[0], &fn[0]);
		}

		bool Libcurl::HttpGet(const char * url, const char * responseFile)
		{
			auto result = false;
			FILE * file = std::fopen(responseFile, "wb");
			if (nullptr != file)
			{
				std::function<size_t(char *, size_t, size_t)> f = [file](char * ptr, size_t size, size_t nmemb)->size_t {
					auto writed = std::fwrite(ptr, size, nmemb, file);
					return writed;
				};
				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteBufferCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &f);

				auto res = curl_easy_perform(curl);
				std::fclose(file);

				if (CURLE_OK == res)
				{
					long code = 0;
					curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
					if (404 != code)
					{
						result = true;
					}
				}
			}
			return result;
		}

		bool Libcurl::HttpGet(const wchar_t * url, char * responseBuffer, size_t bufferSize)
		{
			int len = std::wcslen(url);
			std::vector<char> u(2 * len + 1, '\0');
			{
				mbstate_t mbs;
				std::mbrlen(NULL, 0, &mbs);
				std::wcsrtombs(&u[0], &url, u.size() - 1, &mbs);
			}
			return HttpGet(&u[0], responseBuffer, bufferSize);
		}

		bool Libcurl::HttpGet(const char * url, char * responseBuffer, size_t bufferSize)
		{
			auto result = false;
			if (nullptr != responseBuffer && bufferSize > 0)
			{
				size_t curPos = 0;
				auto f = [responseBuffer, bufferSize, &curPos](char * ptr, size_t size, size_t nmemb) {
					size_t result = nmemb;
					size_t c = size * nmemb;
					if (c > 0)
					{
						if (curPos + c > bufferSize)
						{
							c = bufferSize - curPos;
						}

						if (c > 0)
						{
							std::memcpy(responseBuffer + curPos, ptr, c);
							curPos += c;
						}
						else
						{
							result = -1;
						}
					}
					return result;
				};

				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_READFUNCTION, nullptr);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteBufferCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &f);

				auto res = curl_easy_perform(curl);

				if (CURLE_OK == res)
				{
					long code = 0;
					curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
					if (404 != code)
					{
						result = true;
					}
				}
			}
			return result;
		}

		bool Libcurl::HttpGet(const wchar_t * url, std::vector<char>& responseBuffer)
		{
			int len = std::wcslen(url);
			std::vector<char> u(2 * len + 1, '\0');
			{
				mbstate_t mbs;
				std::mbrlen(NULL, 0, &mbs);
				std::wcsrtombs(&u[0], &url, u.size() - 1, &mbs);
			}
			return HttpGet(&u[0], responseBuffer);
		}

		bool Libcurl::HttpGet(const char * url, std::vector<char>& responseBuffer)
		{
			auto result = false;
			{
				size_t curPos = 0;
				std::function<size_t(char *, size_t, size_t)> f = [&responseBuffer](char * ptr, size_t size, size_t nmemb)->size_t {
					size_t result = nmemb;
					if (size * nmemb > 0)
					{
						responseBuffer.insert(responseBuffer.end(), ptr, ptr + size*nmemb);
					}
					return result;
				};

				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_READFUNCTION, nullptr);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteBufferCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &f);

				auto res = curl_easy_perform(curl);

				if (CURLE_OK == res)
				{
					long code = 0;
					curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
					if (404 != code)
					{
						result = true;
					}
				}
			}
			return result;
		}

		bool Libcurl::HttpGet(const wchar_t * url, std::function<size_t(char*ptr, size_t size, size_t nmemb)>& responseFun)
		{
			int len = std::wcslen(url);
			std::vector<char> u(2 * len + 1, '\0');
			{
				mbstate_t mbs;
				std::mbrlen(NULL, 0, &mbs);
				std::wcsrtombs(&u[0], &url, u.size() - 1, &mbs);
			}
			return HttpGet(&u[0], responseFun);
		}

		bool Libcurl::HttpGet(const char * url, std::function<size_t(char*ptr, size_t size, size_t nmemb)>& responseFun)
		{
			auto result = false;
			{
				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_READFUNCTION, nullptr);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteBufferCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseFun);

				auto res = curl_easy_perform(curl);

				if (CURLE_OK == res)
				{
					long code = 0;
					curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
					if (404 != code)
					{
						result = true;
					}
				}
			}
			return result;
		}

		size_t Libcurl::WriteBufferCallback(char * ptr, size_t size, size_t nmemb, void * userdata)
		{
			auto f = (std::function<size_t(char *, size_t, size_t)> *)userdata;
			size_t result = 0;
			if (nullptr != f)
			{
				result = (*f)(ptr, size, nmemb);
			}
			return result;
		}
	}
}