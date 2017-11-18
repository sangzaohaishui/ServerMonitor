/************************************************************************/
/* �첽���������																									*/
/************************************************************************/
#pragma once

#include "aqueue.h"
#include <iostream>

using namespace std;

namespace dahlia
{
    class aqueue_obj_t;

    class   aqueue_manager_t
    {
    public:
        aqueue_manager_t(int _msg_count, int _timer_count);
        virtual ~aqueue_manager_t(void);

        //���첽��������Ϣ
        int send_cmd(const string& id,  int _msg_id, int _lparam, long _wparam);

        //���첽������Buff(С��512�ֽ�)
        int send_short_buff(const string& id,  int _msg_id, const void* _buff, long _length);

        //���Ͷ��Ϸ���ĳ��ֽ�����(�ǵö����ݵ��ͷ�)
        int send_long_buff(const string& id,  int _msg_id, const void* _buff, long _length);

        //ע�ᶨʱ��
        int regist_timer(const string& id, int _timer_id, int _time_out);

        //ȡ����ʱ��
        int unregist_timer(const string& id,  int _timer_id);

        //���Ӷ����߳� 
        int attach_obj(aqueue_obj_t* _obj, thread_type _type = e_msg);

        //�����߳�
        int detch_obj(const string& id);

    private:
        void*       _ctx;//�����Ķ���
        int		    _msgs;//��Ϣ�߳�
        int         _timers;//��ʱ���߳�
    private:
        aqueue_manager_t (const aqueue_manager_t&);
        const aqueue_manager_t &operator = (const aqueue_manager_t&);
    };
}