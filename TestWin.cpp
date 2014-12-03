#include <stdio.h>
#include <stdlib.h>

#include <QtCore>
#include <QtGui>
#include <QBoxLayout>
#include <QVBoxLayout>


#include "TestWin.h"


TestWin::TestWin(QWidget *parent) : QWidget(parent)
{
    // setup layout
    setupLayout();

    // set help message
    QString szHelp =
            QString("Demo usage:\n") +
            QString("----------------------------\n") +
            QString("UP    - Pitch +\n") +
            QString("DOWN  - Pitch -\n") +
            QString("LEFT  - Roll -\n") +
            QString("RIGHT - Roll +\n") +
            QString("A     - Yaw +\n") +
            QString("D     - Yaw -\n") +
            QString("W     - Alt +\n") +
            QString("S     - Alt -\n") +
            QString("J     - H +\n") +
            QString("K     - H -\n");
    m_helpMsg->setText(szHelp);
    m_helpMsg->setFont(QFont("DejaVu Sans YuanTi Mono", 10));

    // set default info
    m_infoList->getData()["roll"]  = QString("%1").arg(m_ADI->getRoll());
    m_infoList->getData()["pitch"] = QString("%1").arg(m_ADI->getPitch());
    m_infoList->getData()["yaw"]   = QString("%1").arg(m_Compass->getYaw());
    m_infoList->getData()["alt"]   = QString("%1").arg(m_Compass->getAlt());
    m_infoList->getData()["H"]     = QString("%1").arg(m_Compass->getH());
    m_infoList->listReload();

    // set window minimum size
    this->setMinimumSize(800, 600);

    // set focus to this window
    this->setFocus();

    // window title
    setWindowTitle("Demo of Flight Instruments");
}

TestWin::~TestWin()
{

}

int TestWin::setupLayout(void)
{
    // right pannel
    m_helpMsg = new QTextEdit(this);
    m_helpMsg->setReadOnly(true);
    m_helpMsg->setFocusPolicy(Qt::NoFocus);

    // left pannel
    QWidget *wLeftPanel = new QWidget(this);
    QVBoxLayout *vl = new QVBoxLayout(wLeftPanel);
    wLeftPanel->setLayout(vl);
    wLeftPanel->setFocusPolicy(Qt::NoFocus);

    m_ADI      = new QADI(this);
    m_Compass  = new QCompass(this);
    m_infoList = new QKeyValueListView(this);

    vl->addWidget(m_ADI,      0, Qt::AlignTop|Qt::AlignHCenter);
    vl->addWidget(m_Compass,  0, Qt::AlignTop|Qt::AlignHCenter);
    vl->addWidget(m_infoList, 2, 0);
    vl->setMargin(0);
    vl->setSpacing(4);

    // overall layout
    QHBoxLayout *hl = new QHBoxLayout(this);
    this->setLayout(hl);

    hl->addWidget(m_helpMsg, 1);
    hl->addWidget(wLeftPanel, 0);

    return 0;
}


void TestWin::keyPressEvent(QKeyEvent *event)
{
    int     key;
    double  v;

    key = event->key();

    if( key == Qt::Key_Up ) {
        v = m_ADI->getPitch();
        m_ADI->setPitch(v+1.0);
    } else if ( key == Qt::Key_Down ) {
        v = m_ADI->getPitch();
        m_ADI->setPitch(v-1.0);
    } else if ( key == Qt::Key_Left ) {
        v = m_ADI->getRoll();
        m_ADI->setRoll(v-1.0);
    } else if ( key == Qt::Key_Right ) {
        v = m_ADI->getRoll();
        m_ADI->setRoll(v+1.0);
    } else if ( key == Qt::Key_A ) {
        v = m_Compass->getYaw();
        m_Compass->setYaw(v+1.0);
    } else if ( key == Qt::Key_D ) {
        v = m_Compass->getYaw();
        m_Compass->setYaw(v-1.0);
    } else if ( key == Qt::Key_W ) {
        v = m_Compass->getAlt();
        m_Compass->setAlt(v+1.0);
    } else if ( key == Qt::Key_S ) {
        v = m_Compass->getAlt();
        m_Compass->setAlt(v-1.0);
    } else if ( key == Qt::Key_J ) {
        v = m_Compass->getH();
        m_Compass->setH(v+1.0);
    } else if ( key == Qt::Key_K ) {
        v = m_Compass->getH();
        m_Compass->setH(v-1.0);
    }

    m_infoList->getData()["roll"]  = QString("%1").arg(m_ADI->getRoll());
    m_infoList->getData()["pitch"] = QString("%1").arg(m_ADI->getPitch());
    m_infoList->getData()["yaw"]   = QString("%1").arg(m_Compass->getYaw());
    m_infoList->getData()["alt"]   = QString("%1").arg(m_Compass->getAlt());
    m_infoList->getData()["H"]     = QString("%1").arg(m_Compass->getH());
    m_infoList->listReload();
}

void TestWin::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
}

void TestWin::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
}

