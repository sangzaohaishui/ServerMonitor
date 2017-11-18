#pragma once

#include <string>
#include <stdint.h>
#include "globaldefine.h"
using namespace std;

namespace dahlia
{
    class redis_client_t;
    class process_node_t
    {
    public:
        process_node_t(redis_client_t* redis_mgr_, 
            const string& spath_,
            int pid_, 
            bool timeout_reboot_,
            unsigned int check_time
            );
        ~process_node_t(void);
        //����ʱ��
        void update_time();
        //���̵�ǰ״̬
        bool check(int& mem);
        //��������
        bool reboot();
        //ֹͣ����
        void stop();
        //��ֹ����
        bool termate();
        //��ⴰ���Ƿ�������Դ���
        bool  contins_assert_wnd();

        string get_path();
        string get_processname();
        std::string get_processname(const string& spath_);
    public:
        bool kill_by_path(const string& spath_);
    private:

    public:
        string          _process_name;//����·��������
        unsigned int    _pid;//pid
        bool            _be_reload;//�Ƿ����
        bool            _timeout_reboot;
        uint64_t        _lastupdate_ticket;//�������ʱ��
        int             _heart_check_tick;//������ʱ���
        PROCESS_STATE   _state;
    private:
        process_node_t (const process_node_t&);
        const process_node_t &operator = (const process_node_t&);
        //��������,��������
        bool start();
    private:
        redis_client_t* _redis_mgr;
    };
}