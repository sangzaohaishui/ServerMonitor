
/************************************************************************
*	��������:															$
*		�ж�����redis�ͻ��˲�������										$
*	��Ŀ����:�ϲ�-�翹�ж����											$
*	����:																$
*	��Ȩ:																$
*	�ο�:�ж���ظɻ����.doc												$
***********************************************************************
* $ID: ActionCtrRedis.h 2017-3-5 16:27 zhp								$
* $DATE: 2017-3-5 16:27													$
* $REVERSVERION: 0														$
* $HEAD_URL:https://nb111/svn/NBLZ/NBLZ_XDKZ/include/NB_XDKZ/			$
* ActionCtrRedis.h														$
************************************************************************/

#pragma once

#include <stdint.h>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <QString>
#include "globaldefine.h"

#ifdef PROCESSL_REDIS_EXPORTS
#define ACTCTRL_REDIS_API __declspec(dllexport)
#else
#define ACTCTRL_REDIS_API __declspec(dllimport)
#endif

namespace dahlia{
    class nb_mutex_t;
}
using namespace std;

//using namespace ;
class  CPMonQueryClientObserver
{
public:
    //��Ӧִ�з����仯,iChangeType 0:ִֹͣ��,1:��ʼִ��
    virtual void OnOpreateChange( std::string strFANM , int iChangeType ) = 0;

};

namespace acl
{
    class IDataService;
};

class ProcessCtrClientRedis
{
public:
    ProcessCtrClientRedis();
    ~ProcessCtrClientRedis(void);

    bool SetObserver(CPMonQueryClientObserver* observer);

    //��ʼ��redis
    bool InitRedis();

    static ProcessCtrClientRedis* CreateInstence();

    bool ConnectRedis();

    //��������
    bool LoadConfig();

    //ͨ���������û�ȡ
    bool LoadConfigByLocal();

    void PublishMsg( const char* szChannel,const char* szMsg );
    //void Test();
    bool ClearChannels(const string& sChannel);

    //���������͵�redis
    bool SendOpreate(const opreate_process& opreate);
    //��ѯ�����Ľ���״̬
    //bool QueryHostProcessTstates(vector<HeartBate>& );
    bool QueryHostProcessTstates(const QString& sKey, char** );

    //��ѯ��־��Ϣ
    bool QueryLogs( const string& sProcess, const string& sIp, long lStart, long lEnd , vector<montior_log_redis>&);
    bool ClearLogs(const string& sProcess, const string& sIp);
public:
    uint64_t	m_LastConnectTick;//���һ������ʱ��
    static string		m_ServiceIp;
    static int			m_ServicePort;
public:
    static ProcessCtrClientRedis* m_Instence;
    acl::IDataService* m_pRedis;
    dahlia::nb_mutex_t*		m_RedisLock;//����ʵ����
    static acl::IDataService* m_pSubRedis;

};

