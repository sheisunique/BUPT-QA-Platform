#ifndef ANSWER_H
#define ANSWER_H

#include <QVector>
#include <QTextStream>

#include "basicinfo.h"
#include "question.h"
#include "user.h"

class Question;
class CUser;

class Answer : public BasicInfo
{
    friend QTextStream & operator<<(QTextStream &out, Answer &obj);
    friend QTextStream & operator >> (QTextStream &in, Answer &obj);

public:
    Answer() {}
    Answer(QString creator_id, QString content);

    int getPraiseNum() { return praiseNum; }
    int increasePraiseNum() { return ++praiseNum; }

    QVector<CUser*> m_vecPrise;//谁点赞，存谁

private:
    int praiseNum;//点赞次数
    CUser* m_pUser;//该答案对应的创建者
    QString str;//保存点赞用户的ID，使用,号分割

public:
    QString format_print();
    int search(const QString key);//如果该问题的title包含可以则返回0，否则返回-1
    int search(QDateTime start, QDateTime end);//查看该问题的创建时间是否在某个时间段范围内，是返回0，否则返回-1
    int create();

    void setUser(CUser* pUser) { m_pUser = pUser; }
    CUser* getUser() { return m_pUser; }

    QString getStr() { return str; }  //返回点赞用户的ID
};

#endif // ANSWER_H
