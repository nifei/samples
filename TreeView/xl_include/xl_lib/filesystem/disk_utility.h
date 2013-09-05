/*******************************************************************************
* ��Ȩ���� (C)2009,������Ѹ�����缼�����޹�˾��
* ϵͳ����	: Ѹ�׹�����
* �ļ�����	: disk_utility.h
* ����ժҪ	: ����ʹ����йصĲ���
* ��ǰ�汾	: 1.0
* ��    ��	: ��������������
* �������	: 2009��3��6��
* �޸ļ�¼	: 
* ��    ��		��    ��		�޸��� 		�޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __DISK_UTILITY_H_AF0A0C1D_A4A5_4002_A639_DB0DFDD3EDE2__  
#define __DISK_UTILITY_H_AF0A0C1D_A4A5_4002_A639_DB0DFDD3EDE2__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"

XL_NAMESPACE_BEGIN(filesystem)

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/
class disk_utility
{
public:
    static sint64	get_partition_capacity(const tchar* path);
    static sint64	get_free_partition_space(const tchar* path);
    static float64	get_partition_utilization_ratio(const tchar* path);
    static tstring  get_disk_format(const tchar* path);

#ifdef __LINUX__
    static sint64	get_partition_capacity(sint32 fd);
    static sint64	get_free_partition_space(sint32 fd);
    static float64	get_partition_utilization_ratio(sint32 fd);
#endif
};

XL_NAMESPACE_END(filesystem)

#endif // end of __DISK_UTILITY_H_AF0A0C1D_A4A5_4002_A639_DB0DFDD3EDE2__
