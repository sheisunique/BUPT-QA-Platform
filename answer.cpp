#include "answer.h"

Answer::Answer(QString creator_id, QString content)
    : BasicInfo(creator_id, content), praiseNum(0)
{

}


QString Answer::format_print()
{
    int size = this->m_vecPrise.size();
    QString str;
    for (int i = 0; i < size; i++)
    {
        str += this->m_vecPrise.at(i)->getId();
        str += ",";
    }
    QString out = QString("%1\n%2\n%3\n%4\n%5\n")
            .arg(id)
            .arg(content)
            .arg(this->praiseNum)
            .arg(creator_id)
            .arg(create_time.toString("yyyy-MM-dd hh:mm:ss"));
    out += str;
    out += "\n";
    cout<<out.toLatin1().data();

    return out;
}

int Answer::search(const QString key)
{
    return 0;
}

int Answer::search(QDateTime start, QDateTime end)
{
    if (this->create_time >= start && this->create_time <= end)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

int Answer::create()
{
    return 0;
}

QTextStream & operator<<(QTextStream &out, Answer &obj)
{
    out << obj.format_print();
    return out;
}

QTextStream & operator>>(QTextStream &in, Answer &obj)
{
    obj.id = in.readLine();
    obj.content = in.readLine();
    QString str_praiseNum;
    str_praiseNum = in.readLine();
    obj.praiseNum = str_praiseNum.toInt();
    obj.creator_id = in.readLine();
    QString time;
    time = in.readLine();
    obj.create_time = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
    obj.str = in.readLine();
    return in;
}

