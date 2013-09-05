/*******************************************************************************
* ��Ȩ���� (C)2009,������Ѹ�����缼�����޹�˾��
* ϵͳ����	: Ѹ�׹�����
* �ļ�����	: dir_utility.h
* ����ժҪ	: �����Ŀ¼�йصĲ���
* ��ǰ�汾	: 1.0
* ��    ��	: ��������������
* �������	: 2009��3��6��
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸��� 		�޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __DIR_UTILITY_H_C33E6E58_8299_467f_82F3_A3E38BC00F1B__  
#define __DIR_UTILITY_H_C33E6E58_8299_467f_82F3_A3E38BC00F1B__

#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(filesystem)
/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/
class dir_utility
{
public:
    
    static bool     is_dir(const tchar* path);
    static bool     is_empty_directory(const tchar* dir_path);
    static tstring  get_current_working_directory();	
    static bool     set_current_working_directory(const tchar* dir_path);

    static bool     delete_dir(const tchar* dir_path);
    static bool     delete_dir_tree(const tchar* dir_path);

#if defined(WIN32)

    static bool     make_dir(const tchar* dir_path);
    static bool     make_dir_tree(const tchar* dir_path);

#elif defined(__LINUX__)
    
    static bool     is_dir(sint32 fd);
    static sint32   make_dir(const tchar* path, mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO);
    static sint32   make_dir_tree(const tchar* path, mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO);

#endif
};

XL_NAMESPACE_END(filesystem)

#endif // end of __DIR_UTILITY_H_C33E6E58_8299_467f_82F3_A3E38BC00F1B__
