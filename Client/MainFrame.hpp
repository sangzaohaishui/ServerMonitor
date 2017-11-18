#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

/**
  *@brief: ������
  *
  *@param:
  *
  *@author: zhaoHP
  *
  *@date: 2016/10/22
  */

#include <QWidget>


enum enumCenterType
{
    eWidgetPop,//����ʽ
    eWidgetEmbed//Ƕ�����Ĵ���

};

typedef QPair<enumCenterType , QWidget*> ExtenType;

typedef QMap<long, ExtenType> WidgetType;

class QShadeTopWidget;
class MainHeader;
class QLeftWidget;
class QRightAction;
class QSliderButton;
class QCenterWidget;

#include "ui_MainFrame.h"

class MainFrame : public QWidget
{
    Q_OBJECT

public:
    explicit MainFrame(QWidget *parent = 0);
    ~MainFrame();
    virtual void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *e);

    //�ڴ���������ӽ���
    void addCenterWidget(const QString& title,
                         const QString& ojbName,
                         int serial,
                         enumCenterType type,
                         QWidget *widget);
    //������ô���
    void addSetWidget(const QString& title,
                      const QString& ojbName,
                      int serial,
                       enumCenterType type,
                      QWidget *widget);

    void testWidget();
public slots:
    void onMaxMinized();
    void onMove(QPoint &);
    void onShowLight();
    void onItemClicked(const QString &, long);
    void onShowRight();
    void onSetWindowTle(const QString&);
private:
    QShadeTopWidget*    mLightBox;
    MainHeader*         mHeader;
    QLeftWidget*        mLeftList;
    QRightAction*       mRightAct;
    QCenterWidget*      mCenter;//�м䴰��
    WidgetType          mWidgetType;//��������
    QPoint  mTgtPnt;
    QPoint  mDragPnt;
    QPoint  mResizeDown;
    QRect   mMouseDown;
    Ui::MainFrame ui;
    bool    mBDrag;
    bool    mMoved;
};

#endif // MAINFRAME_HPP
