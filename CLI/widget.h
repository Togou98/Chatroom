#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDialog>
#include "startup.h"
#include <QTimer>
#include <QTcpSocket>
#include "reg.h"
#include <QMessageBox>
#include <json/json.h>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
     Ui::Widget *ui;
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    QString IP();
private slots:
    void Addrslot(QString addr);
    void on_Reg_clicked();
    void Msgparser();

    void lighton();
    void lightoff();
    void on_login_clicked();

    void on_send_clicked();

    void on_onlineuser_clicked();

private:
    void lightok(const QString &n);
    void Handlemsg(Json::Value &r);
    void Handlelogret(Json::Value &r);
    void Handlesys(Json::Value  &r);
    const std::string M = "Method";
    QTimer *timer;
    Startup *setaddr;
    QTcpSocket *socket;
    const quint16 Transport = 8080;
};

#endif // WIDGET_H
