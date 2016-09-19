#include "ServiceHelper.h"
#include <Windows.h>
#include <experimental\filesystem>

namespace peace {
	namespace win {
		ServiceHelper::ServiceHelper()
		{
		}

		ServiceHelper::~ServiceHelper()
		{
		}


		bool ServiceHelper::IsInstalled(const wchar_t * name)
		{
			auto result = false;

			//打开服务控制管理器  
			SC_HANDLE scm = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

			if (scm != NULL)
			{
				//打开服务  
				SC_HANDLE service = ::OpenService(scm, name, SERVICE_QUERY_CONFIG);
				if (service != NULL)
				{
					result = TRUE;
					::CloseServiceHandle(service);
				}
				::CloseServiceHandle(scm);
			}
			return result;
		}

		bool ServiceHelper::Install(const wchar_t * name)
		{
			if (!IsInstalled(name))
			{
				//打开服务控制管理器  
				SC_HANDLE scm = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);
				if (scm == nullptr)
				{
					return false;
				}

				// Get the executable file path  
				TCHAR filePath[MAX_PATH];
				::GetModuleFileName(NULL, filePath, MAX_PATH);
				auto p = std::experimental::filesystem::canonical(filePath);
				//创建服务  
				SC_HANDLE service = ::CreateService(
					scm, name, name,
					SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
					SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
					p.c_str(), NULL, NULL, L"", NULL, NULL);

				if (service == nullptr)
				{
					::CloseServiceHandle(scm);
					return false;
				}

				::CloseServiceHandle(service);
				::CloseServiceHandle(scm);
			}
			return true;
		}

		bool ServiceHelper::UnInstall(const wchar_t * name)
		{
			if (IsInstalled(name))
			{
				SC_HANDLE scm = ::OpenSCManager(nullptr, nullptr, SC_MANAGER_ALL_ACCESS);

				if (scm == nullptr)
				{
					return false;
				}

				SC_HANDLE service = ::OpenService(scm, name, SERVICE_STOP | DELETE);

				if (service == nullptr)
				{
					::CloseServiceHandle(scm);
					return false;
				}
				SERVICE_STATUS status;
				::ControlService(service, SERVICE_CONTROL_STOP, &status);

				//删除服务  
				bool deleted = ::DeleteService(service);
				::CloseServiceHandle(service);
				::CloseServiceHandle(scm);

				return deleted;
			}
			return true;
		}
	}
}