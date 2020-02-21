#include "reg.h"
#include "ui_reg.h"
#include "widget.h"

#include <string>

#include <json/json.h>
#include <QMessageBox>
#include <QDebug>

Reg::Reg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reg),
   _socket(new QTcpSocket(this))
{

    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    setWindowFlags( windowFlags()| Qt::WindowStaysOnTopHint |Qt::WindowCloseButtonHint |Qt::CustomizeWindowHint);
    setFixedSize(250,150);
    setWindowIcon(QIcon(":/ico.ico"));
    connect(_socket,SIGNAL(readyRead()),this,SLOT(gotcha()));
}

void Reg::closeEvent(QCloseEvent *event){
   auto pa = dynamic_cast<Widget*>(parentWidget());
   pa->show();
   event->accept();
   this->~Reg();
}
QByteArray Reg::packettojson(){

    auto method = "REG";
    auto account = ui->account->text();
    auto password = ui->password1->text();
    auto nickname = ui->nickname->text();

    Json::Value ret;
    Json::FastWriter fw;
    ret["Method"] = method;
    ret["Account"] = account.toStdString().c_str();
    ret["Password"] = password.toStdString().c_str();
    ret["Nickname"] = nickname.toStdString().c_str();

    return QByteArray::fromStdString(fw.write(ret));
}
QTcpSocket* Reg::socket(){
    return _socket;
}
Reg::~Reg()
{
    delete ui;
    delete _socket;
}
void Reg::gotcha(){
    const std::string echo[2] ={"OK","FAIL"};
    QByteArray byte = _socket->read(10);
    qDebug() << byte;
    if ( byte.toStdString() == echo[0]){
    QMessageBox::information(nullptr, "恭喜", "账号注册成功 你可以去登录了");
    show();
    } else if( byte.toStdString() == echo[1] ){
    QMessageBox::warning(nullptr,"嗯?就这","注册失败, 服务端错误,建议暴捶作者");
    ui->btnreg->setVisible(true);
    show();
    } else{
        QMessageBox::critical(nullptr, "Oops!", "注册错误,请关机退出软件 然后回到现实生活,再见");
        exit(0);
    }
}
void Reg::on_btnreg_clicked()
{
    if (ui->account->text().isEmpty() || ui->password1->text().isEmpty() || ui->password1->text().isEmpty() || ui->nickname->text().isEmpty() ){
        QMessageBox::warning(this,"出错了","所有内容必须填写"); return;
    } else if(ui->password1->text() != ui->password2->text())   {QMessageBox::critical(this,"出错了","密码不相符") ; return;}

    qDebug() << packettojson();
    _socket->write(packettojson());
    ui->btnreg->setVisible(false);
    hide();
}
