#ifndef __TEST_WIN_H__
#define __TEST_WIN_H__

#include <QtCore>
#include <QtGui>
#include <QTextEdit>

#include "qFlightInstruments.h"


class TestWin : public QWidget
{
public:
    TestWin(QWidget *parent = NULL);
    virtual ~TestWin();

    virtual int setupLayout(void);


protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

protected:
    QADI                *m_ADI;
    QCompass            *m_Compass;
    QKeyValueListView   *m_infoList;

    QTextEdit           *m_helpMsg;
};

#endif // end of __TeST_WIN_H__
