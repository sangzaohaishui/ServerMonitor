#pragma once
#include "globaldefine.h"
namespace dahlia
{
    class session_server_t;
    class aqueue_manager_t;
    class aqueue_obj_t;
    class daemon_instence_t
    {
    public:
        daemon_instence_t(void);
        ~daemon_instence_t(void);
        //��������
        void on_proc_hearts(const nodemsg_heart* heart);
        //����ע����Ϣ
        void on_proc_register(const nodemsg_register* );
    private:
        void init();
    private:
        //����ʵ��
        session_server_t*   _heart_server;
        //�첽�������ʵ��
        aqueue_manager_t*   _aqueue_mgr;
        //���̼�ع������
        aqueue_obj_t*       _process_mgr;
    private:
        daemon_instence_t (const daemon_instence_t&);
        const daemon_instence_t &operator = (const daemon_instence_t&);
    };
}