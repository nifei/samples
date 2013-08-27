/*******************************************************************************
* ��Ȩ���� (C)2009 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : ie_proxy_setting.h
* ����ժҪ  : һ����ȡIE������Ϣ�ĸ����࣬��Ҫ���� Wininet.lib
* ��ǰ�汾  : 1.0
* ��    ��  : ��Դ�����ؿ⣬�������޶�
* �������  : 2009��3��26��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __IE_PROXY_SETTING_H_41FD5898_E8D8_4797_A6D0_954CC6107C95__
#define __IE_PROXY_SETTING_H_41FD5898_E8D8_4797_A6D0_954CC6107C95__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

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

    type            m_type; // ��������
    tstring         m_host; // ����������
    uint16          m_port; // �������˿�
    tstring         m_user; // �û���
    tstring         m_pass; // ����
};

XL_NAMESPACE_END(net)

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

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



