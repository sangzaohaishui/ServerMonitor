#ifndef PROCESSNODEPRESERCHECK_H
#define PROCESSNODEPRESERCHECK_H

#include <QWidget>
#include <QIcon>
#include <QTreeWidgetItem>
#include "windows.h"
#include "globaldefine.h"
#include "8Predis.h"
#include <map>
#include < QList >
#include <QAction>
#include "stdio.h"
#pragma comment(lib, "Advapi32")
using namespace std;

class GodTreeItem : public QTreeWidgetItem
{
public:
    enum itemtype
    {
        EnumParent,//���ڵ�
        EnumProcess,//���̽ڵ�
    };
    GodTreeItem(itemtype type = EnumParent)
    {
        m_Type = type;
    }
    //��ȡ�ڵ�����
    itemtype GetType()
    {
        return m_Type;
    }
protected:
    itemtype m_Type;
};
class ProcessParentNode;
//���̽ڵ�
class ProcessNode:public GodTreeItem
{
public:
    ProcessNode(ProcessParentNode* parent = 0);
public:
    void    InitTreeItem();                         //��ʼ��TreeItem
    void    InitProcess(const QString& sProcessName,  ProcessParentNode* hostNode);//��ʼ������
    void    SetNodeState(PROCESS_STATE  state, bool bUpTm = true);             //����Item��״̬
    void    SetMemory(int nMemory);
    void   SetPid(int nPid);
    void    SetErr(const QString& sErr);

public:
    QIcon       m_SelfCheckRes[7]; //�Լ�״̬
    QString     m_ProcessName;//��������
    ProcessParentNode*  m_Host;//����
    quint64      m_clock;
};

//������ʶ
class ProcessParentNode:public GodTreeItem
{
public:
    ProcessParentNode(const QString& sParentName,QTreeWidget* parent = 0);
public:
    void    InitTreeItem();                         //��ʼ��TreeItem
    void    SetHostFlag();//��������IP
    bool     IsExitsProcess(const QString& sProcess);
    void    AddNode(const QString& sProcessName,int nPid, int nMemoey, PROCESS_STATE  state = STATE_UNKNUWN);  //����һ���ڵ㣨�������ƺ�·����
    void    SetNodeState(const QString& sProcessName, PROCESS_STATE  state);             //����Item��״̬���������ƺ�״̬��
    void    SetState(PROCESS_STATE  state = STATE_UNKNUWN, bool bUpTm = true);
    void    SetErr(const QString& sProcessName, const QString& sErr);//���ñ�����Ϣ���������ƣ��ʹ�����Ϣ��
public:
    QString  m_ParentName;  //���ڵ�����(��ӦIP)
    map<QString,ProcessNode*>  m_SelfCheckItem;//�Լ�Item
    quint64      m_clock;
    QIcon       m_SelfCheckRes[7]; //�Լ�״̬

};

//����״̬
class ProcessNodePreSerCheck : public QTreeWidget
{
    Q_OBJECT

public:
    ProcessNodePreSerCheck(ProcessCtrClientRedis* redis , QTreeWidget *parent = 0);
    ~ProcessNodePreSerCheck();
    //���ӽڵ�(�����ڵ㣬�������ƣ�����·��������״̬)
    void AddNode(const QString& sHost, const QString& sProcessName, int nPid, int nMemoey, PROCESS_STATE state = STATE_UNKNUWN);
    //�����Ƿ��Ƿ����1,���������ڣ�2���̲�����
    int IsExitsProcess(const QString& sHost, const QString& sProcess);
    //ɾ������
    int DelHost(const QString& sHost);
    //ע������//�Ӽ�������ɾ����Ӧ�Ľڵ�
    int DelProcess(const QString& sHost, const QString& sProcess);
    int Stop(const QString& sHost, const QString& sProcess);
    int Start(const QString& sHost, const QString& sProcess);
    //���ڵ���Ϣ
    void Check();

signals:
    void sigLogQuery(const QString& sProcess, const QString& sIp);
private slots:
        void onMenu(const QPoint&);
            void  onDel();
            void  onStop();
            void  onStart();
            void  onLog();
private:
    map<QString, ProcessParentNode*>    m_HostNode;
    QString m_Process;
    QString m_Ip;
    QAction* m_pAdd ;
    QAction* m_pStop;
    QAction* m_pDel;
    QAction* m_pLog;
    ProcessCtrClientRedis* m_Redis;
};

#endif // PROCESSNODEPRESERCHECK_H
