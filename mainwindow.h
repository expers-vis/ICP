#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "timetabledisplay.h"
#include "myscene.h"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    timetableDisplay *timetableObj;
    QTimer *timer;

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

    struct t_street {
        QString name;
        QGraphicsLineItem *obj;
    };

    struct t_stop {
        QString name;
        QGraphicsRectItem *obj;
    };

    struct t_bus {
        int line_id;        // id of line it belongs to
        QGraphicsEllipseItem *obj;
        void move(QVector<QPointF>);
    };

    /* visual line objects */
    struct t_line {
        int id;
        QVector<t_street*> streets;
        QVector<t_stop*> stops;
        QVector<t_bus*> buses;
        QVector<QPointF> route;
        void setId(int _id) {
            t_line::id = _id;
        };
        void claimStreets(QVector<t_street*>*);
        void claimStops(QVector<t_stop*>*);
        void makeRoute();
        ~t_line() {
            QVector<t_street*>::iterator i;
            QVector<t_bus*>::iterator j;
            QVector<t_stop*>::iterator k;

            for(i = t_line::streets.begin(); i != t_line::streets.end(); ++i) {
                delete (*i);
            }
            for(j = t_line::buses.begin(); j != t_line::buses.end(); ++j) {
                delete (*i);
            }
            for(k = t_line::stops.begin(); k != t_line::stops.end(); k++) {
                delete (*i);
            }
        }
    };

    QVector<t_street*> *street_list;
    QVector<t_stop*> *stop_list;

    t_line *line1;
    t_line *line2;
    t_line *line4;
    t_line *line20;

private:
    void initScene();
    void initSceneStreets(MyScene* scene);
    void initSceneBuses(MyScene* scene);
    void initSceneStops(MyScene* scene);
    void initTimer();
    void initPens();
    void initSelectBox();
    void highlight_line(t_line*);
    void drop_highlight_line(t_line*);

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
};
#endif // MAINWINDOW_H
