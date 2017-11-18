#pragma once

#include "aqueue.h"
#include "i_aqueue_callback.h"

#include <string>

using namespace std;

namespace dahlia
{
    class aqueue_manager_t;

    class aqueue_obj_t: public i_asynobj_events
    {
    public:
        aqueue_obj_t(const string& obj_id_, aqueue_manager_t* mgr_);
        ~aqueue_obj_t(void);

        /************************************************************************
        * ���ƣ�on_time
        * ˵����//��ʱ��ʵ����ʵ�ָú�����
        * ������id��ʱ���¼�ID
        * ����ֵ����
        /************************************************************************/
        virtual void on_time(int id) = 0;  

        /************************************************************************
        * ���ƣ�on_attach
        * ˵�������Ӷ����̳߳ɹ��¼�
        * ��������
        * ����ֵ����
        /************************************************************************/
        virtual void on_attach() = 0;  

        /************************************************************************
        * ���ƣ�on_detach
        * ˵�������̷߳����¼�
        * ��������
        * ����ֵ����
        /************************************************************************/
        virtual void on_detach() = 0;  

        /************************************************************************
        * ���ƣ�on_msg
        * ˵������Ӧ�����̵߳���Ϣ
        * ������cmd ����ID,lparm,wparam
        * ����ֵ����
        /************************************************************************/
        virtual void on_msg(int cmd_, int lparm_, long wparam_) = 0;  

        /************************************************************************
        * ���ƣ�get_id
        * ˵������ȡ���첽�����ID
        * ��������
        * ����ֵ������ID
        /************************************************************************/
        string get_id();

        /************************************************************************
        * ���ƣ�get_mgr
        * ˵������ȡ��ǰ�첽���������
        * ��������
        * ����ֵ������ID
        /************************************************************************/
        aqueue_manager_t* get_mgr();

        /************************************************************************
        * ���ƣ�add_timer
        * ˵�������ض����Ƿ񱻳�ʼ��
        * ��������
        * ����ֵ������ID
        /************************************************************************/


        void add_timer( int tmid_, int tmout_ );


    private:
        /************************************************************************/
        /* �첽����̳�ʵ��																								 */
        /************************************************************************/
        virtual void attach_event () ;
        virtual void dettach_event ();
        virtual void timer_event (int id_);
        virtual void msg_event(aqueue::aqueue_msg_t* msg_);
    private:
        string  _obj_id;//����ID
        aqueue_manager_t* _aobj_mgr;//�첽���������
       
    private:
        aqueue_obj_t (const aqueue_obj_t&);
        const aqueue_obj_t &operator = (const aqueue_obj_t&);
    };
}