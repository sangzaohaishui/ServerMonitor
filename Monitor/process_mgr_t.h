#pragma once

#include <string>
#include <map>
#include "process_node_t.h"

using namespace std;

namespace dahlia
{
    class redis_client_t;
    class process_mgr_t
    {
    public:
        process_mgr_t(redis_client_t* redis_mgr_);
        ~process_mgr_t(void);

        //ע��ڵ�
        void register_node(const string& path, int npid, bool timeout_reboot_, int check_tick_);

        //��������
        void heart(const string& path, int npid);

        //��ʱ���
        void check();

        //ֹͣ����
        void stop(const string& path/*, int npid*/);

        //��������
        void start(const string& path/*, int npid*/);

        //ɾ��������Ϣ
        void del(const string& path/*, int npid*/);
    public:
        string          _local_ip;//����ip��ַ
    private:
        process_mgr_t (const process_mgr_t&);
        const process_mgr_t &operator = (const process_mgr_t&);
    private:
        redis_client_t* _redis_mgr;
        map<string , process_node_t* > _nodes;//path,pid,nodeinfo
        bool            _reboot_flag;//����û���ϱ������Ĵ���ʽtrueΪ����,falseΪ������
    };
}