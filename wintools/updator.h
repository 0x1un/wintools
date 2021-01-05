#pragma once
#include <string>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include "utils.h"
#pragma comment( lib,"Urlmon.lib")

using namespace std;

class Updator
{
public:
	void update();
private:
	bool have_update();
	bool download_files(LPCWSTR url, LPCWSTR save_name);
private:
	Utils util;
};

