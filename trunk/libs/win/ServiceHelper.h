#pragma once
namespace peace {
	namespace win {

		class ServiceHelper
		{
		private:
			ServiceHelper();
			~ServiceHelper();

		public:

			static bool IsInstalled(const wchar_t * name);

			static bool Install(const wchar_t * name);

			static bool UnInstall(const wchar_t * name);
		};
	}
}

