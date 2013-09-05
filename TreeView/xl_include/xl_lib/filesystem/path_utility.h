/*******************************************************************************
* ��Ȩ���� (C)2009 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : path_utility.h
* ����ժҪ  : �ļ�·������ĸ���������Windows����ϵͳshell32.lib
* ��ǰ�汾  : 1.0
* ��    ��  : ������ ������
* �������  : 2009��3��25��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __PATH_UTILITY_H_2D9D5B34_5895_49D8_B6DF_0CE426AEE7A2__
#define __PATH_UTILITY_H_2D9D5B34_5895_49D8_B6DF_0CE426AEE7A2__

#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(filesystem)
/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

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


