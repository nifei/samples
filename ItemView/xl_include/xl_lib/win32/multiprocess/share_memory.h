/*******************************************************************************
* 版权所有 (C)2009 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : share_memory.h
* 内容摘要  : 
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2009年7月6日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __SHARE_MEMORY_H_EAB50836_958C_4C16_A726_214EC01E2BBD__
#define __SHARE_MEMORY_H_EAB50836_958C_4C16_A726_214EC01E2BBD__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN( win32 )
NAMESPACE_BEGIN( multiprocess )

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

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
    // 如果需要初始化共享内存内容，则可以派生该类，重载该函数。默认为清空内存。
    virtual void on_init_memory( xl::byte* mem_ptr, xl::uint32 mem_size );

public:
    HANDLE      m_memory;
    HANDLE      m_mutex;
    xl::byte*   m_memory_ptr;
};

NAMESPACE_END( multiprocess )
XL_NAMESPACE_END( win32 )

#endif // end of __SHARE_MEMORY_H_EAB50836_958C_4C16_A726_214EC01E2BBD__
