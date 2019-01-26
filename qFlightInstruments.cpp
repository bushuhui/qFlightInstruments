#include "qFlightInstruments.h"

#include <QtCore>
#include <QtGui>
#include <QHeaderView>
#include <QTableWidget>

#include <cstdio>
#include <cstdlib>
#include <cstring>

static bool isApproxEqual(double one, double two, double precision);

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

QADI::QADI(QWidget* parent):
    QWidget(parent),
    m_sizeMin(200),
    m_sizeMax(600),
    m_offset(2),
    m_size(m_sizeMin - (2*m_offset)),
    m_roll(0.0),
    m_pitch(0.0)
{
    setMinimumSize(m_sizeMin, m_sizeMin);
    setMaximumSize(m_sizeMax, m_sizeMax);
    resize(m_sizeMin, m_sizeMin);

    setFocusPolicy(Qt::NoFocus);

    connect(this, SIGNAL(canvasReplot(void)),
            this, SLOT(canvasReplot_slot(void)));
}

void QADI::canvasReplot_slot()
{
    update();
}

void QADI::resizeEvent(QResizeEvent* event)
{
    m_size = qMin(width(),height()) - (2 * m_offset);

    QWidget::resizeEvent(event);
}

bool QADI::setVarPitch(double val)
{
    double pitch = val;

    if(pitch < -90.0) pitch = -90.0;
    if(pitch > 90.0) pitch = 90.0;

    if(!isApproxEqual(pitch, m_pitch, 0.05))
    {
        m_pitch = pitch;

        return true;
    }

    return false;
}

bool QADI::setVarRoll(double val)
{
    double roll = val;

    if(roll < -180.0) roll = -180.0;
    if(roll > 180.0) roll = 180.0;

    if(!isApproxEqual(roll, m_roll, 0.05))
    {
        m_roll = roll;

        return true;
    }

    return false;
}

void QADI::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QBrush bgSky(QColor(48,172,220));
    QBrush bgGround(QColor(247,168,21));

    QPen whitePen(Qt::white);
    QPen blackPen(Qt::black);
    QPen pitchPen(Qt::white);
    QPen pitchZero(Qt::green);

    whitePen.setWidth(2);
    blackPen.setWidth(2);
    pitchZero.setWidth(3);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width()/2.0, height()/2.0);
    painter.rotate(m_roll);

    // FIXME: AHRS output left-hand values
    qreal pitch_tem = -m_pitch;

    // draw background
    {
        qreal y_min, y_max;

        y_min = m_size/2.0*-40.0/45.0;
        y_max = m_size/2.0* 40.0/45.0;

        qreal y = m_size/2.0*pitch_tem/45.0;
        if( y < y_min ) y = y_min;
        if( y > y_max ) y = y_max;

        qreal x = sqrt(m_size*m_size/4.0 - y*y);
        qreal gr = atan(y/x);
        gr = gr * 180./M_PI;

        painter.setPen(blackPen);
        painter.setBrush(bgSky);
        painter.drawChord(-m_size/2, -m_size/2, m_size, m_size,
                          int(gr*16), int((180-2*gr)*16));

        painter.setBrush(bgGround);
        painter.drawChord(-m_size/2, -m_size/2, m_size, m_size,
                          int(gr*16), int(-(180+2*gr)*16));
    }

    // draw pitch lines & marker
    {
        qreal x, y, x1, y1;
        int textWidth;
        qreal p, r;
        qreal ll = m_size/8.0, l;

        int     fontSize = 8;
        QString s;

        pitchPen.setWidth(2);
        painter.setFont(QFont("", fontSize));

        // draw lines
        for(int i=-9; i<=9; i++)
        {
            p = i*10;

            s = QString("%1").arg(-p);

            if( i % 3 == 0 )
            {
                l = ll;
            }
            else
            {
                l = ll/2;
            }

            if( i == 0 )
            {
                painter.setPen(pitchZero);
                l = l * 1.8;
            }
            else
            {
                painter.setPen(pitchPen);
            }

            y = m_size/2.0*p/45.0 - m_size/2.0*pitch_tem/45.0;
            x = l;

            r = sqrt(x*x + y*y);

            if( r > m_size/2.0 ) continue;

            painter.drawLine(QPointF(-l, 1.0*y), QPointF(l, 1.0*y));

            textWidth = 100;

            if( i % 3 == 0 && i != 0 )
            {
                painter.setPen(QPen(Qt::white));

                x1 = -x-2-textWidth;
                y1 = y - fontSize/2.0 - 1.0;
                painter.drawText(QRectF(x1, y1, textWidth, fontSize+2),
                                 Qt::AlignRight|Qt::AlignVCenter, s);
            }
        }

        // draw marker
        int     markerSize = m_size/20;
        qreal  fx1, fy1, fx2, fy2, fx3, fy3;

        painter.setBrush(QBrush(Qt::red));
        painter.setPen(Qt::NoPen);

        fx1 = markerSize;
        fy1 = 0;
        fx2 = fx1 + markerSize;
        fy2 = -markerSize/2.0;
        fx3 = fx1 + markerSize;
        fy3 = markerSize/2.0;

        QPointF points[3] =
        {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };
        painter.drawPolygon(points, 3);

        QPointF points2[3] =
        {
            QPointF(-fx1, fy1),
            QPointF(-fx2, fy2),
            QPointF(-fx3, fy3)
        };
        painter.drawPolygon(points2, 3);
    }

    // draw roll degree lines
    {
        int     nRollLines = 36;
        qreal   rotAng = 360.0 / nRollLines;
        int     rollLineLeng = m_size/25;
        qreal   fx1, fy1, fx2, fy2;
        int     fontSize = 8;
        QString s;

        blackPen.setWidth(1);
        painter.setPen(blackPen);
        painter.setFont(QFont("", fontSize));

        for(int i=0; i<nRollLines; i++)
        {
            if( i < nRollLines/2 )
            {
                s = QString("%1").arg(-i*rotAng);
            }
            else
            {
                s = QString("%1").arg(360-i*rotAng);
            }

            fx1 = 0;
            fy1 = -m_size/2 + m_offset;
            fx2 = 0;

            if( i % 3 == 0 )
            {
                fy2 = fy1 + rollLineLeng;
                painter.drawLine(QPointF(fx1, fy1), QPointF(fx2, fy2));

                fy2 = fy1 + rollLineLeng+2;
                painter.drawText(QRectF(-50, fy2, 100, fontSize+2),
                                 Qt::AlignCenter, s);
            }
            else
            {
                fy2 = fy1 + rollLineLeng/2;
                painter.drawLine(QPointF(fx1, fy1), QPointF(fx2, fy2));
            }

            painter.rotate(rotAng);
        }
    }

    // draw roll marker
    {
        int   rollMarkerSize = m_size/25;
        qreal fx1, fy1, fx2, fy2, fx3, fy3;

        painter.rotate(-m_roll);
        painter.setBrush(QBrush(Qt::black));

        fx1 = 0;
        fy1 = -m_size/2.0 + m_offset;
        fx2 = fx1 - rollMarkerSize/2.0;
        fy2 = fy1 + rollMarkerSize;
        fx3 = fx1 + rollMarkerSize/2.0;
        fy3 = fy1 + rollMarkerSize;

        QPointF points[3] =
        {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };

        painter.drawPolygon(points, 3);
    }

    QWidget::paintEvent(event);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

QCompass::QCompass(QWidget* parent):
    QWidget(parent),
    m_sizeMin(200),
    m_sizeMax(600),
    m_offset(2),
    m_size(m_sizeMin - 2*m_offset),
    m_yaw(0.0),
    m_alt(0.0),
    m_h(0.0)
{
    setMinimumSize(m_sizeMin, m_sizeMin);
    setMaximumSize(m_sizeMax, m_sizeMax);
    resize(m_sizeMin, m_sizeMin);

    setFocusPolicy(Qt::NoFocus);

    connect(this, SIGNAL(canvasReplot(void)),
            this, SLOT(canvasReplot_slot(void)));
}

void QCompass::canvasReplot_slot()
{
    update();
}

void QCompass::resizeEvent(QResizeEvent* event)
{
    m_size = qMin(width(),height()) - 2*m_offset;

    QWidget::resizeEvent(event);
}

bool QCompass::setVarYaw(double val)
{
    double yaw = val;

    if(yaw < 0.0) m_yaw = 360.0 + m_yaw;
    if(yaw > 360.0) m_yaw = m_yaw - 360.0;

    if(!isApproxEqual(yaw, m_yaw, 0.05))
    {
        m_yaw  = yaw;

        return true;
    }

    return false;
}

bool QCompass::setVarAlt(double val)
{
    if(isApproxEqual(val, m_alt, 0.05)) return false;

    m_alt = val;

    return true;
}

bool QCompass::setVarH(double val)
{
    if(isApproxEqual(val, m_h, 0.05)) return false;

    m_h = val;

    return true;
}

void QCompass::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    QBrush bgGround(QColor(48,172,220));

    QPen whitePen(Qt::white);
    QPen blackPen(Qt::black);
    QPen redPen(Qt::red);
    QPen bluePen(Qt::blue);
    QPen greenPen(Qt::green);

    whitePen.setWidth(1);
    blackPen.setWidth(2);
    redPen.setWidth(2);
    bluePen.setWidth(2);
    greenPen.setWidth(2);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.translate(width() / 2.0, height() / 2.0);


    // draw background
    {
        painter.setPen(blackPen);
        painter.setBrush(bgGround);

        painter.drawEllipse(-m_size/2, -m_size/2, m_size, m_size);
    }


    // draw yaw lines
    {
        int     nyawLines = 36;
        qreal  rotAng = 360.0 / nyawLines;
        int     yawLineLeng = m_size/25;
        qreal  fx1, fy1, fx2, fy2;
        int     fontSize = 8;
        QString s;

        blackPen.setWidth(1);
        painter.setPen(blackPen);

        for(int i=0; i<nyawLines; i++)
        {
            if( i == 0 )
            {
                s = "N";
                painter.setPen(bluePen);

                painter.setFont(QFont("", int(fontSize*1.3)));
            }
            else if ( i == 9 )
            {
                s = "W";
                painter.setPen(blackPen);

                painter.setFont(QFont("", int(fontSize*1.3)));
            }
            else if ( i == 18 )
            {
                s = "S";
                painter.setPen(redPen);

                painter.setFont(QFont("", int(fontSize*1.3)));
            }
            else if ( i == 27 )
            {
                s = "E";
                painter.setPen(blackPen);

                painter.setFont(QFont("", int(fontSize*1.3)));
            }
            else
            {
                s = QString("%1").arg(i*rotAng);
                painter.setPen(blackPen);

                painter.setFont(QFont("", fontSize));
            }

            fx1 = 0;
            fy1 = -m_size/2.0 + m_offset;
            fx2 = 0;

            if ( i % 3 == 0 )
            {
                fy2 = fy1 + yawLineLeng;
                painter.drawLine(QPointF(fx1, fy1), QPointF(fx2, fy2));

                fy2 = fy1 + yawLineLeng+4;
                painter.drawText(QRectF(-50, fy2, 100, fontSize+2),
                                 Qt::AlignCenter, s);
            }
            else
            {
                fy2 = fy1 + yawLineLeng/2.0;
                painter.drawLine(QPointF(fx1, fy1), QPointF(fx2, fy2));
            }

            painter.rotate(-rotAng);
        }
    }

    // draw S/N arrow
    {
        int     arrowWidth = m_size/5;
        qreal  fx1, fy1, fx2, fy2, fx3, fy3;

        fx1 = 0;
        fy1 = -m_size/2.0 + m_offset + m_size/25.0 + 15.0;
        fx2 = -arrowWidth/2.0;
        fy2 = 0;
        fx3 = arrowWidth/2.0;
        fy3 = 0;

        painter.setPen(Qt::NoPen);

        painter.setBrush(QBrush(Qt::blue));
        QPointF pointsN[3] =
        {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };
        painter.drawPolygon(pointsN, 3);

        fx1 = 0;
        fy1 = m_size/2.0 - m_offset - m_size/25.0 - 15.0;
        fx2 = -arrowWidth/2.0;
        fy2 = 0;
        fx3 = arrowWidth/2.0;
        fy3 = 0;

        painter.setBrush(QBrush(Qt::red));
        QPointF pointsS[3] =
        {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };
        painter.drawPolygon(pointsS, 3);
    }


    // draw yaw marker
    {
        int    yawMarkerSize = m_size/12;
        qreal  fx1, fy1, fx2, fy2, fx3, fy3;

        painter.rotate(-m_yaw);
        painter.setBrush(QBrush(QColor(0xFF, 0x00, 0x00, 0xE0)));

        fx1 = 0;
        fy1 = -m_size/2.0 + m_offset;
        fx2 = fx1 - yawMarkerSize/2.0;
        fy2 = fy1 + yawMarkerSize;
        fx3 = fx1 + yawMarkerSize/2.0;
        fy3 = fy1 + yawMarkerSize;

        QPointF points[3] =
        {
            QPointF(fx1, fy1),
            QPointF(fx2, fy2),
            QPointF(fx3, fy3)
        };

        painter.drawPolygon(points, 3);

        painter.rotate(m_yaw);
    }

    // draw altitude
    {
        int     altFontSize = 13;
        int     fx, fy, w, h;
        QString s;
        char    buf[200];

        w  = 130;
        h  = 2*(altFontSize + 8);
        fx = -w/2;
        fy = -h/2;

        blackPen.setWidth(2);
        painter.setPen(blackPen);
        painter.setBrush(QBrush(Qt::white));
        painter.setFont(QFont("", altFontSize));

        painter.drawRoundedRect(fx, fy, w, h, 6, 6);

        painter.setPen(bluePen);
        sprintf(buf, "ALT: %6.1f m", m_alt);
        s = buf;
        painter.drawText(QRectF(fx, fy+2, w, h/2), Qt::AlignCenter, s);

        sprintf(buf, "H: %6.1f m", m_h);
        s = buf;
        painter.drawText(QRectF(fx, fy+h/2, w, h/2), Qt::AlignCenter, s);
    }    

    QWidget::paintEvent(event);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


QKeyValueListView::QKeyValueListView(QWidget* parent):
    QTableWidget(parent)
{
    connect(this, SIGNAL(listUpdate()), this, SLOT(listUpdate_slot()));

    m_mutex = new QMutex();

    // set row & column numbers
    setRowCount(0);
    setColumnCount(2);

    // set no headers
    verticalHeader()->hide();
    horizontalHeader()->hide();

    // set last section is stretch-able
    QHeaderView* HorzHdr = horizontalHeader();
    HorzHdr->setStretchLastSection(true);
    HorzHdr->resizeSection(0, 80);     // set first column width

    // disable table edit & focus
    setEditTriggers(QTableWidget::NoEditTriggers);
    setFocusPolicy(Qt::NoFocus);
}

QKeyValueListView::~QKeyValueListView()
{
    delete m_mutex;
}

void QKeyValueListView::listUpdate_slot()
{
    int                 i, n;
    ListMap::iterator   it;

    QColor              clCL1, clCL2;
    QColor              clB1, clB2;

    int                 fontSize = 8;
    int                 rowHeight = 20;

    clCL1 = QColor(0x00, 0x00, 0xFF);
    clCL2 = QColor(0x00, 0x00, 0x00);
    clB1  = QColor(0xFF, 0xFF, 0xFF);
    clB2  = QColor(0xE0, 0xE0, 0xE0);

    m_mutex->lock();

    n = m_data.size();
    setRowCount(n);
    setColumnCount(2);

    for(i=0, it=m_data.begin(); it!=m_data.end(); i++, it++)
    {
        // set name cell
        if( this->item(i, 0) != nullptr )
        {
            this->item(i, 0)->setText(it.key());
        }
        else
        {
            auto item = new QTableWidgetItem();
            item->setText(it.key());

            item->setTextColor(clCL1);
            if( i % 2 == 0 )
            {
                item->setBackgroundColor(clB1);
            }
            else
            {
                item->setBackgroundColor(clB2);
            }

            item->setFont(QFont("", fontSize));

            this->setItem(i, 0, item);
        }

        // set value cell
        if( this->item(i, 1) != nullptr )
        {
            this->item(i, 1)->setText(it.value());
        }
        else
        {
            auto item = new QTableWidgetItem();
            item->setText(it.value());

            item->setTextColor(clCL2);
            if( i % 2 == 0 )
            {
                item->setBackgroundColor(clB1);
            }
            else
            {
                item->setBackgroundColor(clB2);
            }

            item->setFont(QFont("", fontSize));

            this->setItem(i, 1, item);
        }

        setRowHeight(i, rowHeight);
    }

    m_mutex->unlock();
}

static bool isApproxEqual(double one, double two, double precision)
{
    return fabs(one - two) <= precision;
}
