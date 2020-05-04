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
    delete line1_buses;
    delete line2_buses;
    delete line4_buses;
    delete line20_buses;

    delete line1_stops;
    delete line2_stops;
    delete line4_stops;
    delete line20_stops;

    delete line1_streets;
    delete line2_streets;
    delete line4_streets;
    delete line20_streets;

    delete timer;
    delete ui;
}

void MainWindow::initScene()
{
    auto *scene = new MyScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    /* render background */
    auto bg_img = QPixmap("/home/xhiner00/Desktop/ICP/original", "png", Qt::AutoColor);     // load image
    bg_img = bg_img.scaledToWidth(400, Qt::TransformationMode::FastTransformation);
    bg_img = bg_img.scaledToHeight(400, Qt::TransformationMode::FastTransformation);
    auto bg_ptr = scene->addPixmap(bg_img);     // insert image into scene
    bg_ptr->setPos(-35, 25);
    auto sqr = scene->addRect(-35, 25, 400, 400, street_highlight); // background bounds for testing

    /* render street lines */
    initSceneStreets(scene);

    /* render bus stops */
    initSceneStops(scene);

    /* render bus icons */
    initSceneBuses(scene);
}

void MainWindow::initSceneStreets(MyScene* scene) {
    /* insert streets into scene */
    auto chrenovska = scene->addLine(230,230,230,110, street_default);
    auto krizna_1 = scene->addLine(100,230,110,230, street_default);
    auto krizna_2 = scene->addLine(110,230,190,230, street_default);
    auto krizna_3 = scene->addLine(190,230,230,230, street_default);
    auto zelena = scene->addLine(180,110,230,110, street_default);
    auto modra = scene->addLine(100,230,100,180, street_default);
    auto fialova = scene->addLine(100,180,180,110, street_default);
    auto prazena = scene->addLine(230,110,230,60, street_default);
    auto severna = scene->addLine(230,60,50,60, street_default);
    auto dobogan_1 = scene->addLine(50,100,50,180, street_default);
    auto dobogan_2 = scene->addLine(50,60,50,100, street_default);
    auto spojna = scene->addLine(50,180,100,180, street_default);
    auto bengoro = scene->addLine(50,180,100,230, street_default);
    auto dobra = scene->addLine(110,230,110,300, street_default);
    auto marianska = scene->addLine(190,230,170,300, street_default);
    auto koleso_1 = scene->addLine(100,300,110,300, street_default);
    auto koleso_2 = scene->addLine(170,300,110,300, street_default);
    auto lesna = scene->addLine(170,300,170,360, street_default);
    auto prava = scene->addLine(170,360,220,400, street_default);
    auto lava = scene->addLine(170,360,120,380, street_default);
    auto masna = scene->addLine(230,110,280,110, street_default);
    auto uholna = scene->addLine(280,110,230,230, street_default);
    auto heso = scene->addLine(280,110,280,60, street_default);
    auto blazena = scene->addLine(280,60,230,60, street_default);
    auto sumracna = scene->addLine(280,110,280,350, street_default);
    auto ranna = scene->addLine(280,350,170,360, street_default);
    auto drisliak = scene->addLine(280,350,170,300, street_default);
    auto jankovska = scene->addLine(50,100,30,100, street_default);
    auto martinska = scene->addLine(30,100,30,230, street_default);
    auto maticna = scene->addLine(30,230,100,230, street_default);
    auto kotolna = scene->addLine(100,230,100,300, street_default);
    auto vodna = scene->addLine(100,300,120,380, street_default);
    auto nivy = scene->addLine(100,300,40,300, street_default);
    auto sedlacka = scene->addLine(40,300,40,350, street_default);

    /* create containers */
    line1_streets = new QVector<QGraphicsLineItem*>;
    line2_streets = new QVector<QGraphicsLineItem*>;
    line4_streets = new QVector<QGraphicsLineItem*>;
    line20_streets = new QVector<QGraphicsLineItem*>;

    /* store streets in bus line containers */
    line1_streets->append(prava);
    line1_streets->append(lava);
    line1_streets->append(vodna);
    line1_streets->append(kotolna);
    line1_streets->append(modra);
    line1_streets->append(fialova);
    line1_streets->append(zelena);
    line1_streets->append(masna);
    line1_streets->append(sumracna);
    line1_streets->append(ranna);

    line2_streets->append(sedlacka);
    line2_streets->append(nivy);
    line2_streets->append(kotolna);
    line2_streets->append(bengoro);
    line2_streets->append(dobogan_1);
    line2_streets->append(dobogan_2);
    line2_streets->append(severna);
    line2_streets->append(blazena);
    line2_streets->append(heso);
    line2_streets->append(sumracna);
    line2_streets->append(drisliak);
    line2_streets->append(koleso_1);
    line2_streets->append(koleso_2);

    line4_streets->append(maticna);
    line4_streets->append(martinska);
    line4_streets->append(jankovska);
    line4_streets->append(dobogan_1);
    line4_streets->append(spojna);
    line4_streets->append(fialova);
    line4_streets->append(zelena);
    line4_streets->append(masna);
    line4_streets->append(uholna);
    line4_streets->append(krizna_3);
    line4_streets->append(marianska);
    line4_streets->append(koleso_2);
    line4_streets->append(dobra);
    line4_streets->append(krizna_1);

    line20_streets->append(chrenovska);
    line20_streets->append(prazena);
    line20_streets->append(blazena);
    line20_streets->append(heso);
    line20_streets->append(sumracna);
    line20_streets->append(ranna);
    line20_streets->append(lesna);
    line20_streets->append(koleso_2);
    line20_streets->append(koleso_1);
    line20_streets->append(kotolna);
    line20_streets->append(krizna_1);
    line20_streets->append(krizna_2);
    line20_streets->append(krizna_3);
}

void MainWindow::initSceneStops(MyScene *scene) {
    /* insert bus stop icons into scene | x-5,y-5 to center */
    auto line1_stop1 = scene->addRect(275, 195, 10, 10, stop_pen_default, stop_brush_default);  //sumracna

    /* create containers */
    line1_stops = new QVector<QGraphicsRectItem*>;
    line2_stops = new QVector<QGraphicsRectItem*>;
    line4_stops = new QVector<QGraphicsRectItem*>;
    line20_stops = new QVector<QGraphicsRectItem*>;

    /* store bus stop pointers in container */
    line1_stops->append(line1_stop1);
}

void MainWindow::initSceneBuses(MyScene *scene) {
    /* insert bus icons into scene */
    auto line1_bus1 = scene->addEllipse(220, 400, 10, 10, bus_pen_default, bus_brush_default);

    /* create containers */
    line1_buses = new QVector<QGraphicsEllipseItem*>;
    line2_buses = new QVector<QGraphicsEllipseItem*>;
    line4_buses = new QVector<QGraphicsEllipseItem*>;
    line20_buses = new QVector<QGraphicsEllipseItem*>;

    /* store bus pointers in container */
    line1_buses->append(line1_bus1);
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
    drop_highlight_line(line1_streets, line1_stops, line1_buses);
    drop_highlight_line(line2_streets, line2_stops, line2_buses);
    drop_highlight_line(line4_streets, line4_stops, line4_buses);
    drop_highlight_line(line20_streets, line20_stops, line20_buses);

    /* select which line to highlight */
    switch(idx) {
        case 0: break;
        case 1:
            highlight_line(line1_streets, line1_stops, line1_buses);
        break;
        case 2:
            highlight_line(line2_streets, line2_stops, line2_buses);
        break;
        case 3:
            highlight_line(line4_streets, line4_stops, line4_buses);
        break;
        case 4:
            highlight_line(line20_streets, line20_stops, line20_buses);
        break;
        default:
            break;
    }
}

void MainWindow::highlight_line(QVector<QGraphicsLineItem *> *streets, QVector<QGraphicsRectItem *> *stops, QVector<QGraphicsEllipseItem *> *buses) {
    QVector<QGraphicsLineItem*>::iterator i;        // street iterator
    QVector<QGraphicsEllipseItem*>::iterator j;     // bus iterator
    QVector<QGraphicsRectItem*>::iterator k;        // bus stop iterator

    for(i = streets->begin(); i != streets->end(); ++i) {
        (*i)->setPen(street_highlight);
        (*i)->setZValue(1);
    }
    for(j = buses->begin(); j != buses->end(); ++j) {
        (*j)->setPen(bus_pen_highlight);
        (*j)->setBrush((bus_brush_highlight));
        (*j)->setZValue(1);
    }
    for(k = stops->begin(); k != stops->end(); k++) {
        (*k)->setPen(stop_pen_highlight);
        (*k)->setBrush(stop_brush_hightlight);
        (*j)->setZValue(1);
    }
}

void MainWindow::drop_highlight_line(QVector<QGraphicsLineItem *> *streets, QVector<QGraphicsRectItem *> *stops, QVector<QGraphicsEllipseItem *> *buses) {
    QVector<QGraphicsLineItem*>::iterator i;        // street iterator
    QVector<QGraphicsEllipseItem*>::iterator j;     // bus iterator
    QVector<QGraphicsRectItem*>::iterator k;        // bus stop iterator

    for(i = streets->begin(); i != streets->end(); ++i) {
        (*i)->setPen(street_default);   // default pen
        (*i)->setZValue(0);             // default depth
    }
    for(j = buses->begin(); j != buses->end(); ++j) {
        (*j)->setPen(bus_pen_default);
        (*j)->setBrush((bus_brush_default));
        (*j)->setZValue(0);
    }
    for(k = stops->begin(); k != stops->end(); k++) {
        (*k)->setPen(stop_pen_default);
        (*k)->setBrush(stop_brush_default);
        (*j)->setZValue(0);
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
