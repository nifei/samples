#ifndef __ASYN_HTTP_FILE_THREAD_H_C922B218_069F_4A67_B3F8_38BCAA67F1C2__
#define __ASYN_HTTP_FILE_THREAD_H_C922B218_069F_4A67_B3F8_38BCAA67F1C2__

#include "asyn_http_file.h"

#define DOWN_SUCCESS_MSG WM_USER+1000
#define DOWN_FAILED_MSG WM_USER+1001

typedef xl::error_code(__stdcall *DOWN_SUCCESS)(xl::uint32 id,  const std::string& url,
                                      const xl::tstring& file_full_path, xl::win32::net::asyn_http_file_event::HTTP_FILE_STATUS status, void* pParam);
typedef xl::error_code (__stdcall *DOWN_FAIL)( xl::uint32 id, const std::string& url,
                                    xl::error_code err, void* pParam);

XL_WIN32_NAMESPACE_BEGIN(net)

class asyn_http_file_in_thread : public xl::win32::net::asyn_http_file_event
{
public:
    asyn_http_file_in_thread();
    ~asyn_http_file_in_thread();
    virtual xl::error_code on_asyn_http_file_ready( xl::uint32 id, const std::string& url,
        const xl::tstring& file_full_path, xl::win32::net::asyn_http_file_event::HTTP_FILE_STATUS status );
    virtual xl::error_code on_asyn_http_file_error( xl::uint32 id, const std::string& url,
        xl::error_code err );

    bool request_file( xl::uint32 id, const std::string& url, 
        const xl::net::protocol::http::HEADERS& request_headers, 
        const xl::tstring& file_full_path, DOWN_SUCCESS lpDownSuccessProc, DOWN_FAIL lpDownFailProc, void* pParam);

    bool is_requesting() const;

    xl::uint64 get_downloaded_length() const;

    xl::uint64 get_file_length() const;

    void close();

    void set_recv_timeout(xl::uint32 millsec);
    std::string get_response_header(const std::string &strfield_name);

private:
    static unsigned int __stdcall RequestThreadProc(void* pParam);
    static LRESULT __stdcall WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    xl::uint32 m_id;
    std::string m_url;
    xl::net::protocol::http::HEADERS m_header;
    xl::tstring m_full_path;
    xl::win32::net::asyn_http_file* m_asyn_http_file;
    HTTP_FILE_STATUS m_status;
    xl::error_code m_err;
    static HWND m_hMessageWnd;
    DOWN_SUCCESS m_lpDownSuccess;
    DOWN_FAIL m_lpDownFail;
    void* m_pParam;
};

XL_WIN32_NAMESPACE_END(net)
#endif