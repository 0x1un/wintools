#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <atlstr.h>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <winnetwk.h>

#include <comutil.h>
#include <atlbase.h>
#include <memory>
#include <codecvt>
#include <vector>
#include <Wbemidl.h>

using namespace std;

class NetConfig
{
public:
	//初始化
	bool init();
	NetConfig() {}
	NetConfig(const string& key)
		:key_(key) { }
	~NetConfig();

	//设置网络设备GUID
	void set_key(const string& key) {
		clear();
		key_ = key;
	}
	//启用DHCP
	bool enable_dhcp();
	//启动静态IP,设置IP,掩码,网关
	bool set_ip_config(const string& ip, const string& mask, const string& gateway);
	bool set_ip_config(const string& ip, const string& mask);
	//设置网关
	bool set_gateway(const string& gateway);
	//设置DNS地址
	bool set_dns(const string& default_dns, const string& backup_dns);
	//设置自动DNS
	bool set_auto_dns();

	int get_last_error_code()const {
		return last_error_code_;
	}

	void clear();

private:
	NetConfig(const NetConfig& rhs) = delete;
	NetConfig& operator = (const NetConfig& rhs) = delete;

private:
	//创建COM数组
	shared_ptr<SAFEARRAY> create_SAFEARRAY(const vector<string>& args);
	bool set_dns_base(bool is_auto, const string& default_dns, const string& backup_dns);
	bool exec_method(const wchar_t* method, IWbemClassObject* params_instance);

private:
	string key_;
	bool is_init_ = false;
	IWbemLocator* p_instance_ = nullptr;
	IWbemServices* p_service_ = nullptr;
	IEnumWbemClassObject* p_enum_ = nullptr;
	IWbemClassObject* p_obj_ = nullptr;
	IWbemClassObject* p_config = nullptr;
	VARIANT path_;
	int last_error_code_ = 0;
};


