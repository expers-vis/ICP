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

/*!
 * \brief The Main Window class
 *
 * This class is dedicated to running the simulation task and operating the user inteface.
 * Contains declarations of:
 * containers and other various objects needed for simulation,
 * functions to initialise the map and run the simulation,
 * slots and signals to respond to user input.
 */
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
    /*!
     * \brief Initialise Graphics Scene
     *
     * Creates Graphics Scene and fills it with with objects to create a map.
     * Created objects are assingned to their bus line containers.
     */
    void initScene();

    /*!
     * \brief Initialise Streets
     *
     * Streets of map are loaded from file and inserted into Graphics Scene to form map.
     * Data for each street is stored in their container structure \see t_street
     * All streets are stored in their vector from which they are assigned to their bus line.
     * After streets are assigned, lines are then prompted to construct their bus route.
     *
     * \pre File named "streets" with data in format "street_name x1 y1 x2 y2"
     *
     * \param scene Pointer to the scene, streets are to be inserted into
     */
    void initSceneStreets(QGraphicsScene* scene);

    /*!
     * \brief Initialise Buses
     *
     * Buses are loaded from file and assigned to their bus line based on their bus ID.
     * All bus data is stored in their container \see t_bus
     * Before inserting intot the Graphics Scene, 5px are subtracted from their x and y values to center the icon.
     *
     * \pre File named "busess" with data in format "line_id start_delay x y"
     *
     * \param scene Pointer to the scene, buses are to be inserted into
     */
    void initSceneBuses(QGraphicsScene* scene);

    /*!
     * \brief Initialise Bus Stops
     *
     * Inserts bus stops in Graphics Scene.
     * Data for each stop is stored in their container structure \see t_stop
     * All stops are stored in their vector from which they are assigned to their bus line.
     *
     * \pre File named "stops" with data in format "stop_name x y"
     *
     * \param scene Pointer to the scene, streets are to be inserted into
     */
    void initSceneStops(QGraphicsScene* scene);

    /*!
     * \brief Initialise Timer
     *
     * Creates timer with default time interval 1 second.
     * Timer is then connected witch slot to update bus position every simulated second.
     */
    void initTimer();

    /*!
     * \brief Initialise graphic options
     *
     * Initialises outline and fill options to color code the map in Graphics Scene.
     */
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
     * Inserts options for every line to selectbox.
     */
    void initSelectBox();

    /*!
     * \brief Highlight line
     *
     * Highlights chosen line's route, stops and buses.
     * All previous highlights are cleared.
     *
     * \pre Graphic options have to be initialised.
     *
     * \param line Pointer to line container to be highlighted
     */
    void highlight_line(t_line *line);

    /*!
     * \brief Drop highlight of line
     *
     * Clears highlight of currently highlighted line.
     *
     * \pre Graphic options need to be initialised.
     *
     * \param line Pointer to line container to clear highlight
     */
    void drop_highlight_line(t_line *line);
    /*!
     * \brief Format time from seconds to readable format
     *
     * Formats time from seconds to HH:MM:SS format returned as string.
     *
     * \param sec Time in seconds to format
     * \return String of the formatted time
     */
    inline QString formatTime(long int sec);

    /*!
     * \brief Changes the speed of simulated time
     *
     * Changes speed of simulated timer by given percents.
     * Negative value slows time, positive speeds it up.
     * Current time speed is then displayed.
     *
     * \param change Change of time speed percents
     */
    inline void changeTimeSpeed(int change);

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
     * selected in dropbox.
     */
    void showTimetable();

    /*!
     * \brief Bus searching slot
     *
     * Recieves bus icon that was clicked on inside the Graphics Scene.
     * Looks for bus represented by recieved icon in all lines.
     * If given bus exists, it's line is highlighted and timetable for this bus is displayed in separate window.
     * If bus doesn't exist, highlight is cleared from map.
     *
     * \param Bus icon from Graphics Scene
     */
    void findBus(QGraphicsEllipseItem*);

    /*!
     * \brief Start simulation slot
     *
     * Starts simulation based on chosen starting time.
     * Graphics Scene map is inititalised and buses are moved to correct positions based on selected time.
     * Shows all controls of simulation.
     */
    void on_startBtn_clicked();

signals:
    /*!
     * \brief Line timetable signal
     *
     * Carries index of line to be displayed in timetable dialog window
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

/*!
 * \brief The MyScene class
 *
 * Graphics Scene class used to capture user input in the form of clicking inside the simulated map.
 */
class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = nullptr);

public:
    /*!
     * \brief Capture mouse click on the map
     *
     * Funcion analyses which object on map was clicked on.
     * If bus icon was clicked on, it is sent via signal to highlight and display timetable.
     * If anything else is clicked on, all highlights are cleared.
     *
     * \param event Information about the click event
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    /*!
     * \brief Send bus that was clicked on
     *
     * Sends bus icon that was clicked on to determine which line it belongs to.
     *
     * \param Pointer to bus icon that was clicked on
     */
    void sendBus(QGraphicsEllipseItem*);
};

#endif // MAINWINDOW_H
