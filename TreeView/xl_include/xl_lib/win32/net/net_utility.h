/*******************************************************************************
* 版权所有 (C)2009 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : net_utility.h
* 内容摘要  : 定义一组与网络信息获取相关的工具函数
* 当前版本  : 1.0
* 作    者  : 韦秋实
* 设计日期  : 2009年8月6日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __NET_UTILITY_H_83258B8F_0D10_40B7_B2AA_B0D16EBC4287__
#define __NET_UTILITY_H_83258B8F_0D10_40B7_B2AA_B0D16EBC4287__
#include <vector>
#include <string>

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_WIN32_NAMESPACE_BEGIN(net)
/********************************** 常量和宏 **********************************/

typedef xl::uint32 ipv4;
/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/


/*********************************** 类定义 ***********************************/
template<class _char> class net_utility_t;

typedef net_utility_t<tchar> net_utility;

template<class _char>
class net_utility_t
{
public:
    typedef std::basic_string<_char> _string;

    /*******************************************************************************
    * 函数名称	: socket_utility::is_loopback_addr
    * 功能描述	: 判断是否为回环ip地址
    * 参　　数	: ipv4 ip_addr		需要判断的ip地址，主机序
    * 返 回 值	: bool					true表示是回环ip，false表示不是回环ip
    * 作　　者	: 王卫华
    * 设计日期	: 2009年3月16日
    * 修改日期		   修改人		  修改内容
    *******************************************************************************/
    static bool is_loopback_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x000000ff) == 0x0000007f);
    }

    /*******************************************************************************
    * 函数名称	: socket_utility::is_multicast_addr
    * 功能描述	: 判断一个ip地址是不是组播ip地址
    * 参　　数	: ipv4 ip_addr		需要判断的ip地址，主机序
    * 返 回 值	: bool					true表示是组播ip，false表示不是组播ip
    * 作　　者	: 王卫华
    * 设计日期	: 2009年3月7日
    * 修改日期		   修改人		  修改内容
    *******************************************************************************/
    static bool is_multicast_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x000000f0) == 0x000000e0);
    }

    /*******************************************************************************
    * 函数名称	: socket_utility::is_badclass_addr
    * 功能描述	: 判断一个ip地址是不是非法ip地址
    * 参　　数	: ipv4 ip_addr		需要判断的ip地址，主机序
    * 返 回 值	: bool					true表示是非法ip，false表示不是非法ip
    * 作　　者	: 王卫华
    * 设计日期	: 2009年3月16日
    * 修改日期		   修改人		  修改内容
    *******************************************************************************/
    static bool is_badclass_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x000000f0) == 0x000000f0);
    }

    /*******************************************************************************
    * 函数名称	: socket_utility::is_zeronet_addr
    * 功能描述	: 判断一个ip地址是不是零网地址类
    * 参　　数	: ipv4 ip_addr		需要判断的ip地址，主机序
    * 返 回 值	: bool					true表示是零网地址类，false表示不是零网地址类
    * 作　　者	: 王卫华
    * 设计日期	: 2009年3月16日
    * 修改日期		   修改人		  修改内容
    *******************************************************************************/
    static bool is_zeronet_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x000000ff) == 0x00000000);
    }

    /*******************************************************************************
    * 函数名称	: socket_utility::is_localmcast_addr
    * 功能描述	: 判断一个ip地址是不是局域组播ip地址
    * 参　　数	: ipv4 ip_addr			需要判断的ip地址，主机序
    * 返 回 值	: bool						true表示是局域组播ip，false表示不是局域组播ip
    * 作　　者	: 王卫华
    * 设计日期	: 2009年3月16日
    * 修改日期		   修改人		  修改内容
    *******************************************************************************/
    static bool is_localmcast_addr(ipv4 ip_addr)
    {
        return ((ip_addr & 0x00FFFFFF) == 0x000000E0);
    }

    static ipv4 ip_aton(const _char* string_ip);

    static _string ip_ntoa(ipv4 ip_addr);

    static bool is_ip(const _char* string_ip);

    static _string resolve_to_address(const _char* domain_name);

    static void resolve_to_address(const _char* domain_name, std::vector<_string>& ip);

    static _string get_localhost_name();

};

XL_WIN32_NAMESPACE_END(net)
#endif // end of __NET_UTILITY_H_83258B8F_0D10_40B7_B2AA_B0D16EBC4287__


