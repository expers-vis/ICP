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
    /*!
     * \brief Connect all signals
     *
     * Function in which all signals are connected
     */
    void connectSignals();
    /*!
     * \brief Initialize selectbox
     *
     * Inserts options for every line to selectbox
     */
    void initSelectBox();
    /*!
     * \brief Highlight line
     *
     * Highlights chosen line and its buses when it´s chosen
     * in selectbox or clicked on. Highlights it by coloring
     * line and buses.
     *
     * \param Line to be highlited
     */
    void highlight_line(t_line*);
    /*!
     * \brief Drop highlight of line
     *
     * Drops highlight of currently highlighted line when
     * you click away or choose another line in selectbox.
     * Changes coloring back to normal.
     *
     * \param Line which is currently highlighted and should be dropped
     */
    void drop_highlight_line(t_line*);
    /*!
     * \brief Time formation
     *
     * Formats time from seconds to hours:minutes:seconds
     *
     * \param Time in seconds which should be formated
     */
    inline QString formatTime(long int);

    /*!
     * \brief Time speed change
     *
     * Cahnges speed of simulated timer depenig on given parameter.
     * Displays current speed.
     *
     * \param Coefficient to change speed
     */
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
    /*!
     * \brief Time speed up slot
     *
     * Speeds up rate of calling funciton timerAction in second
     * every time it´s called until maximum speed is reached.
     */
    void speedUp();
    /*!
     * \brief Time speed down slot
     *
     * Slows down rate of calling function timerAction in second
     * every time it´s called until minimum speed is reached.
     */
    void speedDown();
    /*!
     * \brief Time speed normal slot
     *
     * Sets rate of calling function timerAction in second
     * to normal which is one.
     */
    void speedNorm();
    /*!
     * \brief Highlight line slot
     *
     * Highlights chosen line by coloring its streets and buses
     *
     * \param Index of line to be highlighted
     */
    void highlight(int);
    /*!
     * \brief Timetable displaying slot
     *
     * Displays dialog window with timetable information of chosen line
     * selected in dropbox
     */
    void showTimetable();
    /*!
     * \brief Bus searching slot
     *
     * Looks for bus given in parameter in all lines and decides
     * to which one he belongs to to decide what timetable should be
     * displayed.
     *
     * \param Bus which timetable should be shown
     */
    void findBus(QGraphicsEllipseItem*);
    /*!
     * \brief Start simulation slot
     *
     * Starts simulation based on chosen starting time.
     * Move all buses to correct positions.
     * Shows all controls of simulation.
     */
    void on_startBtn_clicked();

signals:
    /*!
     * \brief Line timetable signal
     *
     * Carries index of line to be displayed in timetable
     * dialog window
     *
     * \param Index of line
     */
    void timetableNumber(int);
    /*!
     * \brief Bus timetable siganl
     *
     * Carries current time in which timetable
     * should be displayed to locate current postion
     * and bus whose timetable should be displayed.
     *
     * \param Current time
     * \param Chosen bus
     */
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
