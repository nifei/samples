/*******************************************************************************
* 版权所有 (C)2009,深圳市迅雷网络技术有限公司。
* 系统名称	: 迅雷公共库
* 文件名称	: file_utility.h
* 内容摘要	: 处理和文件有关的操作
* 当前版本	: 1.0
* 作    者	: 王卫华，杨晓虎
* 设计日期	: 2009年3月6日
* 修改记录	: 
* 日    期		版    本		修改人 		修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __FILE_UTILITY_H_58C50C12_BED7_48ca_9758_403197F8E08F__  
#define __FILE_UTILITY_H_58C50C12_BED7_48ca_9758_403197F8E08F__

#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(filesystem)

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/
class file_utility
{
public:	

	static bool			is_file(const tchar* path);	
	static bool			is_file_exist(const tchar* path);
	static sint64		get_filesize(const tchar* path);
	
#ifdef __LINUX__
	static bool			is_file(sint32 fd);	
	static sint64		get_filesize(sint32 fd); 

    static bool			is_symlink(const tchar* path);
	static sint32		create_symlink(const tchar* old_path,const tchar* new_path);
	static sint32		delete_symlink(const tchar* symlink_path);
	static sint32		get_real_path_from_symlink(const tchar* symlink_path, tchar* real_path_buff, uint32 buff_size);
#endif
};

XL_NAMESPACE_END(filesystem)

#endif // end of __FILE_UTILITY_H_58C50C12_BED7_48ca_9758_403197F8E08F__
