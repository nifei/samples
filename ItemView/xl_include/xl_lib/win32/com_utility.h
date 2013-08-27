/*******************************************************************************
* 版权所有 (C)2009 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : com_utility.h
* 内容摘要  : 定义了一组与COM相关的辅助函数
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2009年3月24日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __COM_UTILITY_H_A72F5242_B8D9_4D34_8194_A30EF5BCF11F__
#define __COM_UTILITY_H_A72F5242_B8D9_4D34_8194_A30EF5BCF11F__

#include "xl_lib/tchar.h"
#include <Unknwn.h>

XL_NAMESPACE_BEGIN(win32)

/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

class com_utility
{
public:
    static uint32       get_refer_count( IUnknown* pUnknown );

    static bool         query_dll_can_unload_now(HINSTANCE hDll);

    static bool         create_instance_from_dll(HINSTANCE hDll, REFCLSID rclsid, REFIID riid, LPVOID * ppv);
    
    static HINSTANCE    create_instance_from_dll(LPCTSTR lpDllName, REFCLSID rclsid, REFIID riid, LPVOID * ppv);

    static tstring      get_IUnknown_module_path( IUnknown* pUnknown );

    // 从一个 COM 指针获取第X个方法地址以及所在的 DLL 名称
    // 返回字符串形式为 "plugin.dll[版本号]:0x基地址[偏移地址]"
    // 第一个为快速版本，第二个为安全版本
    static tstring      get_IUnknown_method_address( IUnknown* pUnknown, uint32 methodIndex = 0 );
    static tstring      get_IUnknown_method_address_safe( IUnknown* pUnknown, uint32 methodIndex = 0 );

    // GUID 相关的函数
    static tstring      GUID_to_str( const GUID & guid );
    static GUID         str_to_GUID( const tstring & guid );

    static bool         is_GUID( const tstring & guid_str );

    static bool         is_equal_GUID( const GUID & guid1, const GUID & guid2 );
    static bool         is_equal_GUID( const GUID & guid1, const tstring & guid2_str );
    static bool         is_equal_GUID( const tstring & guid1_str, const tstring & guid2_str );

    // 用于以stl中容器以GUID为键的排序操作。
    // 例如 std::map<GUID, YourValueType, com_utility::less_GUID> guid_map;
    struct less_GUID
    {
        bool operator()(const GUID& _Left, const GUID& _Right) const;
    };

    // 生成一个新的 GUID
    static GUID         generate_GUID();
};

XL_NAMESPACE_END(win32)

#endif // end of __COM_UTILITY_H_A72F5242_B8D9_4D34_8194_A30EF5BCF11F__

