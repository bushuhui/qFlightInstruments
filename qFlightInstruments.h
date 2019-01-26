#ifndef QFLIGHTINSTRUMENTS_H
#define QFLIGHTINSTRUMENTS_H

#include <QMap>
#include <QMutex>
#include <QTableWidget>
#include <QWidget>

class NumberReceiver;

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///
/// \brief The Attitude indicator class
///
class QADI:
        public QWidget
{
    Q_OBJECT

public:
    explicit QADI(QWidget* parent = nullptr);
    virtual ~QADI() = default;

    ///
    /// \brief Set roll & pitch values (in degree)
    /// \param roll  - roll
    /// \param pitch - pitch
    ///
    void setData(double pitch, double roll)
    {
        setVarPitch(pitch);
        setVarRoll(roll);

        emit canvasReplot();
    }

    ///
    /// \brief Set roll angle (in degree)
    /// \param val - roll
    ///
    void setRoll(double val)
    {
        if(setVarRoll(val))
        {
            emit canvasReplot();
        }
    }

    ///
    /// \brief Set pitch value (in degree)
    /// \param val
    ///
    void setPitch(double val)
    {
        if(setVarPitch(val))
        {
            emit canvasReplot();
        }
    }

    ///
    /// \brief Get roll angle (in degree)
    /// \return roll angle
    ///
    double getRoll() {return m_roll;}

    ///
    /// \brief Get pitch angle (in degree)
    /// \return pitch angle
    ///
    double getPitch(){return m_pitch;}

signals:
    void canvasReplot();

protected slots:
    void canvasReplot_slot();

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

protected:
    int     m_sizeMin, m_sizeMax;        // < widget's min/max size (in pixel)
    int     m_offset, m_size;            // < current size & offset

    double  m_roll;                      // < roll angle (in degree)
    double  m_pitch;                     // < pitch angle (in degree)

private:
    bool setVarPitch(double val);
    bool setVarRoll(double val);

private:
    NumberReceiver* modelRoll;
    NumberReceiver* modelPitch;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///
/// \brief The Compass & altitude display class
///
class QCompass:
        public QWidget
{
    Q_OBJECT

public:
    explicit QCompass(QWidget* parent = nullptr);
    virtual ~QCompass() = default;

    ///
    /// \brief Set all data (yaw, alt, height)
    ///
    /// \param yaw    - yaw ( in degree)
    /// \param alt    - altitude ( in m)
    /// \param height - height from ground (in m)
    ///
    void setData(double alt, double height, double yaw)
    {
        setVarAlt(alt);
        setVarH(height);
        setVarYaw(yaw);

        emit canvasReplot();
    }

    ///
    /// \brief Set yaw angle (in degree)
    /// \param val - yaw angle (in degree)
    ///
    void setYaw(double val)
    {
        if(setVarYaw(val))
        {
            emit canvasReplot();
        }
    }

    ///
    /// \brief Set altitude value
    /// \param val - altitude (in m)
    ///
    void setAlt(double val)
    {
        if(setVarAlt(val))
        {
            emit canvasReplot();
        }
    }

    ///
    /// \brief Set height from ground
    /// \param val - height (in m)
    ///
    void setH(double val)
    {
        if(setVarH(val))
        {
            emit canvasReplot();
        }
    }

    ///
    /// \brief Get yaw angle
    /// \return yaw angle (in degree)
    ///
    double getYaw() {return m_yaw;}

    ///
    /// \brief Get altitude value
    /// \return altitude (in m)
    ///
    double getAlt() {return m_alt;}

    ///
    /// \brief Get height from ground
    /// \return height from ground (in m)
    ///
    double getH() {return m_h;}

signals:
    void canvasReplot();

protected slots:
    void canvasReplot_slot();

protected:
    void paintEvent(QPaintEvent* event);
    void resizeEvent(QResizeEvent* event);

protected:
    int     m_sizeMin, m_sizeMax;        // < widget min/max size (in pixel)
    int     m_offset, m_size;            // < widget size and offset size

    double  m_yaw;                       // < yaw angle (in degree)
    double  m_alt;                       // < altitude (in m)
    double  m_h;                         // < height from ground (in m)

private:
    bool setVarYaw(double val);
    bool setVarAlt(double val);
    bool setVarH(double val);
};


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

using ListMap = QMap<QString, QString>;

///
/// \brief The List view class, it will display key-value pair in lines
///
class QKeyValueListView :
        public QTableWidget
{
public:
    Q_OBJECT

public:
    explicit QKeyValueListView(QWidget* parent = nullptr);
    virtual ~QKeyValueListView();

    ///
    /// \brief Set list data
    /// \param d - list data
    ///
    void setData(const ListMap& d) {
        m_data = d;
        emit listUpdate();
    }

    ///
    /// \brief Get list data
    /// \param d - list data obj
    ///
    ListMap& getData() {
        return m_data;
    }

    void beginSetData() {
        m_mutex->lock();
    }

    void endSetData() {
        m_mutex->unlock();
    }

    ///
    /// \brief Reloat data to table widget
    ///
    void listReload() {
        emit listUpdate();
    }

signals:
    void listUpdate();

protected slots:
    void listUpdate_slot();

protected:
    ListMap m_data;
    QMutex* m_mutex;
};

#endif // end of QFLIGHTINSTRUMENTS_H
