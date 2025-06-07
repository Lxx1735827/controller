#ifndef GAMEGRAM_H
#define GAMEGRAM_H

#include<droneobj.h>
#include<QList>
#include<obstacleobj.h>

class GameGram
{
public:
    QList<DroneObj> _drones;
    int _left_time;
    QList<ObstacleObj> _obstacles;
    QString _proto_ver;
    QString _stage;

    GameGram();
    GameGram(QList<DroneObj> drones, int left_time, QList<ObstacleObj> obstacles, QString proto_ver, QString stage);
    static GameGram fromJson(const QJsonObject& json);
};

#endif // GAMEGRAM_H
