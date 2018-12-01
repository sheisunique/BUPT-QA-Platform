#include "user.h"

CUser::CUser()
    :id(""), name(""), password("")
{
    ;
}

CUser::CUser(QString id, QString name, QString password)
{
    this->id = id;
    this->password = password;
    this->name = name;
}

int CUser::addUser(CUser* pu)
{
    focuslist.push_back(pu);
    return 0;
}

int CUser::addQuestion(Question* pQuestion)
{
    m_vecQuestion.push_back(pQuestion);
    return 0;
}

int CUser::addAnswer(Answer* pAns)
{
    m_vecAnswer.push_back(pAns);
    return 0;
}

QString CUser::getId()
{
    return id;
}

CUser* CUser::getUser(QString id)
{
    int size = focuslist.size();

    for (int i = 0; i < size; i++)
    {
        CUser* pUser = focuslist.at(i);
        if (pUser->getId().compare(id) == 0)
        {
            return pUser;
        }
    }
    return NULL;
}

int CUser::login(QString id, QString password)
{
    if (this->id.compare(id) == 0
            && this->password.compare(password) == 0)
    {
        return 0;//如果用户名和密码都正确返回0
    }
    return 1;//任何一个错误都返回1
}

int CUser::searchQuestion(const QString keyWord, QVector<BasicInfo*>& vecQuestion)
{
    int size = m_vecQuestion.size();

    int count = 0;
    for (int i = 0; i < size; i++)
    {
        BasicInfo* pBasic = m_vecQuestion.at(i);
        if (pBasic->search(keyWord) == 0)
        {
            vecQuestion.push_back(pBasic);
            count++;
        }
    }

    return count;
}

int CUser::searchQuestion(QDateTime start, QDateTime end, QVector<BasicInfo*>& vecQuestion)
{
    int size = m_vecQuestion.size();

    int count = 0;
    for (int i = 0; i < size; i++)
    {
        BasicInfo* pBasic = m_vecQuestion.at(i);
        if (pBasic->search(start, end) == 0)
        {
            vecQuestion.push_back(pBasic);
            count++;
        }
    }

    return count;
}

QString CUser::format_print()
{
    QString out = QString("%1\n%2\n%3\n")
            .arg(id)
            .arg(name)
            .arg(password);
    cout<<out.toLatin1().data();//QString转换为char*

    return out;
}

QTextStream & operator<<(QTextStream &out, CUser &obj)
{
    out << obj.format_print();
    return out;
}

QTextStream & operator>>(QTextStream &in, CUser &obj)
{
    /*in >> obj.id;
    in >> obj.name;
    in >> obj.password;*/
    obj.id = in.readLine();
    obj.name = in.readLine();
    obj.password = in.readLine();
    return in;
}
