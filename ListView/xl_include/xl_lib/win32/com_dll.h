/*******************************************************************************
* ��Ȩ���� (C)2010 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : com_dll.h
* ����ժҪ  : ��װ�� COM DLL ������ļ��� dll��ж�� dll���Լ����� COM ����Ĳ���
              ע�⣬������ͷ�ļ�֮ǰ����Ҫ��ǰ
              #include <Windows.h>
              #include <atlbase.h>
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2010��9��10��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __COM_DLL_H_03639B4B_D295_4C59_AB84_53B6B0828EE0__
#define __COM_DLL_H_03639B4B_D295_4C59_AB84_53B6B0828EE0__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(win32)
/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

class com_dll
{
public:
    HINSTANCE m_instance; // ��������ˣ�����
    //ע�⣬������ͷ�ļ�֮ǰ����Ҫ��ǰ
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
            ATLASSERT(("��������ʧ��",SUCCEEDED(hr)));
        }
        else
        {
            ATLASSERT(("����dllʧ��",NULL));
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
