#ifndef DRONEOBJ_H
#define DRONEOBJ_H

#include<QString>
#include<QJsonArray>
#include<QJsonObject>
#include <QStringLiteral>

class DroneObj
{
public:
    int _hp;
    QString _status;
    QChar _team;
    QString _uid;
    double _vx, _vy;
    double _x, _y;

    DroneObj();
    DroneObj(int hp, QString status, QChar team, QString uid, double vx, double vy, double x, double y);
    static DroneObj fromJson(const QJsonObject& json);
};

#endif // DRONEOBJ_H
