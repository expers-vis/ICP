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

    /* open files */
    if (timetable_file.open(QIODevice::ReadOnly | QIODevice::Text) && bus_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream t_stream(&timetable_file);
        QTextStream b_stream(&bus_file);
        QString printLine;      // output line

        /* print header with stop names */
        printLine.sprintf("%6s\t", " ");

        while(!t_stream.atEnd()) {
            QString t_line = t_stream.readLine();
            QList t_list = t_line.split(" ");

            printLine.append(QString().sprintf("%*s\t", t_list.at(0).size(), t_list.at(0).toLocal8Bit().constData()));
        }
        ui->timetableList->addItem(printLine);  // print line
        t_stream.seek(0);   // reset stream

        /* print arrival time for each bus */
        int bus_num = 0;    // bus number within line

        while(!b_stream.atEnd()) {
            QString b_line = b_stream.readLine();
            QList b_list = b_line.split(" ");

            /* print buses only for requested line */
            int bus_id = QString(b_list.at(0).toLocal8Bit().constData()).toInt();
            if(bus_id == line_id) {
                printLine.sprintf("%s #%d\t", "Bus", ++bus_num);

                int bus_time = QString(b_list.at(1).toLocal8Bit().constData()).toInt();

                /* append arrival to the stop */
                while(!t_stream.atEnd()) {
                    QString t_line = t_stream.readLine();
                    QList t_list = t_line.split(" ");

                    int stop_time = QString(t_list.at(1).toLocal8Bit().constData()).toInt();
                    QString time = formatTime(stop_time + bus_time);

                    printLine.append(QString().sprintf("%*s\t", time.size(),time.toLocal8Bit().constData()));

                }

                t_stream.seek(0);

                /* print bus to list */
                ui->timetableList->addItem(printLine);
            }
        }

    }
    else{
        QMessageBox::information(0,"info",timetable_file.errorString());
        exit(1);
    }

    ui->timetableList->setMinimumWidth(ui->timetableList->sizeHintForColumn(0) + 2 * ui->timetableList->frameWidth());    // adjust list width
    ui->timetableList->setMinimumHeight(ui->timetableList->sizeHintForRow(0) * ui->timetableList->count() + 2 * ui->timetableList->frameWidth());
    timetable_file.close();
    bus_file.close();
}

inline QString timetableDisplay::formatTime(long int sec) {
    int hour = sec / 3600;
    sec = sec % 3600;
    int min = sec / 60;
    if(sec % 60 >= 30) {
        min++;
        if(min == 60) {
            hour++;
            min = 0;
        }
    }

    return QString().sprintf("%02d:%02d", hour, min);
}
