/*******************************************************************************
* 版权所有 (C)2009 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : version_utility.h
* 内容摘要  : 定义了一组和文件版本相关的工具函数。工程需要链接系统的 version.lib。
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2009年3月24日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __VERSION_UTILITY_H_DCDAD645_86EE_45F5_A94B_28FA164B83B2__
#define __VERSION_UTILITY_H_DCDAD645_86EE_45F5_A94B_28FA164B83B2__

#include "xl_lib/tchar.h"
/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

XL_NAMESPACE_BEGIN(win32)

class version_utility
{
public:

    static bool get_file_version(   const tchar * file_path, uint16 & v1, uint16 & v2, 
                                    uint16 & v3, uint16 & v4 );
    static tstring get_file_version(const tchar* file_path, tchar seperator = '.');

    static bool get_product_version(   const tchar * file_path, uint16 & v1, uint16 & v2, 
        uint16 & v3, uint16 & v4 );

    static tstring get_product_version(const tchar* file_path, tchar seperator = '.');

    // 比较两个版本，如果前者大于后者，则 result返回1，相等返回0，小于返回-1
    // 错误则函数返回 false
    // 版本字符串形如 "1.2.3.4"
    static bool compare_version( const tchar* ver1, const tchar* ver2, sint32& result );

};

XL_NAMESPACE_END(win32)

#endif // end of __VERSION_UTILITY_H_DCDAD645_86EE_45F5_A94B_28FA164B83B2__


