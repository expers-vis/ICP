/*
 * Projekt ICP
 * Autori: Adam Ševčík - xsevci64, Martin Hiner - xhiner00
*/

#include "mainwindow.h"
#include "timetabledisplay.h"
#include "ui_mainwindow.h"
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QTimer>
#include <QFile>
#include <QMessageBox>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* hide time display */
    ui->timeDisplay->hide();
    ui->timeSpeedlabel->hide();
    ui->zoominbtn->hide();
    ui->zoomoutbtn->hide();
    ui->zoomslider->hide();
    ui->menubar->hide();
    ui->lineSelectBox->hide();
    ui->statusbar->hide();
    ui->speedDownBtn->hide();
    ui->speedNormBtn->hide();
    ui->speedUpBtn->hide();
    ui->timetableBtn->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();

    /* initialize objects */
    initPens();
    initSelectBox();
    ui->timeDisplay->setFont(QFont("arial", 30, 10, false));
    ui->timeSpeedlabel->setFont(QFont("arial", 15, 10, false));
}

MainWindow::~MainWindow()
{
    /* delete containers */
    delete line1;
    delete line2;
    delete line4;
    delete line20;

    delete street_list;
    delete stop_list;

    delete timer;
    delete ui;
}

MyScene::MyScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void MainWindow::initScene()
{
    auto *scene = new MyScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);


    /* render background */
    /*auto bg_img = QPixmap(QCoreApplication::applicationDirPath() + "/original.png", "png", Qt::AutoColor);     // load image
    bg_img = bg_img.scaledToWidth(400, Qt::TransformationMode::FastTransformation);
    bg_img = bg_img.scaledToHeight(400, Qt::TransformationMode::FastTransformation);
    auto bg_ptr = scene->addPixmap(bg_img);     // insert image into scene
    bg_ptr->setPos(-40, 10);*/
    scene->addRect(-40, 10, 400, 400, street_highlight); // background bounds for testing

    street_list = new QVector<t_street*>;
    stop_list = new QVector<t_stop*>;

    /* create line containers and set their id */
    line1 = new t_line;
    line2 = new t_line;
    line4 = new t_line;
    line20 = new t_line;

    line1->setId(1);
    line2->setId(2);
    line4->setId(4);
    line20->setId(20);

    /* render street lines */
    initSceneStreets(scene);

    /* render bus stops */
    initSceneStops(scene);

    /* render bus icons */
    initSceneBuses(scene);

    line1->getTimetable("line1_timetable.txt");
    line2->getTimetable("line2_timetable.txt");
    line4->getTimetable("line4_timetable.txt");
    line20->getTimetable("line20_timetable.txt");
}

void MainWindow::initSceneStreets(QGraphicsScene* scene) {
    QFile file("data/streets.txt");      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){

            line = stream.readLine();
            QStringList list = line.split(" ");

            t_street *street = new t_street;

            double x1 = QString(list.at(1).toLocal8Bit().constData()).toDouble();
            double y1 = QString(list.at(2).toLocal8Bit().constData()).toDouble();
            double x2 = QString(list.at(3).toLocal8Bit().constData()).toDouble();
            double y2 = QString(list.at(4).toLocal8Bit().constData()).toDouble();

            street->obj = scene->addLine(x1,y1,x2,y2, street_default);
            street->name = list.at(0).toLocal8Bit().constData();

            street_list->append(street);
        }

    }
    else{
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();

    /* load streets into their lines */
    line1->claimStreets(street_list);
    line2->claimStreets(street_list);
    line4->claimStreets(street_list);
    line20->claimStreets(street_list);

    /* calculate routes */
    line1->makeRoute();
    line2->makeRoute();
    line4->makeRoute();
    line20->makeRoute();
    /*QVector<QPointF>::iterator i;
    for(i = line2->route.begin(); i != line2->route.end(); i++) {
        auto p =scene->addEllipse((*i).x() - 1, (*i).y() - 1, 2, 2, bus_pen_highlight, bus_brush_highlight);
    }*/
}

void MainWindow::initSceneStops(QGraphicsScene *scene) {
    QFile file("data/stops.txt");      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){

            line = stream.readLine();
            QStringList list = line.split(" ");

            t_stop *stop = new t_stop;

            double x = QString(list.at(1).toLocal8Bit().constData()).toDouble();
            double y = QString(list.at(2).toLocal8Bit().constData()).toDouble();

            stop->obj = scene->addRect(x - 5, y - 5, 10, 10, stop_pen_default, stop_brush_default);
            stop->name = list.at(0).toLocal8Bit().constData();
            stop->pos = QPointF(x, y);

            stop_list->append(stop);
        }

    }
    else{
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();

    /* load stops into their lines */
    line1->claimStops(stop_list);
    line2->claimStops(stop_list);
    line4->claimStops(stop_list);
    line20->claimStops(stop_list);
}

void MainWindow::initSceneBuses(QGraphicsScene *scene) {
    QFile file("data/buses.txt");      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){

            line = stream.readLine();
            QStringList list = line.split(" ");

            t_bus *bus = new t_bus;

            double x = QString(list.at(2).toLocal8Bit().constData()).toDouble();
            double y = QString(list.at(3).toLocal8Bit().constData()).toDouble();


            bus->obj = scene->addEllipse(x - 5, y - 5, 10, 10, bus_pen_default, bus_brush_1);
            bus->obj->hide();
            bus->c_pos = QPointF(x, y);
            bus->line_id = QString(list.at(0).toLocal8Bit().constData()).toInt();
            bus->delay = QString(list.at(1).toLocal8Bit().constData()).toInt();
            bus->start_delay = QString(list.at(1).toLocal8Bit().constData()).toInt();

            /* assign to line */
            switch(bus->line_id) {
                case 1:
                    line1->buses.append(bus);
                    bus->obj->setBrush(bus_brush_1);
                break;

                case 2:
                    line2->buses.append(bus);
                    bus->obj->setBrush(bus_brush_2);
                break;

                case 4:
                    line4->buses.append(bus);
                    bus->obj->setBrush(bus_brush_4);
                break;

                case 20:
                    line20->buses.append(bus);
                    bus->obj->setBrush(bus_brush_20);
                break;

            default:
                break;
            }
        }

    }
    else{
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();
}

void MainWindow::initTimer() {

    timer = new QTimer(this);
    timer->setSingleShot(false);    // explicit repeat

    timer->start(1000);     //default 1 sec events

    connect(timer, &QTimer::timeout,  this, &MainWindow::timerAction);
}

void MainWindow::initPens() {
    /* default pen for streets */
    street_default.setColor("black");
    street_default.setWidth(1);

    /* pen for highlighting streets */
    street_highlight.setColor("red");
    street_highlight.setWidth(3);

    /* default bus stop pen and brush */
    stop_pen_default.setColor("black");
    stop_pen_default.setWidth(1);

    stop_brush_default.setColor("grey");
    stop_brush_default.setStyle(Qt::SolidPattern);

    /* highlighted bus stop pen and brush */
    stop_pen_highlight.setColor("red");
    stop_pen_highlight.setWidth(3);

    stop_brush_hightlight.setColor("yellow");
    stop_brush_hightlight.setStyle(Qt::SolidPattern);

    /* default bus pen and brush */
    bus_pen_default.setColor("grey");
    bus_pen_default.setWidth(1);

    bus_brush_1.setColor("blue");
    bus_brush_1.setStyle(Qt::SolidPattern);
    bus_brush_2.setColor("green");
    bus_brush_2.setStyle(Qt::SolidPattern);
    bus_brush_4.setColor("yellow");
    bus_brush_4.setStyle(Qt::SolidPattern);
    bus_brush_20.setColor("purple");
    bus_brush_20.setStyle(Qt::SolidPattern);

    /* highlighted bus pen and brush */
    bus_pen_highlight.setColor("orange");
    bus_pen_highlight.setWidth(3);

    bus_brush_highlight.setColor("red");
    bus_brush_highlight.setStyle(Qt::SolidPattern);
}

void MainWindow::initSelectBox() {
    ui->lineSelectBox->addItem("None", 0);
    ui->lineSelectBox->addItem("Line 1", 1);
    ui->lineSelectBox->addItem("Line 2", 2);
    ui->lineSelectBox->addItem("Line 4", 4);
    ui->lineSelectBox->addItem("Line 20", 20);
}

void MainWindow::connectSignals() {
    /* connect signals to slots */
    connect(ui->zoominbtn, &QPushButton::clicked, this, &MainWindow::zoomin);
    connect(ui->zoomoutbtn, &QPushButton::clicked, this, &MainWindow::zoomout);
    connect(ui->zoomslider, &QSlider::valueChanged,this, &MainWindow::zoom);

    connect(ui->speedUpBtn, &QPushButton::clicked, this, &MainWindow::speedUp);
    connect(ui->speedDownBtn, &QPushButton::clicked, this, &MainWindow::speedDown);
    connect(ui->speedNormBtn, &QPushButton::clicked, this, &MainWindow::speedNorm);

    connect(ui->lineSelectBox, SIGNAL(currentIndexChanged(int)), this, SLOT(highlight(int)));
    connect(ui->timetableBtn, &QPushButton::clicked, this, &MainWindow::showTimetable);
    connect(ui->graphicsView->scene(), SIGNAL(sendBus(QGraphicsEllipseItem*)), this, SLOT(findBus(QGraphicsEllipseItem*)));
}

void MainWindow::zoomin()
{
    //ui->graphicsView->scale(1.25,1.25);
    auto val = ui->zoomslider->value();
    ui->zoomslider->setValue(val + 2);
}

void MainWindow::zoomout()
{
    //ui->graphicsView->scale(0.8,0.8);
    auto val = ui->zoomslider->value();
    ui->zoomslider->setValue(val - 2);
}

void MainWindow::zoom(int z)
{
    auto org = ui->graphicsView->transform();
    qreal scale = z / 10.0;
    ui->graphicsView->setTransform(QTransform(scale,org.m12(), org.m21(), scale, org.dx(), org.dy()));
}

void MainWindow::speedUp(){
    if(time_speed == 10000) {
        /* limit 100x speed */
        return;
    }
    if(time_speed >= 5000) {
        changeTime(1000);
        return;
    }
    if(time_speed >= 2000) {
        changeTime(500);
        return;
    }
    if(time_speed >= 600) {
        changeTime(200);
        return;
    }
    if(time_speed >= 200) {
        changeTime(100);
        return;
    }
    if(time_speed >= 30){
        changeTime(10);
        return;
    }
    if(time_speed < 30){
        changeTime(5);
        return;
    }
}

void MainWindow::speedDown(){
    if(time_speed == 5) {
        /* lowest speed */
        return;
    }
    if(time_speed <= 30){
        changeTime(-5);
        return;
    }
    if(time_speed <= 200) {
        changeTime(-10);
        return;
    }
    if(time_speed <= 600) {
        changeTime(-100);
        return;
    }
    if(time_speed <= 2000) {
        changeTime(-200);
        return;
    }
    if(time_speed <= 5000) {
        changeTime(-500);
        return;
    } else {
        changeTime(-1000);
        return;
    }
}

void MainWindow::speedNorm(){
    timer->setInterval(1000);
    time_speed = 100;

    ui->timeSpeedlabel->setText(QString().sprintf("Speed: %.2fx", (double)time_speed/100));
}

inline void MainWindow::changeTime(int change) {
    time_speed += change;
    double speed = (double)time_speed / 100;
    double new_time = 1000 * (1 / speed);
    timer->setInterval(new_time);
    ui->timeSpeedlabel->setText(QString().sprintf("Speed: %.2fx", speed));
}

void MainWindow::findBus(QGraphicsEllipseItem* bus) {
    QVector<t_bus*>::iterator i;

    for(i = line1->buses.begin(); i != line1->buses.end(); ++i) {
        if(bus == (*i)->obj) {
            ui->lineSelectBox->setCurrentIndex(1);

            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 1 Bus Timetable");

            /* inform window which line to display */
            connect(this, SIGNAL(timetableBus(long int, t_bus*)), timetableObj, SLOT(recieveBus(long int, t_bus*)));
            emit timetableBus(time, (*i));
            disconnect(this, SIGNAL(timetableBus(long int, t_bus*)), timetableObj, SLOT(recieveBus(long int, t_bus*)));

            timetableObj->show();

            return;
        }
    }
    for(i = line2->buses.begin(); i != line2->buses.end(); ++i) {
        if(bus == (*i)->obj) {
            ui->lineSelectBox->setCurrentIndex(2);

            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 2 Bus Timetable");

            /* inform window which line to display */
            connect(this, SIGNAL(timetableBus(long int, t_bus*)), timetableObj, SLOT(recieveBus(long int, t_bus*)));
            emit timetableBus(time, (*i));
            disconnect(this, SIGNAL(timetableBus(long int, t_bus*)), timetableObj, SLOT(recieveBus(long int, t_bus*)));

            timetableObj->show();

            return;
        }
    }
    for(i = line4->buses.begin(); i != line4->buses.end(); ++i) {
        if(bus == (*i)->obj) {
            ui->lineSelectBox->setCurrentIndex(3);

            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 4 Bus Timetable");

            /* inform window which line to display */
            connect(this, SIGNAL(timetableBus(long int, t_bus*)), timetableObj, SLOT(recieveBus(long int, t_bus*)));
            emit timetableBus(time, (*i));
            disconnect(this, SIGNAL(timetableBus(long int, t_bus*)), timetableObj, SLOT(recieveBus(long int, t_bus*)));

            timetableObj->show();

            return;
        }
    }
    for(i = line20->buses.begin(); i != line20->buses.end(); ++i) {
        if(bus == (*i)->obj) {
            ui->lineSelectBox->setCurrentIndex(4);

            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 20 Bus Timetable");

            /* inform window which line to display */
            connect(this, SIGNAL(timetableBus(long int, t_bus*)), timetableObj, SLOT(recieveBus(long int, t_bus*)));
            emit timetableBus(time, (*i));
            disconnect(this, SIGNAL(timetableBus(long int, t_bus*)), timetableObj, SLOT(recieveBus(long int, t_bus*)));

            timetableObj->show();

            return;
        }
    }

    /* no bus found */
    ui->lineSelectBox->setCurrentIndex(0);
}

void MainWindow::highlight(int idx) {
    /* clear any previous highlights */
    drop_highlight_line(line1);
    drop_highlight_line(line2);
    drop_highlight_line(line4);
    drop_highlight_line(line20);

    /* select which line to highlight */
    switch(idx) {
        case 0: break;
        case 1:
            highlight_line(line1);
        break;
        case 2:
            highlight_line(line2);
        break;
        case 3:
            highlight_line(line4);
        break;
        case 4:
            highlight_line(line20);
        break;
        default:
            break;
    }
}

void MainWindow::highlight_line(t_line *line) {
    QVector<t_street*>::iterator i;        // street iterator
    QVector<t_bus*>::iterator j;     // bus iterator
    QVector<t_stop*>::iterator k;        // bus stop iterator

    for(i = line->streets.begin(); i != line->streets.end(); ++i) {
        (*i)->obj->setPen(street_highlight);
        (*i)->obj->setZValue(1);
    }
    for(j = line->buses.begin(); j != line->buses.end(); ++j) {
        (*j)->obj->setPen(bus_pen_highlight);
        (*j)->obj->setBrush((bus_brush_highlight));
        (*j)->obj->setZValue(1);
    }
    for(k = line->stops.begin(); k != line->stops.end(); ++k) {
        (*k)->obj->setPen(stop_pen_highlight);
        (*k)->obj->setBrush(stop_brush_hightlight);
        (*k)->obj->setZValue(1);
    }
}

void MainWindow::drop_highlight_line(t_line *line) {
    QVector<t_street*>::iterator i;        // street iterator
    QVector<t_bus*>::iterator j;     // bus iterator
    QVector<t_stop*>::iterator k;        // bus stop iterator

    for(i = line->streets.begin(); i != line->streets.end(); ++i) {
        (*i)->obj->setPen(street_default);
        (*i)->obj->setZValue(0);
    }
    for(j = line->buses.begin(); j != line->buses.end(); ++j) {
        (*j)->obj->setPen(bus_pen_default);
        switch((*j)->line_id) {
            case 0: break;
            case 1:
                (*j)->obj->setBrush((bus_brush_1));
            break;
            case 2:
                (*j)->obj->setBrush((bus_brush_2));
            break;
            case 4:
                (*j)->obj->setBrush((bus_brush_4));
            break;
            case 20:
                (*j)->obj->setBrush((bus_brush_20));
            break;
            default:
                break;
        }

        (*j)->obj->setZValue(0);
    }
    for(k = line->stops.begin(); k != line->stops.end(); ++k) {
        (*k)->obj->setPen(stop_pen_default);
        (*k)->obj->setBrush(stop_brush_default);
        (*k)->obj->setZValue(0);
    }
}

void MainWindow::showTimetable() {
    int curr_line = ui->lineSelectBox->currentIndex();
    switch(curr_line) {
        case 0:
        break;
        case 1:
            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 1 Timetable");

            /* inform window which line to display */
            connect(this, SIGNAL(timetableNumber(int)), timetableObj, SLOT(recieveTimetable(int)));
            emit timetableNumber(curr_line);
            disconnect(this, SIGNAL(timetableNumber(int)), timetableObj, SLOT(recieveTimetable(int)));

            timetableObj->show();
        break;
        case 2:
            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 2 Timetable");

            /* inform window which line to display */
            connect(this, SIGNAL(timetableNumber(int)), timetableObj, SLOT(recieveTimetable(int)));
            emit timetableNumber(curr_line);
            disconnect(this, SIGNAL(timetableNumber(int)), timetableObj, SLOT(recieveTimetable(int)));

            timetableObj->show();
        break;
        case 3:
            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 4 Timetable");

            /* inform window which line to display */
            connect(this, SIGNAL(timetableNumber(int)), timetableObj, SLOT(recieveTimetable(int)));
            emit timetableNumber(curr_line);
            disconnect(this, SIGNAL(timetableNumber(int)), timetableObj, SLOT(recieveTimetable(int)));

            timetableObj->show();
        break;
        case 4:
            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 20 Timetable");

            /* inform window which line to display */
            connect(this, SIGNAL(timetableNumber(int)), timetableObj, SLOT(recieveTimetable(int)));
            emit timetableNumber(curr_line);
            disconnect(this, SIGNAL(timetableNumber(int)), timetableObj, SLOT(recieveTimetable(int)));

            timetableObj->show();
        break;
    }
}

void MainWindow::timerAction(){
    /* move the busses */
    QVector<t_bus*>::iterator i;

    for(i = line1->buses.begin(); i != line1->buses.end(); ++i) {
        (*i)->move(line1,time);
    }
     for(i = line2->buses.begin(); i != line2->buses.end(); ++i) {
        (*i)->move(line2,time);
    }
    for(i = line4->buses.begin(); i != line4->buses.end(); ++i) {
        (*i)->move(line4,time);
    }
    for(i = line20->buses.begin(); i != line20->buses.end(); ++i) {
        (*i)->move(line20,time);
    }

    /* forward time */
    time++;

    /* print time */
    QString tm = formatTime(time);
    ui->timeDisplay->setText(tm);
}

inline QString MainWindow::formatTime(long int sec) {
    int hour = (sec / 3600) % 24;
    sec = sec % 3600;
    int min = sec / 60;
    sec = sec % 60;

    return QString().sprintf("%02d:%02d:%02ld", hour, min, sec);
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    for (auto* item : items(event->scenePos())) {
        if (auto bus = dynamic_cast<QGraphicsEllipseItem*>(item); bus) {
            emit sendBus(bus);
            return;
        }
    }

    emit sendBus(nullptr);

    QGraphicsScene::mousePressEvent(event);
}

void MainWindow::on_startBtn_clicked()
{
    time = QTime(0, 0, 0).secsTo(ui->timeEdit->time());     // get inputted time in seconds

    initScene();

    /* print time */
    QString tm = formatTime(time);
    ui->timeDisplay->setText(tm);

    /* simulate elapsed time to set position */
    int  elapsed = time % 3600;

    for(int j = 0; j < elapsed; j++) {
        QVector<t_bus*>::iterator i;

        for(i = line1->buses.begin(); i != line1->buses.end(); ++i) {
            (*i)->move(line1,j);
        }
         for(i = line2->buses.begin(); i != line2->buses.end(); ++i) {
            (*i)->move(line2,j);
        }
        for(i = line4->buses.begin(); i != line4->buses.end(); ++i) {
            (*i)->move(line4,j);
        }
        for(i = line20->buses.begin(); i != line20->buses.end(); ++i) {
            (*i)->move(line20,j);
        }
    }

    /* start timer */
    initTimer();

    /* connect signals */
    connectSignals();

    /* show time display */
    ui->timeDisplay->show();
    ui->timeSpeedlabel->show();
    ui->zoominbtn->show();
    ui->zoomoutbtn->show();
    ui->zoomslider->show();
    ui->menubar->show();
    ui->lineSelectBox->show();
    ui->statusbar->show();
    ui->speedDownBtn->show();
    ui->speedNormBtn->show();
    ui->speedUpBtn->show();
    ui->timetableBtn->show();
    ui->label_2->show();
    ui->label_3->show();
    ui->label_4->show();

    /* hide button */
    ui->startBtn->hide();
    ui->timeEdit->hide();
}
