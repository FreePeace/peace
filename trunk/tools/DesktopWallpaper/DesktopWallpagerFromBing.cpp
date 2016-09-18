

#include <regex>
#include <fstream>
#include <experimental\filesystem>
#include "net\Libcurl.h"
#include "win\Desktop.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	std::string imgUrl;
	{
		peace::net::Libcurl lib;
		std::vector<char> res;
		lib.HttpGet("http://cn.bing.com/", res);
		res.push_back('\0');

		std::regex regItem(";g_img=\\{url:\\s*\"(http://[^\"]+)\"[^\\}]+id:'bgDiv'");
		std::cmatch m;
		char * start = &res[0];
#ifdef _DEBUG
		{
			std::ofstream fs;
			fs.open("bing.html", std::ios::out | std::ios::trunc);
			fs << &res[0];
			fs.close();
		}
#endif // _DEBUG
		if(std::regex_search(start, m, regItem))
		{
			imgUrl = m.str(1);
		}
	}

	if (!imgUrl.empty())
	{
		std::string fileName;
		{
			auto images = "images";
			if (!std::experimental::filesystem::exists(images))
			{
				std::experimental::filesystem::create_directory(images);
			}

			auto i = imgUrl.find_last_of('/');
			if (i != std::string::npos)
			{
				fileName = images;
				fileName.append("\\");
				fileName.append(imgUrl.substr(i + 1));
			}
		}
		if (!fileName.empty())
		{
			if (!std::experimental::filesystem::exists(fileName))
			{
				peace::net::Libcurl lib;
				lib.HttpGet(imgUrl.c_str(), fileName.c_str());
			}

			if (std::experimental::filesystem::exists(fileName))
			{
				peace::win::Desktop::SetDesktopWallpager(fileName.c_str());
			}
		}
	}
}

