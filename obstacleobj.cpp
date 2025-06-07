#include "obstacleobj.h"

ObstacleObj::ObstacleObj()
{

}

ObstacleObj::ObstacleObj(QString id, double r, QString type, double x, double y)
    : _id(id), _r(r), _type(type), _x(x), _y(y)
{

}

ObstacleObj ObstacleObj::fromJson(const QJsonObject &json)
{
    QString id = json["id"].toString();
    double r = json["r"].toDouble();
    QString type = json["type"].toString();
    double x = json["x"].toDouble();
    double y = json["y"].toDouble();

    return ObstacleObj(id, r, type, x, y);
}
