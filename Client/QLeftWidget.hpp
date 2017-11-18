#ifndef QLeftWidget_HPP
#define QLeftWidget_HPP

/**
  *@brief: �󴰿�
  *
  *@param:
  *
  *@author: zhaoHP
  *
  *@date: 2016/10/22
  */

#include <QWidget>

#include "ui_QLeftWidget.h"
#include "QHeaderGround.hpp"

class QFlashList;

class QLeftWidget: public QWidget
{
    Q_OBJECT
public:
    QLeftWidget(QWidget* paren = NULL);

    virtual void focusOutEvent(QFocusEvent *event);

    //��������б�ڵ�
    void addThreamNode(const QString& strName,
                       int serial,
                       QString NormalIcon = "",
                       QString PressIcon = "");

    //��������б�ڵ�
    void addSetNode(const QString& strName,
                       int serial,
                       QString NormalIcon = "",
                       QString PressIcon = "");
    //���ñ���ͷ����ͼƬ
    void setHeadBackPng(QString& strBackPng);

    void AnimationLeft(bool flag);

public slots:
    void showEvent(QShowEvent *_event);

    void onCloseAnima();

signals:
    void sigItem(const QString& , long);

private:
   // QFlashList*     mFlashList;
    QHeaderGround*  mHeaerGround;
    QFlashList*     mThreamList; //�����б�
    //QFlashList*     mSetList;//�����б�
    QSize       mNormalSize;
    Ui::QLeftWidget ui;
};

#endif // QLeftWidget_HPP
