#include "NodeManager.h"
#include "nbclock.h"
#include "stringutil.h"
#include <QMenu>

#define  COVERSTRIN(s)  #s

static QString GetSate(PROCESS_STATE state)
{
    switch(state)
    {
    case STATE_UNKNUWN:
        {
            return QString::fromWCharArray(L"δ֪");
        }
        break;
    case STATE_RUNNING:
        {
             return QString::fromWCharArray(L"����");
        }
        break;
    case STATE_STOP:
        {
             return QString::fromWCharArray(L"ֹͣ");
        }
        break;
    case STATE_OUT_TIME:
        {
             return QString::fromWCharArray(L"���ӳ�ʱ");
        }     
        break;
    case STATE_REBOOTING:
        {
             return QString::fromWCharArray(L"������");
        }
        break;
    case STATE_LOCAL_OUT_TIME:
        {
             return QString::fromWCharArray(L"���س�ʱ");
        }
        break;
    case STATE_ASSERTING:
        {
            return QString::fromWCharArray(L"����");
        }
        break;
    }
}
//////////////////////���̽ڵ�////////////////////////////////////
void ProcessNode::InitTreeItem()
{
    m_SelfCheckRes[STATE_UNKNUWN] = QIcon(":/image/ball_red.png");  //δ֪
    m_SelfCheckRes[STATE_RUNNING] = QIcon(":/image/ball_green.png");//�ҵ�
    m_SelfCheckRes[STATE_ASSERTING] = QIcon(":/image/ball_green.png");//�ҵ�
    m_SelfCheckRes[STATE_STOP] = QIcon(":/image/ball_red.png");    //ֹͣ��
    m_SelfCheckRes[STATE_OUT_TIME] = QIcon(":/image/ball_red.png"); //������ʱ
    m_SelfCheckRes[STATE_REBOOTING] = QIcon(":/image/ball_red.png"); //������
    m_SelfCheckRes[STATE_LOCAL_OUT_TIME] = QIcon(":/image/ball_red.png"); //���ؼ�ⳬʱ
}

ProcessNode::ProcessNode( ProcessParentNode* parent /*= 0*/ ):
GodTreeItem(EnumProcess),
        m_Host(parent)
{
    InitTreeItem();
    SetNodeState(STATE_UNKNUWN);
}

void ProcessNode::InitProcess( const QString& sProcessName,  ProcessParentNode* hostNode )
{
    this->setText(1, sProcessName.toLocal8Bit().data());
    m_Host = hostNode;
    hostNode->addChild(this);
}



void ProcessNode::SetNodeState( PROCESS_STATE  state, bool bUpTm /*= true*/ )
{
    //����ʱ�䣻
    if (bUpTm)
    {
        m_clock = nbclock::now_us() / 1000000;
    }
    setIcon(0, m_SelfCheckRes[state]);
    setData(0, Qt::UserRole, state);
    setText(3, GetSate(state));
    if (STATE_LOCAL_OUT_TIME == state)
    {
        setText(4, "-1 K");
    }
}

void ProcessNode::SetErr( const QString& sErr )
{

}

void ProcessNode::SetMemory( int nMemory )
{
    setText(4,QString::number(nMemory) + " K");
}

void ProcessNode::SetPid( int nPid )
{
    setText(2,QString::number(nPid));
}

//////////////////////////////////////////�����ڵ�///////////////////////////////
void ProcessParentNode::AddNode( const QString& sProcessName,int nPid, int nMemoey,  PROCESS_STATE  state /*= EnumInit*/ )
{
    map<QString,ProcessNode*>::iterator it = m_SelfCheckItem.find(sProcessName);
    if (it != m_SelfCheckItem.end())
    {
        it->second->SetNodeState(state);
        it->second->SetMemory(nMemoey);
        it->second->SetPid(nPid);
    }
    else
    {
        ProcessNode* pNode = new ProcessNode(this);
        pNode->InitProcess(sProcessName, this);
        pNode->SetNodeState(state);
        pNode->SetMemory(nMemoey);
        pNode->SetPid(nPid);
        m_SelfCheckItem[sProcessName] = pNode;
    }
}

void ProcessParentNode::SetNodeState( const QString& sProcessName, PROCESS_STATE state )
{

}

void ProcessParentNode::InitTreeItem()
{
    m_SelfCheckRes[STATE_UNKNUWN] = QIcon(":/image/ball_red.png");  //δ֪
    m_SelfCheckRes[STATE_RUNNING] = QIcon(":/image/ball_green.png");//�ҵ�
     m_SelfCheckRes[STATE_ASSERTING] = QIcon(":/image/ball_green.png");//�ҵ�
    m_SelfCheckRes[STATE_STOP] = QIcon(":/image/ball_red.png");    //ֹͣ��
    m_SelfCheckRes[STATE_OUT_TIME] = QIcon(":/image/ball_red.png"); //������ʱ
    m_SelfCheckRes[STATE_REBOOTING] = QIcon(":/image/ball_red.png"); //������
    m_SelfCheckRes[STATE_LOCAL_OUT_TIME] = QIcon(":/image/ball_red.png"); //���ؼ�ⳬʱ

}

void ProcessParentNode::SetErr( const QString& sProcessName, const QString& sErr )
{

}

ProcessParentNode::ProcessParentNode( const QString& sParentName,QTreeWidget* parent /*= 0*/ ):
GodTreeItem(GodTreeItem::EnumParent)
{
    InitTreeItem();
    setText(0, sParentName);
    SetState(STATE_UNKNUWN, true);
}

bool ProcessParentNode::IsExitsProcess( const QString& sProcess )
{
    //if (m_SelfCheckItem.find(sProcess) != sProcess.end())
    //{
    //    return true;
    //}

    return false;
}

void ProcessParentNode::SetState( PROCESS_STATE state /*= STATUS_UNKUOW*/, bool bUpTm /*= false*/ )
{
    //����ʱ�䣻
    if (bUpTm)
    {
        m_clock = nbclock::now_us() / 1000000;
    }
    setIcon(0,m_SelfCheckRes[state]);
    //setData(0, Qt::UserRole, state);
}

ProcessNodePreSerCheck::ProcessNodePreSerCheck(ProcessCtrClientRedis* redis ,QTreeWidget *parent)
    : QTreeWidget(parent),
    m_Redis(redis)
{
    //ui.setupUi(this);
    QStringList headers;
    headers<<QString::fromWCharArray(L"ϯλ��ַ")<<QString::fromWCharArray(L"��������")
        <<QString::fromWCharArray(L"PID")
        <<QString::fromWCharArray(L"״̬")
        <<QString::fromWCharArray(L"�ڴ�");

    setColumnCount(headers.size());
    setHeaderLabels(headers);
    m_pAdd = new QAction(QString::fromWCharArray(L"��������") , this);
    m_pStop = new QAction(QString::fromWCharArray(L"ֹͣ����"), this);
    m_pDel = new QAction(QString::fromWCharArray(L"ֹͣ���"), this);
    m_pLog = new QAction(QString::fromWCharArray(L"�鿴��־"), this);
    connect(m_pAdd, SIGNAL(triggered(bool )), this, SLOT(onStart()));
    connect(m_pStop, SIGNAL(triggered(bool  )), this, SLOT(onStop( )));
    connect(m_pDel, SIGNAL(triggered(bool  )), this, SLOT(onDel()));
    connect(m_pLog, SIGNAL(triggered(bool  )), this, SLOT(onLog()));

    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint& )), this, SLOT(onMenu(const QPoint& )));
    m_Process   = "";
    m_Ip        = "";

    this->setColumnWidth(0, 120);
    this->setColumnWidth(1, 360);
    this->setColumnWidth(2, 60);
    this->setColumnWidth(3, 40);
    this->setColumnWidth(4, 60);

}

ProcessNodePreSerCheck::~ProcessNodePreSerCheck()
{

}

void ProcessNodePreSerCheck::Check()
{
    quint64  nNow = nbclock::now_us() / 1000000;
    map<QString, ProcessParentNode*>::iterator hIt =  m_HostNode.begin();
    for ( ; hIt != m_HostNode.end(); ++hIt)
    {
        if (fabs((nNow - hIt->second->m_clock) * 1.0) > 25)//��������ڵ�
        {
            hIt->second->SetState(STATE_LOCAL_OUT_TIME, false);
        }

        map<QString,ProcessNode*>::iterator pIt = hIt->second->m_SelfCheckItem.begin();
        for ( ; pIt != hIt->second->m_SelfCheckItem.end(); ++pIt)
        {
            if (fabs((nNow - pIt->second->m_clock) * 1.0) > 20)//�����̽ڵ�
            {
                pIt->second->SetNodeState(STATE_LOCAL_OUT_TIME, false);
            }
        }
    }
}

void ProcessNodePreSerCheck::AddNode( const QString& sHost, const QString& sProcessName, int nPid, int nMemoey,  PROCESS_STATE  state /*= EnumInit*/ )
{

    map<QString, ProcessParentNode*>::iterator hIt =  m_HostNode.find(sHost);
    if (hIt != m_HostNode.end())//��������
    {
        //���������������Ϣ
        if (state != STATE_MONITOR_HEART)
        {
            //��������״̬��Ϣ(Ϊ����)������ʱ��            
            map<QString,ProcessNode*>::iterator pIt = hIt->second->m_SelfCheckItem.find(sProcessName);
            if (pIt != hIt->second->m_SelfCheckItem.end())
            {
                //���½���״̬(����ʱ��)
                pIt->second->SetNodeState(state);
                pIt->second->SetMemory(nMemoey);
                pIt->second->SetPid(nPid);
            }
            else
            {
                //���ӽ��̽ڵ�
                hIt->second->AddNode(sProcessName, nPid, nMemoey, state);
            }
            //��������״̬��Ϣ(Ϊ����)������ʱ��
        }
        hIt->second->SetState(STATE_RUNNING);

    }
    else
    {
        ProcessParentNode* pParent = new ProcessParentNode(sHost);
      

        m_HostNode[sHost] = pParent;

        //���������������Ϣ
        if (state == STATE_MONITOR_HEART)
        {
            //��������״̬��Ϣ(Ϊ����)������ʱ��
            pParent->SetState(STATE_RUNNING);
        }
        else
        {
            pParent->SetState(state);
            //���ӽ��̽ڵ�
            pParent->AddNode(sProcessName, nMemoey, state);
           
        }
         this->addTopLevelItem(pParent);
    }
}


int ProcessNodePreSerCheck::DelHost( const QString& sHost )
{
    return -1;
}

int ProcessNodePreSerCheck::DelProcess( const QString& sHost, const QString& sProcess )
{
    if (m_Redis)
    {
        opreate_process opreate;
        strcpy_s(opreate.ip, sHost.toLocal8Bit().data());
        string sprocess = sProcess.toLocal8Bit().data();
        sprocess = ConvertUtf8ToGBK(sprocess);
        strcpy_s(opreate.ppath,sprocess.c_str());
        opreate.msgtype = DELPROCESS_TYPE;
        m_Redis->SendOpreate(opreate);
        return  0;
    }
    return -1;
}


int ProcessNodePreSerCheck::Stop( const QString& sHost, const QString& sProcess )
{
    if (m_Redis)
    {
        opreate_process opreate;
        strcpy_s(opreate.ip, sHost.toLocal8Bit().data());
        std::string sprocess = sProcess.toStdString();
        sprocess = ConvertUtf8ToGBK(sprocess);
        strcpy_s(opreate.ppath,sprocess.c_str());
        opreate.msgtype = STOPPROCESS_TYPE;
        m_Redis->SendOpreate(opreate);
        return  0;
    }

    return -1;

}

int ProcessNodePreSerCheck::Start( const QString& sHost, const QString& sProcess )
{

    opreate_process opreate;
    strcpy_s(opreate.ip, sHost.toLocal8Bit().data());
    string sprocess = sProcess.toLocal8Bit().data();
    sprocess = ConvertUtf8ToGBK(sprocess);
    strcpy_s(opreate.ppath,sprocess.c_str());
    opreate.msgtype = STARTPROCESS_TYPE;
    m_Redis->SendOpreate(opreate);
    return -1;
}

void ProcessNodePreSerCheck::onMenu(const QPoint& point)
{
    QList<QAction*> actions;
    QTreeWidgetItem *pItem =  itemAt(point);
   
    if (pItem)
    {
        GodTreeItem* pGodItem = (GodTreeItem*)pItem;
        if (pGodItem->GetType() == ::GodTreeItem::EnumProcess)
        {
           ProcessNode *pProcess = (ProcessNode*)pItem;
           if (pProcess->m_Host)
           {
               m_Process   = pGodItem->text(1);
               m_Ip        = pProcess->m_Host->text(0);

               if (m_Process == "" ||
                   m_Ip   == "")
               {
                   return;
               }

               int data = pItem->data(0, Qt::UserRole).toInt();

               switch(data)
               {
               case STATE_RUNNING:      //�ҵ�
               case STATE_ASSERTING:    //����
                   {
                       //ֹͣ
                       //actions.push_back(m_pDel);
                       actions.push_back(m_pStop);
                       actions.push_back(m_pAdd);//����
                        actions.push_back(m_pLog);//����
                       //actions.push_back(m_pDel);//ע��
                   }
                   break;
               case STATE_UNKNUWN:  //δ֪
               case STATE_STOP:     //ֹͣ��
               case STATE_OUT_TIME: //������ʱ
               case STATE_REBOOTING: //������
               case STATE_LOCAL_OUT_TIME:
               
                   {
                       //����
                      // actions.push_back(m_pDel);//ע��
                       actions.push_back(m_pAdd);//����
                       actions.push_back(m_pLog);//����

                   }
                   break;
               }
           }
           
        }
        QMenu::exec(actions, QCursor::pos());
    }
}

void ProcessNodePreSerCheck::onDel()
{
    DelProcess(m_Ip, m_Process);
}

void ProcessNodePreSerCheck::onStop()
{
    Stop(m_Ip, m_Process);
}

void ProcessNodePreSerCheck::onStart()
{
    Start(m_Ip, m_Process);
}

void ProcessNodePreSerCheck::onLog()
{
    sigLogQuery(m_Process, m_Ip);
}
