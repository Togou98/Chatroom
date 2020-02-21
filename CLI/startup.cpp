#include "startup.h"
#include "ui_startup.h"
#include <unistd.h>
#include <QMessageBox>
#include "nettools.h"

Startup::Startup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Startup)
{
    ui->setupUi(this);
   // setWindowFlags(Qt::Window  | Qt::WindowStaysOnTopHint);
    setWindowTitle("请输入服务器地址");
    setWindowFlag(Qt::FramelessWindowHint);

}

/*void Startup::closeEvent(QCloseEvent *event){
    exit(0);
}
*/
Startup::~Startup()
{
    delete ui;
}
void Startup::on_btn2_clicked()
{
    exit(-100);//从设置服务器的界面退出就return -100;
}
void Startup::on_btn1_clicked(){
    QString tmp = ui->lineEdit->text();
    if(judgeip(tmp)){
            emit Addrsig(tmp);
        this->~Startup();
    }else QMessageBox::warning(this, "警告","不是一个IP地址或者不合法");
}
