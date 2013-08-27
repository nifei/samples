/*******************************************************************************
* 版权所有 (C)2010 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : scope_utility.h
* 内容摘要  : 用于一些在固定范围内设置的临时设置，在退出范围时恢复
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2010年10月13日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __SCOPE_UTILITY_H_C122B673_AE2C_4C02_9E9B_E4A4E0B7D906__
#define __SCOPE_UTILITY_H_C122B673_AE2C_4C02_9E9B_E4A4E0B7D906__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(win32)
NAMESPACE_BEGIN(scope_utility)
/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

// 该类用于在作用范围内切换当前工作目录，析构时恢复之前的工作目录
class scope_set_directory
{
public:
    xl::tchar       m_old_current_dir[MAX_PATH];

    scope_set_directory( const xl::tchar* new_dir )
    {
        ::GetCurrentDirectory(MAX_PATH,m_old_current_dir); ::SetCurrentDirectory(new_dir);
    }
    ~scope_set_directory()
    {
        ::SetCurrentDirectory(m_old_current_dir);
    }
};

NAMESPACE_END(scope_utility)
XL_NAMESPACE_END(win32)

#endif // end of __SCOPE_UTILITY_H_C122B673_AE2C_4C02_9E9B_E4A4E0B7D906__
