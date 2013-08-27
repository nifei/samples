/*******************************************************************************
* 版权所有 (C)2010 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : asyn_tcp_socket.h
* 内容摘要  : 基于 windows 的窗口消息通知机制的异步 tcp 客户端封装类。
              该类必须在单线程中使用，并且必须是包含 windows 窗口消息循环的线程。
              该类还对外屏蔽了 DNS 和 CONNECT 操作，init 之后就可以直接 send_data，使用方便。
              必须在使用之前调用过 WSAStartUp，否则无法使用。（因为 WSAStartUp 是进程内唯一的，所以只能由使用者来保证）
              另外，本类依赖的 windows 头文件基于某些考虑必须由用户在 include 之前指定：
              #include <Windows.h>
              #include <WinSock.h>
              #include <atlbase.h>
              #include <atlwin.h>
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2010年1月26日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __ASYN_SOCKET_H_8146CF2A_FADA_41B5_8A07_EC34E4330502__
#define __ASYN_SOCKET_H_8146CF2A_FADA_41B5_8A07_EC34E4330502__

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

class asyn_tcp_socket_event
{
public:
    asyn_tcp_socket_event(){}
    virtual ~asyn_tcp_socket_event(){}

    /*******************************************************************************
    * 函数名称  : on_socket_connect
    * 功能描述  : TCP 连接成功事件。在 TCP 连接成功之后激发该事件。
    * 参　　数  : xl::uint32 client_ip      TCP连接上本机的IPv4地址，网络序。
    * 参　　数  : xl::uint16 client_port    TCP连接上本机的端口，本机序。
    * 参　　数  : xl::uint32 server_ip      TCP连接上对方的IPv4地址，网络序。
    * 参　　数  : xl::uint16 server_port    TCP连接上对方的端口，本机序。
    * 返 回 值  : xl::error_code            上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_socket_connect( xl::uint32 client_ip, xl::uint16 client_port, xl::uint32 server_ip, xl::uint16 server_port ) = 0;

    /*******************************************************************************
    * 函数名称  : on_socket_send
    * 功能描述  : 数据发送成功事件。每一次调用 send，完全发送成功后，才会有该回调。
                  即一次 send 对应一次 on_socket_send。
    * 参　　数  : xl::uint32 send_bytes     本次发送的字节数。一定等同于 send_data 中要发送的字节数。
    * 返 回 值  : xl::error_code            上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_socket_send( xl::uint32 send_bytes ) = 0;

    /*******************************************************************************
    * 函数名称  : on_socket_send_piece
    * 功能描述  : 数据发送进度事件。当send调用后，每当发送了部分数据，都会触发该事件。
                  所以每一次调用 send，会触发0或多次该事件。（与send发送的数据量有关系）
                  注意，当发送完成时，不会再触发on_socket_send_piece，将直接触发on_socket_send。
                  即sent_bytes永远不可能等于total_bytes。
    * 参　　数  : xl::uint32 sent_bytes     已经发送的数据字节数
    * 参　　数  : xl::uint32 total_bytes    要发送的全部数据字节数，与调用send时的参数一致
    * 返 回 值  : xl::error_code            上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_socket_send_piece( xl::uint32 sent_bytes, xl::uint32 total_bytes ) { return NO_ERR; }

    /*******************************************************************************
    * 函数名称  : on_socket_recv
    * 功能描述  : 接收到数据事件。
                  当连接建立后，只要客户端接收到数据，就会 on_socket_recv。
                  所以不要想当然的以为，只有在send_data之后才会on_socket_recv，
                  也不要想当然的以为，一次send_data对应一次on_socket_recv。
                  asyn_tcp_socket 在连接建立后，即自动开始 recv，只要接收到数据，就会激发该事件。                  
                  接收到的数据会保存在一个缓冲区中，用户指定 taken_bytes 来从头取走。
                  如果用户不取走，那么下一次收到新数据再次通知时，数据仍然在缓冲区中。
                  例如：首次 on_socket_recv 通知收到了 "ABC"，如果用户返回的 taken_bytes 为0，
                        则tcp再收到 "DEF" 的时候，会 on_socket_recv 通知 "ABCDEF"，
                        如果用户返回 taken_byte 为 4，那么 tcp 会将 "ABCD" 从缓冲区中去掉。
                        当tcp再收到 "G" 的时候，会 on_socket_recv 通知 "EFG"，以此类推。
    * 参　　数  : const xl::byte* data_ptr  收到的数据缓冲区首地址
    * 参　　数  : xl::uint32 data_size      收到的数据总字节数（总共收到的 减去 用户取走的）
    * 参　　数  : xl::uint32* taken_bytes   用户指定要取走的字节数。
                                            缺省为 data_size，即缺省情况下假设数据都被用户取走。
    * 返 回 值  : xl::error_code            上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_socket_recv( const xl::byte* data_ptr, xl::uint32 data_size, xl::uint32* taken_bytes ) = 0;

    /*******************************************************************************
    * 函数名称  : on_socket_error
    * 功能描述  : 设备出错，可能是连接、发送、或接收等失败。
                  在收到 on_socket_error 事件后，asyn_tcp_socket 已经被 reset 过，
                  回到了 init 之后的干净状态，可以立即重新调用 send_data 方法。
    * 参　　数  : xl::error_code err        错误码。
    * 返 回 值  : xl::error_code            上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_socket_error( xl::error_code err ) = 0;

    /*******************************************************************************
    * 函数名称  : on_socket_gracefully_closed
    * 功能描述  : tcp连接被对方正常关闭。
                  总是发生在没有发送的，只有接收的时候，对方关闭了连接，则接收到该事件。
    * 返 回 值  : xl::error_code            上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
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
        m_owner = NULL; // 2011-3-8 避免Windows环境下有遗漏的消息造成未知后果
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
        // 测试发现这个类默认的处理中，会对 WM_QUERYENDSESSION 的响应不正确，
        // 导致Windows无法关机，所以只有自己实现响应
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
    bool    m_bFinalDel;        // 是否需要在onfinalmessage里面删除自己
    bool    m_bOnDestroy;       // 是否已经调用到DestroyWindow或者EndDialog
};



/*******************************************************************************
* 备注: 必须由使用者来初始化 Winsock。代码范例：

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
    * 函数名称  : init/uninit
    * 功能描述  : 初始化和反初始化。必须已经调用过 WSAStartUp，否则会失败。
                  必须在任何操作之前调用 init，重复调用 init 会返回 false。
                  init/uninit 必须成对调用，多次调用 uninit 没有关系。
                  注意！init中并不会connect服务器，只有在首次send_data时才会开始connect。
    * 参　　数  : const std::string& server_host    对方网络地址。可以是域名或者IP字符串。
    * 参　　数  : xl::uint16 server_port            对方网络端口。本机序。
    * 返 回 值  : bool                              初始化是否成功。
                                                    一般只有在重复调用，或者参数非法的情况下，才会返回失败。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    bool init( const std::string& server_host, xl::uint16 server_port );
    void uninit();

    /*******************************************************************************
    * 函数名称  : reset
    * 功能描述  : 关闭当前连接，并重置状态，回到 init 之后的干净状况。
                  一般用于需要关闭当前连接，重新连接的时候。
                  reset 之后就可以立即 send_data。
    * 返 回 值  : void
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    void reset();

    /*******************************************************************************
    * 函数名称  : send_data
    * 功能描述  : 异步发送数据。调用后会立即返回，数据并未真正发送出去。
                  只有在收到 on_socket_send 事件之后，数据才真正发送成功。
                  一次 send_data 会引发一次 on_socket_send 事件。
                  只有收到 on_socket_send 事件后，才能再一次 send_data。
    * 参　　数  : const xl::byte* data_ptr  要发送的数据缓冲区首地址。该缓冲区调用时有效即可。
    * 参　　数  : xl::uint32 data_size      要发送的数据缓冲区大小。大小不限。
    * 返 回 值  : bool                      本次调用是否成功。一般只有在重复调用，或者参数非法的情况下，才会返回失败。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    bool send_data( const xl::byte* data_ptr, xl::uint32 data_size );

    /*******************************************************************************
    * 函数名称  : is_sending
    * 功能描述  : tcp socket 是否在发送数据中。即判断前次 send_data 要发送的数据尚未真正发送结束。
    * 返 回 值  : bool              true 表示 tcp socket 正在发送中。false 表示 tcp socket 空闲中。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    bool is_sending();

    /*******************************************************************************
    * 函数名称  : get_socket_handle
    * 功能描述  : 获取 SOCKET 句柄，在首次调用 send_data 之后就可以获取到该句柄。
    * 返 回 值  : SOCKET        返回 SOCKET 句柄。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    SOCKET get_socket_handle();

    /*******************************************************************************
    * 函数名称  : set_recv_timeout
    * 功能描述  : 设置TCP接收超时，如果在规定时间内没有收到任何数据，则触发错误995。
                  该方法可以在任何时机调用，都会生效。
    * 参　　数  : xl::uint32 millsec        超时时间，单位毫秒。UINT32_MAX 或者 0 表示不设置超时。
    * 返 回 值  : void                      一定返回true。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2011年4月27日
    * 修改日期         修改人         修改内容
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
    //缓存DNS，会导致运行时一个域名永远只对应一个IP的问题
    //static std::map<std::string,xl::win32::net::ipv4>   s_dns_cache;
    //xl::win32::multithread::mutex                       s_dns_cache_lock;
};

XL_WIN32_NAMESPACE_END(net)

#endif // end of __ASYN_SOCKET_H_8146CF2A_FADA_41B5_8A07_EC34E4330502__
