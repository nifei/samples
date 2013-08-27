/*******************************************************************************
* 版权所有 (C)2010 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : asyn_http_file.h
* 内容摘要  : 基于 asyn_http 实现的异步 HTTP 文件下载类。支持 Cache（NOT_MODIFIED） 机制，避免重复下载。
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
#ifndef __ASYN_HTTP_H_C922B218_069F_4A67_B3F8_38BCAA67F1C2__
#define __ASYN_HTTP_H_C922B218_069F_4A67_B3F8_38BCAA67F1C2__

#include "xl_lib/tchar.h"
#include "xl_lib/error.h"
#include "xl_lib/net/protocol/http.h"
#include "xl_lib/win32/net/asyn_http.h"
#include "xl_lib/design_pattern/singleton.h"
#include "xl_lib/config/config.h"

XL_WIN32_NAMESPACE_BEGIN(net)
/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

class asyn_http_file_event
{
public:
    asyn_http_file_event(){}
    virtual ~asyn_http_file_event(){}

    enum HTTP_FILE_STATUS
    {
        FILE_AVAILABLE_NEW_DOWNLOAD,    // 文件刚刚通过网络下载成功
        FILE_AVAILABLE_NOT_MODIFIED,    // 文件刚刚通过网络交互确定尚未过期依然有效
        FILE_AVAILABLE_MAY_EXPIRE,      // 文件存在，但是由于网络通信失败无法确定该文件是否过期
    };

    /*******************************************************************************
    * 函数名称  : on_asyn_http_file_ready
    * 功能描述  : 文件就绪事件。
                  文件可能刚刚下载到本地，或文件已存在并且查询服务器得知文件依然有效，
                  或文件已经存在但是与服务器通信失败所以文件可能有效也可能过期，需要上层斟酌使用。
    * 参　　数  : uint32 id                 调用 request_file 时传入的自定义 ID。
    * 参　　数  : const std::string& url    文件的 HTTP URL。
    * 参　　数  : const xl::tstring& file_full_path 文件下载到本地的保存路径。
    * 参　　数  : HTTP_FILE_STATUS status           文件的状态，见 HTTP_FILE_STATUS 的注释说明。
    * 返 回 值  : xl::error_code                    上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月4日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_file_ready( uint32 id, const std::string& url,
        const xl::tstring& file_full_path, HTTP_FILE_STATUS status ) = 0;
    
    /*******************************************************************************
    * 函数名称  : on_asyn_http_file_error
    * 功能描述  : 文件获取出错事件。有两种可能：
                  1.本地没有下载过该文件，并且在下载中出错，原因可能为网络故障或磁盘故障。
                  2.无论本地是否曾经下载过该文件，服务器返回结果说明服务器上并不存在该文件。
    * 参　　数  : uint32 id                 调用 request_file 时传入的自定义 ID。
    * 参　　数  : const std::string& url    文件的 HTTP URL。
    * 参　　数  : xl::error_code err        错误码。
    * 返 回 值  : xl::error_code            上层出现错误，事件后对象不可用 
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月4日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    virtual xl::error_code on_asyn_http_file_error( uint32 id, const std::string& url,
        xl::error_code err ) = 0;
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
class asyn_http_file : public xl::win32::net::asyn_http_event
{
public:
    asyn_http_file( asyn_http_file_event* event_ptr );
    virtual ~asyn_http_file();

    /*******************************************************************************
    * 函数名称  : request_file
    * 功能描述  : 请求一个 HTTP URL 对应的文件。
                  不能重复调用。在收到成功或失败事件之后，才再次调用 request_file。
                  file_full_path 表示文件下载到本地的保存绝对路径，如果该文件已经存在，
                  则内部将自动询问服务器该文件是否依然有效来避免重复下载。
    * 参　　数  : xl::uint32 id             用户自定义ID。
    * 参　　数  : const std::string& url    HTTP URL。
    * 参　　数  : const xl::net::protocol::http::HEADERS& request_headers   用户自定义 HTTP 头部信息，如不需要则设为空。
    * 参　　数  : const xl::tstring& file_full_path     文件下载到本地保存的绝对路径。
    * 返 回 值  : bool                      返回值。只有在参数非法，以及重复调用时才会返回 false。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月4日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    bool request_file( xl::uint32 id, const std::string& url, 
        const xl::net::protocol::http::HEADERS& request_headers, 
        const xl::tstring& file_full_path );

    /*******************************************************************************
    * 函数名称  : is_requesting
    * 功能描述  : 当前是否正在交互中。如果为 true 则表示不能调用 request_file。
    * 返 回 值  : bool      是否正在交互中。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月4日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    bool is_requesting() const;

    /*******************************************************************************
    * 函数名称  : get_downloaded_length
    * 功能描述  : 得到当前已接收的数据大小。
    * 返 回 值  : xl::uint64      已接收到的数据大小。
    * 作　　者  : 韦秋实
    * 设计日期  : 2012年8月1日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    xl::uint64 get_downloaded_length() const;

    /*******************************************************************************
    * 函数名称  : get_file_length
    * 功能描述  : 得到文件总大小。
    * 返 回 值  : xl::uint64      文件的总大小。
    * 由于asyn_http_file不支持断点续传，因此直接用了Context-Length做为文件的大小处理
    * 作　　者  : 韦秋实
    * 设计日期  : 2012年8月1日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    xl::uint64 get_file_length() const;
   
    /*******************************************************************************
    * 函数名称  : close
    * 功能描述  : 终止请求。可以重复调用。建议在程序退出前务必调用一次。
                  凡是调用过 request_file，在没有收到事件之前退出程序，则一定要调用 close。
    * 返 回 值  : void
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月4日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    void close();

    /*******************************************************************************
    * 函数名称  : set_recv_timeout
    * 功能描述  : 设置HTTP接收超时，如果在规定时间内没有收到任何数据，则触发错误995。
                  该方法可以在request_file之前或之后调用，都会生效。
    * 参　　数  : xl::uint32 millsec        超时时间，单位毫秒。UINT32_MAX 或者 0 表示不设置超时。
    * 返 回 值  : void                      一定返回true。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2011年4月27日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    void   set_recv_timeout(xl::uint32 millsec);

    // 获取HTTP头
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
