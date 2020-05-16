/*! \file linelib.h
 * \brief Library of bus line structures
 *
 * \author Martin Hiner - xhiner00
 * \author Adam Ševčík - xsevci64
 *
 * \date 17.5.2020
 *
 * This library contains structures needed for work with each part of bus line such as streets, stops, buses, bus routes and timetables.
*/

#ifndef LINELIB_H
#define LINELIB_H

#include <QMainWindow>
#include <QGraphicsScene>

struct t_line;

/*! \struct t_street
 * \brief Structure for single street belonging to bus line
 *
 * This structure is used to identify the street by it's name and modify the object representing the street within Graphics Scene.
 * All member datat is loaded from file.
 */
struct t_street {
    /*!
     * \brief Street's name
     *
     * This variable is used to store streets name for identification.
     * It is later used when each line claims it's streets for highlighting and construction of route for buses to follow.
     */
    QString name;

    /*!
     * \brief Poiner to the stret object
     *
     * Pointer to the object, a simple line, within the Graphics Scene.
     * This is used to get coordiantes for route making or to modify the object.
     */
    QGraphicsLineItem *obj;
};

/*! \struct t_stop
 * \brief Bus stop structure
 *
 * Structure for storing information about a bus stop.
 * All this data is loaded from file at the start of the application.
 */
struct t_stop {
    /*!
     * \brief Bus stop name
     *
     * Name of the bus stop used for identification of the stop.
     */
    QString name;

    /*!
     * \brief Stop's position initial within Graphics Scene
     *
     * Variable that stores stop's position with which it is created in Graphics Scene.
     * This was necessary as only relative to parent position can be accessed through the object pointer, which is always (0, 0).
     * Position is used by buses to calculate length to the stop and determine if they need to stop at the bus stop.
     */
    QPointF pos;

    /*!
     * \brief Pointer to the bus stop object
     *
     * Points to the object within Graphics Scene representing the stop.
     * Mainly used for highlighting.
     */
    QGraphicsRectItem *obj;
};

/*! \struct t_bus
 * \brief Structure for individual buses
 *
 * Structure represents single bus entity and contains data needed for movement as well as member function to execute this movement.
 */
struct t_bus {
    /*!
     * \brief Identification number of the bus
     *
     * Contains number specifying to which line the bus belongs.
     * This information is loaded from file and is used to assign bus to the corresponding line.
     */
    int line_id;

    /*!
     * \brief Wait delay
     *
     * Used to store how many seconds, more precisely timer ticks, should bus wait before moving.
     * This is used by member function move.
     */
    int delay;

    /*!
     * \brief Delay before strating route
     *
     * Holds how many seconds should bus wait before strating it's route.
     * used for synchronisation of buses within lines.
     */
    int start_delay;

    /*!
     * \brief Index number of the next stop in timetable
     *
     * Stores index of timetable vector, containing time in seconds, that bus needs to wait on the stop.
     */
    int stop_num = 0;

    /*!
     * \brief Index number of the destination point
     *
     * Stores index of route vector, containing points of route.
     * Indexes point that bus is moving towards to - \var dest .
     */
    int idx;

    /*!
     * \brief Number of timer ticks to ignore stop
     *
     * Used to prevent bus from stopping again on the same stop by ignoring the stop for certain time.
     */
    int ign = 0;

    /*!
     * \brief Current position of the bus
     *
     * Holds current positio of the bus in point format.
     * This is used to move the bus icon.
     */
    QPointF c_pos;

    /*!
     * \brief Point the bus is moving towards to.
     *
     * Holds point of route bus is moving to.
     * If this point is reached new one is selected from route vector using variable \var idx.
     */
    QPointF dest;

    /*!
     * \brief One-time initialisation flag
     *
     * Used to initialise destination point by loading it from route index.
     */
    bool init = true;

    /*!
     * \brief Hide flag
     *
     * If true, bus is hidden.
     * This is used to hide buses when they are not on route.
     */
    bool hide = false;

    /*!
     * \brief Pointer to the bus object
     *
     * Points to the bus icon in Graphics Scene.
     * Used for highligting and updating position when moving.
     */
    QGraphicsEllipseItem *obj;

    /*!
     * \brief Member function executing movement
     *
     * This fuction is used to move bus on the map depending on line's route.
     * At first call, destination point is selected from line's route vector.
     * Each call length from buses position to destination point is calculated and bus is moved towards that poin.
     * If the destination point is reached, new one is selected.
     * Function also calculates length to the line's stops and stopping if stop is reached.
     *
     * \param line gives bus acess to the line's route and timetable vector
     * \param time current time, used to set wait time
     */
    void move(t_line* line, int time);
};


/*! \struct t_line
 * \brief Structure containig all of the data of single bus line
 *
 * This structure holds information about every street, stops and buses belonging to the line.
 * Mamber functions are used to load data either from existing structuress or files.
 */
struct t_line {
    /*!
     * \brief Line identification number
     *
     * Used as identity for line.
     * Buses are assigned based on this number
     */
    int id;

    /*!
     * \brief Streets belonging to the bus line
     *
     * Streets are loaded based on file from vector containig all the streets.
     */
    QVector<t_street*> streets;

    /*!
     * \brief Stops belonging to the bus line
     *
     * Stops are loaded based on file from vector containig all the stops.
    */
    QVector<t_stop*> stops;

    /*!
     * \brief Buses belonging to the bus line
     *
     * Buses are loaded straight from file and assigned based on their ID.
     */
    QVector<t_bus*> buses;

    /*!
     * \brief Line route for buses
     *
     * This vector contains corner points representing route bus will move on.
     * Route is constructed from streets belonging to line and are ordered in cycle.
     */
    QVector<QPointF> route;

    /*!
     * \brief Line timetable
     *
     * Contains time in seconds of buses arrival to each stop in bus line.
     */
    QVector<int> timetable;

    /*!
     * \brief Set line identification number
     *
     * \param _id line's identification number
     */
    void setId(int _id) {
        t_line::id = _id;
    };

    /*!
     * \brief Load streets belonging to the bus line
     *
     * Member function loads streets from vector of all streets based on data from file.
     *
     * \param street_list Vector containing all streets on the map.
     */
    void claimStreets(QVector<t_street*> *street_list);

    /*!
     * \brief Load stops belonging to the bus line
     *
     * Member function loads stops from vector of all stops based on data from file.
     *
     * \param stop_list Vector containing all stops on the map.
     */
    void claimStops(QVector<t_stop*> *stop_list);

    /*!
     * \brief Loads timetable for line
     *
     * Member function loads departure times for each stop of line.
     *
     * \param filename Name of the file containing the data
     */
    void getTimetable(QString filename);

    /*!
     * \brief Constructs route out of streets of line
     */
    void makeRoute();

    /*!
     * \brief Structure destructor
     *
     * Frees every bus for line.
     * Streets and stops are freed through vectors containig all of then.
     */
    ~t_line() {
        QVector<t_bus*>::iterator j;

        for(j = t_line::buses.begin(); j != t_line::buses.end(); ++j) {
            delete (*j);
        }
    }
};

#endif // LINELIB_H
