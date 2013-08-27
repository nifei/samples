/*******************************************************************************
* 版权所有 (C)2010 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : asyn_http.h
* 内容摘要  : 基于 asyn_tcp_socket 实现的异步 HTTP 通信类。自动支持 HTTP 重定向。
              该类必须在单线程中使用，并且必须是包含 windows 窗口消息循环的线程。
              必须在使用之前调用过 WSAStartUp，否则无法使用。（因为 WSAStartUp 是进程内唯一的，所以只能由使用者来保证）
              另外，本类依赖的 windows 头文件基于某些考虑必须由用户在 include 之前指定：
              #include <Windows.h>
              #include <WinSock.h>
              #include <atlbase.h>
              #include <atlwin.h>
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2010年1月22日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __ASYN_HTTP_H_7CBB6C29_05B6_4FB7_90AA_B8A94E95E64F__
#define __ASYN_HTTP_H_7CBB6C29_05B6_4FB7_90AA_B8A94E95E64F__

#include "xl_lib/tchar.h"
#include "xl_lib/error.h"
#include "xl_lib/net/protocol/http.h"
#include "xl_lib/win32/net/asyn_tcp_socket.h"

XL_WIN32_NAMESPACE_BEGIN(net)
/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

class asyn_http_event
{
public:
    asyn_http_event(){}
    virtual ~asyn_http_event(){}

    /*******************************************************************************
    * 函数名称  : on_asyn_http_response
    * 功能描述  : 收到 HTTP 回复。
                  该类仅自动处理重定向，即使 HTTP 回复不是 200 状态码（例如 403），
                  也会作为正常的 HTTP 回复处理，通知该事件。
                  response.body 包含了对方回复的数据包体，如果是请求下载文件的 response，
                  body 的长度会很大。对长度很大的 body 操作会比较卡，请谨慎使用。
                  经过测试，将 13M 大的 body 写入文件，花费了 79ms，仅供参考。
    * 参　　数  : uint32 id  用户request时传入的自定义ID
    * 参　　数  : const xl::net::protocol::http::RESPONSE& response HTTP回复
    * 返 回 值  : xl::error_code                                    上层出现错误，事件后对象不可用  
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_response( uint32 id, const xl::net::protocol::http::RESPONSE& response ) = 0;
    
    /*******************************************************************************
    * 函数名称  : on_asyn_http_error
    * 功能描述  : 发生了网络错误，或者对方回复的数据不符合 HTTP 协议格式，或者重定向死循环。
                  如果 HTTP 回复不是 200 状态码（例如 403），也会作为正常的 HTTP 回复处理，
                  通知 on_asyn_http_response 事件而不是 on_asyn_http_error。
    * 参　　数  : uint32 id             用户request时传入的自定义ID
    * 参　　数  : xl::error_code err    错误码
    * 返 回 值  : xl::error_code                                    上层出现错误，事件后对象不可用  
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_error( uint32 id, xl::error_code err ) = 0;

    /*******************************************************************************
    * 函数名称  : on_asyn_http_response_piece
    * 功能描述  : 高级事件。该事件一般用户不必实现。只需要关注 on_asyn_http_response 即可。
                  该事件应用于下载体积较大的文件。在文件体积巨大的情况下，
                  如果将数据写入到 response.body 中，会导致内存占用过大。使用事件则可以将
                  body 数据直接写入到磁盘，而不需要写入到 response.body 中，避免占用内存。
    * 参　　数  : uint32 id     用户在 request 时传入的自定义ID。
    * 参　　数  : const xl::net::protocol::http::RESPONSE& response HTTP回复包头信息
    * 参　　数  : const xl::byte* body_piece                        HTTP回复BODY的数据片段
    * 参　　数  : xl::uint32 body_piece_size                        数据片段长度
    * 参　　数  : xl::uint64 body_piece_pos                         数据片段在整个BODY中的位置
    * 参　　数  : bool& append_piece_to_body                        上层决定是否将该数据写入到BODY中
    * 返 回 值  : xl::error_code                                    上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月2日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_response_piece( uint32 id, 
        const xl::net::protocol::http::RESPONSE& response,
        const xl::byte* body_piece, xl::uint32 body_piece_size, xl::uint64 body_piece_pos,
        bool& append_piece_to_body ) { /*该事件一般用户不需要实现*/ return NO_ERR;}

    /*******************************************************************************
    * 函数名称  : on_asyn_http_request_send_piece
    * 功能描述  : 高级事件。该事件一般用户不必实现。
                  该事件应用于HTTP在POST较大的文件时，能否告诉上层进度信息。
                  在发送HTTP的请求时，该事件会调用0次或多次，用于上层了解请求发送的进度情况。
                  注意，当请求发送成功时，sent_size会等于total_size。
    * 参　　数  : uint32 id     用户在 request 时传入的自定义ID。
    * 参　　数  : const xl::net::protocol::http::REQUEST& request HTTP回复包头信息
    * 参　　数  : xl::uint64 sent_size                              已经发送的请求包数据长度
    * 参　　数  : xl::uint64 total_size                             整个请求包的数据长度
    * 返 回 值  : xl::error_code                                    上层出现错误，事件后对象不可用  
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月2日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_request_send_piece( uint32 id, 
        const xl::net::protocol::http::REQUEST& request,
        xl::uint64 sent_size, xl::uint64 total_size ) { /*该事件一般用户不需要实现*/ return NO_ERR;}
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
class asyn_http : public asyn_tcp_socket_event
{
public:
    asyn_http( asyn_http_event* event_ptr );
    virtual ~asyn_http();

    /*******************************************************************************
    * 函数名称  : request
    * 功能描述  : 发送一个 HTTP 请求。异步调用，函数会立即返回。用户等待接收 asyn_http_event 事件。
                  在收到事件之前，不能重复提交 HTTP 请求。
                  必须在一次问答完成之后，才能进行下一次问答，即不支持一次性提交多个HTTP请求。
    * 参　　数  : uint32 id 用户自定义的ID，用于在事件中区分。不需要可以填0。
    * 参　　数  : const std::string& url 用户传入的 URL，函数内部会根据 URL 自动生成 HTTP 包头。
    * 参　　数  : const xl::net::protocol::http::HEADERS& headers 用户自定义的包头内容。
    * 参　　数  : const xl::net::protocol::http::REQUEST& http_request 用户完全自定义整个 HTTP 请求内容。
    * 返 回 值  : bool
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    bool request( uint32 id, const std::string& url );
    bool request( uint32 id, const std::string& url, const xl::net::protocol::http::HEADERS& headers );
    bool request( uint32 id, const xl::net::protocol::http::REQUEST& http_request );

    /*******************************************************************************
    * 函数名称  : set_recv_timeout
    * 功能描述  : 设置TCP接收超时，如果在规定时间内没有收到任何数据，则触发错误995。
                  该方法可以在request之前或之后调用，都会生效。
    * 参　　数  : xl::uint32 millsec        超时时间，单位毫秒。UINT32_MAX 或者 0 表示不设置超时。
    * 返 回 值  : void                      一定返回true。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2011年4月27日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    void   set_recv_timeout(xl::uint32 millsec);

    /*******************************************************************************
    * 函数名称  : is_requesting
    * 功能描述  : 判断当前 http 对象是否正在进行网络交互中。
                  当 is_requesting 为 true 的时候，不能调用 request。
    * 返 回 值  : bool      true 为是，false 为否。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月2日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    bool is_requesting() const;

    /*******************************************************************************
    * 函数名称  : close
    * 功能描述  : 关闭当前的 HTTP 请求连接，重置状态。
                  在 close 之后，前一次 HTTP 请求作废，可以立即再次 request。
                  如果调用过 request，在退出之前，一定要调用 close。多次调用 close 没有关系。
    * 返 回 值  : void
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月29日
    * 修改日期         修改人         修改内容
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
