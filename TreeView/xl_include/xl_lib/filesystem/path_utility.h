/*******************************************************************************
* 版权所有 (C)2009 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : path_utility.h
* 内容摘要  : 文件路径处理的辅助函数。Windows依赖系统shell32.lib
* 当前版本  : 1.0
* 作    者  : 王卫华 杨晓虎
* 设计日期  : 2009年3月25日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __PATH_UTILITY_H_2D9D5B34_5895_49D8_B6DF_0CE426AEE7A2__
#define __PATH_UTILITY_H_2D9D5B34_5895_49D8_B6DF_0CE426AEE7A2__

#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(filesystem)
/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

class path_utility
{
public:
    static tstring	get_file_name(const tchar* file_path);

    static tstring	get_file_main_name(const tchar* file_path);
    static tstring	get_file_ext(const tchar* file_path);

    static tstring	get_path_root(const tchar* file_path);
    static tstring	get_path_dir(const tchar* file_path);

    static tstring  get_upper_dir(const tchar* dir_path);

    static tstring  get_current_exe_path();
    static tstring  get_current_exe_dir();
    static tstring  get_current_module_path();
    static tstring  get_current_module_dir();

    static tstring  get_common_appdata_dir();
    static tstring  get_appdata_dir();
    static tstring  get_tmpdata_dir();
};

XL_NAMESPACE_END(filesystem)

#endif // end of __PATH_UTILITY_H_2D9D5B34_5895_49D8_B6DF_0CE426AEE7A2__


