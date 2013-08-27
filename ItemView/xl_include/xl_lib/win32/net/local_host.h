/*******************************************************************************
* ��Ȩ���� (C)2009 ������Ѹ�����缼�����޹�˾��
* ϵͳ����  : Ѹ�׹�����
* �ļ�����  : local_host.h
* ����ժҪ  : �������ڻ�ȡ���������������������Ϣ������MAC��ַ��IP��ַ
* ��ǰ�汾  : 1.0
* ��    ��  : Τ��ʵ
* �������  : 2009��8��8��
* �޸ļ�¼  : 
* ��    ��      ��    ��        �޸���      �޸�ժҪ
*******************************************************************************/
/**************************** ��������ѡ���ͷ�ļ� ****************************/
#ifndef __LOCAL_HOST_H_068EC4D7_D5B5_43FE_9E77_336067645578__
#define __LOCAL_HOST_H_068EC4D7_D5B5_43FE_9E77_336067645578__

#include <vector>
#include <string>
#include "xl_lib/type.h"
#include "xl_lib/tchar.h"
#include "xl_lib/text/transcode.h"
#include "xl_lib/win32/net/net_utility.h"

XL_WIN32_NAMESPACE_BEGIN(net)

/********************************** �����ͺ� **********************************/

typedef xl::uint32 ipv4;

/********************************** �������� **********************************/

/********************************** �������� **********************************/

/*********************************** �ඨ�� ***********************************/

template<class _char> class local_host_t;

typedef local_host_t<tchar> loacl_host;

template<class _char>
class local_host_t
{
public:
    typedef typename std::basic_string<_char> _string;

public:
    local_host_t();
    virtual ~local_host_t();

    // �ṩ��һ�����ʹ�õļ�㷽��
public:
    /*******************************************************************************
    * ��������  : get_default_ipaddr/get_default_ipaddr_mask
    * ��������  : ��ȡ������ȱʡIP��ַ/��ȡ������ȱʡIP��ַ����
                  ʵ���ϣ����ڻ��������ж��������һ�����������ж��IP���ú�����Ϊ��
                  ����һ�������÷��㣬ȡ��һ�������ĵ�ǰIP��ַ��
    * �� �� ֵ  : _string             ����IP�ַ���������"192.168.1.101"������ʧ���򷵻ؿ��ַ�����
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    _string get_default_ipaddr();
    _string get_default_ipaddr_mask();

    /*******************************************************************************
    * ��������  : get_default_gateway/get_default_gateway_mask
    * ��������  : ��ȡ������ȱʡ���ص�ַ/��ȡ������ȱʡ���ص�ַ����
                  ʵ���ϣ����ڻ��������ж��������һ�����������ж��IP���ú�����Ϊ��
                  ����һ�������÷��㣬ȡ��һ�������ĵ�һ�����ص�ַ��
    * �� �� ֵ  : _string             ����IP�ַ���������"192.168.1.1"������ʧ���򷵻ؿ��ַ�����
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    _string get_default_gateway();
    _string get_default_gateway_mask();

    // �ṩ���߼�����ʹ�õĸ��ӷ���
public:
    /*******************************************************************************
    * ��������  : get_adapter_num
    * ��������  : ���������������һ�����������ж��������
    * �� �� ֵ  : uint32 
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    uint32  get_adapter_num();

    /*******************************************************************************
    * ��������  : get_adapter_name
    * ��������  : ��ȡָ�����������ơ�
    * �Ρ�����  : xl::uint32 adapter_index  ������š�ֵ��Χ [ 0, get_adapter_num() )
    * �� �� ֵ  : _string             �����������ơ�
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    _string get_adapter_name(xl::uint32 adapter_index);

    /*******************************************************************************
    * ��������  : get_mac_addr
    * ��������  : ��ȡָ��������MAC��ַ��һ������ֻ��һ��MAC��ַ��
    * �Ρ�����  : xl::uint32 adapter_index  ������š�ֵ��Χ [ 0, get_adapter_num() )
    * �� �� ֵ  : _string             ��������MAC��ַ�����磺E4-1F-13-30-51-A2
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    _string get_mac_addr(xl::uint32 adapter_index);

    /*******************************************************************************
    * ��������  : get_netcard_description
    * ��������  : ��ȡָ��������������Ϣ��
    * �Ρ�����  : xl::uint32 adapter_index  ������š�ֵ��Χ [ 0, get_adapter_num() )
    * �� �� ֵ  : _string             ����������������Ϣ�����磺Broadcom BCM5709C NetXtreme II GigE 
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    _string get_netcard_description(xl::uint32 adapter_index);

    /*******************************************************************************
    * ��������  : get_ipaddr_num
    * ��������  : ���ָ��������IP������һ�����������ж��IP��ַ��
    * �Ρ�����  : xl::uint32 adapter_index  ������š�ֵ��Χ [ 0, get_adapter_num() )
    * �� �� ֵ  : uint32 
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    uint32  get_ipaddr_num(xl::uint32 adapter_index);

    /*******************************************************************************
    * �ṹ������: ip_address_string
    * ��������  : ����IP��ַ�������롣
    * �Ρ�����  : ip_addr   IP��ַ
    * �Ρ�����  : ip_mask   ����
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    struct ip_address_string
    {
        _string ip_addr;
        _string ip_mask;
    };

    /*******************************************************************************
    * ��������  : get_ipaddr
    * ��������  : ��ȡָ��������ָ��IP��ַ��
    * �Ρ�����  : xl::uint32 adapter_index  ������š�ֵ��Χ [ 0, get_adapter_num() )
    * �Ρ�����  : xl::uint32 ipaddr_index   IP��ַ��š�ֵ��Χ [ 0, get_ipaddr_num(adapter_index) )
    * �� �� ֵ  : ip_address_string
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    ip_address_string get_ipaddr(xl::uint32 adapter_index, xl::uint32 ipaddr_index)
    {
        return adapter_index<m_info.size()?
            (ipaddr_index<m_info[adapter_index].ip_address.size()?m_info[adapter_index].ip_address[ipaddr_index]:ip_address_string())
            :ip_address_string();
    }

    /*******************************************************************************
    * ��������  : get_gateway_num
    * ��������  : ��ȡָ�����������ص�ַ��һ�����������ж�����ص�ַ��
    * �Ρ�����  : xl::uint32 adapter_index  ������š�ֵ��Χ [ 0, get_adapter_num() )
    * �� �� ֵ  : uint32 
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    uint32  get_gateway_num(xl::uint32 adapter_index);

    /*******************************************************************************
    * ��������  : get_gateway_address
    * ��������  : ��ȡָ��������ָ�����ص�ַ��
    * �Ρ�����  : xl::uint32 adapter_index  ������š�ֵ��Χ [ 0, get_adapter_num() )
    * �Ρ�����  : xl::uint32 gateway_index  ������š�ֵ��Χ [ 0, get_gateway_num(adapter_index) )
    * �� �� ֵ  : ip_address_string
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    ip_address_string get_gateway_address(xl::uint32 adapter_index, xl::uint32 gateway_index)
    {
        return adapter_index<m_info.size()?
            (gateway_index<m_info[adapter_index].gate_way.size()?m_info[adapter_index].gate_way[gateway_index]:ip_address_string())
            :ip_address_string();
    }

    /*******************************************************************************
    * ��������  : is_local_ip_addr
    * ��������  : �ж�IP�Ƿ��Ǳ�����IP��
    * �Ρ�����  : ipv4 ip_addr
    * �� �� ֵ  : bool 
    * ��������  : ������
    * �������  : 2010��7��14��
    * �޸�����         �޸���         �޸�����
    *******************************************************************************/
    bool    is_local_ip_addr(ipv4 ip_addr);

protected:

    void init();

    struct adapter_info
    {
        _string physical_address;
        _string card_name;
        _string description;
        std::vector<ip_address_string> ip_address;
        std::vector<ip_address_string> gate_way;
    };

    std::vector<adapter_info> m_info;
};

template<class _char>
xl::win32::net::local_host_t<_char>::~local_host_t()
{
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_default_ipaddr()
{
    return m_info.empty()?std::basic_string<_char>()
        :(m_info[0].ip_address.empty()?std::basic_string<_char>():m_info[0].ip_address[0].ip_addr);
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_default_ipaddr_mask()
{
    return m_info.empty()?std::basic_string<_char>()
        :(m_info[0].ip_address.empty()?std::basic_string<_char>():m_info[0].ip_address[0].ip_mask);
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_default_gateway()
{
    return m_info.empty()?std::basic_string<_char>():
    (m_info[0].gate_way.empty()?std::basic_string<_char>():m_info[0].gate_way[0].ip_addr);
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_default_gateway_mask()
{
    return m_info.empty()?std::basic_string<_char>():
    (m_info[0].gate_way.empty()?std::basic_string<_char>():m_info[0].gate_way[0].ip_mask);
}

template<class _char>
uint32 local_host_t<_char>::get_adapter_num()
{
    return (xl::uint32)m_info.size();
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_adapter_name(xl::uint32 adapter_index)
{
    return adapter_index<m_info.size()?m_info[adapter_index].card_name:std::basic_string<_char>();
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_mac_addr(xl::uint32 adapter_index)
{
    return adapter_index<m_info.size()?m_info[adapter_index].physical_address:std::basic_string<_char>();
}

template<class _char>
std::basic_string<_char> local_host_t<_char>::get_netcard_description(xl::uint32 adapter_index)
{
    return adapter_index<m_info.size()?m_info[adapter_index].description:std::basic_string<_char>();
}

template<class _char>
uint32  local_host_t<_char>::get_ipaddr_num(xl::uint32 adapter_index)
{
    return (xl::uint32)(adapter_index<m_info.size()?m_info[adapter_index].ip_address.size():0);
}

template<class _char>
uint32  local_host_t<_char>::get_gateway_num(xl::uint32 adapter_index)
{
    return (xl::uint32)(adapter_index<m_info.size()?m_info[adapter_index].gate_way.size():0);
}

template<class _char>
bool local_host_t<_char>::is_local_ip_addr(ipv4 ip_addr)
{
    _string addr = xl::win32::net::net_utility_t<_char>::ip_ntoa(ip_addr);
    for(uint32 x = 0; x < m_info.size(); x ++)
    {
        for(uint32 y = 0; y < m_info[x].ip_address.size(); x ++)
        {
            if(m_info[x].ip_address[y].ip_addr == addr)
            {
                return true;
            }
        }
    }

    return false;
}


XL_WIN32_NAMESPACE_END(net)

#endif // end of __LOCAL_HOST_H_068EC4D7_D5B5_43FE_9E77_336067645578__
