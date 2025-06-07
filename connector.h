#ifndef CONNECTOR_H
#define CONNECTOR_H

#include<QVsoa>
#include <QJsonDocument>
#include<QVariantMap>
#include<QDebug>
#include <QVsoaPayload>
#include<QString>
#include <functional>
#include<ctrlgram.h>
#include<gamegram.h>

class Connector: public QObject
{
    Q_OBJECT
public:
    using GameMessageHandler = std::function<void(const GameGram& gram)>;
    explicit Connector(QObject *parent = nullptr);
    void setGameMessageHandler(GameMessageHandler handler);
    void sendData(const CtrlGram& gram);
    void enableReceive();
private:
    QVsoaClient m_client;
    GameMessageHandler m_gameMessageHandler;
};

#endif // CONNECTOR_H
