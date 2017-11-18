#include "session_client_t.h"
#include "net_nb.h"
#include "msg_define.hpp"
#include <Windows.h>
#include <assert.h>
#pragma comment(lib, "ws2_32.lib")

#define MAX_NET_PACK_LEN (8192 + 128)


//���������
long __stdcall callback__crash_func(EXCEPTION_POINTERS*excp)
{

    return EXCEPTION_EXECUTE_HANDLER;
}



dahlia::session_client_t::session_client_t()
{
    SetUnhandledExceptionFilter(callback__crash_func);//������������

    //���ӷ����
    //������ط�����
    net::i_linkinfo_t linkinfo;
    //����������
    linkinfo.m_bIsCheckNet = 0;
    //�ڵ��
    linkinfo.m_bIsOpen = 1;
    //��������ʱ��
    linkinfo.m_NetCheckTimeOut = 50000;
    //��������˿�
    linkinfo.m_nRemotePort = 65543;
    //�ڵ�ID
    linkinfo.m_nLinkId = 14;
    //����ΪTcp�ͻ���
    linkinfo.m_nLinkType = net::LT_TCP_CLIENT;
    //��Ȿ�ض˿�
    strcpy_s(linkinfo.m_strRemoteIp, "127.0.0.1");
    //����ʱ��
    linkinfo.m_reconTime = 5000;

    net_init(linkinfo, recv_data, status);
}

dahlia::session_client_t::~session_client_t( void )
{

}


dahlia::session_client_t* dahlia::session_client_t::instence()
{
    static dahlia::session_client_t client;
    return &client;
}

void dahlia::session_client_t::recv_data( int dwLinkAd, int sockfd, int dwBufSize, void* pBuffer )
{

}

void dahlia::session_client_t::status( int dwLinkAd, int sockfd, int dwBufSize, void* pBuffer )
{
    //���ӶϿ�Ϊ1
    //���ӳ�ʱҲΪ1
    //������Ϊ0
    //�пͻ���������ҲΪ0
    //����������˾���ͻ���ע��
    if (dwBufSize == sizeof(int))
    {
        int* pFlag = (int*)pBuffer;
        if (0 == (*pFlag)&&
            dahlia::session_client_t::instence())
        {
            dahlia::session_client_t::instence()->register_process();//���س���ע��
        }
    }
}


void dahlia::session_client_t::set_observer( client_repar_t* instence, const string& process_name_, unsigned int pid_ )
{
    _daemon_instence = instence;
    _process_name   = process_name_;
    _pid            = pid_;
}

void dahlia::session_client_t::heart()
{
    nodemsg_heart heart_;
    heart_.pid = _pid;
    strcpy_s(heart_.ppath, _process_name.c_str());
    send_buff((char*)&heart_, sizeof(nodemsg_heart));
}

void dahlia::session_client_t::register_process()
{
    nodemsg_register regist_;
    regist_.pid = _pid;
    regist_.timeout_reboot = 0;//��ʱ������
    regist_.heart_check_tick = 15000;//������ʱ����ʱ��
    strcpy_s(regist_.ppath, _process_name.c_str());
    send_buff((char*)&regist_, sizeof(nodemsg_register));

}

void dahlia::session_client_t::send_buff( char* buff, int size )
{
    //�鿴��Ӧ������ͷ����
    if (size > 8192)
    {
        return;
    }
    net::net_msg_header netheader_;
    net::net_msg_end    netend_;
    char queue_buf[MAX_NET_PACK_LEN];

    netheader_.msg_len = sizeof(net::net_msg_header) + size + sizeof(net::net_msg_end);
    memcpy(queue_buf, &netheader_, sizeof(net::net_msg_header));
    memcpy(queue_buf+ sizeof(net::net_msg_header), buff, size);
    memcpy(queue_buf+ sizeof(net::net_msg_header) + size, &netend_, sizeof(net::net_msg_end));

    net_asyn_send(14, 0,netheader_.msg_len, (char*)queue_buf);
}

