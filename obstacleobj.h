#ifndef OBSTACLEOBJ_H
#define OBSTACLEOBJ_H

#include<QString>
#include<QJsonArray>
#include<QJsonObject>
#include <QStringLiteral>

class ObstacleObj
{
public:

    QString _id;
    double _r;
    QString _type;
    double _x, _y;

    ObstacleObj();
    ObstacleObj(QString id, double r, QString type, double x, double y);
    static ObstacleObj fromJson(const QJsonObject& json);
};

#endif // OBSTACLEOBJ_H
