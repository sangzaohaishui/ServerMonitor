#include "redis_client_t.h"
#include "back_repar_t.h"
#include "DataServiceInterface.h"
#include "processmon.pb.h"
#include "stringutil.h"
#include "dahlia_utils.h"
#include "nbclock.h"
#include "nbmutex.h"
#include <sstream>

string  dahlia::redis_client_t::_service_ip = "127.0.0.1";
int	    dahlia::redis_client_t::_service_port  = 6379;

// ���庯��ָ��
typedef int (*CIPCPQ_GetCnctParm)(char* segPref,char* segVer,char* DBPref ,char* DBVer , char* serverIP , 
    char* DBSID , char* userName , char* userPwd , char* serviceName , char* ODBCName);
CIPCPQ_GetCnctParm m_GetCnctParm;
typedef 	void (*CIPCPQ_DllFreeMem)(void *);
CIPCPQ_DllFreeMem m_DllFreeMem;

const string g_chnnel_str   = "DlmonProcess";

int redis_call_back(string recv_chnnel,string recv_mssage)
{
    //������KeyΪ��name_ip
    if (recv_chnnel == "process_opreate")
    {
        if (dahlia::redis_client_t::instence())
        {
            //��������
           dahlia::redis_client_t::instence()->on_proc_client(recv_mssage);
        }
    }

    return 0;
}
//����ת���߳�
UINT subcribe_message_thread_func(LPVOID lpParam)
{
    acl::IDataService* pRedis = NULL;
    do 
    {
        pRedis =  acl::ICreateDataService(dahlia::redis_client_t::_service_ip.c_str(),dahlia::redis_client_t::_service_port); 
        Sleep(3000);
    } while (pRedis == NULL);

    char* channel="process_opreate";

    if (pRedis)
    {
        pRedis->RegistCalBakFuc(channel,redis_call_back);
    }	

    vector<string> channels;
    channels.push_back(channel);

    if (pRedis)
    {
        pRedis->SubscribeMessageNew(channels);  
    }

    return 1;
}



dahlia::redis_client_t::redis_client_t( void ):
    _reper_observer(NULL)
{
    _redis_reg_lock = new nb_mutex_t;
    //��ȡ�����ļ�
    if(load_config())
    {
        if(!init_redis())
        {
            while(!connect_redis())
            {
                Sleep(10000);
            }
        }
    }
}

dahlia::redis_client_t::~redis_client_t( void )
{

}

bool dahlia::redis_client_t::init_redis()
{
    _redis_instence = acl::ICreateDataService(dahlia::redis_client_t::_service_ip.c_str(),dahlia::redis_client_t::_service_port); 
    _last_con_tick = nbclock::now_us() / 1000000.0; //���һ������ʱ��
    if(!_redis_instence)
    {
        //GetLog4Mgr().LogMsgError("����Redis��������ʧ�ܣ���������...");
        return false;
    }
    else
    {
        //���̶߳������ݱ仯
        _redis_reg_lock->lock();

        DWORD dwThread(0);
        ::CreateThread(0,0,(LPTHREAD_START_ROUTINE)subcribe_message_thread_func,_redis_instence,0,&dwThread);

        _redis_reg_lock->unlock();
    }
    return true;
}

dahlia::redis_client_t* dahlia::redis_client_t::instence()
{
    static redis_client_t instence_;
    return& instence_;
}

bool dahlia::redis_client_t::connect_redis()
{
    if (_redis_instence)
    {
        //������ӶϿ�
        if(!_redis_instence->IsConnected())
        {
            //������ϴ����Ӳ�ֵ����40s����������
            if (fabs(_last_con_tick - nbclock::now_us() / 1000000.0) > 10.0)
            {
                _last_con_tick = nbclock::now_us() / 1000000.0; //�������һ������ʱ��
                _redis_reg_lock->lock();
                if (_redis_instence)
                {
                    _last_con_tick = nbclock::now_us() / 1000000.0; //�������һ������ʱ��
                    if(!_redis_instence->ReConnected())
                    {
                        //��������ʧ��
                        _redis_reg_lock->unlock();
                        return false;
                    }
                    else
                    {
                        _redis_reg_lock->unlock();
                        return true;
                    }
                }
                _redis_reg_lock->unlock();
            }
        }
        else
        {
            return true;
        }

    }
    else
    {
        int nDis = fabs(_last_con_tick - nbclock::now_us() / 1000000.0);
        if (nDis > 10.0)
        {
            _redis_reg_lock->lock();
            if (NULL == _redis_instence)
            {
                _redis_instence = acl::ICreateDataService(dahlia::redis_client_t::_service_ip.c_str(),dahlia::redis_client_t::_service_port); 
                _last_con_tick = nbclock::now_us() / 1000000.0; //���һ������ʱ��
                if(!_redis_instence)
                {
                    //����ʧ��
                    _redis_reg_lock->unlock();
                    return false;
                }
                else
                {
                    //���̶߳������ݱ仯
                    DWORD dwThread(0);
                    ::CreateThread(0,0,(LPTHREAD_START_ROUTINE)subcribe_message_thread_func,_redis_instence,0,&dwThread);
                    _redis_reg_lock->unlock();
                    return true;
                }
            }
            _redis_reg_lock->unlock();
        }
    }
    return false;
}

bool dahlia::redis_client_t::load_config()
{
    string s_file = get_app_path();
    s_file = s_file + "/redisconfig.ini";

    //��ȡ����˿�
    dahlia::redis_client_t::_service_port = GetPrivateProfileIntA("RedisServer","MemeryServerPort",6379,s_file.c_str() );

    //ͨ�������ļ����ж�ȡ
    char s_ip[42] = {0};
    GetPrivateProfileStringA("RedisServer","ServerIP","127.0.0.1",s_ip,32,s_file.c_str());

    //ͨ����ǰ׺����
    dahlia::redis_client_t::_service_ip = s_ip;

    return true;
}

bool dahlia::redis_client_t::load_config_fromlocal()
{
    return true;
}

void dahlia::redis_client_t::publish_msg( const char* channel,const char* szMsg )
{
    if (_redis_instence)
    {
        if (NULL != channel && NULL != szMsg)
        {
            _redis_instence->PublishMessage(channel,szMsg);
        }
    }
}

bool dahlia::redis_client_t::clear_channels( const string& channel )
{
    if (!connect_redis())
    {
        return false;
    }

    if (!_redis_instence)
    {
        return false;
    }

    string strKeyPat = "*";
    return _redis_instence->DeleteKeys(channel.c_str(),strKeyPat.c_str());
}

bool dahlia::redis_client_t::send_process_state( const string& key, const redis_process_state& hearts )
{
    if (!connect_redis())
    {
        return false;
    }

    if (!_redis_instence)
    {
        return false;
    }

    string s_chnnel = g_chnnel_str;
    s_chnnel += ":process_moitor";

    bool bRet = _redis_instence->SetBuffer(s_chnnel.c_str(), key.c_str() , (char*)&hearts, sizeof(redis_process_state));

    _redis_instence->PublishMessage("process_moitor", key.c_str());
    return true;
}

void dahlia::redis_client_t::set_observer( back_repar_t* observer_)
{
    _reper_observer = observer_;
}

void dahlia::redis_client_t::on_proc_client( const string& key_)
{
    if (dahlia::redis_client_t::instence() &&
        dahlia::redis_client_t::instence()->_reper_observer)
    {
        //��ȡ���ݣ���������
        dahlia::redis_client_t::instence()->_reper_observer->on_proc_client(key_);
    }
}

bool dahlia::redis_client_t::querry_Buff( const string& key, char** buff,unsigned int& len )
{
    int nDataLen = len;

    string s_chnnel = g_chnnel_str;
    s_chnnel += ":process_opreate";

    bool bRet = _redis_instence->GetBuffer(s_chnnel.c_str(), key.c_str(), *buff, len);
    if (!bRet ||
        nDataLen != len)
    {
        return false;
    }

    return true;
}

void dahlia::redis_client_t::clear_monitor( const std::string& s_ip )
{
    if (!connect_redis())
    {
        return ;
    }

    if (!_redis_instence)
    {
        return ;
    }

    string s_chnnel = g_chnnel_str;
    s_chnnel += ":process_moitor";

    string strKeyPat = s_ip;
    strKeyPat += "*";

    bool bRet = _redis_instence->DeleteKeys(s_chnnel.c_str(),strKeyPat.c_str());

    /*  bool bRet  = false;
    for (int i = 0; i < vKeys.size(); ++i)
    {
    string strKey = vKeys[i];
    bRet = _redis_instence->DeleteKey(s_chnnel.c_str(),strKey.c_str());
    }*/
    return ;
}

void dahlia::redis_client_t::set_log( const montior_log_redis& log_)
{
    MonLogMSG pro_log_;
    pro_log_.set_log_state(log_.log_state);
    pro_log_.set_sprocess(log_.s_process);
    pro_log_.set_shost(log_.s_ip);
    pro_log_.set_s_time(log_.s_time);
    pro_log_.set_s_info(log_.s_info);
    pro_log_.set_s_data_exten(log_.s_data_exten);
    pro_log_.set_n_type(log_.n_type);

    stringstream ss;
    ss <<log_.s_ip;
    ss <<":";
    ss <<log_.s_process;
    ss << ":";
    ss <<(log_.log_state);
    ss <<":";
    ss <<log_.s_time;

    string sKey;
    ss >>sKey;

    string s_chnnel = g_chnnel_str;
    s_chnnel += ":process_log";

    bool bRet = _redis_instence->SetMessage(s_chnnel.c_str(),sKey.c_str() , &pro_log_);

}

void dahlia::redis_client_t::set_log(
    const string& s_ip,/*ip */ 
    const string& s_process,/*�������� */
    LOG_TYPE log_state,/*����״̬ */ 
    const string& s_info,/*��Ϣ,�Զ��������ֶ����������ԣ� */ 
    int n_type,/*��չ���������� */ 
    const string& s_data_exten )
{
    montior_log_redis log_;
    (log_.log_state) = log_state;
    (log_.s_process) = s_process;
    (log_.s_ip) = s_ip;
    SYSTEMTIME curTime;
    ::GetLocalTime( &curTime );
    log_.s_time =  (toSeconds(curTime));
    (log_.s_info) = s_info;
    (log_.s_data_exten) = s_data_exten;
    (log_.n_type) = n_type;

    set_log(log_);
}
