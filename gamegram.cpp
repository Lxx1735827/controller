#include "gamegram.h"

GameGram::GameGram()
{

}

GameGram::GameGram(QList<DroneObj> drones, int left_time, QList<ObstacleObj> obstacles, QString proto_ver, QString stage)
    : _drones(drones), _left_time(left_time), _obstacles(obstacles), _proto_ver(proto_ver), _stage(stage)
{

}

GameGram GameGram::fromJson(const QJsonObject &json)
{
    QList<DroneObj> drones;
    QJsonArray dronesArray = json["drones"].toArray();
    for (const QJsonValue& value : dronesArray) {
        drones.append(DroneObj::fromJson(value.toObject()));
    }

    QList<ObstacleObj> obstacles;
    QJsonArray obstaclesArray = json["obstacles"].toArray();
    for (const QJsonValue& value : obstaclesArray) {
        obstacles.append(ObstacleObj::fromJson(value.toObject()));
    }

    int left_time = json["left_time"].toInt();
    QString proto_ver = json["proto_ver"].toString();
    QString stage = json["stage"].toString();

    return GameGram(drones, left_time, obstacles, proto_ver, stage);
}
