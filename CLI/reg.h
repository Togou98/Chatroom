#ifndef REG_H
#define REG_H

#include <QDialog>
#include <QTcpSocket>
#include <QCloseEvent>
namespace Ui {
class Reg;
}

class Reg : public QDialog
{
    Q_OBJECT

public:
    void closeEvent(QCloseEvent *event);
    explicit Reg(QWidget *parent = nullptr);
    ~Reg();
    QTcpSocket* socket();
    const quint16 Regport = 8081;
    QByteArray packettojson();
private slots:
    void gotcha();
    void on_btnreg_clicked();

private:
    Ui::Reg *ui;

    QTcpSocket *_socket;
};

#endif // REG_H
