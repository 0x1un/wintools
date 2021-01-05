#include "utils.h"
#include <iostream>

using namespace std;

string get_md5(LPCWSTR filename);

string Utils::md5sum_file(LPCWSTR in)
{
	return get_md5(in);
}


string get_md5(LPCWSTR filename)
{
	DWORD dwStatus = 0;
	BOOL bResult = FALSE;
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HANDLE hFile = NULL;
	BYTE rgbFile[BUFSIZE];
	DWORD cbRead = 0;
	BYTE rgbHash[MD5LEN];
	DWORD cbHash = 0;
	CHAR rgbDigits[] = "0123456789abcdef";
	// Logic to check usage goes here.

	hFile = CreateFile(filename,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_SEQUENTIAL_SCAN,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwStatus = GetLastError();
		cout << "Error opening file " << filename << "\nError: " << dwStatus << endl;
		return string();
	}

	// Get handle to the crypto provider
	if (!CryptAcquireContext(&hProv,
		NULL,
		NULL,
		PROV_RSA_FULL,
		CRYPT_VERIFYCONTEXT))
	{
		dwStatus = GetLastError();
		cout << "CryptAcquireContext failed: " << dwStatus << endl;
		CloseHandle(hFile);
		return string();
	}

	if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		dwStatus = GetLastError();
		cout << "CryptAcquireContext failed: " << dwStatus << endl;
		CloseHandle(hFile);
		CryptReleaseContext(hProv, 0);
		return string();
	}

	while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
		&cbRead, NULL))
	{
		if (0 == cbRead)
		{
			break;
		}

		if (!CryptHashData(hHash, rgbFile, cbRead, 0))
		{
			dwStatus = GetLastError();
			cout << "CryptHashData failed: " << dwStatus << endl;
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
			return string();
		}
	}

	if (!bResult)
	{
		dwStatus = GetLastError();
		cout << "ReadFile failed: " << dwStatus << endl;
		CryptReleaseContext(hProv, 0);
		CryptDestroyHash(hHash);
		CloseHandle(hFile);
		return string();
	}

	cbHash = MD5LEN;
	vector<CHAR> hash_str;
	if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
	{
		for (DWORD i = 0; i < cbHash; i++)
		{
			hash_str.push_back(rgbDigits[rgbHash[i] >> 4]);
			hash_str.push_back(rgbDigits[rgbHash[i] & 0xf]);
		}
	}
	else
	{
		dwStatus = GetLastError();
		cout << "CryptGetHashParam failed: " << dwStatus << endl;
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	CloseHandle(hFile);
	string str(hash_str.begin(), hash_str.end());
	return str;
}
