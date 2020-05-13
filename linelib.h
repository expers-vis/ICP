#ifndef LINELIB_H
#define LINELIB_H

#include <QMainWindow>
#include <QGraphicsScene>

struct t_line;

struct t_street {
    QString name;
    QGraphicsLineItem *obj;
};

struct t_stop {
    QString name;
    QPointF pos;
    QGraphicsRectItem *obj;
};

struct t_bus {
    int line_id;        // id of line it belongs to
    int delay;          // delay starting of the bus, in timer ticks
    int stop_num = 0;   // how many stops passed
    int start_delay;    // initial delay of starting bus
    QGraphicsEllipseItem *obj;
    QPointF c_pos;      // current position of the bus
    QPointF dest;       // point the bus is moving to
    int idx;            // index in route vector
    bool init = true;   // let the bus lead its position and route
    void move(t_line*, int);
    int ign = 0;
    bool hide = false;
};

/* visual line objects */
struct t_line {
    int id;
    QVector<t_street*> streets;
    QVector<t_stop*> stops;
    QVector<t_bus*> buses;
    QVector<QPointF> route;
    QVector<int> timetable;
    void setId(int _id) {
        t_line::id = _id;
    };
    void claimStreets(QVector<t_street*>*);
    void claimStops(QVector<t_stop*>*);
    void getTimetable(QString);
    void makeRoute();
    ~t_line() {
        QVector<t_bus*>::iterator j;

        for(j = t_line::buses.begin(); j != t_line::buses.end(); ++j) {
            delete (*j);
        }
    }
};

#endif // LINELIB_H
