/*******************************************************************************
* 版权所有 (C)2010 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : synch_utility.h
* 内容摘要  : 提供了一些网络同步操作的实用函数，包括HTTP下载，P2SP下载等。
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2010年1月4日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __SYNCH_UTILITY_H_548B141E_AA0F_4DFF_A9EE_7B45C883E798__
#define __SYNCH_UTILITY_H_548B141E_AA0F_4DFF_A9EE_7B45C883E798__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"
#include "xl_lib/error.h"
#include "xl_lib/net/protocol/http.h"

XL_WIN32_NAMESPACE_BEGIN(net)

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

class synch_utility
{
public:

    /*******************************************************************************
    * 函数名称  : static xl::error_code http_download( const xl::tstring& url, const xl::tstring& save_path )
    * 功能描述  : 同步下载 HTTP 文件，内部使用 WinInet API 实现。
                  注意，工程需要链接 Wininet.lib。
    * 参　　数  : const xl::tstring& url                HTTP URL
    * 参　　数  : const xl::tstring& save_dir           文件下载到本地的目录名，必须是绝对路径
    * 参　　数  : const xl::tstring& save_file_name     文件下载到本地的文件名
    * 参　　数  : HANDLE cancel_event                   中途取消调用的事件（EVENT）。
                                                        由于是同步调用，所以程序应该传入一个 Event（例如由CreateEvent得到）
                                                        在程序需要退出时，SetEvent，然后该函数将终止执行并返回 ERROR_OPERATION_ABORTED（995） 
    * 返 回 值  : xl::error_code                        NO_ERR 表示成功。否则为失败码。ERROR_OPERATION_ABORTED（995）表示同步操作被取消。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月4日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    static xl::error_code http_download( const xl::tstring& url, const xl::tstring& save_dir, 
        const xl::tstring& save_file_name, HANDLE cancel_event = NULL );


    /*******************************************************************************
    * 函数名称  : http_request
    * 功能描述  : 同步调用实现 HTTP 问答，内部使用 WinInet API 实现。
                  注意，工程需要链接 Wininet.lib。
    * 参　　数  : const xl::net::protocol::http::REQUEST& http_request  HTTP 请求
    * 参　　数  : xl::net::protocol::http::RESPONSE& http_response      HTTP 回复
    * 参　　数  : HANDLE cancel_event = NULL                            中途取消调用的事件（EVENT）。
    * 返 回 值  : xl::error_code                                        NO_ERR 表示成功。否则为失败码。ERROR_OPERATION_ABORTED（995）表示同步操作被取消。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年2月1日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    static xl::error_code http_request( const xl::net::protocol::http::REQUEST& http_request,
        xl::net::protocol::http::RESPONSE& http_response,
        HANDLE cancel_event = NULL );

    /*******************************************************************************
    * 函数名称  : p2sp_download
    * 功能描述  : 同步使用 P2SP 下载文件，内部调用下载库 ThunderService 实现。
                  注意：
                  1.本机必须安装有下载库。
                  2.download_interface_dll_path 指向下载库download_interface.dll的绝对路径。
                  3.在调用函数之前，必须确保保存目录合法。函数中不会自动创建不存在的目录。
                  4.由于p2sp会自动对文件进行改名，所以调用结束后，返回的 save_file_name 才是最终文件的保存名称。
    * 参　　数  : const xl::tstring& download_interface_dll_path download_interface_dll 的文件绝对路径。
    * 参　　数  : const xl::tstring& url                HTTP URL。或者任何迅雷支持的 URL。
    * 参　　数  : const xl::tstring& save_dir           文件下载到本地的目录名，必须是绝对路径
    * 参　　数  : xl::tstring& save_file_name           传入用户预期的文件下载到本地的文件名。
                                                        如果传入空字符串，则传出下载库智能分析得到的文件名。
                                                        如果文件名和已有文件重复，则在文件名后添加类似(1)的后缀。
    * 参　　数  : HANDLE cancel_event                   中途取消调用的事件（EVENT）。
                                                        由于是同步调用，所以程序应该传入一个 Event（例如由CreateEvent得到）
                                                        在程序需要退出时，SetEvent，然后该函数将终止执行并返回 ERROR_OPERATION_ABORTED（995） 
    * 参　　数  : DL_HUB_TYPE hub_type                  任务的 HUB 类型，普通 hub 或者 issue hub。
    * 参　　数  : P2SP_DOWNLOAD_PROGRESS progress_ptr   用于多线程调用时给外界返回当前的下载进度信息，若不需要可以传入NULL。
    * 参　　数  : bool try_continue_task                如果存在上一次下载的临时文件，则尝试创建断点续传任务
    * 参　　数  : bool keep_temp_file                   下载完成时，是否保留临时文件，以备下次断点续传
    * 返 回 值  : xl::error_code                        NO_ERR 表示成功。否则为失败码。ERROR_OPERATION_ABORTED（995）表示同步操作被取消。
    * 作　　者  : 杨晓虎
    * 设计日期  : 2010年1月4日
    * 修改日期         修改人         修改内容
    *******************************************************************************/
    typedef enum tagDL_HUB_TYPE
    {
        DL_NORMAL_HUB   = 0,            // DL means Download Lib.
        DL_ISSUE_HUB    = 1,
        DL_UNKNOWN_HUB  = 0xFFFFFFFF,
    }DL_HUB_TYPE;

    typedef struct tagP2SP_DOWNLOAD_PROGRESS
    {
        xl::uint64  file_size;          // 文件大小，0表示未知
        xl::uint64  down_size;          // 文件已下载大小
    }P2SP_DOWNLOAD_PROGRESS;

    static xl::error_code p2sp_download( 
        const xl::tstring& download_interface_dll_path,
        const std::string& url,const xl::tstring& save_dir, 
        xl::tstring& save_file_name, HANDLE cancel_event = NULL,
        DL_HUB_TYPE hub_type = DL_NORMAL_HUB, P2SP_DOWNLOAD_PROGRESS* progress_ptr = NULL,
        bool try_continue_task = false, bool keep_temp_file = false );
};

XL_WIN32_NAMESPACE_END(net)

#endif // end of __SYNCH_UTILITY_H_548B141E_AA0F_4DFF_A9EE_7B45C883E798__
