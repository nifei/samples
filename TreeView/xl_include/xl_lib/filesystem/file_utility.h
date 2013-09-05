/*******************************************************************************
* ��Ȩ���� (C)2009,������Ѹ�����缼�����޹�˾��
* ϵͳ����	: Ѹ�׹�����
* �ļ�����	: file_utility.h
* ����ժҪ	: ������ļ��йصĲ���
* ��ǰ�汾	: 1.0
* ��    ��	: ��������������
* �������	: 2009��3��6��
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸��� 		�޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __FILE_UTILITY_H_58C50C12_BED7_48ca_9758_403197F8E08F__  
#define __FILE_UTILITY_H_58C50C12_BED7_48ca_9758_403197F8E08F__

#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(filesystem)

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/
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
