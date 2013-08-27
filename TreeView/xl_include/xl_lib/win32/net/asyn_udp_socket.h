/*******************************************************************************
* 版权所有 (C)2010 深圳市迅雷网络技术有限公司。
* 系统名称	: 迅雷公共库
* 文件名称	: asyn_udp_socket.h
* 内容摘要	: 基于 windows 的窗口消息通知机制的异步 udp 客户端封装类。
              该类必须在单线程中使用，并且必须是包含 windows 窗口消息循环的线程。
              该类还对外屏蔽了 DNS 和 BIND 操作，init 之后就可以直接 send_data，使用方便。
              必须在使用之前调用过 WSAStartUp，否则无法使用。（因为 WSAStartUp 是进程内唯一的，所以只能由使用者来保证）
              另外，本类依赖的 windows 头文件基于某些考虑必须由用户在 include 之前指定：
              #include <Windows.h>
              #include <WinSock.h>
              #include <atlbase.h>
              #include <atlwin.h>
* 当前版本	: 1.0
* 作    者	: 黄文祥
* 设计日期	: 2010年12月16日
* 修改记录	: 
* 日    期		版    本		修改人		修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __ASYN_UDP_SOCKET_H_E1DC0E94_9844_401D_A8C7_4624372CA57A__
#define __ASYN_UDP_SOCKET_H_E1DC0E94_9844_401D_A8C7_4624372CA57A__

#include <map>
#include "xl_lib/type.h"
#include "xl_lib/tchar.h"
#include "xl_lib/error.h"
#include "xl_lib/win32/net/net_utility.h"


XL_WIN32_NAMESPACE_BEGIN(net)
/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/


class asyn_udp_socket_event
{
public:
	asyn_udp_socket_event(){}
	virtual ~asyn_udp_socket_event(){}
	/*******************************************************************************
	* 函数名称	: on_socket_send
	* 功能描述	: 数据发送成功事件。每一次调用 send，完全发送成功后，才会有该回调。
                  即一次 send 对应一次 on_socket_send。
	* 参　　数	: xl::uint32 send_bytes      本次发送的字节数。一定等同于 send_data 中要发送的字节数。
	* 返 回 值	: void 
	* 作　　者	: 黄文祥
	* 设计日期	: 2010年12月16日
	* 修改日期		   修改人		  修改内容
	*******************************************************************************/
	virtual void on_socket_send( xl::uint32 send_bytes ) = 0;
	/*******************************************************************************
	* 函数名称	: on_socket_recv
	* 功能描述	: asyn_udp_socket 在绑定套接字到本地地址后，即自动开始 recvfrom，只要接收到数据，就会激发该事件。                  
	* 参　　数	: const xl::byte* data_ptr			收到的数据缓冲区首地址
	* 参　　数	: xl::uint32 data_size				收到的数据总字节数（总共收到的 减去 用户取走的）
	* 参　　数	: xl::win32::net::ipv4				对方网络地址。
	* 参　　数	: xl::uint16 reomte_port			对方网络端口。本机序。
	* 返 回 值	: void 
	* 作　　者	: 黄文祥
	* 设计日期	: 2010年12月16日
	* 修改日期		   修改人		  修改内容
	*******************************************************************************/
	virtual void on_socket_recv( const xl::byte* data_ptr, xl::uint32 data_size, xl::win32::net::ipv4 remote_ip, xl::uint16 reomte_port ) = 0;
	/*******************************************************************************
	* 函数名称	: on_socket_error
	* 功能描述	: 设备出错，可能是绑定、发送、或接收等失败。
	* 参　　数	: xl::error_code err	错误码。
	* 返 回 值	: void 
	* 作　　者	: 黄文祥
	* 设计日期	: 2010年12月16日
	* 修改日期		   修改人		  修改内容
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
		// 测试发现这个类默认的处理中，会对 WM_QUERYENDSESSION 的响应不正确，
        // 导致Windows无法关机，所以只有自己实现响应
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
			// 窗口还没有销毁，切忌直接delete this，只是mark
			m_bFinalDel = true;

			if (!m_bOnDestroy)
			{
				DestroyWindow();
			}
		}
		else if (m_dwState & WINSTATE_DESTROYED)
		{
			// 窗口已经销毁，但是还没有到达最后一条消息
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
	bool m_bFinalDel;		// 是否需要在onfinalmessage里面删除自己
	bool m_bOnDestroy;		// 是否已经调用到DestroyWindow或者EndDialog
};




/*******************************************************************************
*备注：必须由使用者来初始化 Winsock。代码范例：
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
	* 函数名称	: init/uninit
	* 功能描述	: 初始化和反初始化。必须已经调用过 WSAStartUp，否则会失败。
                  必须在任何操作之前调用 init，重复调用 init 会返回 false。
                  init/uninit 必须成对调用，多次调用 uninit 没有关系。
	* 参　　数	: xl::uint16 local_port					一个本机端口。本机序,缺省值为 0,系统随机分配一个端口
	* 参　　数	: const xl::win32::net::ipv4 local_ip	一个本地ip地址。缺省值为 INADDR_ANY。
	* 返 回 值	: bool									初始化是否成功。
	* 作　　者	: 黄文祥
	* 设计日期	: 2010年12月16日
	* 修改日期		   修改人		  修改内容
	*******************************************************************************/
	bool init( xl::uint16 local_port = 0, const xl::win32::net::ipv4 local_ip = INADDR_ANY );
	void uninit();

	/*******************************************************************************
	* 函数名称	: get_local_address
	* 功能描述	: 获取与 SOCKET 绑定的本地ip地址和端口号，
				  必须在调用 init 后才能调用 get_local_address。
	* 参　　数	: xl::win32::net::ipv4 &ip_bound	绑定的本地ip地址。
	* 参　　数	: xl::uint16 &port_bound			绑定的端口号。
	* 返 回 值	: bool								获取成功时返回 true，否则返回 false。
	* 作　　者	: 黄文祥
	* 设计日期	: 2010年12月21日
	* 修改日期		   修改人		  修改内容
	*******************************************************************************/
	bool get_local_address( xl::win32::net::ipv4 &ip_bound, xl::uint16 &port_bound );

	/*******************************************************************************
	* 函数名称	: send_data
	* 功能描述	: 异步发送数据。调用后会立即返回，数据并未真正发送出去。
                  只有在收到 on_socket_send 事件之后，数据才真正发送成功。
                  一次 send_data 会引发一次 on_socket_send 事件。
                  只有收到 on_socket_send 事件后，才能再一次 send_data。
	* 参    数	: const std::string& remote_host	对方网络地址。可以是域名或者IP字符串。
	* 参	数  : xl::uint16 remote_port			对方网络端口。本机序。
	* 参　　数	: const xl::byte* data_ptr			要发送的数据缓冲区首地址。该缓冲区调用时有效即可。
	* 参　　数	: xl::uint32 data_size				要发送的数据缓冲区大小。大小不限。
	* 返 回 值	: bool								本次调用是否成功。一般只有在重复调用，或者参数非法的情况下，才会返回失败。
	* 作　　者	: 黄文祥
	* 设计日期	: 2010年12月16日
	* 修改日期		   修改人		  修改内容
	*******************************************************************************/
	bool send_data( const std::string& remote_host, xl::uint16 remote_port, const xl::byte* data_ptr, xl::uint32 data_size );
	
	/*******************************************************************************
	* 函数名称	: is_sending
	* 功能描述	: udp socket 是否在发送数据中。即判断前次 send_data 要发送的数据尚未真正发送结束。
	* 返 回 值	: bool				true 表示 udp socket 正在发送中。false 表示 udp socket 空闲中。
	* 作　　者	: 黄文祥
	* 设计日期	: 2010年12月16日
	* 修改日期		   修改人		  修改内容
	*******************************************************************************/
	bool is_sending();

	/*******************************************************************************	
	* 函数名称	: get_socket_handle
	* 功能描述	: 获取 SOCKET 句柄，在首次调用 send_data 之后就可以获取到该句柄。
	* 返 回 值	: SOCKET		返回 SOCKET 句柄。
	* 作　　者	: 黄文祥
	* 设计日期	: 2010年12月16日
	* 修改日期		   修改人		  修改内容
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
	
	std::string				m_remote_host;				// 对方网络地址
	xl::uint16				m_remote_port;				// 对方网络端口
	xl::win32::net::ipv4	m_remote_ip;

	xl::win32::net::ipv4	m_local_ip;
	xl::uint16				m_local_port;

	char					m_dns_result[MAXGETHOSTSTRUCT];
	HANDLE					m_dns_handle;

};

XL_WIN32_NAMESPACE_END(net);

#endif // end of __ASYN_UDP_SOCKET_H_E1DC0E94_9844_401D_A8C7_4624372CA57A__
