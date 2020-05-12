#include "timetabledisplay.h"
#include "ui_timetabledisplay.h"
#include "mainwindow.h"
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
            displayTimetable("line1_timetable.txt", 1);
        break;

        case 2:
            displayTimetable("line2_timetable.txt", 2);
        break;

        case 3:
            displayTimetable("line4_timetable.txt", 4);
        break;

        case 4:
            displayTimetable("line20_timetable.txt", 20);
        break;

    default:
        break;
    }
}

void timetableDisplay::displayTimetable(QString t_filename, int line_id) {
    QFile timetable_file("timetables/" + t_filename);
    QFile bus_file("data/buses.txt");

    if (timetable_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream t_stream(&timetable_file);

        while(!t_stream.atEnd()) {
            QString t_line = t_stream.readLine();
            QList t_list = t_line.split(" ");

            QString printLine;
            printLine.sprintf("%20s", t_list.at(0).toLocal8Bit().constData());
            int stop_time = QString(t_list.at(1).toLocal8Bit().constData()).toInt();


            if(bus_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream b_stream(&bus_file);

                while(!b_stream.atEnd()) {
                    QString b_line = b_stream.readLine();
                    QList b_list = b_line.split(" ");

                    int bus_id = QString(b_list.at(0).toLocal8Bit().constData()).toInt();

                    if(line_id == bus_id) {
                        printLine.append("\t");

                        int bus_time = QString(b_list.at(1).toLocal8Bit().constData()).toInt();

                        printLine.append(formatTime(stop_time + bus_time));
                    }
                }

                //b_stream.seek(0);

            }
            bus_file.close();

            /* print line to list */
            ui->timetableList->addItem(printLine);
        }

    }
    else{
        QMessageBox::information(0,"info",timetable_file.errorString());
        exit(1);
    }

    timetable_file.close();
}

inline QString timetableDisplay::formatTime(long int sec) {
    int hour = sec / 3600;
    sec = sec % 3600;
    int min = sec / 60;
    sec = sec % 60;

    return QString().sprintf("%02d:%02d:%02ld", hour, min, sec);
}
