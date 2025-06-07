#include "droneobj.h"

DroneObj::DroneObj()
{

}

DroneObj::DroneObj(int hp, QString status, QChar team, QString uid, double vx, double vy, double x, double y)
    : _hp(hp), _status(status), _team(team), _uid(uid), _vx(vx), _vy(vy), _x(x), _y(y)
{

}

DroneObj DroneObj::fromJson(const QJsonObject &json)
{
    int hp = json["hp"].toInt();
    QString status = json["status"].toString();
    QChar team = json["team"].toString().at(0);
    QString uid = json["uid"].toString();
    double vx = json["vx"].toDouble();
    double vy = json["vy"].toDouble();
    double x = json["x"].toDouble();
    double y = json["y"].toDouble();

    return DroneObj(hp, status, team, uid, vx, vy, x, y);
}
