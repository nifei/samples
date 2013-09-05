/*******************************************************************************
* 版权所有 (C)2009,深圳市迅雷网络技术有限公司。
* 系统名称	: 迅雷公共库
* 文件名称	: dir_utility.h
* 内容摘要	: 处理和目录有关的操作
* 当前版本	: 1.0
* 作    者	: 王卫华，杨晓虎
* 设计日期	: 2009年3月6日
* 修改记录	: 
* 日    期		版    本		修改人 		修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __DIR_UTILITY_H_C33E6E58_8299_467f_82F3_A3E38BC00F1B__  
#define __DIR_UTILITY_H_C33E6E58_8299_467f_82F3_A3E38BC00F1B__

#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(filesystem)
/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/
class dir_utility
{
public:
    
    static bool     is_dir(const tchar* path);
    static bool     is_empty_directory(const tchar* dir_path);
    static tstring  get_current_working_directory();	
    static bool     set_current_working_directory(const tchar* dir_path);

    static bool     delete_dir(const tchar* dir_path);
    static bool     delete_dir_tree(const tchar* dir_path);

#if defined(WIN32)

    static bool     make_dir(const tchar* dir_path);
    static bool     make_dir_tree(const tchar* dir_path);

#elif defined(__LINUX__)
    
    static bool     is_dir(sint32 fd);
    static sint32   make_dir(const tchar* path, mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO);
    static sint32   make_dir_tree(const tchar* path, mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO);

#endif
};

XL_NAMESPACE_END(filesystem)

#endif // end of __DIR_UTILITY_H_C33E6E58_8299_467f_82F3_A3E38BC00F1B__
