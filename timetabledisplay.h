#ifndef TIMETABLEDISPLAY_H
#define TIMETABLEDISPLAY_H

#include <QDialog>

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

private:
    Ui::timetableDisplay *ui;
};

#endif // TIMETABLEDISPLAY_H
