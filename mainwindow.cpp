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
#include <myscene.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* initialize objects */
    initPens();
    initScene();
    initTimer();
    initSelectBox();

    /* connect signals to slots */
    connect(ui->zoominbtn, &QPushButton::clicked, this, &MainWindow::zoomin);
    connect(ui->zoomoutbtn, &QPushButton::clicked, this, &MainWindow::zoomout);
    connect(ui->zoomslider, &QSlider::valueChanged,this, &MainWindow::zoom);

    connect(ui->speedUpBtn, &QPushButton::clicked, this, &MainWindow::speedUp);
    connect(ui->speedDownBtn, &QPushButton::clicked, this, &MainWindow::speedDown);
    connect(ui->speedNormBtn, &QPushButton::clicked, this, &MainWindow::speedNorm);

    connect(ui->lineSelectBox, SIGNAL(currentIndexChanged(int)), this, SLOT(highlight(int)));
    connect(ui->timetableBtn, &QPushButton::clicked, this, &MainWindow::showTimetable);
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

void MainWindow::initScene()
{
    auto *scene = new MyScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    /* render background */
    qDebug() << QCoreApplication::applicationDirPath();
    auto bg_img = QPixmap(QCoreApplication::applicationDirPath() + "/original.png", "png", Qt::AutoColor);     // load image
    bg_img = bg_img.scaledToWidth(400, Qt::TransformationMode::FastTransformation);
    bg_img = bg_img.scaledToHeight(400, Qt::TransformationMode::FastTransformation);
    auto bg_ptr = scene->addPixmap(bg_img);     // insert image into scene
    bg_ptr->setPos(-35, 25);
    auto sqr = scene->addRect(-35, 25, 400, 400, street_highlight); // background bounds for testing

    street_list = new QVector<t_street*>;
    stop_list = new QVector<t_stop*>;

    line1 = new t_line;
    line2 = new t_line;
    line4 = new t_line;
    line20 = new t_line;

    /* render street lines */
    initSceneStreets(scene);

    /* render bus stops */
    initSceneStops(scene);

    /* render bus icons */
    initSceneBuses(scene);
}

void MainWindow::initSceneStreets(MyScene* scene) {
    QFile file("streets.txt");      // filename
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
}

void MainWindow::initSceneStops(MyScene *scene) {
    QFile file("stops.txt");      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){

            line = stream.readLine();
            QStringList list = line.split(" ");

            t_stop *stop = new t_stop;

            double x = QString(list.at(1).toLocal8Bit().constData()).toDouble();
            double y = QString(list.at(2).toLocal8Bit().constData()).toDouble();

            stop->obj = scene->addRect(x, y, 10, 10, stop_pen_default, stop_brush_default);
            stop->name = list.at(0).toLocal8Bit().constData();

            stop_list->append(stop);
        }

    }
    else{
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();
}

void MainWindow::initSceneBuses(MyScene *scene) {
    QFile file("buses.txt");      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){

            line = stream.readLine();
            QStringList list = line.split(" ");

            t_bus *bus = new t_bus;

            double x = QString(list.at(1).toLocal8Bit().constData()).toDouble();
            double y = QString(list.at(2).toLocal8Bit().constData()).toDouble();


            bus->obj = scene->addEllipse(x, y, 10, 10, bus_pen_default, bus_brush_default);
            bus->line_id = QString(list.at(0).toLocal8Bit().constData()).toInt();

            /* assign to line */
            switch(bus->line_id) {
                case 1:
                    line1->buses.append(bus);
                break;

                case 2:
                    line2->buses.append(bus);
                break;

                case 4:
                    line4->buses.append(bus);
                break;

                case 20:
                    line20->buses.append(bus);
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

    bus_brush_default.setColor("light blue");
    bus_brush_default.setStyle(Qt::SolidPattern);

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
    qreal scale = z /10.0;
    ui->graphicsView->setTransform(QTransform(scale,org.m12(), org.m21(), scale, org.dx(), org.dy()));
}

void MainWindow::speedUp(){
    if(timer->interval() > 100){
        int new_time = timer->interval() - 100;
        timer->setInterval(new_time);
    }
}

void MainWindow::speedDown(){
    int new_time = timer->interval() + 100;
    timer->setInterval(new_time);
}

void MainWindow::speedNorm(){
    timer->setInterval(1000);
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
    for(k = line->stops.begin(); k != line->stops.end(); k++) {
        (*k)->obj->setPen(stop_pen_highlight);
        (*k)->obj->setBrush(stop_brush_hightlight);
        (*j)->obj->setZValue(1);
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
        (*j)->obj->setBrush((bus_brush_default));
        (*j)->obj->setZValue(0);
    }
    for(k = line->stops.begin(); k != line->stops.end(); k++) {
        (*k)->obj->setPen(stop_pen_default);
        (*k)->obj->setBrush(stop_brush_default);
        (*j)->obj->setZValue(0);
    }
}

void MainWindow::showTimetable() {
    int curr_line = ui->lineSelectBox->currentIndex();
    switch(curr_line) {
        case 0:
        break;
        case 1:
            qDebug() << "Printing info 1\n";
            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 1 Timetable");
            timetableObj->show();
        break;
        case 2:
            qDebug() << "Printing info 2\n";
            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 2 Timetable");
            timetableObj->show();
        break;
        case 3:
            qDebug() << "Printing info 4\n";
            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 4 Timetable");
            timetableObj->show();
        break;
        case 4:
            qDebug() << "Printing info 20\n";
            timetableObj = new timetableDisplay(this);
            timetableObj->setWindowTitle("Line 20 Timetable");
            timetableObj->show();
        break;
    }
}

void MainWindow::timerAction(){
    static bool time = false;

    if(time) {
        //ui->label->setPixmap(QPixmap("/home/xhiner00/Desktop/ICP/original", "png", Qt::AutoColor));
        time = false;
    } else {
        //ui->label->setPixmap(QPixmap("/home/xhiner00/Desktop/ICP/boof", nullptr, Qt::AutoColor));
        time = true;
    }
}

void MainWindow::t_line::claimStreets() {
    /* claim streets */
    QFile file("lines.txt");      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){

            line = stream.readLine();
            QStringList list = line.split(" ");

            if(t_line::id == QString(list.at(0).toLocal8Bit().constData()).toInt()) {
                QStringList::iterator i = list.begin();
                /*while(i != list.end()) {
                    MainWindow::street_list->
                }*/
            }
        }

    }
    else{
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();
}
