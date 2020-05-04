#include "timetabledisplay.h"
#include "ui_timetabledisplay.h"
#include <QDebug>

timetableDisplay::timetableDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::timetableDisplay)
{
    ui->setupUi(this);
    ui->timetableList->addItem("Started");
    /* determine what timetable to display */
    QString name = timetableDisplay::windowTitle();
    if(name.compare("Line 1 Timetable") == 0) {
        qDebug() << "Displaying line 1 timetable";
        ui->timetableList->addItem("Line 1");
    } else
    if(name.compare("Line 2 Timetable") == 0) {
        qDebug() << "Displaying line 2 timetable";
        ui->timetableList->addItem("Line 2");
    } else
    if(name.compare("Line 4 Timetable") == 0) {
        qDebug() << "Displaying line 4 timetable";
        ui->timetableList->addItem("Line 4");
    } else
    if(name.compare("Line 20 Timetable") == 0) {
        qDebug() << "Displaying line 20 timetable";
        ui->timetableList->addItem("Line 20");
    }
}

timetableDisplay::~timetableDisplay()
{
    delete ui;
}

void timetableDisplay::on_timetableExitBtn_clicked()
{
    timetableDisplay::close();
}
