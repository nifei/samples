/*******************************************************************************
* ��Ȩ���� (C)2009 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : version_utility.h
* ����ժҪ  : ������һ����ļ��汾��صĹ��ߺ�����������Ҫ����ϵͳ�� version.lib��
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2009��3��24��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __VERSION_UTILITY_H_DCDAD645_86EE_45F5_A94B_28FA164B83B2__
#define __VERSION_UTILITY_H_DCDAD645_86EE_45F5_A94B_28FA164B83B2__

#include "xl_lib/tchar.h"
/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

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

    // �Ƚ������汾�����ǰ�ߴ��ں��ߣ��� result����1����ȷ���0��С�ڷ���-1
    // ������������ false
    // �汾�ַ������� "1.2.3.4"
    static bool compare_version( const tchar* ver1, const tchar* ver2, sint32& result );

};

XL_NAMESPACE_END(win32)

#endif // end of __VERSION_UTILITY_H_DCDAD645_86EE_45F5_A94B_28FA164B83B2__


