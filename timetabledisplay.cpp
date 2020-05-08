#include "timetabledisplay.h"
#include "ui_timetabledisplay.h"
#include <QDebug>

timetableDisplay::timetableDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::timetableDisplay)
{
    ui->setupUi(this);
}

timetableDisplay::~timetableDisplay()
{
    delete ui;
}

void timetableDisplay::on_timetableExitBtn_clicked()
{
    timetableDisplay::close();
}

void timetableDisplay::recieveTimetable(int idx) {
    switch(idx) {
        case 1:
            displayTimetable("line1_timetable.txt");
        break;

        case 2:
            displayTimetable("line2_timetable.txt");
        break;

        case 4:
            displayTimetable("line4_timetable.txt");
        break;

        case 20:
            displayTimetable("line20_timetable.txt");
        break;

    default:
        break;
    }
}

void timetableDisplay::displayTimetable(QString filename) {
    QFile file("timetables/" + filename);      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){
            line = stream.readLine();
            ui->timetableList->addItem(line);
        }

    }
    else{
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();
}
