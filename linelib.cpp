#include "linelib.h"
#include <QFile>
#include <QTextStream>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <math.h>

void t_line::getTimetable(QString filename)
{
    QFile file("timetables/" + filename);      // filename
    QString line;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        while (!stream.atEnd()){
            line = stream.readLine();
            QStringList list = line.split(" ");

            timetable.append(QString(list.at(1).toLocal8Bit().constData()).toInt());
        }
    } else {
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();
}

void t_line::claimStreets(QVector<t_street*>* street_list) {
    /* claim streets */
    QFile file("data/lines.txt");      // filename
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

    } else {
        QMessageBox::information(0,"info",file.errorString());
        exit(1);
    }

    file.close();
}

void t_line::claimStops(QVector<t_stop*>* stop_list) {
    /* claim streets */
    QFile file("data/stops_info.txt");      // filename
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

void t_line::makeRoute() {
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
           t_line::route.append(p1);
       }
       if(!t_line::route.isEmpty()) {
           if(t_line::route.indexOf(p2) == -1) {
               t_line::route.append(p2);
           }
       }
    }
}

void t_bus::move(t_line * line, int time) {
    /* initialize route info */
    if(t_bus::init) {
        t_bus::dest = line->route[1];
        t_bus::idx = 1;
        t_bus::init = false;
    }



    /* wait out delay */
    if(delay > 0){
        delay--;
        return;
    }

    if(hide == true){
        obj->hide();
    }
    else{
        /* show bus when starting route */
        obj->show();
    }

    int x_pol = 1;
    int y_pol = 1;

    if(line->route[idx].x()<= c_pos.x() && line->route[idx].y() <= c_pos.y()){
        x_pol = -1;
        y_pol = -1;
    }
    else if(line->route[idx].x()>= c_pos.x() && line->route[idx].y() <= c_pos.y()){
        x_pol = 1;
        y_pol = -1;
    }
    else if(line->route[idx].x()>= c_pos.x() && line->route[idx].y() >= c_pos.y()){
        x_pol = 1;
        y_pol = 1;
    }
    else if(line->route[idx].x()<= c_pos.x() && line->route[idx].y() >= c_pos.y()){
        x_pol = -1;
        y_pol = 1;
    }
    else if(line->route[idx].x() == c_pos.x() && line->route[idx].y() >= c_pos.y()){
        x_pol = 1;
        y_pol = 1;
    }
    else if(line->route[idx].x() == c_pos.x() && line->route[idx].y() <= c_pos.y()){
        x_pol = -1;
        y_pol = 1;
    }
    else if(line->route[idx].x() <= c_pos.x() && line->route[idx].y() == c_pos.y()){
        x_pol = -1;
        y_pol = 1;
    }
    else if(line->route[idx].x() >= c_pos.x() && line->route[idx].y() == c_pos.y()){
        x_pol = 1;
        y_pol = 1;
    }

    /* stop@stop */
    if(ign == 0) {
        for(int i=0;i<line->stops.size();i++) {
            /* calculate length to stop */
            double length = QLineF(c_pos.x(),c_pos.y(),line->stops[i]->pos.x(),line->stops[i]->pos.y()).length();

            /* stop if nearby stop */
            if(length <= 1.5){


                if(stop_num < line->timetable.size()){
                    delay = start_delay + line->timetable[stop_num] - time;
                    ign = 5;    // guard againt stopping again on the same stop

                    stop_num++;
                }
                else{
                    stop_num = 0;
                    hide = true;
                    start_delay = start_delay + 86400;
                }
            }
        }
    } else {
       ign--;
    }

    /* get length of route */
    float x = abs(c_pos.x() - dest.x());
    float y = abs(c_pos.y() - dest.y());
    float length = sqrt(x*x + y*y);

    /* calculate where to move in each axis */
    float x_coef = 1/(x_pol*(length/x));
    float y_coef = 1/(y_pol*(length/y));

    /* zero division correction */
    if(length == 0) {
        x_coef = 0;
        y_coef = 0;
    }

    t_bus::obj->moveBy(x_coef , y_coef);    // move in scene

    c_pos += QPointF(x_coef, y_coef);       // actualise current position

    /* if destination is reached, set a next one */
    if(length <= 1){
        idx=(idx + 1) % line->route.size();
        dest = line->route[idx];
        //qDebug() << dest << " next index: " << idx;
    }


}
