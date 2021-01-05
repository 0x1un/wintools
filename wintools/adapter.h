#pragma once
#include <WinSock2.h>
#include <atlbase.h>
#include <atlconv.h>
#include <iostream>
#include <string>
#include <vector>
#include <iphlpapi.h>

#include "netconfig.h"
#pragma comment(lib, "Iphlpapi.lib")
using namespace std;

class NetCardInfo {
public:
    NetCardInfo();

    struct Info {
        string adapter_name;      // 适配器的GUID
        string friendly_name;    // 适配器的名字
        string physical_addr;   // mac地址
        string ip;             // ip
        string ip_mask;       // 子网掩码
        string gateway;      // 网关
        string default_dns; // 默认dns
        string backup_dns; // 备选dns
    };

    static vector<shared_ptr<NetCardInfo>> GetNetworkInfo();

    const Info& GetNetCardInfo();

    void show();

    bool SetDns(const string& default_dns, const string& backup_dns);

    bool SetAutoDns(); // To clear the gateway,call this function.

    bool SetIpConfig(const string& ip, const string& ip_mask);

    // To clear the gateway,set your gateway to the same IP you use.
    bool SetGateway(const string& gateway);
    bool EnableDHCP();

protected:
    bool ParseInfo(PIP_ADAPTER_INFO adapter_info);

    bool ParseInfo(PIP_ADAPTER_ADDRESSES adapter_addresses);
private:
    Info info_; // 网卡信息
    shared_ptr<NetConfig> config_;
};
