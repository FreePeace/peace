#include "Desktop.h"

#include <windows.h>
#include <wininet.h>
#include <shlobj.h>

#include <experimental\filesystem>


namespace peace
{
	namespace win
	{
		Desktop::Desktop()
		{
		}

		Desktop::~Desktop()
		{
		}

		bool Desktop::SetDesktopWallpager(const wchar_t * img)
		{
			bool result = false;
			std::experimental::filesystem::path p = img;
			p = std::experimental::filesystem::absolute(p);
			p = std::experimental::filesystem::canonical(p);

			if (SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (LPVOID)p.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE))
			{
				// Refresh the desktop:
				//LPITEMIDLIST pidl;
				//SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
				//SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, pidl, 0);
				//result = true;
				result = true;
			}
			return result;
		}

		bool Desktop::SetDesktopWallpager(const char * img)
		{
			bool result = false;
			std::experimental::filesystem::path p = img;
			p = std::experimental::filesystem::absolute(p);
			p = std::experimental::filesystem::canonical(p);
			//SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (LPVOID)p.c_str(), SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE)
			if (SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (LPVOID)p.c_str(), 0))
			{
				// Refresh the desktop:
				//LPITEMIDLIST pidl;
				//SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl);
				//SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, pidl, 0);
				result = true;
			}
			else
			{
				IActiveDesktop* pActiveDesktop = nullptr;
				CoInitialize(NULL);
				auto hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_ALL, IID_IActiveDesktop, (LPVOID*)&pActiveDesktop);
				if (SUCCEEDED(hr))
				{
					hr = pActiveDesktop->SetWallpaper(p.c_str(), 0);
					if (SUCCEEDED(hr))
					{
						hr = pActiveDesktop->ApplyChanges(AD_APPLY_ALL);
						if (SUCCEEDED(hr)) {
							result = true;
						}						
					}
				}
				pActiveDesktop->Release();
				CoUninitialize();
			}
			return result;
		}
	}
}
