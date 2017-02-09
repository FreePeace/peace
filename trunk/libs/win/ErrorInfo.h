#pragma once
#include <string>
namespace peace
{
	namespace win
	{
		class ErrorInfo
		{
		private:
			ErrorInfo();
			~ErrorInfo();

		public:
			static uint32_t LastErrorInfo(std::wstring & info);
			static void AddTextToFile(const std::wstring & name, const std::wstring & text);
		private:

		};
	}
}