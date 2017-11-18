#pragma once

#include <map>
#include <string>
#include "globaldefine.h"
#include <stdint.h>

using namespace std;

namespace acl
{
    class IDataService;
};


namespace dahlia
{
    class nb_mutex_t;
    class back_repar_t;
    class redis_client_t
    {
    public:
        redis_client_t(void);
        ~redis_client_t(void);

        //��ʼ��redis
        bool init_redis();

        static redis_client_t* instence();

        //������Ϣ���մ���Ĺ۲���
        void   set_observer(back_repar_t* );

        //����redis
        bool connect_redis();

        //��������
        bool load_config();

        //ͨ���������û�ȡ
        bool load_config_fromlocal();

        //������Ϣ
        void publish_msg( const char* channel,const char* szMsg );
        
        //����һ��Ƶ��
        bool clear_channels(const string& channel);

        //��������Ϣ���͵�redis��path��
        bool send_process_state(const string& key, const redis_process_state& hearts );

        //����ͻ��˲���
        static void on_proc_client(const string& key_);

        bool querry_Buff(const string& key, char** buff, unsigned int& len);

        void clear_monitor(const std::string& s_ip);

        void set_log(  
           const string& s_ip,//ip
            const string& s_process,//��������
            LOG_TYPE log_state,//����״̬
           const string& s_info,//��Ϣ,�Զ��������ֶ����������ԣ�
            int     n_type,//��չ����������
            const string& s_data_exten);//��չ����

        void set_log(const montior_log_redis&);

    public:
        uint64_t	_last_con_tick;//���һ������ʱ��
        static string		_service_ip;    //����IP
        static int			_service_port;  //����˿�
    public:
        acl::IDataService*  _redis_instence;
        nb_mutex_t*		    _redis_reg_lock;//����ʵ����
        back_repar_t*       _reper_observer;
    private:
        redis_client_t (const redis_client_t&);
        const redis_client_t &operator = (const redis_client_t&);
    };
}
