/*******************************************************************************
* ��Ȩ���� (C)2009 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : com_utility.h
* ����ժҪ  : ������һ����COM��صĸ�������
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2009��3��24��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __COM_UTILITY_H_A72F5242_B8D9_4D34_8194_A30EF5BCF11F__
#define __COM_UTILITY_H_A72F5242_B8D9_4D34_8194_A30EF5BCF11F__

#include "xl_lib/tchar.h"
#include <Unknwn.h>

XL_NAMESPACE_BEGIN(win32)

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

class com_utility
{
public:
    static uint32       get_refer_count( IUnknown* pUnknown );

    static bool         query_dll_can_unload_now(HINSTANCE hDll);

    static bool         create_instance_from_dll(HINSTANCE hDll, REFCLSID rclsid, REFIID riid, LPVOID * ppv);
    
    static HINSTANCE    create_instance_from_dll(LPCTSTR lpDllName, REFCLSID rclsid, REFIID riid, LPVOID * ppv);

    static tstring      get_IUnknown_module_path( IUnknown* pUnknown );

    // ��һ�� COM ָ���ȡ��X��������ַ�Լ����ڵ� DLL ����
    // �����ַ�����ʽΪ "plugin.dll[�汾��]:0x����ַ[ƫ�Ƶ�ַ]"
    // ��һ��Ϊ���ٰ汾���ڶ���Ϊ��ȫ�汾
    static tstring      get_IUnknown_method_address( IUnknown* pUnknown, uint32 methodIndex = 0 );
    static tstring      get_IUnknown_method_address_safe( IUnknown* pUnknown, uint32 methodIndex = 0 );

    // GUID ��صĺ���
    static tstring      GUID_to_str( const GUID & guid );
    static GUID         str_to_GUID( const tstring & guid );

    static bool         is_GUID( const tstring & guid_str );

    static bool         is_equal_GUID( const GUID & guid1, const GUID & guid2 );
    static bool         is_equal_GUID( const GUID & guid1, const tstring & guid2_str );
    static bool         is_equal_GUID( const tstring & guid1_str, const tstring & guid2_str );

    // ������stl��������GUIDΪ�������������
    // ���� std::map<GUID, YourValueType, com_utility::less_GUID> guid_map;
    struct less_GUID
    {
        bool operator()(const GUID& _Left, const GUID& _Right) const;
    };

    // ����һ���µ� GUID
    static GUID         generate_GUID();
};

XL_NAMESPACE_END(win32)

#endif // end of __COM_UTILITY_H_A72F5242_B8D9_4D34_8194_A30EF5BCF11F__

