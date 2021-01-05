#include "updator.h"
#include "utils.h"
#include < stdio.h>

using namespace std;

static LPCWSTR UpdateStr = _T("http://10.100.100.11:8000/wintools.exe");
static LPCWSTR StoreStr = _T("C:\\ProgramData\\wintools.exe");
static LPCWSTR CurrentStr = _T("C:\\wintools.exe");


void Updator::update()
{
	cout << "check updates..." << endl;
	if (!have_update())
	{
		cout << "no update" << endl;
		return;
	}
	cout << "update now..." << endl;
	// TODO: replace the binary file
	if (MoveFileEx(StoreStr, CurrentStr, MOVEFILE_REPLACE_EXISTING)) {
		cout << "update successful!" << endl;
		return;
	}
	cout << "update failed!" << endl;
	printf("Error: %d\n", GetLastError());
	return;
}


bool Updator::have_update()
{
	// download file into the "C:\ProgramData"
	if (!download_files(UpdateStr, StoreStr)) {
		return false;
	}
	string update_hash = util.md5sum_file(StoreStr);
	cout << "remote file hash: " << update_hash << endl;
	string local_hash = util.md5sum_file(CurrentStr);
	cout << "local file hash: " << local_hash << endl;
	if (update_hash == local_hash) {
#if _DEBUG
		cout << "no update" << endl;
#endif
		return false;
	}
	return true;
}

bool Updator::download_files(LPCWSTR url, LPCWSTR save_name)
{

	bool ret = false;
	HRESULT hr = URLDownloadToFile(0, url, save_name, 0, 0);
	switch (hr)
	{
	case S_OK:
#if _DEBUG
		cout << "Successful download\n";
#endif
		ret = true;
		break;
	case E_OUTOFMEMORY:
#if _DEBUG
		cout << "Out of memory error\n";
#endif
		break;
	case INET_E_DOWNLOAD_FAILURE:
#if _DEBUG
		cout << "Cannot access server data\n";
#endif
		break;
	default:
#if _DEBUG
		cout << "Unknown error\n";
#endif
		break;
	}
	return ret;
}
