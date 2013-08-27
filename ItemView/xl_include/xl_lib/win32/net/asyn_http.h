/*******************************************************************************
* ��Ȩ���� (C)2010 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : asyn_http.h
* ����ժҪ  : ���� asyn_tcp_socket ʵ�ֵ��첽 HTTP ͨ���ࡣ�Զ�֧�� HTTP �ض���
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
#ifndef __ASYN_HTTP_H_7CBB6C29_05B6_4FB7_90AA_B8A94E95E64F__
#define __ASYN_HTTP_H_7CBB6C29_05B6_4FB7_90AA_B8A94E95E64F__

#include "xl_lib/tchar.h"
#include "xl_lib/error.h"
#include "xl_lib/net/protocol/http.h"
#include "xl_lib/win32/net/asyn_tcp_socket.h"

XL_WIN32_NAMESPACE_BEGIN(net)
/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

class asyn_http_event
{
public:
    asyn_http_event(){}
    virtual ~asyn_http_event(){}

    /*******************************************************************************
    * ��������  : on_asyn_http_response
    * ��������  : �յ� HTTP �ظ���
                  ������Զ������ض��򣬼�ʹ HTTP �ظ����� 200 ״̬�루���� 403����
                  Ҳ����Ϊ������ HTTP �ظ�����֪ͨ���¼���
                  response.body �����˶Է��ظ������ݰ��壬��������������ļ��� response��
                  body �ĳ��Ȼ�ܴ󡣶Գ��Ⱥܴ�� body ������ȽϿ��������ʹ�á�
                  �������ԣ��� 13M ��� body д���ļ��������� 79ms�������ο���
    * �Ρ�����  : uint32 id  �û�requestʱ������Զ���ID
    * �Ρ�����  : const xl::net::protocol::http::RESPONSE& response HTTP�ظ�
    * �� �� ֵ  : xl::error_code                                    �ϲ���ִ����¼�����󲻿���  
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_response( uint32 id, const xl::net::protocol::http::RESPONSE& response ) = 0;
    
    /*******************************************************************************
    * ��������  : on_asyn_http_error
    * ��������  : ������������󣬻��߶Է��ظ������ݲ����� HTTP Э���ʽ�������ض�����ѭ����
                  ��� HTTP �ظ����� 200 ״̬�루���� 403����Ҳ����Ϊ������ HTTP �ظ�����
                  ֪ͨ on_asyn_http_response �¼������� on_asyn_http_error��
    * �Ρ�����  : uint32 id             �û�requestʱ������Զ���ID
    * �Ρ�����  : xl::error_code err    ������
    * �� �� ֵ  : xl::error_code                                    �ϲ���ִ����¼�����󲻿���  
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_error( uint32 id, xl::error_code err ) = 0;

    /*******************************************************************************
    * ��������  : on_asyn_http_response_piece
    * ��������  : �߼��¼������¼�һ���û�����ʵ�֡�ֻ��Ҫ��ע on_asyn_http_response ���ɡ�
                  ���¼�Ӧ������������ϴ���ļ������ļ�����޴������£�
                  ���������д�뵽 response.body �У��ᵼ���ڴ�ռ�ù���ʹ���¼�����Խ�
                  body ����ֱ��д�뵽���̣�������Ҫд�뵽 response.body �У�����ռ���ڴ档
    * �Ρ�����  : uint32 id     �û��� request ʱ������Զ���ID��
    * �Ρ�����  : const xl::net::protocol::http::RESPONSE& response HTTP�ظ���ͷ��Ϣ
    * �Ρ�����  : const xl::byte* body_piece                        HTTP�ظ�BODY������Ƭ��
    * �Ρ�����  : xl::uint32 body_piece_size                        ����Ƭ�γ���
    * �Ρ�����  : xl::uint64 body_piece_pos                         ����Ƭ��������BODY�е�λ��
    * �Ρ�����  : bool& append_piece_to_body                        �ϲ�����Ƿ񽫸�����д�뵽BODY��
    * �� �� ֵ  : xl::error_code                                    �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��2��2��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_response_piece( uint32 id, 
        const xl::net::protocol::http::RESPONSE& response,
        const xl::byte* body_piece, xl::uint32 body_piece_size, xl::uint64 body_piece_pos,
        bool& append_piece_to_body ) { /*���¼�һ���û�����Ҫʵ��*/ return NO_ERR;}

    /*******************************************************************************
    * ��������  : on_asyn_http_request_send_piece
    * ��������  : �߼��¼������¼�һ���û�����ʵ�֡�
                  ���¼�Ӧ����HTTP��POST�ϴ���ļ�ʱ���ܷ�����ϲ������Ϣ��
                  �ڷ���HTTP������ʱ�����¼������0�λ��Σ������ϲ��˽������͵Ľ��������
                  ע�⣬�������ͳɹ�ʱ��sent_size�����total_size��
    * �Ρ�����  : uint32 id     �û��� request ʱ������Զ���ID��
    * �Ρ�����  : const xl::net::protocol::http::REQUEST& request HTTP�ظ���ͷ��Ϣ
    * �Ρ�����  : xl::uint64 sent_size                              �Ѿ����͵���������ݳ���
    * �Ρ�����  : xl::uint64 total_size                             ��������������ݳ���
    * �� �� ֵ  : xl::error_code                                    �ϲ���ִ����¼�����󲻿���  
    * ��������  : ������
    * �������  : 2010��2��2��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_request_send_piece( uint32 id, 
        const xl::net::protocol::http::REQUEST& request,
        xl::uint64 sent_size, xl::uint64 total_size ) { /*���¼�һ���û�����Ҫʵ��*/ return NO_ERR;}
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
class asyn_http : public asyn_tcp_socket_event
{
public:
    asyn_http( asyn_http_event* event_ptr );
    virtual ~asyn_http();

    /*******************************************************************************
    * ��������  : request
    * ��������  : ����һ�� HTTP �����첽���ã��������������ء��û��ȴ����� asyn_http_event �¼���
                  ���յ��¼�֮ǰ�������ظ��ύ HTTP ����
                  ������һ���ʴ����֮�󣬲��ܽ�����һ���ʴ𣬼���֧��һ�����ύ���HTTP����
    * �Ρ�����  : uint32 id �û��Զ����ID���������¼������֡�����Ҫ������0��
    * �Ρ�����  : const std::string& url �û������ URL�������ڲ������ URL �Զ����� HTTP ��ͷ��
    * �Ρ�����  : const xl::net::protocol::http::HEADERS& headers �û��Զ���İ�ͷ���ݡ�
    * �Ρ�����  : const xl::net::protocol::http::REQUEST& http_request �û���ȫ�Զ������� HTTP �������ݡ�
    * �� �� ֵ  : bool
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    bool request( uint32 id, const std::string& url );
    bool request( uint32 id, const std::string& url, const xl::net::protocol::http::HEADERS& headers );
    bool request( uint32 id, const xl::net::protocol::http::REQUEST& http_request );

    /*******************************************************************************
    * ��������  : set_recv_timeout
    * ��������  : ����TCP���ճ�ʱ������ڹ涨ʱ����û���յ��κ����ݣ��򴥷�����995��
                  �÷���������request֮ǰ��֮����ã�������Ч��
    * �Ρ�����  : xl::uint32 millsec        ��ʱʱ�䣬��λ���롣UINT32_MAX ���� 0 ��ʾ�����ó�ʱ��
    * �� �� ֵ  : void                      һ������true��
    * ��������  : ������
    * �������  : 2011��4��27��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    void   set_recv_timeout(xl::uint32 millsec);

    /*******************************************************************************
    * ��������  : is_requesting
    * ��������  : �жϵ�ǰ http �����Ƿ����ڽ������罻���С�
                  �� is_requesting Ϊ true ��ʱ�򣬲��ܵ��� request��
    * �� �� ֵ  : bool      true Ϊ�ǣ�false Ϊ��
    * ��������  : ������
    * �������  : 2010��2��2��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    bool is_requesting() const;

    /*******************************************************************************
    * ��������  : close
    * ��������  : �رյ�ǰ�� HTTP �������ӣ�����״̬��
                  �� close ֮��ǰһ�� HTTP �������ϣ����������ٴ� request��
                  ������ù� request�����˳�֮ǰ��һ��Ҫ���� close����ε��� close û�й�ϵ��
    * �� �� ֵ  : void
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    void close();

    // asyn_tcp_socket_event
protected:
    virtual xl::error_code on_socket_connect( xl::uint32 client_ip, xl::uint16 client_port, xl::uint32 server_ip, xl::uint16 server_port );
    virtual xl::error_code on_socket_send( xl::uint32 send_bytes );
    virtual xl::error_code on_socket_send_piece( xl::uint32 sent_bytes, xl::uint32 total_bytes );
    virtual xl::error_code on_socket_recv( const xl::byte* data_ptr, xl::uint32 data_size, xl::uint32* taken_bytes );
    virtual xl::error_code on_socket_error( xl::error_code err );
    virtual xl::error_code on_socket_gracefully_closed();

protected:
    void reset();
    
    xl::error_code raise_response();
    xl::error_code raise_error( xl::error_code err );

    void append_body_data( const xl::byte* data_ptr, xl::uint32 data_size );

    bool set_server_by_host_field( const std::string& host_field );

protected:
    asyn_http_event*                    m_event_ptr;

    asyn_tcp_socket                     m_tcp_socket;
    xl::uint32                          m_recv_timeout;

    xl::uint32                          m_id;
    bool                                m_is_requesting;
    bool                                m_request_sent;
    bool                                m_response_recved;
    
    std::string                         m_server_host;
    xl::uint16                          m_server_port;

    xl::uint32                          m_redirect_time;

    xl::net::protocol::http::REQUEST    m_request;
    xl::net::protocol::http::RESPONSE*  m_response;
    xl::net::protocol::http::RESPONSE_PARSER*  m_response_parser;
};

XL_WIN32_NAMESPACE_END(net)

#endif // end of __ASYN_HTTP_H_7CBB6C29_05B6_4FB7_90AA_B8A94E95E64F__
