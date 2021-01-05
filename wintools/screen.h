#pragma once
#include <wtypes.h>
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

struct Resolution {
	int height;
	int width;
};

class Screen {
public:
	Resolution get_resolution();
	void set_wallpaper(const wchar_t* wallpaper_path);
	void set_resolution(int width, int height);
};
