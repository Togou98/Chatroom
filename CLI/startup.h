#ifndef STARTUP_H
#define STARTUP_H

#include <QDialog>

namespace Ui {
class Startup;
}

class Startup : public QDialog
{
    Q_OBJECT

public:
    explicit Startup(QWidget *parent = nullptr);
    ~Startup();
signals:
    void Addrsig(QString);


protected:
    //void closeEvent(QCloseEvent *event);
private slots:

    void on_btn2_clicked();

    void on_btn1_clicked();

private:
    Ui::Startup *ui;
};

#endif // STARTUP_H
