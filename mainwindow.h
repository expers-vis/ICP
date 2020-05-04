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
    void initScene();
    void initSceneStreets(MyScene* scene);
    void initSceneBuses(MyScene* scene);
    void initSceneStops(MyScene* scene);
    void initTimer();
    void initPens();
    void initSelectBox();
    void highlight_line(QVector<QGraphicsLineItem*>*, QVector<QGraphicsRectItem*>*, QVector<QGraphicsEllipseItem*>*);
    void drop_highlight_line(QVector<QGraphicsLineItem*>*, QVector<QGraphicsRectItem*>*, QVector<QGraphicsEllipseItem*>*);

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

    /* vectors containing street pointerss for their bus lines */
    QVector<QGraphicsLineItem*> *line1_streets;
    QVector<QGraphicsLineItem*> *line2_streets;
    QVector<QGraphicsLineItem*> *line4_streets;
    QVector<QGraphicsLineItem*> *line20_streets;

    /* vectors containing stop pointers for their bus lines */
    QVector<QGraphicsRectItem*> *line1_stops;
    QVector<QGraphicsRectItem*> *line2_stops;
    QVector<QGraphicsRectItem*> *line4_stops;
    QVector<QGraphicsRectItem*> *line20_stops;

    /* vectors containing buse pointers for their bus lines */
    QVector<QGraphicsEllipseItem*> *line1_buses;
    QVector<QGraphicsEllipseItem*> *line2_buses;
    QVector<QGraphicsEllipseItem*> *line4_buses;
    QVector<QGraphicsEllipseItem*> *line20_buses;
};
#endif // MAINWINDOW_H
