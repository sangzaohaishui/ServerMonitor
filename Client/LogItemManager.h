#ifndef LOGITEMMANAGER_H
#define LOGITEMMANAGER_H

#include <QWidget>
#include "ui_LogItemManager.h"
#include "globaldefine.h"
#include "QTreeWidgetItem"
class GodLogTreeItem : public QTreeWidgetItem
{
public:
    enum itemtype
    {
        EnumLogType,//��ʶ��־���ͽڵ�
        EnumLogData,//��־���ݽڵ�
    };
    GodLogTreeItem(itemtype type = EnumLogType)
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
//��־���ݽڵ�
class ProcessLogNode:public GodLogTreeItem
{
public:
    ProcessLogNode(ProcessParentNode* parent = 0);
public:


public:
    montior_log_redis log;
};

//��־���ͽڵ�
class ProcessLogParentNode:public GodLogTreeItem
{
public:
    ProcessLogParentNode(QTreeWidget* parent = 0);
    ~ProcessLogParentNode();
public:
    void    AddNode(const montior_log_redis& log_);  //����һ���ڵ㣨�������ƺ�·����
    void    clearNode();
    int     getCount();
public:
    map<QString,ProcessLogNode*>  m_SelfCheckItem;//�Լ�Item
};

class LogItemManager : public QTreeWidget
{
    Q_OBJECT

public:
    LogItemManager(QWidget *parent = 0);
    ~LogItemManager();
    void clearNode();
    void addNode(const montior_log_redis& log_);
private:
    Ui::LogItemManager ui;
    map<LOG_TYPE, ProcessLogParentNode*> m_Nodes;
    //��־����
    map<string, montior_log_redis> m_datas;
};

#endif // LOGITEMMANAGER_H
