#ifndef NETTOOLS_H
#define NETTOOLS_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QTcpSocket>
#include <QByteArray>

#include <json/json.h>
//#include <json/json.h>
bool judgeip(const QString& ip){
            //return true; //调试的时候先不判断了
            auto charpointnum = ip.split('.');
            if(ip.length() >= 7 && ip.length() <= 15 && charpointnum.length() == 4 ){
                QVector<QString> qstrvec;
                for(int i = 0 ; i < 4 ; i++){
                    qstrvec.push_back(ip.section('.',i,i));
                }
                for(int i = 0 ; i < 4 ; i++){
                    if(qstrvec.at(i).toInt() >= 0 && qstrvec.at(i).toInt() <= 255)  continue;
                    else {
                        return false;
                    }
                }
                return true;

            } else  return false;
}
bool kvcmp(Json::Value &r , std::string k, std::string v){
   return  r[k].asString() == v;
}
#endif
