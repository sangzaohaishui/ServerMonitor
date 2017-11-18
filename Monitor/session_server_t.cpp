#include "session_server_t.h"
#include "daemon_instence_t.h"
#include "GlobalDefine.h"
#include "net_nb.h"
#include "msg_define.hpp"
#pragma comment(lib, "ws2_32.lib")

dahlia::session_server_t::session_server_t( ):
    _daemon_instence(NULL)
{
    //������ط�����
    net::i_linkinfo_t linkinfo;
    //����������
    linkinfo.m_bIsCheckNet = 0;
    //�ڵ��
    linkinfo.m_bIsOpen = 1;
    //��������ʱ��
    linkinfo.m_NetCheckTimeOut = 1000;
    //��������˿�
    linkinfo.m_nHostPort = 65543;
    //�ڵ�ID
    linkinfo.m_nLinkId = 12;
    //����ΪTcp������
    linkinfo.m_nLinkType = net::LT_TCP_SERVER;
    //��Ȿ�ض˿�
    strcpy_s(linkinfo.m_strHostIp, "127.0.0.1");
    //����ʱ��
    linkinfo.m_reconTime = 3000;

    net_init(linkinfo, recv_data, status);
}

dahlia::session_server_t::~session_server_t( void )
{

}

dahlia::session_server_t* dahlia::session_server_t::instence()
{
    static dahlia::session_server_t server;
    return &server;
}

void dahlia::session_server_t::recv_data( int dwLinkAd, int sockfd, int dwBufSize, void* pBuffer )
{
    if (dahlia::session_server_t::instence() &&
        dahlia::session_server_t::instence()->_daemon_instence)
    {
        //��������
        if (dwBufSize >= sizeof(nodemsg_header_t))
        {
            net::net_msg_header* pNetHeader = (net::net_msg_header*)pBuffer;
            int nDataLen = dwBufSize - (sizeof(net::net_msg_header) + sizeof(net::net_msg_end));
            nodemsg_header_t* pHeader = (nodemsg_header_t*)((char*)pBuffer + sizeof(net::net_msg_header));
            switch (pHeader->type)
            {
                case HEARTS_TYPE://������Ϣ
                    {
                        //�ж����ݳ���
                        nodemsg_heart* pHeart = (nodemsg_heart*)pHeader;
                        //���ݳ���һ�²�������
                        if (nDataLen == sizeof(nodemsg_heart)&&
                            (sizeof(nodemsg_heart) - sizeof(nodemsg_header_t)) ==
                            pHeart->header.data_len) 
                        {
                            //�����̷߳���������Ϣ
                            dahlia::session_server_t::instence()->_daemon_instence->on_proc_hearts(pHeart);
                        }
                    }
                    break;
                case REGISTER_TYPE://ע����Ϣ
                    {
                        //�ж����ݳ���
                        nodemsg_register* pRegister = (nodemsg_register*)pHeader;
                        //���ݳ���һ�²�������
                        if (nDataLen == sizeof(nodemsg_register)&&
                            (sizeof(nodemsg_register) - sizeof(nodemsg_header_t)) ==
                            pRegister->header.data_len) 
                        {
                            //�����̷߳���������Ϣ
                            dahlia::session_server_t::instence()->_daemon_instence->on_proc_register(pRegister);
                        }
                    }
                    break;
            }
        }
    }
}

void dahlia::session_server_t::status( int dwLinkAd, int sockfd, int dwBufSize, void* pBuffer )
{
    if (dahlia::session_server_t::instence() &&
        dahlia::session_server_t::instence()->_daemon_instence)
    {
        //���ӶϿ�Ϊ1
        //���ӳ�ʱҲΪ1
        //������Ϊ0
        //�пͻ���������ҲΪ0
    }
}

void dahlia::session_server_t::set_observer( daemon_instence_t* instence )
{
    _daemon_instence = instence;
}
