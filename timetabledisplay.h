/*! \file timetabledisplay.h
 * \brief Header file for displaying timetables
 *
 * \author Martin Hiner - xhiner00
 * \author Adam Ševčík - xsevci64
 *
 * \date 17.5.2020
 *
 * Contains functions for working with timetables.
 * Timetables are displayed in new separate window.
*/

#ifndef TIMETABLEDISPLAY_H
#define TIMETABLEDISPLAY_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>
#include "linelib.h"

namespace Ui {
class timetableDisplay;
}

/*!
 * \brief The timetableDisplay class
 *
 * This class holds timetable functions, slots to recieve signal and user interface setup.
 */
class timetableDisplay : public QDialog
{
    Q_OBJECT

public:
    /*!
     * \brief Class constructor
     *
     * \param parent Class' parent
     */
    explicit timetableDisplay(QWidget *parent = nullptr);

    /*!
     * \brief Class destructor
     */
    ~timetableDisplay();

private slots:
    /*!
     * \brief Slot to recieve timetable
     *
     * Slot recieves index of requested timetable.
     * Function \fn displayTimetable is the called to display the timetable.
     *
     * \param idx Index of bus line from which timetable is requested
     */
    void recieveTimetable(int idx);

    /*!
     * \brief Slot to recieve and display timetable for specific bus
     *
     * Slot recieves current time and a  pointer to structure \struct t_bus.
     * Based on a id of the recieved bus, corresponding timetable is loaded.
     * Stops and their departure times are then displayed based on recieved time and time loaded from timetable file.
     * Next stop is highlighted using less-than and more-than signs like this: >Bus stop<.
     *
     * \pre Recieved bus need a valid line id to load correct timetable.
     *
     * \warning Invalid line id will cause file open error and crash application.
     *
     * \param time Current time
     * \param bus Bus for which timetable is requested
     */
    void recieveBus(long int time, t_bus* bus);

    /*!
     * \brief Window exit
     *
     * Exits the timetable window.
     */
    void on_timetableExitBtn_clicked();

private:
    /*!
     * \brief User inteface
     */
    Ui::timetableDisplay *ui;

private:
    /*!
     * \brief Displays bus line's timetable
     *
     * Based on recieved filename and line ID, departure times for every bus is displayed.
     * Departure times are display for duration of whole day.
     * Rows are departure time of bus for every stop.
     * Columns show departure times for every bus.
     *
     * \warning Line identification number and filename need to match to display correct times.
     *
     * \param t_filename Filename to load timetable from.
     * \param line_id Identification number of requested line
     */
    void displayTimetable(QString t_filename, int line_id);

    /*!
     * \brief Format time from seconds to readable format
     *
     * Function converts time supplied in seconds to HH:MM format and returns it as QString.
     * Leftover seconds are rounded up to minute at number 30.
     *
     * \param time Time in seconds to format.
     * \return String of converted time to HH:MM format.
     */
    inline QString formatTime(long int time);
};

#endif // TIMETABLEDISPLAY_H
