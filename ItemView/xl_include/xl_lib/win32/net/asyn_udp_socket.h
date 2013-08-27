/*******************************************************************************
* ��Ȩ���� (C)2010 ������Ѹ�����缼�����޹�˾��
* ϵͳ����	: Ѹ�׹�����
* �ļ�����	: asyn_udp_socket.h
* ����ժҪ	: ���� windows �Ĵ�����Ϣ֪ͨ���Ƶ��첽 udp �ͻ��˷�װ�ࡣ
              ��������ڵ��߳���ʹ�ã����ұ����ǰ��� windows ������Ϣѭ�����̡߳�
              ���໹���������� DNS �� BIND ������init ֮��Ϳ���ֱ�� send_data��ʹ�÷��㡣
              ������ʹ��֮ǰ���ù� WSAStartUp�������޷�ʹ�á�����Ϊ WSAStartUp �ǽ�����Ψһ�ģ�����ֻ����ʹ��������֤��
              ���⣬���������� windows ͷ�ļ�����ĳЩ���Ǳ������û��� include ֮ǰָ����
              #include <Windows.h>
              #include <WinSock.h>
              #include <atlbase.h>
              #include <atlwin.h>
* ��ǰ�汾	: 1.0
* ��    ��	: ������
* �������	: 2010��12��16��
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸���		�޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __ASYN_UDP_SOCKET_H_E1DC0E94_9844_401D_A8C7_4624372CA57A__
#define __ASYN_UDP_SOCKET_H_E1DC0E94_9844_401D_A8C7_4624372CA57A__

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


class asyn_udp_socket_event
{
public:
	asyn_udp_socket_event(){}
	virtual ~asyn_udp_socket_event(){}
	/*******************************************************************************
	* ��������	: on_socket_send
	* ��������	: ���ݷ��ͳɹ��¼���ÿһ�ε��� send����ȫ���ͳɹ��󣬲Ż��иûص���
                  ��һ�� send ��Ӧһ�� on_socket_send��
	* �Ρ�����	: xl::uint32 send_bytes      ���η��͵��ֽ�����һ����ͬ�� send_data ��Ҫ���͵��ֽ�����
	* �� �� ֵ	: void 
	* ��������	: ������
	* �������	: 2010��12��16��
	* �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	virtual void on_socket_send( xl::uint32 send_bytes ) = 0;
	/*******************************************************************************
	* ��������	: on_socket_recv
	* ��������	: asyn_udp_socket �ڰ��׽��ֵ����ص�ַ�󣬼��Զ���ʼ recvfrom��ֻҪ���յ����ݣ��ͻἤ�����¼���                  
	* �Ρ�����	: const xl::byte* data_ptr			�յ������ݻ������׵�ַ
	* �Ρ�����	: xl::uint32 data_size				�յ����������ֽ������ܹ��յ��� ��ȥ �û�ȡ�ߵģ�
	* �Ρ�����	: xl::win32::net::ipv4				�Է������ַ��
	* �Ρ�����	: xl::uint16 reomte_port			�Է�����˿ڡ�������
	* �� �� ֵ	: void 
	* ��������	: ������
	* �������	: 2010��12��16��
	* �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	virtual void on_socket_recv( const xl::byte* data_ptr, xl::uint32 data_size, xl::win32::net::ipv4 remote_ip, xl::uint16 reomte_port ) = 0;
	/*******************************************************************************
	* ��������	: on_socket_error
	* ��������	: �豸���������ǰ󶨡����͡�����յ�ʧ�ܡ�
	* �Ρ�����	: xl::error_code err	�����롣
	* �� �� ֵ	: void 
	* ��������	: ������
	* �������	: 2010��12��16��
	* �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	virtual void on_socket_error( xl::error_code err ) = 0;
};

class asyn_udp_socket;

class safe_udp_window
	:public ATL::CWindowImpl<safe_udp_window> 
{
private:
	
	virtual ~safe_udp_window()
	{
		//assert(m_bOnDestroy);
		//assert(!m_bFinalDel);
	}

public:

	DECLARE_WND_CLASS(_T("asyn_udp_socket"))

	enum CUSTOM_MSG
	{
		WM_SOCKET_EVENT = WM_USER + 1,
		WM_DNS_EVENT	= WM_USER + 2,
		WM_POST_ERROR	= WM_USER + 3,
	};

	safe_udp_window(asyn_udp_socket* owner)
		:m_bFinalDel(false),
		m_bOnDestroy(false),
		m_owner(owner)
	{}
	
	BEGIN_MSG_MAP(safe_udp_window)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SOCKET_EVENT, OnSafeSocketEvent)
		MESSAGE_HANDLER(WM_DNS_EVENT, OnSafeDNSEvent)
		MESSAGE_HANDLER(WM_POST_ERROR, OnSafePostError)
		// ���Է��������Ĭ�ϵĴ����У���� WM_QUERYENDSESSION ����Ӧ����ȷ��
        // ����Windows�޷��ػ�������ֻ���Լ�ʵ����Ӧ
		MESSAGE_HANDLER(WM_QUERYENDSESSION, OnSafeQueryEndSession)
	END_MSG_MAP()
	
public:
	LRESULT OnSafeDNSEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSafeSocketEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSafePostError(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSafeQueryEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:
	LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		bHandled = false;

		///assert(!m_bOnDestroy);
		m_bOnDestroy = true;

		return 0;
	}

	virtual void OnFinalMessage(HWND)
	{
		if (m_bFinalDel)
		{
			m_bFinalDel = false;
			delete(this);
		}
	}

	void safe_delete()
	{
		if (IsWindow())
		{
			// ���ڻ�û�����٣��м�ֱ��delete this��ֻ��mark
			m_bFinalDel = true;

			if (!m_bOnDestroy)
			{
				DestroyWindow();
			}
		}
		else if (m_dwState & WINSTATE_DESTROYED)
		{
			// �����Ѿ����٣����ǻ�û�е������һ����Ϣ
			m_bFinalDel = true;
		}
		else 
		{
			m_bFinalDel = false;
			delete(this);
		}
	}
	
private:
	asyn_udp_socket* m_owner;
	bool m_bFinalDel;		// �Ƿ���Ҫ��onfinalmessage����ɾ���Լ�
	bool m_bOnDestroy;		// �Ƿ��Ѿ����õ�DestroyWindow����EndDialog
};




/*******************************************************************************
*��ע��������ʹ��������ʼ�� Winsock�����뷶����
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

class asyn_udp_socket
{
public:
	asyn_udp_socket( asyn_udp_socket_event* event_ptr );
	virtual ~asyn_udp_socket();

	/*******************************************************************************
	* ��������	: init/uninit
	* ��������	: ��ʼ���ͷ���ʼ���������Ѿ����ù� WSAStartUp�������ʧ�ܡ�
                  �������κβ���֮ǰ���� init���ظ����� init �᷵�� false��
                  init/uninit ����ɶԵ��ã���ε��� uninit û�й�ϵ��
	* �Ρ�����	: xl::uint16 local_port					һ�������˿ڡ�������,ȱʡֵΪ 0,ϵͳ�������һ���˿�
	* �Ρ�����	: const xl::win32::net::ipv4 local_ip	һ������ip��ַ��ȱʡֵΪ INADDR_ANY��
	* �� �� ֵ	: bool									��ʼ���Ƿ�ɹ���
	* ��������	: ������
	* �������	: 2010��12��16��
	* �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	bool init( xl::uint16 local_port = 0, const xl::win32::net::ipv4 local_ip = INADDR_ANY );
	void uninit();

	/*******************************************************************************
	* ��������	: get_local_address
	* ��������	: ��ȡ�� SOCKET �󶨵ı���ip��ַ�Ͷ˿ںţ�
				  �����ڵ��� init ����ܵ��� get_local_address��
	* �Ρ�����	: xl::win32::net::ipv4 &ip_bound	�󶨵ı���ip��ַ��
	* �Ρ�����	: xl::uint16 &port_bound			�󶨵Ķ˿ںš�
	* �� �� ֵ	: bool								��ȡ�ɹ�ʱ���� true�����򷵻� false��
	* ��������	: ������
	* �������	: 2010��12��21��
	* �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	bool get_local_address( xl::win32::net::ipv4 &ip_bound, xl::uint16 &port_bound );

	/*******************************************************************************
	* ��������	: send_data
	* ��������	: �첽�������ݡ����ú���������أ����ݲ�δ�������ͳ�ȥ��
                  ֻ�����յ� on_socket_send �¼�֮�����ݲ��������ͳɹ���
                  һ�� send_data ������һ�� on_socket_send �¼���
                  ֻ���յ� on_socket_send �¼��󣬲�����һ�� send_data��
	* ��    ��	: const std::string& remote_host	�Է������ַ����������������IP�ַ�����
	* ��	��  : xl::uint16 remote_port			�Է�����˿ڡ�������
	* �Ρ�����	: const xl::byte* data_ptr			Ҫ���͵����ݻ������׵�ַ���û���������ʱ��Ч���ɡ�
	* �Ρ�����	: xl::uint32 data_size				Ҫ���͵����ݻ�������С����С���ޡ�
	* �� �� ֵ	: bool								���ε����Ƿ�ɹ���һ��ֻ�����ظ����ã����߲����Ƿ�������£��Ż᷵��ʧ�ܡ�
	* ��������	: ������
	* �������	: 2010��12��16��
	* �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	bool send_data( const std::string& remote_host, xl::uint16 remote_port, const xl::byte* data_ptr, xl::uint32 data_size );
	
	/*******************************************************************************
	* ��������	: is_sending
	* ��������	: udp socket �Ƿ��ڷ��������С����ж�ǰ�� send_data Ҫ���͵�������δ�������ͽ�����
	* �� �� ֵ	: bool				true ��ʾ udp socket ���ڷ����С�false ��ʾ udp socket �����С�
	* ��������	: ������
	* �������	: 2010��12��16��
	* �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	bool is_sending();

	/*******************************************************************************	
	* ��������	: get_socket_handle
	* ��������	: ��ȡ SOCKET ��������״ε��� send_data ֮��Ϳ��Ի�ȡ���þ����
	* �� �� ֵ	: SOCKET		���� SOCKET �����
	* ��������	: ������
	* �������	: 2010��12��16��
	* �޸�����		   �޸���		  �޸�����
	*******************************************************************************/
	SOCKET get_socket_handle();
	
public:
	LRESULT OnDNSEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSocketEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPostError(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnQueryEndSession(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	enum STATUS
	{
		STATUS_NONE,
		STATUS_BOUND,
		STATUS_ERROR,
	};
	STATUS m_status;

	void begin();
	void end();

	void dns();
	void bind( const xl::win32::net::ipv4& local_ip, xl::uint16 local_port );
	void send();

	void handle_ready_to_send();
	void handle_ready_to_recv();

	void raise_error( xl::uint32 err, bool post );

protected:
	asyn_udp_socket_event*	m_event_ptr;
	safe_udp_window*		m_safewindow;
	SOCKET					m_socket;

	std::string				m_send_data;
	
	std::string				m_remote_host;				// �Է������ַ
	xl::uint16				m_remote_port;				// �Է�����˿�
	xl::win32::net::ipv4	m_remote_ip;

	xl::win32::net::ipv4	m_local_ip;
	xl::uint16				m_local_port;

	char					m_dns_result[MAXGETHOSTSTRUCT];
	HANDLE					m_dns_handle;

};

XL_WIN32_NAMESPACE_END(net);

#endif // end of __ASYN_UDP_SOCKET_H_E1DC0E94_9844_401D_A8C7_4624372CA57A__
