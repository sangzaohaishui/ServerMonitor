#include "process_mgr_t.h"
#include "dahlia_utils.h"
#include "globaldefine.h"
#include "redis_client_t.h"
#include "stringutil.h"
#include "nbclock.h"

dahlia::process_mgr_t::process_mgr_t( redis_client_t* redis_mgr_ ):
_redis_mgr(redis_mgr_),
_reboot_flag(false)
{
    _local_ip = get_local_ip_s();
    _redis_mgr->clear_monitor(_local_ip);//�����Ӧϯλ�ļ�������
}

dahlia::process_mgr_t::~process_mgr_t( void )
{
    map<string , process_node_t*>::iterator pIt = _nodes.begin();
    for ( ; pIt != _nodes.end(); ++pIt)
    {
        delete pIt->second;
    }
}

void dahlia::process_mgr_t::register_node( const string& path, int npid , bool timeout_reboot_, int check_tick_)
{
    //�鿴�Ƿ�����ͬ�Ľ�������
    map<string , process_node_t*>::iterator pIt = _nodes.find(path);//path,pid,nodeinfo
    if (pIt != _nodes.end())
    {
        //�鿴֮ǰ�Ƿ���ͬ���Ľ���,���ǽ���ID��һ�����������ɱ��
        if ( pIt->second->_pid != npid)
        {
            pIt->second->termate();
        }

        //����ʱ��
        pIt->second->update_time();
        pIt->second->_pid = npid;
        pIt->second->_process_name = path;
        pIt->second->_timeout_reboot = timeout_reboot_;
        pIt->second->_heart_check_tick = check_tick_;
        pIt->second->_be_reload = true;
    }
    else
    {
        //���ɽڵ�
        _nodes[path] = new process_node_t(_redis_mgr, path, npid, timeout_reboot_, check_tick_);
    }

}

void dahlia::process_mgr_t::heart( const string& path, int npid )
{
    //�����ϱ�ʱ��
    map<string , process_node_t* >::iterator pIt = _nodes.find(path);//path,pid,nodeinfo
    if (pIt != _nodes.end())
    {
        //����ʱ��
        if (npid ==  pIt->second->_pid)
        {
            pIt->second->update_time();
        }
    }
}

void dahlia::process_mgr_t::check()
{
    redis_process_state state_;
    int nmemry_count_ = -1;
    strcpy_s(state_.ip, _local_ip.c_str());

    //ѭ�����ڵ���Ϣ
    uint64_t last_ticket_ = nbclock::now_us() / 1000.0;

    //����ڵ�С��0�������ͼ�������������Ϣ
    if (_nodes.size() <= 0)
    {
        //���ͽ���״̬
        if (_redis_mgr)
        {
            string  sKey = _local_ip;
            sKey    += "_";
            sKey    += "monitor";
            state_.state =  STATE_MONITOR_HEART;
            _redis_mgr->send_process_state(sKey, state_);
        }
    }

    map<string , process_node_t*>::iterator pIt = _nodes.begin();
    for ( ; pIt != _nodes.end(); ++pIt)
    {
        string  sKey = _local_ip;
        sKey    += "_";
        sKey    += num2str(pIt->second->_pid);
       double dis_time =  (last_ticket_ - pIt->second->_lastupdate_ticket);

        strcpy_s(state_.ppath, pIt->second->_process_name.c_str());
        state_.pid = pIt->second->_pid;

        //���������״̬
        if (pIt->second->_heart_check_tick == -1)
        {
            //״̬Ϊ������
            //׼������״̬����,����redis,����Ϊ������
            state_.state = STATE_REBOOTING;
            state_.memcount = nmemry_count_;
            pIt->second->_state = STATE_REBOOTING;

            //���������ʱ�����������
            if (dis_time > 60000)
            {
                 pIt->second->reboot();
            }

            if (_redis_mgr)
            {
                _redis_mgr->send_process_state(sKey, state_);
            }
            continue;
        }

        bool reload_ = false;
      
        //���������ʱ������û���������е�״̬
        if (fabs(dis_time)> 
            pIt->second->_heart_check_tick &&
            pIt->second->_heart_check_tick != -1)//_heart_check_tickΪ-1��ʾΪ������״̬
        {
            //״̬Ϊδ�ϱ���������״̬,
            //��������Ѿ����˾��ж�������־����������
            //���״̬Ϊ���ţ������������Խ�������
            //�ϱ��쳣״̬
            if (!pIt->second->check(nmemry_count_))//�жϽ��̴��״̬
            {
                if (pIt->second->_be_reload)//�����ʶΪ��������������
                {
                    reload_ = true;
                }

                //��������״̬֮ǰ��ֻ����״̬ת����ʱ���ӡ��־
                if (_redis_mgr && 
                    reload_)
                {
                    _redis_mgr->set_log(_local_ip, pIt->second->get_processname(), AUTO_START, "�Զ�����", -1, "");
                }

                state_.state = STATE_STOP;
                pIt->second->_state = STATE_STOP;
                //׼������״̬����,����redis
            }
            else
            {

                if (pIt->second->_timeout_reboot&&
                    pIt->second->_heart_check_tick != -1)//������̻��ţ���ʶΪ������ʱ������,��ʶΪ����״̬
                {
                    reload_ = true;
                    if (_redis_mgr && 
                        reload_)
                    {
                        _redis_mgr->set_log(_local_ip, pIt->second->get_processname(), HEART_OUTTM, "������ʱ", -1, "");
                    }
                } 
                else
                {
                    //�ж��Ƿ��ж��Դ���
                    if (pIt->second->contins_assert_wnd())
                    {
                        //��������״̬֮ǰ��ֻ����״̬ת����ʱ���ӡ��־
                        reload_ = true;
                        if (_redis_mgr && 
                            reload_)
                        {
                            _redis_mgr->set_log(_local_ip, pIt->second->get_processname(), ASSERTING_, "����", -1, "");
                        }


                        state_.state = STATE_ASSERTING;
                        pIt->second->_state = STATE_ASSERTING;
                    }
                    else
                    {
                        //��������״̬֮ǰ��ֻ����״̬ת����ʱ���ӡ��־
                        if (_redis_mgr && 
                            pIt->second->_state != STATE_OUT_TIME)
                        {
                            _redis_mgr->set_log(_local_ip, pIt->second->get_processname(), HEART_OUTTM, "������ʱ", -1, "");
                        }

                        state_.state = STATE_OUT_TIME;
                        pIt->second->_state = STATE_OUT_TIME;
                    }
                }
                
               
                //׼������״̬����,����redis
            }
            state_.memcount = nmemry_count_;
        }
        else
        {
            if (!pIt->second->check(nmemry_count_))//�жϽ��̴��״̬
            {
                if (pIt->second->_be_reload)//�����ʶΪ��������������
                {
                    reload_ = true;
                }

                //��������״̬֮ǰ��ֻ����״̬ת����ʱ���ӡ��־
                if (_redis_mgr && !reload_)
                {
                    _redis_mgr->set_log(_local_ip, pIt->second->get_processname(), AUTO_DIE, "������ֹ", -1, "");
                }

                state_.state = STATE_STOP;
                pIt->second->_state = STATE_STOP;
                //׼������״̬����,����redis
            }
            else
            {
                //�ж��Ƿ��ж��Դ���
                if (pIt->second->contins_assert_wnd())
                {
                    //��������״̬֮ǰ��ֻ����״̬ת����ʱ���ӡ��־


                    reload_ = true;
                    if (_redis_mgr &&reload_)
                    {
                        _redis_mgr->set_log(_local_ip, pIt->second->get_processname(), ASSERTING_, "����", -1, "");
                    }

                    state_.state = STATE_ASSERTING;
                    pIt->second->_state = STATE_ASSERTING;
                }
                else
                {
                    state_.state = STATE_RUNNING;
                    pIt->second->_state = STATE_RUNNING;
                }
                 //׼������״̬����,����redis
            }
            state_.memcount = nmemry_count_;
        }

        //���ͽ���״̬
        if (_redis_mgr)
        {
            _redis_mgr->send_process_state(sKey, state_);
        }

        if (reload_)//��ʶ����
        {
            pIt->second->reboot();
        }
    }
} 

void dahlia::process_mgr_t::stop( const string& path/*, int npid */)
{
    //ֹͣ���̣����ý��̲�������ʶ
    //�����ϱ�ʱ��
    map<string , process_node_t* >::iterator pIt = _nodes.find(path);//path,pid,nodeinfo
    if (pIt != _nodes.end())
    {
        //����ʱ��
            pIt->second->stop();

            if (_redis_mgr)
            {
                _redis_mgr->set_log(_local_ip, pIt->second->get_processname(), UNAUTO_DIE_, "�ֶ��رս���", -1, "");
            }
    }
}

void dahlia::process_mgr_t::start( const string& path/*, int npid*/ )
{
    map<string , process_node_t* >::iterator pIt = _nodes.find(path);//path,pid,nodeinfo
    if (pIt != _nodes.end())
    {
        //����ʱ��
            pIt->second->reboot();

            if (_redis_mgr)
            {
                _redis_mgr->set_log(_local_ip, pIt->second->get_processname(), UNAUTO_START, "�ֶ�����", -1, "");
            }
    }
    else
    {
        //���ɽڵ㣨֮ǰû��ע����ģ����ǿͻ��������������˵ģ�
        process_node_t* pNode = new process_node_t(_redis_mgr, path, -1, false, 15000); 
        _nodes[path] = pNode;
        //ͨ������ɱ������
        pNode->kill_by_path(path);
        pNode->reboot();

        if (_redis_mgr)
        {
            _redis_mgr->set_log(_local_ip, pNode->get_processname(), UNAUTO_START, "�ֶ�����", -1, "");
        }
    }
}

void dahlia::process_mgr_t::del( const string& path/*, int npid*/ )
{
    map<string , process_node_t* >::iterator pIt = _nodes.find(path);//path,pid,nodeinfo
    if (pIt != _nodes.end())
    {
        //����ʱ��
       // pIt->second->stop();
        delete pIt->second;
        _nodes.erase(pIt);
    }
}
