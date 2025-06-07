#include "ctrlgram.h"

CtrlGram::CtrlGram()
{

}

CtrlGram::CtrlGram(QString uid, double vx, double vy)
    :_uid(uid),_vx(vx),_vy(vy)
{

}

QVariantMap CtrlGram::toVariantMap() const {
    QVariantMap map;
    map["uid"] = _uid;
    map["vx"] = _vx;
    map["vy"] = _vy;
    return map;
}

QString CtrlGram::toJson() const
{
    return QJsonDocument::fromVariant(toVariantMap()).toJson();
}
