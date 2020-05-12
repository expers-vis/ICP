#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

public:
    Ui::MainWindow *ui;
    timetableDisplay *timetableObj;
    QTimer *timer;
    long int time = 0;   // run time of application

    /* graphic options */
    QPen street_default;
    QPen street_highlight;

    QPen stop_pen_default;
    QPen stop_pen_highlight;
    QBrush stop_brush_default;
    QBrush stop_brush_hightlight;

    QPen bus_pen_default;
    QPen bus_pen_highlight;
    QBrush bus_brush_default;
    QBrush bus_brush_highlight;

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

    QVector<t_street*> *street_list;
    QVector<t_stop*> *stop_list;

    t_line *line1;
    t_line *line2;
    t_line *line4;
    t_line *line20;

    void drop_highlight_line(t_line*);


private:
    void initScene();
    void initSceneStreets(QGraphicsScene* scene);
    void initSceneBuses(QGraphicsScene* scene);
    void initSceneStops(QGraphicsScene* scene);
    void initTimer();
    void initPens();
    void initSelectBox();
    void highlight_line(t_line*);
    inline QString formatTime(long int);

private slots:
    void zoomin();
    void zoomout();
    void zoom(int);
    void timerAction();
    void speedUp();
    void speedDown();
    void speedNorm();
    void highlight(int);
    void showTimetable();
    void findBus(QGraphicsEllipseItem*);

signals:
    void timetableNumber(int);
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
