#include "connector.h"

Connector::Connector(QObject *parent):
    QObject(parent), m_client()
{
    if(m_client.isInvalid()){
        qDebug()<<"Can not create VSOA client!";
    }

    connect(&m_client, &QVsoaClient::connected, [this](bool ok, QString info){
        if(!ok){
            qDebug() << "Can not connect to VSOA server!";
            return;
        }
        qDebug() << "Successfully connect to VSOA server!";
        m_client.subscribe("/game");
    });
    m_client.connect2server("vsoa://127.0.0.1:3005/game_server", "", 1000);
    m_client.autoConnect(1000, 500);
    m_client.autoConsistent({"/game"}, 1000);
}

void Connector::setGameMessageHandler(GameMessageHandler handler){
    this->m_gameMessageHandler = handler;
}

void Connector::sendData(const CtrlGram& gram){
    //QString jsonStr = QString::fromUtf8(QJsonDocument::fromVariant(msg).toJson());
    m_client.sendDatagram("/ctrl", QVsoaPayload(gram.toJson(), {}));
    qDebug() << "send";
}

void Connector::enableReceive(){
    connect(&m_client, &QVsoaClient::datagram, [this](QString url, QVsoaPayload payload){
       if(url == "/game"){
           auto doc = QJsonDocument::fromJson(payload.param().toLatin1());
           GameGram gram = GameGram::fromJson(doc.object());
           if(m_gameMessageHandler){
               m_gameMessageHandler(gram);
           }
       }
    });
}
