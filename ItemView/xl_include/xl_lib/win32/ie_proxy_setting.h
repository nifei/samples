/*******************************************************************************
* 版权所有 (C)2009 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : ie_proxy_setting.h
* 内容摘要  : 一个读取IE代理信息的辅助类，需要链接 Wininet.lib
* 当前版本  : 1.0
* 作    者  : 来源于下载库，杨晓虎修订
* 设计日期  : 2009年3月26日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __IE_PROXY_SETTING_H_41FD5898_E8D8_4797_A6D0_954CC6107C95__
#define __IE_PROXY_SETTING_H_41FD5898_E8D8_4797_A6D0_954CC6107C95__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

XL_NAMESPACE_BEGIN(net)

struct proxy
{   
    enum type { NO_PROXY, SOCKS5, HTTP, FTP, DEFAULT = UINT32_MAX };

    proxy()
    {
        clear();
    }

    void clear()
    {
        m_type = NO_PROXY;
        m_host.erase();
        m_port = 0;
        m_user.erase();
        m_pass.erase();
    }

    type            m_type; // 代理类型
    tstring         m_host; // 服务器域名
    uint16          m_port; // 服务器端口
    tstring         m_user; // 用户名
    tstring         m_pass; // 密码
};

XL_NAMESPACE_END(net)

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

XL_NAMESPACE_BEGIN(win32)

class ie_proxy_setting  
{
public:
    ie_proxy_setting();
    virtual ~ie_proxy_setting();

    void    read_ie_setting();

    xl::net::proxy   get_http_proxy_info();
    xl::net::proxy   get_ftp_proxy_info();
    xl::net::proxy   get_https_proxy_info();
    xl::net::proxy   get_socks_proxy_info();

protected:

    xl::net::proxy   parse_string( const tstring& setting_str );

private:
    xl::net::proxy   m_http_setting;
    xl::net::proxy   m_ftp_setting;
    xl::net::proxy   m_https_setting;
    xl::net::proxy   m_socks_setting;
};

XL_NAMESPACE_END(win32)

#endif // end of __IE_PROXY_SETTING_H_41FD5898_E8D8_4797_A6D0_954CC6107C95__



