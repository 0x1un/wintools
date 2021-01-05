#pragma once
#include <string>
#include <fstream>
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>
#include <vector>

#define BUFSIZE 1024
#define MD5LEN  16

using namespace std;

class Utils
{
public:
	string md5sum_file(LPCWSTR in);
};

