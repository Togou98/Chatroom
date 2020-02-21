#include "widget.h"
#include "ui_widget.h"
#include <QDateTime>
#include <json/json.h>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget), socket(new QTcpSocket)
{

    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);
    setFixedSize(640,430);
    setWindowIcon(QIcon(":/ico.ico"));
    setaddr = new Startup;

   connect(setaddr,SIGNAL(Addrsig(QString)),SLOT(Addrslot(QString)));
    setaddr->exec();


     timer = new QTimer;
    timer->start(1000);
    connect(timer,&QTimer::timeout,[=](){QDateTime current_time = QDateTime::currentDateTime();
        QString StrCurrentTime = current_time.toString("yyyy-MM-dd hh:mm:ss ddd");
        ui->time->setText(StrCurrentTime);});
    connect(socket,SIGNAL(connected()),this,SLOT(lighton()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(lightoff()));
    ui->send->setEnabled(false);
    ui->onlineuser->setEnabled(false);
     connect(socket,SIGNAL(readyRead()),this,SLOT(Msgparser()));

}
void Widget::lighton(){
    ui->light->setPixmap(QPixmap(":/RUN.png"));
    ui->login->setVisible(false);
}
void Widget::lightoff(){
    ui->light->setPixmap(QPixmap(":/NO.png"));
    ui->login->setVisible(true);
    ui->login->setEnabled(true);
    ui->onlineuser->setEnabled(false);
}
void Widget::lightok(const QString &n){
    ui->light->setPixmap(QPixmap(":/YES.png"));
    ui->nickname->setText(n);
}
void Widget::Addrslot(QString addr){
    this->ui->addr->setText("服务器地址:"+addr + ":8080");
}
QString Widget::IP(){
    return ui->addr->text().section(':',1,1);
}
Widget::~Widget()
{
    delete ui;
    delete timer;
    delete socket;
}


void Widget::on_Reg_clicked()
{
    Reg *reg = new Reg(this); //记得delete 掉内存;
    connect(reg->socket(),SIGNAL(connected()),reg,SLOT(show()));
    connect(reg->socket(),SIGNAL(connected()),this,SLOT(hide()));
    //reg->socket()->connectToHost(this->ip(),reg->Regport);
    reg->socket()->connectToHost(this->IP(),reg->Regport); //先用10000端口测试
    auto ret = reg->socket()->waitForConnected(20000);
    if (!ret){
        QMessageBox::warning(this,"你搁这儿开玩笑呢","怎么注册SRV没开启,打程序员去");
    }
}
//消息解析函数 是一个最关键的函数;
void Widget::Msgparser(){
    auto tmpmsg = socket->readAll();
    tmpmsg.toStdString();
    Json::Value msg;
    Json::Reader jr;
    jr.parse(tmpmsg.toStdString(),msg);
    if (msg[M].asString() == "LRT"){
        Handlelogret(msg);
    }else if( msg[M].asString() == "MSG"){
        Handlemsg(msg);
    }else if( msg[M].asString() == "ROT"){
        Handlesys(msg);
    };

}
void Widget::Handlelogret(Json::Value &r){
    if(r["Ret"].asString() == "OK"){
        ui->send->setEnabled(true);
        QString tmpname = QString::fromStdString(r["Nickname"].asString());
        lightok(tmpname);
        QMessageBox::information(this,"通知","登录成功");
        ui->onlineuser->setEnabled(true);
        ui->login->setVisible(true);
        return;
    }else if(r["Ret"].asString() == "KO"){
        QMessageBox::critical(this,"提示","账号或密码错误,或者根本不存在这个账号");
        ui->login->setVisible(true);
        ui->login->setEnabled(true);
        return;
    }else {
        QMessageBox::critical(this,"未知错误","爷爬了");
    }

}
void Widget::Handlemsg(Json::Value &r){
   auto nickname = r["Nickname"].asString();
   auto content = r["Content"].asString();
    ui->record->appendPlainText(QString::fromStdString( nickname + ": " + content));
}
void Widget::Handlesys(Json::Value &r){
    auto content = r["Content"].asString();
    static std::string sys = "$SYS⚠";
    ui->record->appendPlainText(QString::fromStdString( sys+ ": " + content));
}

void Widget::on_login_clicked()
{
    if( ui->inacc->text().isEmpty() ){
       QMessageBox::warning(this,"就这?","什么都不输,你让我猜你账号呢,账号密码赶紧安排");
       ui->inacc->setFocus();
       return;
    }
    if( ui ->inpasswd ->text().isEmpty() ){
        QMessageBox::warning(this,"🍔🍔","我正在猜你密码呢,你搁着等着哈");
        ui->inpasswd->setFocus();
        return;
    }
    ui->login->setEnabled(false);
    if( socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost(IP(),Transport);
        if(!socket->waitForConnected(30000)){
           QMessageBox::warning(this,"提示","先把聊天服务器打开吧");
           ui->login->setEnabled(true);
        return;
        }
    }
    Json::Value login;
     Json::FastWriter fw;
    login[M] = "LIN";
    login["Account"] = ui->inacc->text().toStdString();
    login["Password"] =  ui->inpasswd->text().toStdString();
    auto token = QByteArray::fromStdString(fw.write(login));
    socket->write(token);
}

void Widget::on_send_clicked()
{
    if(ui->input->text().isEmpty()){
        QMessageBox::warning(this,"呵呵","你发你🐎呢?");
        ui->input->setFocus();
        return;
    }
    static std::string nickname = ui->nickname->text().toStdString();
    auto msg = ui->input->text().toStdString();
    Json::Value root;
    Json::FastWriter fw;
    root[M] = "MSG";
    root["Nickname"] = nickname;
    root["Content"] = msg;
    auto snd = QByteArray::fromStdString(fw.write(root));
    socket->write(snd);
}

void Widget::on_onlineuser_clicked()
{   Json::Value root;
    Json::FastWriter fw;
    root[M] = "OLU";
    auto olu = QByteArray::fromStdString(fw.write(root));
    socket->write(olu);
}
