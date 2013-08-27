/*******************************************************************************
* ��Ȩ���� (C)2010 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : asyn_tcp_socket.h
* ����ժҪ  : ���� windows �Ĵ�����Ϣ֪ͨ���Ƶ��첽 tcp �ͻ��˷�װ�ࡣ
              ��������ڵ��߳���ʹ�ã����ұ����ǰ��� windows ������Ϣѭ�����̡߳�
              ���໹���������� DNS �� CONNECT ������init ֮��Ϳ���ֱ�� send_data��ʹ�÷��㡣
              ������ʹ��֮ǰ���ù� WSAStartUp�������޷�ʹ�á�����Ϊ WSAStartUp �ǽ�����Ψһ�ģ�����ֻ����ʹ��������֤��
              ���⣬���������� windows ͷ�ļ�����ĳЩ���Ǳ������û��� include ֮ǰָ����
              #include <Windows.h>
              #include <WinSock.h>
              #include <atlbase.h>
              #include <atlwin.h>
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2010��1��26��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __ASYN_SOCKET_H_8146CF2A_FADA_41B5_8A07_EC34E4330502__
#define __ASYN_SOCKET_H_8146CF2A_FADA_41B5_8A07_EC34E4330502__

#include <map>
#include "xl_lib/type.h"
#include "xl_lib/tchar.h"
#include "xl_lib/error.h"
#include "xl_lib/win32/net/net_utility.h"


XL_WIN32_NAMESPACE_BEGIN(net)
/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

class asyn_tcp_socket_event
{
public:
    asyn_tcp_socket_event(){}
    virtual ~asyn_tcp_socket_event(){}

    /*******************************************************************************
    * ��������  : on_socket_connect
    * ��������  : TCP ���ӳɹ��¼����� TCP ���ӳɹ�֮�󼤷����¼���
    * �Ρ�����  : xl::uint32 client_ip      TCP�����ϱ�����IPv4��ַ��������
    * �Ρ�����  : xl::uint16 client_port    TCP�����ϱ����Ķ˿ڣ�������
    * �Ρ�����  : xl::uint32 server_ip      TCP�����϶Է���IPv4��ַ��������
    * �Ρ�����  : xl::uint16 server_port    TCP�����϶Է��Ķ˿ڣ�������
    * �� �� ֵ  : xl::error_code            �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_socket_connect( xl::uint32 client_ip, xl::uint16 client_port, xl::uint32 server_ip, xl::uint16 server_port ) = 0;

    /*******************************************************************************
    * ��������  : on_socket_send
    * ��������  : ���ݷ��ͳɹ��¼���ÿһ�ε��� send����ȫ���ͳɹ��󣬲Ż��иûص���
                  ��һ�� send ��Ӧһ�� on_socket_send��
    * �Ρ�����  : xl::uint32 send_bytes     ���η��͵��ֽ�����һ����ͬ�� send_data ��Ҫ���͵��ֽ�����
    * �� �� ֵ  : xl::error_code            �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_socket_send( xl::uint32 send_bytes ) = 0;

    /*******************************************************************************
    * ��������  : on_socket_send_piece
    * ��������  : ���ݷ��ͽ����¼�����send���ú�ÿ�������˲������ݣ����ᴥ�����¼���
                  ����ÿһ�ε��� send���ᴥ��0���θ��¼�������send���͵��������й�ϵ��
                  ע�⣬���������ʱ�������ٴ���on_socket_send_piece����ֱ�Ӵ���on_socket_send��
                  ��sent_bytes��Զ�����ܵ���total_bytes��
    * �Ρ�����  : xl::uint32 sent_bytes     �Ѿ����͵������ֽ���
    * �Ρ�����  : xl::uint32 total_bytes    Ҫ���͵�ȫ�������ֽ����������sendʱ�Ĳ���һ��
    * �� �� ֵ  : xl::error_code            �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_socket_send_piece( xl::uint32 sent_bytes, xl::uint32 total_bytes ) { return NO_ERR; }

    /*******************************************************************************
    * ��������  : on_socket_recv
    * ��������  : ���յ������¼���
                  �����ӽ�����ֻҪ�ͻ��˽��յ����ݣ��ͻ� on_socket_recv��
                  ���Բ�Ҫ�뵱Ȼ����Ϊ��ֻ����send_data֮��Ż�on_socket_recv��
                  Ҳ��Ҫ�뵱Ȼ����Ϊ��һ��send_data��Ӧһ��on_socket_recv��
                  asyn_tcp_socket �����ӽ����󣬼��Զ���ʼ recv��ֻҪ���յ����ݣ��ͻἤ�����¼���                  
                  ���յ������ݻᱣ����һ���������У��û�ָ�� taken_bytes ����ͷȡ�ߡ�
                  ����û���ȡ�ߣ���ô��һ���յ��������ٴ�֪ͨʱ��������Ȼ�ڻ������С�
                  ���磺�״� on_socket_recv ֪ͨ�յ��� "ABC"������û����ص� taken_bytes Ϊ0��
                        ��tcp���յ� "DEF" ��ʱ�򣬻� on_socket_recv ֪ͨ "ABCDEF"��
                        ����û����� taken_byte Ϊ 4����ô tcp �Ὣ "ABCD" �ӻ�������ȥ����
                        ��tcp���յ� "G" ��ʱ�򣬻� on_socket_recv ֪ͨ "EFG"���Դ����ơ�
    * �Ρ�����  : const xl::byte* data_ptr  �յ������ݻ������׵�ַ
    * �Ρ�����  : xl::uint32 data_size      �յ����������ֽ������ܹ��յ��� ��ȥ �û�ȡ�ߵģ�
    * �Ρ�����  : xl::uint32* taken_bytes   �û�ָ��Ҫȡ�ߵ��ֽ�����
                                            ȱʡΪ data_size����ȱʡ����¼������ݶ����û�ȡ�ߡ�
    * �� �� ֵ  : xl::error_code            �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_socket_recv( const xl::byte* data_ptr, xl::uint32 data_size, xl::uint32* taken_bytes ) = 0;

    /*******************************************************************************
    * ��������  : on_socket_error
    * ��������  : �豸�������������ӡ����͡�����յ�ʧ�ܡ�
                  ���յ� on_socket_error �¼���asyn_tcp_socket �Ѿ��� reset ����
                  �ص��� init ֮��ĸɾ�״̬�������������µ��� send_data ������
    * �Ρ�����  : xl::error_code err        �����롣
    * �� �� ֵ  : xl::error_code            �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_socket_error( xl::error_code err ) = 0;

    /*******************************************************************************
    * ��������  : on_socket_gracefully_closed
    * ��������  : tcp���ӱ��Է������رա�
                  ���Ƿ�����û�з��͵ģ�ֻ�н��յ�ʱ�򣬶Է��ر������ӣ�����յ����¼���
    * �� �� ֵ  : xl::error_code            �ϲ���ִ����¼�����󲻿��� 
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    virtual xl::error_code on_socket_gracefully_closed() = 0;
};

class asyn_tcp_socket;

class safe_tcp_window
    : public ATL::CWindowImpl<safe_tcp_window>
{
private:

    virtual ~safe_tcp_window()
    {
        //assert(m_bOnDestroy);
        //assert(!m_bFinalDel);
        m_owner = NULL; // 2011-3-8 ����Windows����������©����Ϣ���δ֪���
    }

public:
    DECLARE_WND_CLASS(_T("asyn_tcp_socket"))

    enum CUSTOM_MSG
    {
        WM_SOCKET_EVENT = WM_USER+1,
        WM_DNS_EVENT    = WM_USER+2,
        WM_POST_ERROR   = WM_USER+3,
    };

    safe_tcp_window(asyn_tcp_socket* owner)
        :m_bFinalDel(false),
        m_bOnDestroy(false),
        m_owner(owner)
    {}

    BEGIN_MSG_MAP(safe_tcp_window)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        MESSAGE_HANDLER(WM_SOCKET_EVENT, OnSafeSocketEvent)
        MESSAGE_HANDLER(WM_DNS_EVENT, OnSafeDNSEvent)
        MESSAGE_HANDLER(WM_POST_ERROR, OnSafePostError)
        MESSAGE_HANDLER(WM_TIMER, OnSafeTimer)
        // ���Է��������Ĭ�ϵĴ����У���� WM_QUERYENDSESSION ����Ӧ����ȷ��
        // ����Windows�޷��ػ�������ֻ���Լ�ʵ����Ӧ
        MESSAGE_HANDLER(WM_QUERYENDSESSION, OnSafeQueryEndSession)
    END_MSG_MAP()

public:
    LRESULT OnSafeDNSEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSafeSocketEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSafePostError(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSafeQueryEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSafeTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
    LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
    virtual void OnFinalMessage(HWND);
    void safe_delete();

private:
    asyn_tcp_socket* m_owner;
    bool    m_bFinalDel;        // �Ƿ���Ҫ��onfinalmessage����ɾ���Լ�
    bool    m_bOnDestroy;       // �Ƿ��Ѿ����õ�DestroyWindow����EndDialog
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
class asyn_tcp_socket
{
public:
    asyn_tcp_socket( asyn_tcp_socket_event* event_ptr );
    virtual ~asyn_tcp_socket();

    /*******************************************************************************
    * ��������  : init/uninit
    * ��������  : ��ʼ���ͷ���ʼ���������Ѿ����ù� WSAStartUp�������ʧ�ܡ�
                  �������κβ���֮ǰ���� init���ظ����� init �᷵�� false��
                  init/uninit ����ɶԵ��ã���ε��� uninit û�й�ϵ��
                  ע�⣡init�в�����connect��������ֻ�����״�send_dataʱ�ŻῪʼconnect��
    * �Ρ�����  : const std::string& server_host    �Է������ַ����������������IP�ַ�����
    * �Ρ�����  : xl::uint16 server_port            �Է�����˿ڡ�������
    * �� �� ֵ  : bool                              ��ʼ���Ƿ�ɹ���
                                                    һ��ֻ�����ظ����ã����߲����Ƿ�������£��Ż᷵��ʧ�ܡ�
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    bool init( const std::string& server_host, xl::uint16 server_port );
    void uninit();

    /*******************************************************************************
    * ��������  : reset
    * ��������  : �رյ�ǰ���ӣ�������״̬���ص� init ֮��ĸɾ�״����
                  һ��������Ҫ�رյ�ǰ���ӣ��������ӵ�ʱ��
                  reset ֮��Ϳ������� send_data��
    * �� �� ֵ  : void
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    void reset();

    /*******************************************************************************
    * ��������  : send_data
    * ��������  : �첽�������ݡ����ú���������أ����ݲ�δ�������ͳ�ȥ��
                  ֻ�����յ� on_socket_send �¼�֮�����ݲ��������ͳɹ���
                  һ�� send_data ������һ�� on_socket_send �¼���
                  ֻ���յ� on_socket_send �¼��󣬲�����һ�� send_data��
    * �Ρ�����  : const xl::byte* data_ptr  Ҫ���͵����ݻ������׵�ַ���û���������ʱ��Ч���ɡ�
    * �Ρ�����  : xl::uint32 data_size      Ҫ���͵����ݻ�������С����С���ޡ�
    * �� �� ֵ  : bool                      ���ε����Ƿ�ɹ���һ��ֻ�����ظ����ã����߲����Ƿ�������£��Ż᷵��ʧ�ܡ�
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    bool send_data( const xl::byte* data_ptr, xl::uint32 data_size );

    /*******************************************************************************
    * ��������  : is_sending
    * ��������  : tcp socket �Ƿ��ڷ��������С����ж�ǰ�� send_data Ҫ���͵�������δ�������ͽ�����
    * �� �� ֵ  : bool              true ��ʾ tcp socket ���ڷ����С�false ��ʾ tcp socket �����С�
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    bool is_sending();

    /*******************************************************************************
    * ��������  : get_socket_handle
    * ��������  : ��ȡ SOCKET ��������״ε��� send_data ֮��Ϳ��Ի�ȡ���þ����
    * �� �� ֵ  : SOCKET        ���� SOCKET �����
    * ��������  : ������
    * �������  : 2010��1��29��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    SOCKET get_socket_handle();

    /*******************************************************************************
    * ��������  : set_recv_timeout
    * ��������  : ����TCP���ճ�ʱ������ڹ涨ʱ����û���յ��κ����ݣ��򴥷�����995��
                  �÷����������κ�ʱ�����ã�������Ч��
    * �Ρ�����  : xl::uint32 millsec        ��ʱʱ�䣬��λ���롣UINT32_MAX ���� 0 ��ʾ�����ó�ʱ��
    * �� �� ֵ  : void                      һ������true��
    * ��������  : ������
    * �������  : 2011��4��27��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    void   set_recv_timeout(xl::uint32 millsec);

public:
    LRESULT OnDNSEvent(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnSocketEvent(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnPostError(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnQueryEndSession(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
    LRESULT OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);

protected:
    enum STATUS
    {
        STATUS_NONE,
        STATUS_DNSING,
        STATUS_CONNECTING,
        STATUS_CONNECTED,
        STATUS_ERROR,
    };
    STATUS      m_status;

    void begin();
    void end();

    void dns();
    void connect();
    void send();

    void handle_connect();
    void handle_ready_to_send();
    void handle_ready_to_recv();
    void handle_close();

    xl::error_code raise_error( xl::uint32 err, bool post );

    void reset_timer( xl::uint32 recv_timeout );

protected:
    asyn_tcp_socket_event*  m_event_ptr;
    safe_tcp_window*        m_safewindow;
    SOCKET                  m_socket;

    std::string             m_send_data;
    xl::uint32              m_sent_size;
    std::string             m_recv_data;

    std::string             m_server_host;
    xl::uint16              m_server_port;
    xl::win32::net::ipv4    m_server_ip;

    char                    m_dns_result[MAXGETHOSTSTRUCT];
    HANDLE                  m_dns_handle;

    xl::uint32              m_recv_timeout;
    xl::uint32              m_recv_timerid;
    //����DNS���ᵼ������ʱһ��������Զֻ��Ӧһ��IP������
    //static std::map<std::string,xl::win32::net::ipv4>   s_dns_cache;
    //xl::win32::multithread::mutex                       s_dns_cache_lock;
};

XL_WIN32_NAMESPACE_END(net)

#endif // end of __ASYN_SOCKET_H_8146CF2A_FADA_41B5_8A07_EC34E4330502__
