#include "screen.h"

using namespace std;

// 获取屏幕分辨率
Resolution Screen::get_resolution() {
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	Resolution ret_value;
	ret_value.width = desktop.right;
	ret_value.height = desktop.bottom;
	return ret_value;
}


void Screen::set_wallpaper(const wchar_t* wallpaper_path)
{
	bool res = SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)wallpaper_path, SPIF_UPDATEINIFILE);
	if (!res) {
		cout << "修改壁纸遇到错误: " << GetLastError() << endl;
		return;
	}
	cout << "成功应用壁纸!"  << endl;
	return;
}

// 设置屏幕分辨率
void Screen::set_resolution(int width, int height) {
	if (width < 800 || height < 600) {
		cout << "not support resolution!" << endl;
		return;
	}
	DEVMODE lpDevMode{};
	if (0 != EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode)) {
		int savew = lpDevMode.dmPelsWidth;
		int saveh = lpDevMode.dmPelsHeight;

		lpDevMode.dmPelsWidth = width;
		lpDevMode.dmPelsHeight = height;
		lpDevMode.dmBitsPerPel = 32;
		lpDevMode.dmDisplayFrequency = 60;
		LONG ret_code = ChangeDisplaySettings(&lpDevMode, CDS_UPDATEREGISTRY);
		switch (ret_code) {
		case DISP_CHANGE_SUCCESSFUL:
			cout << "The display settings were successfully changed." << endl;
			break;
		case DISP_CHANGE_RESTART:;
			cout << "The computer must be restarted for the display changes to take effect." << endl;
			break;
		case DISP_CHANGE_FAILED:
			cout << "The display driver failed the specified graphics mode." << endl;
			break;
		case DISP_CHANGE_BADMODE:
			cout << "The specified graphics mode is not supported." << endl;

			break;
		case DISP_CHANGE_NOTUPDATED:
			cout << "Windows NT/2000: The settings could not be written to the registry." << endl;
			break;
		case DISP_CHANGE_BADFLAGS:
			cout << "An invalid set of flags was specified." << endl;
			break;
		case DISP_CHANGE_BADPARAM:
			cout << "An invalid parameter was specified." << endl;
			break;
		}

	}
}
