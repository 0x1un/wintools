#include "Base.h"
#include "adapter.h"
#include "screen.h"
#include <iostream>

using namespace std;

Screen screen;

void set_resolution(int c) {
	int width = 0, height = 0;
	switch (c) {
	case 1:
		width = 1920;
		height = 1080;
		break;
	case 2:
		width = 1366;
		height = 768;
		break;
	case 3:
		width = 1024;
		height = 768;
		break;
	}
	screen.set_resolution(width, height);
}


void Base::run()
{
	system("chcp 65001 && cls");
	auto netcards_info = NetCardInfo::GetNetworkInfo();
	int i = 0;
	string cmd = "";
	while (true) {
		if (cmd == "q") {
			break;
		}
		if (cmd == "1") {
			cout << "设置IP、子网掩码、网关地址：" << endl;
			string str;
			getline(cin, str);
			vector<string> v;
			size_t pos = 0U, pos_2 = str.find(' ');
			while (string::npos != pos_2) {
				v.push_back(str.substr(pos, pos_2 - pos));
				pos = pos_2 + 1;
				pos_2 = str.find(' ', pos);
			}
			if (pos != str.length()) {
				v.push_back(str.substr(pos));
			}
			auto ip = v[0], mask = v[1];
			string gateway = "";
			if (v.size() == 3) gateway = v[2];
			if (gateway == "") gateway = "0.0.0.0";
			if (netcards_info[0]->SetIpConfig(ip, mask)) {
				cout << "IP与子网掩码设置成功！" << endl;
			}
			if (netcards_info[0]->SetGateway(gateway)) {
				cout << (gateway == "0.0.0.0" ? "清理" : "设置") << " 网关成功！" << endl;
			}
		}
		else if (cmd == "2") {
			cout << "请设置网关地址：" << endl;;
			string gateway;
			getline(cin, gateway);
			if (gateway == "") gateway = "0.0.0.0";
			if (netcards_info[0]->SetGateway(gateway)) {
				cout << (gateway == "0.0.0.0" ? "清理" : "设置") << " 网关成功！" << endl;
			}
		}
		else if (cmd == "3") {
			cout << "请输入DNS地址：" << endl;
			string dns, backup_dns = "";
			cin >> dns >> backup_dns;
			if (netcards_info[0]->SetDns(dns, backup_dns)) {
				cout << "DNS设置成功！" << endl;
			}
			else {
				netcards_info[0]->SetAutoDns();
			}
		}
		else if (cmd == "4") {
			if (netcards_info[0]->SetDns("223.5.5.5", "114.114.114.114")) {
				cout << "自动DNS设置成功！" << endl;
			}
			//if (netcards_info[0]->SetAutoDns()) {
				//cout << "auto dns is successful";
			//}
		}
		else if (cmd == "5") {
			for (auto& info : netcards_info) {
				info->show();
			}
		}
		else if (cmd == "6") {
			netcards_info = NetCardInfo::GetNetworkInfo();
			cout << "重新获取适配器信息中。。。" << endl;
			for (auto& info : netcards_info) {
				info->show();
			}
		}
		else if (cmd == "7") {
			int select_num;
			cout << "--------------------------" << endl;
			cout << "请选择一个分辨率进行设置，键入序号即可: " << endl;
			cout << "| 1. 1920x1080" << endl;
			cout << "| 2. 1366x768" << endl;
			cout << "| 3. 1024x768" << endl;
			cout << "--------------------------" << endl;
			cin >> select_num;
			set_resolution(select_num);
		}
		else if (cmd == "8") {
			if (netcards_info[0]->EnableDHCP()) {
				cout << "DHCP已设置完毕！" << endl;
			}
		}
		else if (cmd == "9") {
			cout << "请输入壁纸的完整路径(如: C:\\abc.jpg): ";
			wstring wall;
			wcin >> wall;
			if (wall.length() == 0) {
				continue;
			}
			const wchar_t* a = wall.c_str();
			screen.set_wallpaper(a);
		}
		else if (cmd == "10") {
			cout << "正在重启本地网卡..." << endl;

			int ret = system("powershell -command \"get-netadapter -Physical | restart-netadapter\"");
			if( ret < 0 ){
				cout << "重启网卡失败, code: " << ret << endl;
			}
			else {
				cout << "重启网卡成功！" << endl;
			}
		}
		else {
			cout << "------------------------------------" << endl;
			cout << "|  1. IP、掩码、网关设置           |" << endl;
			cout << "|  2. 默认网关设置                 |" << endl;
			cout << "|  3. 静态DNS模式设置              |" << endl;
			cout << "|  4. 自动DNS模式设置              |" << endl;
			cout << "|  5. 显示适配器信息               |" << endl;
			cout << "|  6. 刷新适配器信息               |" << endl;
			cout << "|  7. 设置屏幕分辨率               |" << endl;
			cout << "|  8. 设置动态DHCP                 |" << endl;
			cout << "|  9. 设置壁纸                     |" << endl;
			cout << "| 10. 重启本地网卡                 |" << endl;
			cout << "------------------------------------" << endl;
			cout << "input: ";
		}
		getline(cin, cmd);
	}
}

