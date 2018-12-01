#ifndef CUSER_H
#define CUSER_H

#include "basicinfo.h"

#include <QString>
#include <QVector>

#include "question.h"
#include "answer.h"

#include <QTextStream>

class Question;
class Answer;

class CUser
{
    friend QTextStream & operator << (QTextStream &out, CUser &obj);
    friend QTextStream & operator >> (QTextStream &in, CUser &obj);   //重载输入输出运算符

public:
    CUser();
    CUser(QString id, QString name, QString password);

public:
    QVector<CUser*> focuslist;//我关注的用户
    QVector<Question*> m_vecQuestion;//我的提问
    QVector<Answer*> m_vecAnswer;//我的回答

public:
    QVector<Question*>& getQuestionVec() { return m_vecQuestion; }
    QVector<Answer*>& getAnsVec() { return m_vecAnswer; }
    QVector<CUser*>& getUserVec() { return focuslist; }

private:  //成员属性为私有变量
    QString id;
    QString name;
    QString password;

public:
    int addUser(CUser *pu);//添加用户为关注
    int addQuestion(Question* pQuestion);//将我的提问添加到我的问题集合
    int addAnswer(Answer* pAns);//将我的回答添加到我的回答集合

    QString getId();
    CUser* getUser(QString id);//通过关注的用户的用户名返回该用户的类（详细信息）

    QString format_print();//格式化返回用户信息

    /*
    keyWord为搜索的关键字
    vecQuestion为搜索到的问题
    返回值为搜索到的问题的个数
    */
    int searchQuestion(const QString keyWord, QVector<BasicInfo*>& vecQuestion);
    //start为开始时间，end为结束时间，搜索到的问题在这个时间返回内并且包含这两个时间点
    int searchQuestion(QDateTime start, QDateTime end, QVector<BasicInfo*>& vecQuestion);

    int login(QString id, QString password);
};

#endif // CUSER_H
