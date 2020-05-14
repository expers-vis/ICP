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

public:
    Ui::MainWindow *ui;
    timetableDisplay *timetableObj;
    QTimer *timer;
    long int time = 0;   // run time of application
    int time_speed = 100;   // int used because problems with double precision

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
    void connectSignals();
    void initSelectBox();
    void highlight_line(t_line*);
    inline QString formatTime(long int);
    inline void changeTime(int);

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
