/*******************************************************************************
* ��Ȩ���� (C)2009 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : process_messenger.h
* ����ժҪ  : 
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2009��6��29��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
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

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

XL_NAMESPACE_BEGIN( win32 )
NAMESPACE_BEGIN( multiprocess )

// ע�����������಻Ҫ�о�̬����
// ���һ����Ҫ��̬��������ʹ�� xl::design_pattern::singleton_ex ���ﵽͬ��Ч��
class msg_receiver
{
public:
    msg_receiver( const char* name, xl::uint32 msg_buffer_size = DEFAULT_MSG_BUFFER_SIZE );
    virtual ~msg_receiver();

protected:
    // �û���Ҫ�����˺�����������Ϣ
    // ע�⣬�˺����ĵ������� messenger ��˽���̣߳�������һ�������̰߳�ȫ��
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
        SUCC_EVENT, // �����߷��͵���Ϣ�Ƿ���ȷ�Ĵ����ˣ�
        IDLE_EVENT, // Ϊ�˷�ֹ��Ϣ�����ߴ�����Ϣ�����������߳�ʱ��Ȼ���ٷ���Ϣ����ʱǰһ����Ϣ�������Ȼ������ END_SEND_EVENT ��ɵڶ�����Ϣ����Ϊ���ͽ��������Ӹ���
    };
    static std::string get_object_name( OBJECT_NAME object = SHARE_MEMORY, DWORD process_id = 0, const char* module_name = NULL );
    
    static xl::uint32 s_msg_buffer_size; // ������Ϣ���ջ������Ĵ�С

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
