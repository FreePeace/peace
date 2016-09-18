#pragma once

class Desktop
{
private:
	Desktop();
	~Desktop();

public:
	static bool SetDesktopWallpager(const wchar_t * img);
	static bool SetDesktopWallpager(const char * img);
};
