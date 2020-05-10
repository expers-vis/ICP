#ifndef TIMETABLEDISPLAY_H
#define TIMETABLEDISPLAY_H

#include <QDialog>
#include <QFile>
#include <QMessageBox>

namespace Ui {
class timetableDisplay;
}

class timetableDisplay : public QDialog
{
    Q_OBJECT

public:
    explicit timetableDisplay(QWidget *parent = nullptr);
    ~timetableDisplay();

private slots:
    void on_timetableExitBtn_clicked();
    void recieveTimetable(int);

private:
    Ui::timetableDisplay *ui;

private:
    void displayTimetable(QString, int);
    inline QString formatTime(long int);
};

#endif // TIMETABLEDISPLAY_H
