#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <vector>
#include <set>

using namespace std;

/*  Define integer types needed for event interface                          */
#if defined NET_HAVE_SOLARIS || defined NET_HAVE_OPENVMS
#   include <inttypes.h>
#elif defined _MSC_VER && _MSC_VER < 1600
#   ifndef int32_t
typedef __int32 int32_t;
#   endif
#   ifndef uint16_t
typedef unsigned __int16 uint16_t;
#   endif
#else
#   include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
    //�ж��Ƿ������˶�ʵ��
    bool is_signal_instence();

    //ͨ��pid��ȡ���̾��
    HANDLE get_process_handle(unsigned int pid_);

    //ͨ���������ƻ�ȡ����pid
    bool   get_processes_by_pname(const string& pname_, set<int>& );

    //��ȡ�����̵�·��
    std::string get_app_path();

    //��ȡ����������
    std::string get_app_name();

    //��ȡ��ǰ���̵�pid
    unsigned int get_current_pid();

    //��ȡ��ǰ�����ڴ�
    unsigned int get_process_memroy(unsigned int pid_);
    unsigned int get_process_memroy_handle(HANDLE handle);

    //ͨ���������ƺͽ���IDɱ������
    bool kill_process(const string& pname_, unsigned int npid);

    //��ȡ������һ��ip��ַ
    string get_local_ip_s();

    //��ȡ���ص�����ip��ַ�б�
    void get_host_ip_list(vector<string>& v_host_ip);

    //ͨ��pid��ȡ�ļ�·��������
    bool get_exe_path_by_pid(int npid, string& sPath);

    /*�жϴ����Ƿ��ж��Դ���,�ڶ������������Ƿ��ͼ*/
    bool  contions_assert_wnds(int pid_, bool bSave, const string& path);
#ifdef __cplusplus
}
#endif

