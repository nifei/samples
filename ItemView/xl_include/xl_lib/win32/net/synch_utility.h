/*******************************************************************************
* ��Ȩ���� (C)2010 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : synch_utility.h
* ����ժҪ  : �ṩ��һЩ����ͬ��������ʵ�ú���������HTTP���أ�P2SP���صȡ�
* ��ǰ�汾  : 1.0
* ��    ��  : ������
* �������  : 2010��1��4��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __SYNCH_UTILITY_H_548B141E_AA0F_4DFF_A9EE_7B45C883E798__
#define __SYNCH_UTILITY_H_548B141E_AA0F_4DFF_A9EE_7B45C883E798__

#include "xl_lib/type.h"
#include "xl_lib/tchar.h"
#include "xl_lib/error.h"
#include "xl_lib/net/protocol/http.h"

XL_WIN32_NAMESPACE_BEGIN(net)

/********************************** �����ͺ� **********************************/

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

class synch_utility
{
public:

    /*******************************************************************************
    * ��������  : static xl::error_code http_download( const xl::tstring& url, const xl::tstring& save_path )
    * ��������  : ͬ������ HTTP �ļ����ڲ�ʹ�� WinInet API ʵ�֡�
                  ע�⣬������Ҫ���� Wininet.lib��
    * �Ρ�����  : const xl::tstring& url                HTTP URL
    * �Ρ�����  : const xl::tstring& save_dir           �ļ����ص����ص�Ŀ¼���������Ǿ���·��
    * �Ρ�����  : const xl::tstring& save_file_name     �ļ����ص����ص��ļ���
    * �Ρ�����  : HANDLE cancel_event                   ��;ȡ�����õ��¼���EVENT����
                                                        ������ͬ�����ã����Գ���Ӧ�ô���һ�� Event��������CreateEvent�õ���
                                                        �ڳ�����Ҫ�˳�ʱ��SetEvent��Ȼ��ú�������ִֹ�в����� ERROR_OPERATION_ABORTED��995�� 
    * �� �� ֵ  : xl::error_code                        NO_ERR ��ʾ�ɹ�������Ϊʧ���롣ERROR_OPERATION_ABORTED��995����ʾͬ��������ȡ����
    * ��������  : ������
    * �������  : 2010��1��4��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    static xl::error_code http_download( const xl::tstring& url, const xl::tstring& save_dir, 
        const xl::tstring& save_file_name, HANDLE cancel_event = NULL );


    /*******************************************************************************
    * ��������  : http_request
    * ��������  : ͬ������ʵ�� HTTP �ʴ��ڲ�ʹ�� WinInet API ʵ�֡�
                  ע�⣬������Ҫ���� Wininet.lib��
    * �Ρ�����  : const xl::net::protocol::http::REQUEST& http_request  HTTP ����
    * �Ρ�����  : xl::net::protocol::http::RESPONSE& http_response      HTTP �ظ�
    * �Ρ�����  : HANDLE cancel_event = NULL                            ��;ȡ�����õ��¼���EVENT����
    * �� �� ֵ  : xl::error_code                                        NO_ERR ��ʾ�ɹ�������Ϊʧ���롣ERROR_OPERATION_ABORTED��995����ʾͬ��������ȡ����
    * ��������  : ������
    * �������  : 2010��2��1��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    static xl::error_code http_request( const xl::net::protocol::http::REQUEST& http_request,
        xl::net::protocol::http::RESPONSE& http_response,
        HANDLE cancel_event = NULL );

    /*******************************************************************************
    * ��������  : p2sp_download
    * ��������  : ͬ��ʹ�� P2SP �����ļ����ڲ��������ؿ� ThunderService ʵ�֡�
                  ע�⣺
                  1.�������밲װ�����ؿ⡣
                  2.download_interface_dll_path ָ�����ؿ�download_interface.dll�ľ���·����
                  3.�ڵ��ú���֮ǰ������ȷ������Ŀ¼�Ϸ��������в����Զ����������ڵ�Ŀ¼��
                  4.����p2sp���Զ����ļ����и��������Ե��ý����󣬷��ص� save_file_name ���������ļ��ı������ơ�
    * �Ρ�����  : const xl::tstring& download_interface_dll_path download_interface_dll ���ļ�����·����
    * �Ρ�����  : const xl::tstring& url                HTTP URL�������κ�Ѹ��֧�ֵ� URL��
    * �Ρ�����  : const xl::tstring& save_dir           �ļ����ص����ص�Ŀ¼���������Ǿ���·��
    * �Ρ�����  : xl::tstring& save_file_name           �����û�Ԥ�ڵ��ļ����ص����ص��ļ�����
                                                        ���������ַ������򴫳����ؿ����ܷ����õ����ļ�����
                                                        ����ļ����������ļ��ظ��������ļ������������(1)�ĺ�׺��
    * �Ρ�����  : HANDLE cancel_event                   ��;ȡ�����õ��¼���EVENT����
                                                        ������ͬ�����ã����Գ���Ӧ�ô���һ�� Event��������CreateEvent�õ���
                                                        �ڳ�����Ҫ�˳�ʱ��SetEvent��Ȼ��ú�������ִֹ�в����� ERROR_OPERATION_ABORTED��995�� 
    * �Ρ�����  : DL_HUB_TYPE hub_type                  ����� HUB ���ͣ���ͨ hub ���� issue hub��
    * �Ρ�����  : P2SP_DOWNLOAD_PROGRESS progress_ptr   ���ڶ��̵߳���ʱ����緵�ص�ǰ�����ؽ�����Ϣ��������Ҫ���Դ���NULL��
    * �Ρ�����  : bool try_continue_task                ���������һ�����ص���ʱ�ļ������Դ����ϵ���������
    * �Ρ�����  : bool keep_temp_file                   �������ʱ���Ƿ�����ʱ�ļ����Ա��´ζϵ�����
    * �� �� ֵ  : xl::error_code                        NO_ERR ��ʾ�ɹ�������Ϊʧ���롣ERROR_OPERATION_ABORTED��995����ʾͬ��������ȡ����
    * ��������  : ������
    * �������  : 2010��1��4��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    typedef enum tagDL_HUB_TYPE
    {
        DL_NORMAL_HUB   = 0,            // DL means Download Lib.
        DL_ISSUE_HUB    = 1,
        DL_UNKNOWN_HUB  = 0xFFFFFFFF,
    }DL_HUB_TYPE;

    typedef struct tagP2SP_DOWNLOAD_PROGRESS
    {
        xl::uint64  file_size;          // �ļ���С��0��ʾδ֪
        xl::uint64  down_size;          // �ļ������ش�С
    }P2SP_DOWNLOAD_PROGRESS;

    static xl::error_code p2sp_download( 
        const xl::tstring& download_interface_dll_path,
        const std::string& url,const xl::tstring& save_dir, 
        xl::tstring& save_file_name, HANDLE cancel_event = NULL,
        DL_HUB_TYPE hub_type = DL_NORMAL_HUB, P2SP_DOWNLOAD_PROGRESS* progress_ptr = NULL,
        bool try_continue_task = false, bool keep_temp_file = false );
};

XL_WIN32_NAMESPACE_END(net)

#endif // end of __SYNCH_UTILITY_H_548B141E_AA0F_4DFF_A9EE_7B45C883E798__
