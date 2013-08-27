/*******************************************************************************
* 版权所有 (C)2009 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : process_messenger.h
* 内容摘要  : 
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2009年6月29日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __PROCESS_MESSENGER_H_C27C70F1_6515_4C07_821B_EC8E2B640903__
#define __PROCESS_MESSENGER_H_C27C70F1_6515_4C07_821B_EC8E2B640903__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"
#include "xl_lib/design_pattern/singleton.h"
#include "xl_lib/win32/multithread/critical_section.h"
#include "xl_lib/win32/multithread/thread.h"
#include "xl_lib/win32/multithread/lock_guard.h"
#include "share_memory.h"
#include <map>

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

XL_NAMESPACE_BEGIN( win32 )
NAMESPACE_BEGIN( multiprocess )

// 注意此类的派生类不要有静态变量
// 如果一定需要静态变量，请使用 xl::design_pattern::singleton_ex 来达到同等效果
class msg_receiver
{
public:
    msg_receiver( const char* name, xl::uint32 msg_buffer_size = DEFAULT_MSG_BUFFER_SIZE );
    virtual ~msg_receiver();

protected:
    // 用户需要派生此函数，处理消息
    // 注意，此函数的调用是在 messenger 的私有线程，所以请一定做好线程安全。
    virtual void on_process_msg( xl::uint32 msg_id,
        xl::byte* msg_buffer, xl::uint32 msg_size, std::string& reply_msg ) = 0;

    enum 
    {
        DEFAULT_MSG_BUFFER_SIZE = 4000 
    }; 

    std::string m_name;
    friend class messenger;
};

class messenger
    : public xl::design_pattern::singleton_ex<messenger>
    , public xl::win32::multithread::thread
{
public:
    static bool send_message( xl::uint32 dest_process_id, const char* dest_module, 
        const char* receiver_name, xl::uint32 msg_id,
        const xl::byte* msg_buffer, xl::uint32 msg_size,
        std::string& reply_msg, xl::uint32 wait_max_millisec = INFINITE );

protected:
    messenger(void);
    virtual ~messenger(void);

    friend class xl::design_pattern::singleton_ex<messenger>;

protected:
    virtual xl::uint32  thread_proc();
    HANDLE              m_ready_event;

    static xl::uint32  get_version()
    {
        return 0x20090630;
    }

    enum OBJECT_NAME
    {
        SHARE_MEMORY,
        BEGIN_SEND_EVENT,
        END_SEND_EVENT,
        SUCC_EVENT, // 发送者发送的消息是否被正确的处理了？
        IDLE_EVENT, // 为了防止消息接收者处理消息过慢，发送者超时，然后再发消息，此时前一个消息处理完毕然后设置 END_SEND_EVENT 造成第二则消息错以为发送结束，增加该锁
    };
    static std::string get_object_name( OBJECT_NAME object = SHARE_MEMORY, DWORD process_id = 0, const char* module_name = NULL );
    
    static xl::uint32 s_msg_buffer_size; // 本地消息接收缓冲区的大小

protected:
    bool messenger::attach_receiver( msg_receiver* receiver );
    bool messenger::detach_receiver( msg_receiver* receiver );
    msg_receiver* messenger::find_receiver( const char* receiver_name );

    std::map<std::string, msg_receiver*>        m_receivers;
    xl::win32::multithread::critical_section    m_receivers_lock;
    friend class msg_receiver;

protected:    
    bool on_receive_data( xl::byte* share_memory_ptr, xl::uint32 share_memory_size );
};

NAMESPACE_END( multiprocess )
XL_NAMESPACE_END( win32 )

#endif // end of __PROCESS_MESSENGER_H_C27C70F1_6515_4C07_821B_EC8E2B640903__
