#pragma once
#include <Windows.h>
#include <functional>
namespace ATL
{
	class _stdcallthunk;
}

namespace peace {
	namespace win {
		class ServiceData {
		public:
			ServiceData & operator =(const ServiceData & other)
			{
				threadID = other.threadID;
				status = other.status;
				serviceStatus = other.serviceStatus;
				serviceName = other.serviceName;

				runFun = other.runFun;
				stopFun = other.stopFun;

				return *this;
			}
		public:
			DWORD threadID = 0;
			SERVICE_STATUS status;
			SERVICE_STATUS_HANDLE serviceStatus = nullptr;
			std::wstring serviceName;

			std::function<void()> runFun = nullptr;
			std::function<void()> stopFun = nullptr;
		};

		class Service
		{
		public:
			Service();
			~Service();

		public:
			void Run(std::function<void()> fun);

			void Run(const ServiceData & data);

			const wchar_t * ServiceName();

			void ServiceName(const wchar_t * serviceName_);

		protected:
			void Init();
			void WINAPI ServiceMain(DWORD  dwNumServicesArgs, LPWSTR  *lpServiceArgVectors);
			void WINAPI ServiceStrl(DWORD  dwOpcode);
		private:
			ServiceData serviceData;
			ATL::_stdcallthunk* callServiceMain = nullptr;
			ATL::_stdcallthunk* callServiceStrl = nullptr;

		};
	}
}