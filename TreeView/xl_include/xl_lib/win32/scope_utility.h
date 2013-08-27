/*******************************************************************************
* ��Ȩ���� (C)2010 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : scope_utility.h
* ����ժҪ  : ����һЩ�ڹ̶���Χ�����õ���ʱ���ã����˳���Χʱ�ָ�
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2010��10��13��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __SCOPE_UTILITY_H_C122B673_AE2C_4C02_9E9B_E4A4E0B7D906__
#define __SCOPE_UTILITY_H_C122B673_AE2C_4C02_9E9B_E4A4E0B7D906__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(win32)
NAMESPACE_BEGIN(scope_utility)
/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

// �������������÷�Χ���л���ǰ����Ŀ¼������ʱ�ָ�֮ǰ�Ĺ���Ŀ¼
class scope_set_directory
{
public:
    xl::tchar       m_old_current_dir[MAX_PATH];

    scope_set_directory( const xl::tchar* new_dir )
    {
        ::GetCurrentDirectory(MAX_PATH,m_old_current_dir); ::SetCurrentDirectory(new_dir);
    }
    ~scope_set_directory()
    {
        ::SetCurrentDirectory(m_old_current_dir);
    }
};

NAMESPACE_END(scope_utility)
XL_NAMESPACE_END(win32)

#endif // end of __SCOPE_UTILITY_H_C122B673_AE2C_4C02_9E9B_E4A4E0B7D906__
