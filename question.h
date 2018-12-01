#ifndef QUESTION_H
#define QUESTION_H

#include "basicinfo.h"
#include "answer.h"
#include "user.h"

#include <QVector>
#include <QDateTime>

#include <iostream>
using namespace std;

class Answer;
class CUser;

class Question : public BasicInfo
{
    friend QTextStream & operator << (QTextStream &out, Question &obj);
    friend QTextStream & operator >> (QTextStream &in, Question &obj);

public:
    Question() {}
    Question(QString creator_id, QString title, QString content);
    QString format_print();

private:
    QString title;
    CUser* m_pUser;

public:
    QString getTitle() { return title; }
    QVector<Answer*> m_vecAns;//每个问题的回答用集合存储

public:
    int search(const QString key);//如果该问题的title包含可以则返回0，否则返回-1
    int search(QDateTime start, QDateTime end);//查看该问题的创建时间是否在某个时间段范围内，是返回0，否则返回-1
    //int add_answer(Answer* pans);//将该问题的答案添加到集合中
    int create();

    void setUser(CUser* pUser) { m_pUser = pUser; }
    CUser* getUser() { return m_pUser; }
};

#endif // QUESTION_H
