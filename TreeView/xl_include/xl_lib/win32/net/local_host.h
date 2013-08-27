/*******************************************************************************
* 版权所有 (C)2009 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : local_host.h
* 内容摘要  : 该类用于获取机器上网卡的相关配置信息，包括MAC地址，IP地址
* 当前版本  : 1.0
* 作    者  : 韦秋实
* 设计日期  : 2009年8月8日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __LOCAL_HOST_H_068EC4D7_D5B5_43FE_9E77_336067645578__
#define __LOCAL_HOST_H_068EC4D7_D5B5_43FE_9E77_336067645578__

#include <vector>
#include <string>
#include "xl_lib/type.h"
#include "xl_lib/tchar.h"
#include "xl_lib/text/transcode.h"
#include "xl_lib/win32/net/net_utility.h"

XL_WIN32_NAMESPACE_BEGIN(net)

/********************************** 常量和宏 **********************************/

typedef xl::uint32 ipv4;

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

template<class _char> class local_host_t;

typedef local_host_t<tchar> loacl_host;

template<class _char>
class local_host_t
{
public:
    typedef typename std::basic_string<_char> _string;

public:
    local_host_t();
    virtual ~local_host_t();

    // 提供给一般程序使用的简便方法
public:
    /*******************************************************************************
    * 函数名称  : get_default_ipaddr/get_default_ipaddr_mask
    * 功能描述  : 获取本机的缺省IP地址/获取本机的缺省IP地址掩码
                  实质上，由于机器可能有多个网卡，一个网卡可能有多个IP，该函数是为了
                  方便一般程序调用方便，取第一个网卡的当前IP地址。
    * 返 回 值  : _string             返回IP字符串，形如"192.168.1.101"。调用失败则返回空字符串。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    _string get_default_ipaddr();
    _string get_default_ipaddr_mask();

    /*******************************************************************************
    * 函数名称  : get_default_gateway/get_default_gateway_mask
    * 功能描述  : 获取本机的缺省网关地址/获取本机的缺省网关地址掩码
                  实质上，由于机器可能有多个网卡，一个网卡可能有多个IP，该函数是为了
                  方便一般程序调用方便，取第一个网卡的第一个网关地址。
    * 返 回 值  : _string             返回IP字符串，形如"192.168.1.1"。调用失败则返回空字符串。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    _string get_default_gateway();
    _string get_default_gateway_mask();

    // 提供给高级程序使用的复杂方法
public:
    /*******************************************************************************
    * 函数名称  : get_adapter_num
    * 功能描述  : 获得网卡的数量。一个机器可能有多个网卡。
    * 返 回 值  : uint32 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    uint32  get_adapter_num();

    /*******************************************************************************
    * 函数名称  : get_adapter_name
    * 功能描述  : 获取指定网卡的名称。
    * 参　　数  : xl::uint32 adapter_index  网卡序号。值范围 [ 0, get_adapter_num() )
    * 返 回 值  : _string             返回网卡名称。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    _string get_adapter_name(xl::uint32 adapter_index);

    /*******************************************************************************
    * 函数名称  : get_mac_addr
    * 功能描述  : 获取指定网卡的MAC地址。一个网卡只有一个MAC地址。
    * 参　　数  : xl::uint32 adapter_index  网卡序号。值范围 [ 0, get_adapter_num() )
    * 返 回 值  : _string             返回网卡MAC地址。形如：E4-1F-13-30-51-A2
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    _string get_mac_addr(xl::uint32 adapter_index);

    /*******************************************************************************
    * 函数名称  : get_netcard_description
    * 功能描述  : 获取指定网卡的描述信息。
    * 参　　数  : xl::uint32 adapter_index  网卡序号。值范围 [ 0, get_adapter_num() )
    * 返 回 值  : _string             返回网卡的描述信息。形如：Broadcom BCM5709C NetXtreme II GigE 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    _string get_netcard_description(xl::uint32 adapter_index);

    /*******************************************************************************
    * 函数名称  : get_ipaddr_num
    * 功能描述  : 获得指定网卡的IP数量。一个网卡可能有多个IP地址。
    * 参　　数  : xl::uint32 adapter_index  网卡序号。值范围 [ 0, get_adapter_num() )
    * 返 回 值  : uint32 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    uint32  get_ipaddr_num(xl::uint32 adapter_index);

    /*******************************************************************************
    * 结构体名称: ip_address_string
    * 功能描述  : 保存IP地址及其掩码。
    * 参　　数  : ip_addr   IP地址
    * 参　　数  : ip_mask   掩码
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    struct ip_address_string
    {
        _string ip_addr;
        _string ip_mask;
    };

    /*******************************************************************************
    * 函数名称  : get_ipaddr
    * 功能描述  : 获取指定网卡的指定IP地址。
    * 参　　数  : xl::uint32 adapter_index  网卡序号。值范围 [ 0, get_adapter_num() )
    * 参　　数  : xl::uint32 ipaddr_index   IP地址序号。值范围 [ 0, get_ipaddr_num(adapter_index) )
    * 返 回 值  : ip_address_string
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    ip_address_string get_ipaddr(xl::uint32 adapter_index, xl::uint32 ipaddr_index)
    {
        return adapter_index<m_info.size()?
            (ipaddr_index<m_info[adapter_index].ip_address.size()?m_info[adapter_index].ip_address[ipaddr_index]:ip_address_string())
            :ip_address_string();
    }

    /*******************************************************************************
    * 函数名称  : get_gateway_num
    * 功能描述  : 获取指定网卡的网关地址。一个网卡可能有多个网关地址。
    * 参　　数  : xl::uint32 adapter_index  网卡序号。值范围 [ 0, get_adapter_num() )
    * 返 回 值  : uint32 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    uint32  get_gateway_num(xl::uint32 adapter_index);

    /*******************************************************************************
    * 函数名称  : get_gateway_address
    * 功能描述  : 获取指定网卡的指定网关地址。
    * 参　　数  : xl::uint32 adapter_index  网卡序号。值范围 [ 0, get_adapter_num() )
    * 参　　数  : xl::uint32 gateway_index  网关序号。值范围 [ 0, get_gateway_num(adapter_index) )
    * 返 回 值  : ip_address_string
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    ip_address_string get_gateway_address(xl::uint32 adapter_index, xl::uint32 gateway_index)
    {
        return adapter_index<m_info.size()?
            (gateway_index<m_info[adapter_index].gate_way.size()?m_info[adapter_index].gate_way[gateway_index]:ip_address_string())
            :ip_address_string();
    }

    /*******************************************************************************
    * 函数名称  : is_local_ip_addr
    * 功能描述  : 判断IP是否是本机的IP。
    * 参　　数  : ipv4 ip_addr
    * 返 回 值  : bool 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年7月14日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    bool    is_local_ip_addr(ipv4 ip_addr);

protected:

    void init();

    struct adapter_info
    {
        _string physical_address;
        _string card_name;
        _string description;
        std::vector<ip_address_string> ip_address;
        std::vector<ip_address_string> gate_way;
    };

    std::vector<adapter_info> m_info;
};

template<class _char>
xl::win32::net::local_host_t<_char>::~local_host_t()
{
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_default_ipaddr()
{
    return m_info.empty()?std::basic_string<_char>()
        :(m_info[0].ip_address.empty()?std::basic_string<_char>():m_info[0].ip_address[0].ip_addr);
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_default_ipaddr_mask()
{
    return m_info.empty()?std::basic_string<_char>()
        :(m_info[0].ip_address.empty()?std::basic_string<_char>():m_info[0].ip_address[0].ip_mask);
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_default_gateway()
{
    return m_info.empty()?std::basic_string<_char>():
    (m_info[0].gate_way.empty()?std::basic_string<_char>():m_info[0].gate_way[0].ip_addr);
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_default_gateway_mask()
{
    return m_info.empty()?std::basic_string<_char>():
    (m_info[0].gate_way.empty()?std::basic_string<_char>():m_info[0].gate_way[0].ip_mask);
}

template<class _char>
uint32 local_host_t<_char>::get_adapter_num()
{
    return (xl::uint32)m_info.size();
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_adapter_name(xl::uint32 adapter_index)
{
    return adapter_index<m_info.size()?m_info[adapter_index].card_name:std::basic_string<_char>();
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_mac_addr(xl::uint32 adapter_index)
{
    return adapter_index<m_info.size()?m_info[adapter_index].physical_address:std::basic_string<_char>();
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_netcard_description(xl::uint32 adapter_index)
{
    return adapter_index<m_info.size()?m_info[adapter_index].description:std::basic_string<_char>();
}

template<class _char>
uint32  local_host_t<_char>::get_ipaddr_num(xl::uint32 adapter_index)
{
    return (xl::uint32)(adapter_index<m_info.size()?m_info[adapter_index].ip_address.size():0);
}

template<class _char>
uint32  local_host_t<_char>::get_gateway_num(xl::uint32 adapter_index)
{
    return (xl::uint32)(adapter_index<m_info.size()?m_info[adapter_index].gate_way.size():0);
}

template<class _char>
bool local_host_t<_char>::is_local_ip_addr(ipv4 ip_addr)
{
    _string addr = xl::win32::net::net_utility_t<_char>::ip_ntoa(ip_addr);
    for(uint32 x = 0; x < m_info.size(); x ++)
    {
        for(uint32 y = 0; y < m_info[x].ip_address.size(); x ++)
        {
            if(m_info[x].ip_address[y].ip_addr == addr)
            {
                return true;
            }
        }
    }

    return false;
}


XL_WIN32_NAMESPACE_END(net)

#endif // end of __LOCAL_HOST_H_068EC4D7_D5B5_43FE_9E77_336067645578__
