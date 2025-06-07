#ifndef CTRLGRAM_H
#define CTRLGRAM_H

#include<QString>
#include <QVariantMap>
#include<QJsonDocument>

class CtrlGram
{
public:
    QString _uid;
    double _vx, _vy;

    CtrlGram();
    CtrlGram(QString uid, double vx, double vy);
    QVariantMap toVariantMap() const;
    QString toJson() const;
};

#endif // CTRLGRAM_H
