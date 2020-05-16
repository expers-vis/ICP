/*! \file mainwindow.h
 * \brief Header for main window
 *
 * \author Martin Hiner - xhiner00
 * \author Adam Ševčík - xsevci64
 *
 * \date 17.5.2020
 *
 * Header file for main part of the application.
 * Main focus is on Graphics Scene - the map and user interface - how can user interact with application.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "linelib.h"
#include "timetabledisplay.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; class MyScene;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();  

private:
    /*!
     * \brief User Interface
     *
     * Contains setup of user inteface
     */
    Ui::MainWindow *ui;

    /*!
     * \brief New dialog window for timetables
     *
     * When timetable is requested, new dialog window containing the timetable is opened using this pointer.
     */
    timetableDisplay *timetableObj;

    /*!
     * \brief Timer for time simulation
     *
     * Simulates passing of one second.
     * Works by counting down on supplied time interval in milliseconds.
     * Default inteval is 1000 - 1 real time secon.
     * By changing this interval, simulated time can be sped up or slowed down.
     */
    QTimer *timer;

    /*!
     * \brief Internal time
     *
     * Simulated time in seconds.
     * This is set at the start of application and incremented after every timer interval.
     * Used for time displaying and bus synchronisation.
     */
    long int time = 0;

    /*!
     * \brief Speed of time
     *
     * Used to speed up and slow down time by shortening and lengthening the timer interval respectively.
     * Handled like a percent value of one second.
     */
    int time_speed = 100;

    /* graphic options */
    QPen street_default;
    QPen street_highlight;

    QPen stop_pen_default;
    QPen stop_pen_highlight;
    QBrush stop_brush_default;
    QBrush stop_brush_hightlight;

    QPen bus_pen_default;
    QPen bus_pen_highlight;

    QBrush bus_brush_1;
    QBrush bus_brush_2;
    QBrush bus_brush_4;
    QBrush bus_brush_20;

    QBrush bus_brush_highlight;

    //! Vector containig all streets on map
    QVector<t_street*> *street_list;

    //! Vector containing all stops on map
    QVector<t_stop*> *stop_list;

    //! Container for line no. 1
    t_line *line1;

    //! Container for line no. 2
    t_line *line2;

    //! Container for line no. 4
    t_line *line4;

    //! Container for line no. 20
    t_line *line20;

private:
    void initScene();
    void initSceneStreets(QGraphicsScene* scene);
    void initSceneBuses(QGraphicsScene* scene);
    void initSceneStops(QGraphicsScene* scene);
    void initTimer();
    void initPens();
    void connectSignals();
    void initSelectBox();
    void highlight_line(t_line*);
    void drop_highlight_line(t_line*);
    inline QString formatTime(long int);
    inline void changeTimeSpeed(int);

private slots:
    /*!
     * \brief Zoom in slot
     *
     * Zooms in on the map in Graphics Scene.
     * Recieves from Zoom In button.
     */
    void zoomin();

    /*!
     * \brief Zoom out slot
     *
     * Zooms out on the map in Graphics Scene.
     * Recievse from Zoom Out button.
     */
    void zoomout();

    /*!
     * \brief General zoom slot
     *
     * Zooms in or out of map by tranforming the Graphics Scene matrix using supplied coeficient.
     *
     * \param z Transformation coeficient
     */
    void zoom(int z);

    /*!
     * \brief Timer event slot
     *
     * Used to carry out actions every time simulated second passes.
     * In this slot function, every bus is told to move on its route.
     * Time counter is also incremented here.
     */
    void timerAction();
    void speedUp();
    void speedDown();
    void speedNorm();
    void highlight(int);
    void showTimetable();
    void findBus(QGraphicsEllipseItem*);

    void on_startBtn_clicked();

signals:
    void timetableNumber(int);
    void timetableBus(long int, t_bus*);
};

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = nullptr);

public:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void sendBus(QGraphicsEllipseItem*);
};

#endif // MAINWINDOW_H
