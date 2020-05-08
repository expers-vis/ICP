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
#include <math.h>

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
    auto bg_img = QPixmap(QCoreApplication::applicationDirPath() + "/original.png", "png", Qt::AutoColor);     // load image
    bg_img = bg_img.scaledToWidth(400, Qt::TransformationMode::FastTransformation);
    bg_img = bg_img.scaledToHeight(400, Qt::TransformationMode::FastTransformation);
    auto bg_ptr = scene->addPixmap(bg_img);     // insert image into scene
    bg_ptr->setPos(-35, 25);
    auto sqr = scene->addRect(-35, 25, 400, 400, street_highlight); // background bounds for testing

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
    for(i = line1->route.begin(); i != line1->route.end(); i++) {
        auto p =scene->addEllipse((*i).x() - 1, (*i).y() - 1, 2, 2, bus_pen_highlight, bus_brush_highlight);
    }*/
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

    /* load stops into their lines */
    line1->claimStops(stop_list);
    line2->claimStops(stop_list);
    line4->claimStops(stop_list);
    line20->claimStops(stop_list);
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


            bus->obj = scene->addEllipse(x-5, y-5, 10, 10, bus_pen_default, bus_brush_default);
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

    timer->start(100);     //default 1 sec events

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
        (*j)->obj->setBrush((bus_brush_default));
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
    QVector<t_bus*>::iterator i;

    for(i = line1->buses.begin(); i != line1->buses.end(); ++i) {
        (*i)->move(line1->route);
    }
}

void MainWindow::t_line::claimStreets(QVector<t_street*>* street_list) {
    /* claim streets */
    QFile file("lines.txt");      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){

            line = stream.readLine();
            QStringList list = line.split(" ");

            if(t_line::id == QString(list.at(0).toLocal8Bit().constData()).toInt()) {
                /* load from this line */

                for(int i = 1; i != list.size(); i++) {
                    /* find street name in street list */
                    QString name = list.at(i);

                    QVector<t_street*>::iterator j;
                    for(j = street_list->begin(); j != street_list->end(); ++j) {
                        if((*j)->name == name) {
                            t_line::streets.append((*j));
                            break;
                        }
                    }
                }

                /* loading complete */
                file.close();
                return;
            }
        }

    }
    else{
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();
}

void MainWindow::t_line::claimStops(QVector<t_stop*>* stop_list) {
    /* claim streets */
    QFile file("stops_info.txt");      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){

            line = stream.readLine();
            QStringList list = line.split(" ");

            if(t_line::id == QString(list.at(0).toLocal8Bit().constData()).toInt()) {
                /* load from this line */

                for(int i = 1; i != list.size(); i++) {
                    /* find stop name in street list */
                    QString name = list.at(i);

                    QVector<t_stop*>::iterator j;
                    for(j = stop_list->begin(); j != stop_list->end(); ++j) {
                        if((*j)->name == name) {
                            t_line::stops.append((*j));
                            break;
                        }
                    }
                }

                /* loading complete */
                file.close();
                return;
            }
        }

    }
    else{
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();
}

void MainWindow::t_line::makeRoute() {
    QVector<t_street*>::iterator i;

    /* insert start and end points into set to remove duplicates */
    for(i = t_line::streets.begin(); i != t_line::streets.end(); ++i) {
       auto line_obj = (*i)->obj->line();      //get line object of street
       auto p1 = line_obj.p1();         // get first point of line
       auto p2 = line_obj.p2();         // get second point of line

       /* add only unique points */
       if(!t_line::route.isEmpty()) {
           if(t_line::route.indexOf(p1) == -1) {
               t_line::route.append(line_obj.p1());
           }
       } else {
           t_line::route.append(p2);
       }
       if(!t_line::route.isEmpty()) {
           if(t_line::route.indexOf(p2) == -1) {
               t_line::route.append(p2);
           }
       }
    }

    /* add last point */
    auto line_obj = t_line::streets[t_line::streets.size() - 1]->obj->line();
    if(t_line::route.last() == line_obj.p1()) {
        t_line::route.append(line_obj.p2());
    } else {
        t_line::route.append(line_obj.p1());
    }
}

void MainWindow::t_bus::move(QVector<QPointF> route) {
   /*static int c_pos = 0;   // current position in route vector

    c_pos = (c_pos + 1) % route.size();
    t_bus::obj->setPos(route[c_pos]);*/
    static QPointF bus = route[0];
    static QPointF point = route[1];
    static int point_i = 1;
    int x_pol = 1;
    int y_pol = 1;

    if(route[point_i].x()<= bus.x() && route[point_i].y() <= bus.y()){
        x_pol = -1;
        y_pol = -1;
    }
    else if(route[point_i].x()>= bus.x() && route[point_i].y() <= bus.y()){
        x_pol = 1;
        y_pol = -1;
    }
    else if(route[point_i].x()>= bus.x() && route[point_i].y() >= bus.y()){
        x_pol = 1;
        y_pol = 1;
    }
    else if(route[point_i].x()<= bus.x() && route[point_i].y() >= bus.y()){
        x_pol = -1;
        y_pol = 1;
    }
    else if(route[point_i].x() == bus.x() && route[point_i].y() >= bus.y()){
        x_pol = 1;
        y_pol = 1;
    }
    else if(route[point_i].x() == bus.x() && route[point_i].y() <= bus.y()){
        x_pol = -1;
        y_pol = 1;
    }
    else if(route[point_i].x() <= bus.x() && route[point_i].y() == bus.y()){
        x_pol = -1;
        y_pol = 1;
    }
    else if(route[point_i].x() >= bus.x() && route[point_i].y() == bus.y()){
        x_pol = 1;
        y_pol = 1;
    }


    float x;
    float y;
    float length;

    x = abs(bus.x() - point.x());
    y = abs(bus.y() - point.y());


    length = sqrt(x*x + y*y);

    t_bus::obj->moveBy(1/(x_pol*(length/x)),1/(y_pol*(length/y)));

    bus += QPointF(1/(x_pol*(length/x)),1/(y_pol*(length/y)));


    if(length <= 0.5){
        point_i=(point_i+1)%route.size();
        point = route[point_i];
        qDebug() << point << " next index: " << point_i;
    }


}
