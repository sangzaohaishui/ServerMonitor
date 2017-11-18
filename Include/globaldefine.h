#ifndef __MONTT__GLOBAL_DEFINE_HH___II
#define __MONTT__GLOBAL_DEFINE_HH___II

#include <string.h>
#include <string>
using namespace std;

#pragma pack(push , enter_includeglobal)
#pragma pack(1)

enum MSG_TYPE
{
    //��������
    HEARTS_TYPE,
    //ע������
    REGISTER_TYPE,
    //ע�����̣�ɾ���ڵ㣬ɱ�����̣�
    DELPROCESS_TYPE,
    //ֹͣ����(ֻɱ�����̣�������ע����Ϣ)
    STOPPROCESS_TYPE,
    //��������(���������������Ҫ��ǰ̨����ɾ���Ͻ�����Ϣ�������ֱ������������)
    STARTPROCESS_TYPE,
};

enum PROCESS_STATE
{
    STATE_UNKNUWN,  //δ֪
    STATE_RUNNING,      //�ҵ�
    STATE_ASSERTING,//�ж���
    STATE_STOP,     //ֹͣ��
    STATE_LOCAL_OUT_TIME,//���ؼ�ⳬʱ
    STATE_OUT_TIME, //������ʱ
    STATE_REBOOTING, //������
    STATE_MONITOR_HEART,//����������
};

enum BACK_MSG_TYPE
{
    DELETE_BACK_MSG,//ɾ����Ϣ��ִ
};

enum OPREATE_STATE
{
    FAILED_OPREATE = -1,
    OK_OPREATE
};

//���ݰ�ͷ
typedef struct  nodemsg_header_t
{
    MSG_TYPE type;//��������
    int      data_len;//���ݳ���
};

//������Ϣ
typedef struct nodemsg_heart
{
    nodemsg_header_t header;//��Ϣͷ
    unsigned int     pid;//����ID
    char     ppath[1024];   //����·��

    nodemsg_heart()
    {
        header.type = HEARTS_TYPE;
        header.data_len = sizeof(nodemsg_heart) - sizeof(nodemsg_header_t);
    }
};

//ע����Ϣ
typedef struct nodemsg_register
{
    nodemsg_header_t header;//��Ϣͷ
    char     ppath[1024];   //����·��
    unsigned int pid;       //����Pid
    unsigned int timeout_reboot;//0��ʱ������,��������
    unsigned int heart_check_tick;//������ʱ����ʱ��

    nodemsg_register()
    {
        header.type = REGISTER_TYPE;
        header.data_len = sizeof(nodemsg_register) - sizeof(nodemsg_header_t);
        timeout_reboot = 0;
        heart_check_tick = 9000;
    }
};

//ǰ̨��-��������

//����
//����״̬�ϱ�
typedef struct redis_process_state 
{
     char           ppath[1024];    //����·��
     char           ip[32];         //������ip
     unsigned int   pid;            //����Pid
     unsigned int   memcount;       //�����ڴ�
     PROCESS_STATE  state;          //����״̬
     redis_process_state()
     {
        state = STATE_UNKNUWN;
        memset(this, 0, sizeof(redis_process_state));
     }
};

//��ػ�ִ
typedef struct  monitor_back
{
    char            ppath[1024];    //����·��
    BACK_MSG_TYPE   opreatetye;     //��ִ����
    OPREATE_STATE   opreate_result; //ִ��״̬
};

//��־��Ϣ
enum LOG_TYPE
{
    AUTO_START,//�Զ�����
    UNAUTO_START,//�ֶ�����
    ASSERTING_,//����
    HEART_OUTTM,//������ʱ
    UNAUTO_DIE_,//�ֶ��ر�
    AUTO_DIE,//���ֶ����̹ر�
    
};
typedef struct  montior_log_redis
{
    string s_ip;//ip
    string s_process;//��������
    LOG_TYPE log_state;//����״̬
    long    s_time;//ʱ��
    string s_info;//��Ϣ,�Զ��������ֶ����������ԣ�
    int     n_type;//��չ����������
    string  s_data_exten;//��չ����
};

//����
//���̲���
typedef struct  opreate_process
{
    char           ppath[1024];     //����·��
    char           ip[32];         //������ip
    MSG_TYPE       msgtype;         //��Ϣ����
    opreate_process()
    {
        memset(this, 0, sizeof(opreate_process));
    }
};



#pragma  pack(pop , enter_includeglobal)

#endif