#include "question.h"

Question::Question(QString creator_id, QString title, QString content)
    : BasicInfo(creator_id, content)
{
    this->title = title;
}

int Question::search(const QString key)
{
    if (title.contains(key) == true)
    {
        return 0;
    }
    return -1;
}

int Question::search(QDateTime start, QDateTime end)
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

/*int Question::add_answer(Answer* pans)
{
    m_vecAns.push_back(pans);
    return 0;
}*/

int Question::create()
{
    ;
    return 0;
}

QString Question::format_print()
{
    QString out = QString("%1\n%2\n%3\n%4\n%5\n")
            .arg(id)
            .arg(this->title)
            .arg(content)
            .arg(creator_id)
            .arg(create_time.toString("yyyy-MM-dd hh:mm:ss"));
    cout<<out.toLatin1().data();

    return out;
}

QTextStream & operator<<(QTextStream &out, Question &obj)
{
    out << obj.format_print();
    return out;
}

QTextStream & operator>>(QTextStream &in, Question &obj)
{
    obj.id = in.readLine();
    obj.title = in.readLine();
    obj.content = in.readLine();
    obj.creator_id = in.readLine();
    QString time;
    time = in.readLine();
    obj.create_time = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
    return in;
}
