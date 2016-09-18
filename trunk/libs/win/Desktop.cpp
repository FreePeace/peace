#include "Desktop.h"

#include <windows.h>
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
			}
			return result;

		}

		bool Desktop::SetDesktopWallpager(const char * img)
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
			}
			return result;
		}
	}
}
