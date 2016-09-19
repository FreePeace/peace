#include "Service.h"

namespace peace {
	namespace win {
		Service::Service()
		{
		}

		Service::~Service()
		{
		}


		void Service::Run(std::function<void()> fun)
		{
			this->serviceData.threadID = ::GetCurrentThreadId();
			this->serviceData.runFun = fun;
			//SERVICE_TABLE_ENTRY st[] = {
			//	{ (LPWSTR)this->serviceData.serviceName.c_str() , ServiceHelper::ServiceMain },
			//	{ nullptr,nullptr }
			//};

			//if (!::StartServiceCtrlDispatcher(st))
			//{
			//	//error;
			//}

		}

		void Service::Run(const ServiceData & data)
		{
			this->serviceData = data;
			this->Run(data.runFun);
		}

		void Service::Init()
		{
			this->serviceData.serviceStatus = nullptr;
			this->serviceData.status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
			this->serviceData.status.dwCurrentState = SERVICE_STOPPED;
			this->serviceData.status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
			this->serviceData.status.dwWin32ExitCode = 0;
			this->serviceData.status.dwServiceSpecificExitCode = 0;
			this->serviceData.status.dwCheckPoint = 0;
			this->serviceData.status.dwWaitHint = 0;
		}

		const wchar_t * Service::ServiceName()
		{
			return this->serviceData.serviceName.c_str();
		}

		void Service::ServiceName(const wchar_t * serviceName_)
		{
			this->serviceData.serviceName = serviceName_;
		}


		void Service::ServiceMain(DWORD dwNumServicesArgs, LPWSTR * lpServiceArgVectors)
		{
			auto & status = serviceData.status;
			auto & serviceStatus = serviceData.serviceStatus;

			// Register the control request handler  
			status.dwCurrentState = SERVICE_START_PENDING;
			status.dwControlsAccepted = SERVICE_ACCEPT_STOP;

			//×¢²á·þÎñ¿ØÖÆ  
			//serviceStatus = RegisterServiceCtrlHandler(serviceData.serviceName.c_str(), ServiceHelper::ServiceStrl);
			if (serviceData.serviceStatus == nullptr)
			{
				//LogEvent(_T("Handler not installed"));
				return;
			}
			SetServiceStatus(serviceStatus, &status);

			status.dwWin32ExitCode = S_OK;
			status.dwCheckPoint = 0;
			status.dwWaitHint = 0;
			status.dwCurrentState = SERVICE_RUNNING;
			SetServiceStatus(serviceStatus, &status);

			if (nullptr != serviceData.runFun)
			{
				serviceData.runFun();
			}

			status.dwCurrentState = SERVICE_STOPPED;
			SetServiceStatus(serviceStatus, &status);

		}

		void Service::ServiceStrl(DWORD dwOpcode)
		{
			auto & status = serviceData.status;
			auto & serviceStatus = serviceData.serviceStatus;
			auto & threadID = serviceData.threadID;

			switch (dwOpcode)
			{
			case SERVICE_CONTROL_STOP:
			case SERVICE_CONTROL_SHUTDOWN:
				status.dwCurrentState = SERVICE_STOP_PENDING;
				SetServiceStatus(serviceStatus, &status);
				if (nullptr != serviceData.stopFun)
				{
					serviceData.stopFun();
				}
				PostThreadMessage(threadID, WM_CLOSE, 0, 0);
				break;
			case SERVICE_CONTROL_PAUSE:
				break;
			case SERVICE_CONTROL_CONTINUE:
				break;
			case SERVICE_CONTROL_INTERROGATE:
				break;
			default:
				//LogEvent(_T("Bad service request"));
				break;
			}
		}
	}
}
