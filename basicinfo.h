#ifndef BASICINFO_H
#define BASICINFO_H

#include <QString>
#include <QDateTime>

class BasicInfo
{
public:
    BasicInfo() {}
    BasicInfo(QString creator_id, QString content);

protected:
    QString id;
    QString creator_id;
    QDateTime create_time;
    QString content;//内容

public:
    QString getId() { return id; }
    QString getCreateId() { return creator_id; }
    QDateTime getCreateTime() { return create_time; }
    QString getContent() { return content; }

public:
    virtual QString format_print() = 0;
    virtual int create() = 0;
    virtual int search(const QString key) = 0;
    virtual int search(QDateTime start, QDateTime end) = 0;
};

#endif // BASICINFO_H
