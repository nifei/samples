/*******************************************************************************
* ��Ȩ���� (C)2010 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : asyn_http_file.h
* ����ժҪ  : ���� asyn_http ʵ�ֵ��첽 HTTP �ļ������ࡣ֧�� Cache��NOT_MODIFIED�� ���ƣ������ظ����ء�
              ��������ڵ��߳���ʹ�ã����ұ����ǰ��� windows ������Ϣѭ�����̡߳�
              ������ʹ��֮ǰ���ù� WSAStartUp�������޷�ʹ�á�����Ϊ WSAStartUp �ǽ�����Ψһ�ģ�����ֻ����ʹ��������֤��
              ���⣬���������� windows ͷ�ļ�����ĳЩ���Ǳ������û��� include ֮ǰָ����
              #include <Windows.h>
              #include <WinSock.h>
              #include <atlbase.h>
              #include <atlwin.h>
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2010��1��22��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __ASYN_HTTP_H_C922B218_069F_4A67_B3F8_38BCAA67F1C2__
#define __ASYN_HTTP_H_C922B218_069F_4A67_B3F8_38BCAA67F1C2__

#include "xl_lib/tchar.h"
#include "xl_lib/error.h"
#include "xl_lib/net/protocol/http.h"
#include "xl_lib/win32/net/asyn_http.h"
#include "xl_lib/design_pattern/singleton.h"
#include "xl_lib/config/config.h"

XL_WIN32_NAMESPACE_BEGIN(net)
/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

class asyn_http_file_event
{
public:
    asyn_http_file_event(){}
    virtual ~asyn_http_file_event(){}

    enum HTTP_FILE_STATUS
    {
        FILE_AVAILABLE_NEW_DOWNLOAD,    // �ļ��ո�ͨ���������سɹ�
        FILE_AVAILABLE_NOT_MODIFIED,    // �ļ��ո�ͨ�����罻��ȷ����δ������Ȼ��Ч
        FILE_AVAILABLE_MAY_EXPIRE,      // �ļ����ڣ�������������ͨ��ʧ���޷�ȷ�����ļ��Ƿ����
    };

    /*******************************************************************************
    * ��������  : on_asyn_http_file_ready
    * ��������  : �ļ������¼���
                  �ļ����ܸո����ص����أ����ļ��Ѵ��ڲ��Ҳ�ѯ��������֪�ļ���Ȼ��Ч��
                  ���ļ��Ѿ����ڵ����������ͨ��ʧ�������ļ�������ЧҲ���ܹ��ڣ���Ҫ�ϲ�����ʹ�á�
    * �Ρ�����  : uint32 id                 ���� request_file ʱ������Զ��� ID��
    * �Ρ�����  : const std::string& url    �ļ��� HTTP URL��
    * �Ρ�����  : const xl::tstring& file_full_path �ļ����ص����صı���·����
    * �Ρ�����  : HTTP_FILE_STATUS status           �ļ���״̬���� HTTP_FILE_STATUS ��ע��˵����
    * �� �� ֵ  : xl::error_code                    �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��2��4��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_file_ready( uint32 id, const std::string& url,
        const xl::tstring& file_full_path, HTTP_FILE_STATUS status ) = 0;
    
    /*******************************************************************************
    * ��������  : on_asyn_http_file_error
    * ��������  : �ļ���ȡ�����¼��������ֿ��ܣ�
                  1.����û�����ع����ļ��������������г���ԭ�����Ϊ������ϻ���̹��ϡ�
                  2.���۱����Ƿ��������ع����ļ������������ؽ��˵���������ϲ������ڸ��ļ���
    * �Ρ�����  : uint32 id                 ���� request_file ʱ������Զ��� ID��
    * �Ρ�����  : const std::string& url    �ļ��� HTTP URL��
    * �Ρ�����  : xl::error_code err        �����롣
    * �� �� ֵ  : xl::error_code            �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��2��4��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_file_error( uint32 id, const std::string& url,
        xl::error_code err ) = 0;
};

/*******************************************************************************
* ��ע: ������ʹ��������ʼ�� Winsock�����뷶����

#include <Windows.h>
#include <WinSock.h>
#include <atlbase.h>
#include <atlwin.h>

WORD wVersionRequested;
WSADATA wsaData;
int err;

wVersionRequested = MAKEWORD( 2, 2 );

err = WSAStartup( wVersionRequested, &wsaData );
if ( err != 0 ) 
{
return;
}

...

WSACleanup();
*******************************************************************************/
class asyn_http_file : public xl::win32::net::asyn_http_event
{
public:
    asyn_http_file( asyn_http_file_event* event_ptr );
    virtual ~asyn_http_file();

    /*******************************************************************************
    * ��������  : request_file
    * ��������  : ����һ�� HTTP URL ��Ӧ���ļ���
                  �����ظ����á����յ��ɹ���ʧ���¼�֮�󣬲��ٴε��� request_file��
                  file_full_path ��ʾ�ļ����ص����صı������·����������ļ��Ѿ����ڣ�
                  ���ڲ����Զ�ѯ�ʷ��������ļ��Ƿ���Ȼ��Ч�������ظ����ء�
    * �Ρ�����  : xl::uint32 id             �û��Զ���ID��
    * �Ρ�����  : const std::string& url    HTTP URL��
    * �Ρ�����  : const xl::net::protocol::http::HEADERS& request_headers   �û��Զ��� HTTP ͷ����Ϣ���粻��Ҫ����Ϊ�ա�
    * �Ρ�����  : const xl::tstring& file_full_path     �ļ����ص����ر���ľ���·����
    * �� �� ֵ  : bool                      ����ֵ��ֻ���ڲ����Ƿ����Լ��ظ�����ʱ�Ż᷵�� false��
    * ��������  : ������
    * �������  : 2010��2��4��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    bool request_file( xl::uint32 id, const std::string& url, 
        const xl::net::protocol::http::HEADERS& request_headers, 
        const xl::tstring& file_full_path );

    /*******************************************************************************
    * ��������  : is_requesting
    * ��������  : ��ǰ�Ƿ����ڽ����С����Ϊ true ���ʾ���ܵ��� request_file��
    * �� �� ֵ  : bool      �Ƿ����ڽ����С�
    * ��������  : ������
    * �������  : 2010��2��4��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    bool is_requesting() const;

    /*******************************************************************************
    * ��������  : get_downloaded_length
    * ��������  : �õ���ǰ�ѽ��յ����ݴ�С��
    * �� �� ֵ  : xl::uint64      �ѽ��յ������ݴ�С��
    * ��������  : Τ��ʵ
    * �������  : 2012��8��1��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    xl::uint64 get_downloaded_length() const;

    /*******************************************************************************
    * ��������  : get_file_length
    * ��������  : �õ��ļ��ܴ�С��
    * �� �� ֵ  : xl::uint64      �ļ����ܴ�С��
    * ����asyn_http_file��֧�ֶϵ����������ֱ������Context-Length��Ϊ�ļ��Ĵ�С����
    * ��������  : Τ��ʵ
    * �������  : 2012��8��1��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    xl::uint64 get_file_length() const;
   
    /*******************************************************************************
    * ��������  : close
    * ��������  : ��ֹ���󡣿����ظ����á������ڳ����˳�ǰ��ص���һ�Ρ�
                  ���ǵ��ù� request_file����û���յ��¼�֮ǰ�˳�������һ��Ҫ���� close��
    * �� �� ֵ  : void
    * ��������  : ������
    * �������  : 2010��2��4��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    void close();

    /*******************************************************************************
    * ��������  : set_recv_timeout
    * ��������  : ����HTTP���ճ�ʱ������ڹ涨ʱ����û���յ��κ����ݣ��򴥷�����995��
                  �÷���������request_file֮ǰ��֮����ã�������Ч��
    * �Ρ�����  : xl::uint32 millsec        ��ʱʱ�䣬��λ���롣UINT32_MAX ���� 0 ��ʾ�����ó�ʱ��
    * �� �� ֵ  : void                      һ������true��
    * ��������  : ������
    * �������  : 2011��4��27��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    void   set_recv_timeout(xl::uint32 millsec);

    // ��ȡHTTPͷ
	std::string get_response_header(const std::string &strfield_name)
	{
		return m_response_header.get_header(strfield_name);
	}
protected:
    virtual xl::error_code on_asyn_http_response( uint32 id, const xl::net::protocol::http::RESPONSE& response );
    virtual xl::error_code on_asyn_http_error( uint32 id, xl::error_code err );
    virtual xl::error_code on_asyn_http_response_piece( uint32 id, const xl::net::protocol::http::RESPONSE& response,
        const xl::byte* body_piece, xl::uint32 body_piece_size, xl::uint64 body_piece_pos, bool& append_piece_to_body );

protected:
    xl::error_code raise_error( xl::uint32 err );
    xl::error_code raise_ready( asyn_http_file_event::HTTP_FILE_STATUS status );

    std::string get_file_last_modified_gmt_format_time( const xl::tstring& file_full_path );
    xl::tstring get_download_tmp_file_path( const xl::tstring& file_full_path );

	void set_file_last_modified_time( const std::string& time_str, const xl::tstring& file_full_path );

protected:
    asyn_http_file_event*   m_event_ptr;

    asyn_http               m_http;
    xl::uint32              m_id;
    std::string             m_url;

    xl::tstring             m_file_full_path;
    xl::tstring             m_temp_file_path;
    HANDLE                  m_file_handle;
    xl::uint64              m_recv_length;
    xl::uint64              m_file_length;
	xl::net::protocol::http::HEADERS m_response_header;
};

/*
class asyn_http_file_cfg
    : public xl::design_pattern::singleton_ex<asyn_http_file_cfg>
{
public:
    asyn_http_file_cfg();
    virtual ~asyn_http_file_cfg();

    //void       set_file_expire_time( const xl::tstring& file_guid, xl::uint32 expire_time );
    //xl::uint32 get_file_expire_time( const xl::tstring& file_guid );
    
    static xl::tstring get_file_full_path( const xl::tstring& file_name );

protected:
    //xl::tstring         m_cfg_file_path;
    //xl::config::config  m_config;
};
*/

XL_WIN32_NAMESPACE_END(net)

#endif // end of __ASYN_HTTP_H_C922B218_069F_4A67_B3F8_38BCAA67F1C2__
