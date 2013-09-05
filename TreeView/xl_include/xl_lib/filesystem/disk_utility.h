/*******************************************************************************
* 版权所有 (C)2009,深圳市迅雷网络技术有限公司。
* 系统名称	: 迅雷公共库
* 文件名称	: disk_utility.h
* 内容摘要	: 处理和磁盘有关的操作
* 当前版本	: 1.0
* 作    者	: 王卫华，杨晓虎
* 设计日期	: 2009年3月6日
* 修改记录	: 
* 日    期		版    本		修改人 		修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __DISK_UTILITY_H_AF0A0C1D_A4A5_4002_A639_DB0DFDD3EDE2__  
#define __DISK_UTILITY_H_AF0A0C1D_A4A5_4002_A639_DB0DFDD3EDE2__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(filesystem)

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/
class disk_utility
{
public:
    static sint64	get_partition_capacity(const tchar* path);
    static sint64	get_free_partition_space(const tchar* path);
    static float64	get_partition_utilization_ratio(const tchar* path);
    static tstring  get_disk_format(const tchar* path);

#ifdef __LINUX__
    static sint64	get_partition_capacity(sint32 fd);
    static sint64	get_free_partition_space(sint32 fd);
    static float64	get_partition_utilization_ratio(sint32 fd);
#endif
};

XL_NAMESPACE_END(filesystem)

#endif // end of __DISK_UTILITY_H_AF0A0C1D_A4A5_4002_A639_DB0DFDD3EDE2__
