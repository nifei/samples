/*******************************************************************************
* ��Ȩ���� (C)2009 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : share_memory.h
* ����ժҪ  : 
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2009��7��6��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __SHARE_MEMORY_H_EAB50836_958C_4C16_A726_214EC01E2BBD__
#define __SHARE_MEMORY_H_EAB50836_958C_4C16_A726_214EC01E2BBD__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN( win32 )
NAMESPACE_BEGIN( multiprocess )

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

class share_memory
{
public:
    share_memory();
    virtual ~share_memory();

public:
    bool       open( const char* name, xl::uint32 size, bool open_existing = false, xl::uint32 read_only = false );
    void       close();

    xl::byte*  get_ptr();
    xl::uint32 get_size();

    xl::byte*  lock( xl::uint32 wait_millisec = INFINITE );
    void       unlock();

    class scope_lock
    {
    public:
        scope_lock( share_memory& mem )
            : m_mem(mem)
        {
            m_mem_ptr = m_mem.lock(INFINITE);
        }
        ~scope_lock()
        {
            m_mem.unlock();
        }
        xl::byte* get_ptr()
        {
            return m_mem_ptr;
        }
    protected:
        share_memory& m_mem;
        xl::byte*     m_mem_ptr;
    };

protected:
    // �����Ҫ��ʼ�������ڴ����ݣ�������������࣬���ظú�����Ĭ��Ϊ����ڴ档
    virtual void on_init_memory( xl::byte* mem_ptr, xl::uint32 mem_size );

public:
    HANDLE      m_memory;
    HANDLE      m_mutex;
    xl::byte*   m_memory_ptr;
};

NAMESPACE_END( multiprocess )
XL_NAMESPACE_END( win32 )

#endif // end of __SHARE_MEMORY_H_EAB50836_958C_4C16_A726_214EC01E2BBD__
