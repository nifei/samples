/*******************************************************************************
* 版权所有 (C)2010 深圳市迅雷网络技术有限公司。
* 系统名称  : 迅雷公共库
* 文件名称  : com_dll.h
* 内容摘要  : 封装了 COM DLL 最基础的加载 dll，卸载 dll，以及创建 COM 对象的操作
              注意，包含本头文件之前，需要提前
              #include <Windows.h>
              #include <atlbase.h>
* 当前版本  : 1.0
* 作    者  : 杨晓虎
* 设计日期  : 2010年9月10日
* 修改记录  : 
* 日    期      版    本        修改人      修改摘要
*******************************************************************************/
/**************************** 条件编译选项和头文件 ****************************/
#ifndef __COM_DLL_H_03639B4B_D295_4C59_AB84_53B6B0828EE0__
#define __COM_DLL_H_03639B4B_D295_4C59_AB84_53B6B0828EE0__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(win32)
/********************************** 常量和宏 **********************************/

/********************************** 数据类型 **********************************/

/********************************** 函数声明 **********************************/

/*********************************** 类定义 ***********************************/

class com_dll
{
public:
    HINSTANCE m_instance; // 编译出错了？？？
    //注意，包含本头文件之前，需要提前
    //#include <Windows.h>
    //#include <atlbase.h>

    com_dll();
    virtual ~com_dll();

    bool load_dll( const tchar* dll_path );
    void free_dll();

    void attach_instance( HINSTANCE instance );
    HINSTANCE detach_instance();

    HRESULT create_instance(REFCLSID rclsid, REFIID riid, LPVOID * ppv);
};

template < REFCLSID _ClsId, class _InterfaceType >
class com_object : public com_dll
{
public:
    com_object( const tchar* dll_path )
    {
        if ( load_dll(dll_path) )
        {
            HRESULT hr = create_instance(_ClsId, __uuidof(_InterfaceType), (LPVOID*)&m_p );
            ATLASSERT(("创建对象失败",SUCCEEDED(hr)));
        }
        else
        {
            ATLASSERT(("加载dll失败",NULL));
        }
    }

    virtual ~com_object()
    {
        m_p = NULL;
        free_dll();
    }

    CComPtr<_InterfaceType> m_p;
};


XL_NAMESPACE_END(win32)

#endif // end of __COM_DLL_H_03639B4B_D295_4C59_AB84_53B6B0828EE0__
