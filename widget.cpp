#include "widget.h"
#include "ui_widget.h"
#include "base_info.h"

#include <QMessageBox>

#define PAGER_PER 2

#include "login.h"

#include "questionpanel.h"
#include "answerpanel.h"
#include "question.h"
#include "focususer.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    model = new QStandardItemModel();
    model->setColumnCount(5);
    int index = 0;
    model->setHeaderData(index++,Qt::Horizontal,("编号"));
    model->setHeaderData(index++,Qt::Horizontal,("标题"));
    model->setHeaderData(index++,Qt::Horizontal,("内容"));
    model->setHeaderData(index++,Qt::Horizontal,("创建者"));
    model->setHeaderData(index++,Qt::Horizontal,("创建时间"));

    ui->tableView->setSelectionMode( QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setModel(model);
    //表头信息显示居左
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableView->show();

    model2 = new QStandardItemModel();
    model2->setColumnCount(5);
    index = 0;
    model2->setHeaderData(index++,Qt::Horizontal,("编号"));
    model2->setHeaderData(index++,Qt::Horizontal,("内容"));
    model2->setHeaderData(index++,Qt::Horizontal,("点赞数"));
    model2->setHeaderData(index++,Qt::Horizontal,("回答者"));
    model2->setHeaderData(index++,Qt::Horizontal,("回答时间"));

    ui->tableView_2->setSelectionMode( QAbstractItemView::SingleSelection);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setModel(model2);
    //表头信息显示居左
    ui->tableView_2->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableView_2->show();

    q_pager = 1;
    a_pager = 1;

    ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit_2->setDateTime(QDateTime::currentDateTime());

    //load_file();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::setUser(QString username)
{
    m_username = username;
    int size = g_User.size();
    for (int i = 0; i < size; i++)
    {
        CUser* pu = g_User.at(i);
        if (pu->getId().compare(username) == 0)
        {
            m_pUser = pu;
            break;
        }
    }
}

//刷新问题列表
void Widget::on_pushButton_clicked()
{
    //
    g_QuestionSearch.clear();
    refresh_list();
}

//提问 添加问题
void Widget::on_pushButton_2_clicked()
{
    //
    g_QuestionSearch.clear();
    QuestionPanel panel;
    panel.show();
    if(panel.exec() == QDialog::Accepted)
    {
        if (panel.ret == 1)
        {
            //
            Question* q = new Question(m_username, panel.title, panel.content);
            q->setUser(m_pUser);
            m_pUser->m_vecQuestion.push_front(q);//按照最新的时间排序在最前面
            g_Question.push_front(q);
            refresh_list();
        }
    }
}

//刷新问题列表界面
void Widget::refresh_list()
{
    model->removeRows(0,model->rowCount());

    int size = g_Question.size();
    int index = 0;
    int count = 0;
    int mm = 0;
    for(int i = (q_pager-1) * PAGER_PER; i < size && count < PAGER_PER; i++, count++)
    {
        index = 0;
        Question *pq = (Question *)g_Question.at(i);
        model->setItem(mm,index++,new QStandardItem(pq->getId()));
        //设置字符颜色
        //model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));
        //设置字符位置
        //model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(mm,index++,new QStandardItem(pq->getTitle()));
        model->setItem(mm,index++,new QStandardItem(pq->getContent()));
        model->setItem(mm,index++,new QStandardItem(pq->getCreateId()));
        model->setItem(mm,index++,new QStandardItem(pq->getCreateTime().toString("yyyy-MM-dd hh:mm:ss")));
        mm++;
    }
    int k = index - 1;
    model->sort(k, Qt::DescendingOrder);

    int total = g_Question.size() % PAGER_PER == 0 ? g_Question.size() / PAGER_PER : g_Question.size() / PAGER_PER + 1;
    QString display = QString("%1/%2").arg(total > 0 ? q_pager : 0).arg(total);
    ui->label_3->setText(display);
}

//刷新答案列表界面
void Widget::refresh_list2(Question* q)
{
    model2->removeRows(0,model2->rowCount());

    int index = 0;
    int count = 0;
    int mm = 0;
    for (int m = (a_pager - 1) * PAGER_PER; m < q->m_vecAns.size() && count < PAGER_PER; m++, count++)
    {
        index = 0;
        Answer* a = (Answer*)q->m_vecAns.at(m);
        model2->setItem(mm,index++,new QStandardItem(a->getId()));
        model2->setItem(mm,index++,new QStandardItem(a->getContent()));
        model2->setItem(mm,index++,new QStandardItem(QString("%1").arg(a->getPraiseNum())));
        model2->setItem(mm,index++,new QStandardItem(a->getCreateId()));
        model2->setItem(mm,index++,new QStandardItem(a->getCreateTime().toString("yyyy-MM-dd hh:mm:ss")));
        mm++;
    }
    int k = index - 1;
    model2->sort(k, Qt::DescendingOrder);

    int total = pQuestion->m_vecAns.size() % PAGER_PER == 0 ? pQuestion->m_vecAns.size() / PAGER_PER : pQuestion->m_vecAns.size() / PAGER_PER + 1;
    QString display = QString("%1/%2").arg(total > 0 ? a_pager : 0).arg(total);
    ui->label_4->setText(display);
}

//双击某一个问题，查看其所有回答内容
void Widget::on_tableView_doubleClicked(const QModelIndex &index)
{
    //
    model2->removeRows(0,model2->rowCount());

    int curRow=ui->tableView->currentIndex().row();
    if (curRow == -1)
    {
        return;
    }
    QModelIndex idx0 = model->index(curRow, 0);
    QModelIndex idx3 = model->index(curRow, 3);

    QString question_id = model->data(idx0).toString();
    QString creator_id = model->data(idx3).toString();

    for (int i = 0; i < g_User.size(); i++)
    {
        CUser* pUser = g_User.at(i);
        int q_size = pUser->m_vecQuestion.size();
        for (int k = 0; k < q_size; k++)
        {
            Question* q = (Question *)pUser->m_vecQuestion.at(k);
            if (pUser->getId().compare(creator_id) == 0
                    && q->getId().compare(question_id) == 0)
            {
                this->pQuestion = q;
                refresh_list2(q);
                return;
            }
        }
    }
}

//选中某个问题，显示回答
void Widget::on_pushButton_11_clicked()
{
    //
    int curRow=ui->tableView->currentIndex().row();
    if (curRow == -1)
    {
        QMessageBox::information(this, "哎呀", "请选中一个问题进行回答！", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QModelIndex idx0 = model->index(curRow, 0);
    QModelIndex idx3 = model->index(curRow, 3);

    QString question_id = model->data(idx0).toString();
    QString creator_id = model->data(idx3).toString();
    if (m_pUser->getId().compare(creator_id) == 0)
    {
        QMessageBox::information(this, "哎呀", "用户不能回答自己的问题的！", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    for (int i = 0; i < g_User.size(); i++)
    {
        CUser* pUser = g_User.at(i);
        int q_size = pUser->m_vecQuestion.size();
        for (int k = 0; k < q_size; k++)
        {
            Question* q = (Question *)pUser->m_vecQuestion.at(k);
            if (pUser->getId().compare(creator_id) == 0
                    && q->getId().compare(question_id) == 0)
            {
                this->pQuestion = q;
                this->question_id = question_id;
                AnswerPanel panel;
                panel.show();
                if(panel.exec() == QDialog::Accepted)
                {
                    if (panel.ret == 1)
                    {
                        //
                        Answer* a = new Answer(m_username, panel.getContent());
                        a->setUser(m_pUser);
                        //q->m_vecAns.push_back(a);
                        q->m_vecAns.push_front(a);//按照最新的时间排序在最前面
                        m_pUser->m_vecAnswer.push_back(a);
                        refresh_list2(q);
                    }
                }
                return;
            }
        }
    }

}

//注销
void Widget::on_pushButton_12_clicked()
{
    //
    save();
    this->hide();
    Login login;
    login.show();
    int ret = login.exec();
    if(ret == QDialog::Accepted)
    {
        if (login.ret == 1)
        {
            model->removeRows(0,model->rowCount());
            model2->removeRows(0,model2->rowCount());
            this->setUser(login.m_username);
            this->show();
        }
    }
}

//点赞
void Widget::on_pushButton_13_clicked()
{
    //
    int curRow=ui->tableView_2->currentIndex().row();
    if (curRow == -1)
    {
        QMessageBox::information(this, "哎呀", "请选中一个回答点赞！", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    QModelIndex idx0 = model2->index(curRow, 0);
    QModelIndex idx3 = model2->index(curRow, 3);

    QString a_id = model2->data(idx0).toString();
    QString creator_id = model2->data(idx3).toString();
    if (m_pUser->getId().compare(creator_id) == 0)
    {
        QMessageBox::information(this, "哎呀", "用户不能点赞自己的回答的！", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    for (int i = 0; i < g_User.size(); i++)
    {
        CUser* pUser = g_User.at(i);
        //if (pUser->getId().compare(creator_id) != 0)
        //{
        //    continue;
        //}//先找到用户
        int q_size = pUser->m_vecQuestion.size();
        for (int k = 0; k < q_size; k++)
        {
            Question* q = (Question *)pUser->m_vecQuestion.at(k);

            //if (q->getId().compare(question_id) != 0)
            //{
            //    continue;
            //}//再找到问题

            int a_size = q->m_vecAns.size();
            for (int m = 0; m < a_size; m++)
            {
                if (q->m_vecAns.at(m)->getId().compare(a_id) == 0)
                {
                    int z_size = q->m_vecAns.at(m)->m_vecPrise.size();
                    for (int z = 0; z < z_size; z++)
                    {
                        if (q->m_vecAns.at(m)->m_vecPrise.at(z)->getId().compare(pUser->getId()) == 0)
                        {
                            QMessageBox::information(this, "调皮", "该问题你已经点赞过了，你只能点一个赞你知道不", QMessageBox::Ok, QMessageBox::NoButton);
                            return;
                        }
                    }
                    q->m_vecAns.at(m)->increasePraiseNum();
                    q->m_vecAns.at(m)->m_vecPrise.push_back(pUser);
                    refresh_list2(q);
                    return;
                }//找到回答直接把点赞数加1
            }
        }
    }

}

//关注用户
void Widget::on_pushButton_14_clicked()
{
    //
    int curRow=ui->tableView->currentIndex().row();

    if (curRow == -1)
    {
        QMessageBox::information(this, "调皮", "请在问题或者回答中选中要关注的用户，才能关注呀", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    QModelIndex idx3 = model->index(curRow, 3);
    QString creator_id = model->data(idx3).toString();

    if (m_pUser->getId().compare(creator_id) == 0)
    {
        QMessageBox::information(this, "呵呵", "不能关注自己。", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    int size = m_pUser->focuslist.size();
    for (int i = 0; i < size; i++)
    {
        CUser* tmp = m_pUser->focuslist.at(i);
        if (tmp->getId().compare(creator_id) == 0)
        {
            QMessageBox::information(this, "哎呀", "已经关注过该用户！", QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
    }

    for (int i = 0; i < g_User.size(); i++)
    {
        CUser* pUser = g_User.at(i);
        if (pUser->getId().compare(creator_id) == 0)
        {
            m_pUser->focuslist.push_back(pUser);
            QMessageBox::information(this, "耶！", "关注成功~", QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
    }
}

//关注用户通过回答界面
void Widget::on_pushButton_15_clicked()
{

    int curRow=ui->tableView_2->currentIndex().row();

    if (curRow == -1)
    {
        QMessageBox::information(this, "哦弄", "请在问题或者回答中选中要关注的用户", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    QModelIndex idx3 = model2->index(curRow, 3);
    QString creator_id = model2->data(idx3).toString();

    if (m_pUser->getId().compare(creator_id) == 0)
    {
        QMessageBox::information(this, "呵呵", "不能关注自己= =", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    int size = m_pUser->focuslist.size();
    for (int i = 0; i < size; i++)
    {
        CUser* tmp = m_pUser->focuslist.at(i);
        if (tmp->getId().compare(creator_id) == 0)
        {
            QMessageBox::information(this, "哦弄", "已经关注过该用户", QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
    }

    for (int i = 0; i < g_User.size(); i++)
    {
        CUser* pUser = g_User.at(i);
        if (pUser->getId().compare(creator_id) == 0)
        {
            m_pUser->focuslist.push_back(pUser);
            QMessageBox::information(this, "( •̀ ω •́ )y", "关注成功", QMessageBox::Ok, QMessageBox::NoButton);
            return;
        }
    }
}

//1
void Widget::on_pushButton_3_clicked()
{
    //
    q_pager = 1;
    if (g_QuestionSearch.size() > 0)
    {
        this->refresh_list_search();
    }
    else
    {
        this->refresh_list();
    }
}

void Widget::on_pushButton_4_clicked()
{
    //
    if (q_pager == 1)
    {
        return;
    }
    q_pager--;
    if (g_QuestionSearch.size() > 0)
    {
        this->refresh_list_search();
    }
    else
    {
        this->refresh_list();
    }
}

void Widget::on_pushButton_5_clicked()
{
    //
    if (g_QuestionSearch.size() > 0)
    {
        int total = g_QuestionSearch.size() % PAGER_PER == 0 ? g_QuestionSearch.size() / PAGER_PER : g_QuestionSearch.size() / PAGER_PER + 1;
        if (q_pager == total)
        {
            return;
        }
        q_pager++;
        this->refresh_list_search();
    }
    else
    {
        int total = g_Question.size() % PAGER_PER == 0 ? g_Question.size() / PAGER_PER : g_Question.size() / PAGER_PER + 1;
        if (q_pager == total)
        {
            return;
        }
        q_pager++;
        this->refresh_list();
    }
}

void Widget::on_pushButton_6_clicked()
{
    //
    if (g_QuestionSearch.size() > 0)
    {
        q_pager = g_QuestionSearch.size() % PAGER_PER == 0 ? g_QuestionSearch.size() / PAGER_PER : g_QuestionSearch.size() / PAGER_PER + 1;
        this->refresh_list_search();
    }
    else
    {
        q_pager = g_Question.size() % PAGER_PER == 0 ? g_Question.size() / PAGER_PER : g_Question.size() / PAGER_PER + 1;
        this->refresh_list();
    }
}

//1
void Widget::on_pushButton_8_clicked()
{
    //
    a_pager = 1;
    this->refresh_list2(pQuestion);
}

void Widget::on_pushButton_9_clicked()
{
    //
    if (a_pager == 1)
    {
        return;
    }
    a_pager--;
    this->refresh_list2(pQuestion);
}

void Widget::on_pushButton_7_clicked()
{
    //
    int total = pQuestion->m_vecAns.size() % PAGER_PER == 0 ? pQuestion->m_vecAns.size() / PAGER_PER : pQuestion->m_vecAns.size() / PAGER_PER + 1;
    if (a_pager == total)
    {
        return;
    }
    a_pager++;
    this->refresh_list2(pQuestion);
}

void Widget::on_pushButton_10_clicked()
{
    //
    a_pager = pQuestion->m_vecAns.size() % PAGER_PER == 0 ? pQuestion->m_vecAns.size() / PAGER_PER : pQuestion->m_vecAns.size() / PAGER_PER + 1;
    this->refresh_list2(pQuestion);
}


void Widget::on_pushButton_16_clicked()
{
    FocusUser panel;
    panel.postUser(this->m_pUser);
    panel.show();
    if(panel.exec() == QDialog::Accepted)
    {
        ;
    }
}

//按问题的标题关键字搜索
void Widget::on_pushButton_17_clicked()
{
    //
    g_QuestionSearch.clear();
    QString key = ui->lineEdit->text();
    for (int i = 0; i < g_Question.size(); i++)
    {
        if (g_Question.at(i)->search(key) == 0)
        {
            g_QuestionSearch.push_back(g_Question.at(i));//先找到跟关键字匹配的项，然后添加到问题搜索集合里
        }
    }
    refresh_list_search();
}

void Widget::refresh_list_search()
{
    model->removeRows(0,model->rowCount());

    int size = g_QuestionSearch.size();
    int index = 0;
    int count = 0;
    int mm = 0;
    for(int i = (q_pager-1) * PAGER_PER; i < size && count < PAGER_PER; i++, count++)
    {
        index = 0;
        Question *pq = (Question *)g_QuestionSearch.at(i);
        model->setItem(mm,index++,new QStandardItem(pq->getId()));
        //设置字符颜色
        //model->item(i,0)->setForeground(QBrush(QColor(255, 0, 0)));
        //设置字符位置
        //model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(mm,index++,new QStandardItem(pq->getTitle()));
        model->setItem(mm,index++,new QStandardItem(pq->getContent()));
        model->setItem(mm,index++,new QStandardItem(pq->getCreateId()));
        model->setItem(mm,index++,new QStandardItem(pq->getCreateTime().toString("yyyy-MM-dd hh:mm:ss")));
        mm++;
    }
    int k = index - 1;
    model->sort(k, Qt::DescendingOrder);

    int total = g_QuestionSearch.size() % PAGER_PER == 0 ? g_QuestionSearch.size() / PAGER_PER : g_QuestionSearch.size() / PAGER_PER + 1;
    QString display = QString("%1/%2").arg(total > 0 ? q_pager : 0).arg(total);
    ui->label_3->setText(display);
}

//按时间范围搜索
void Widget::on_pushButton_18_clicked()
{
    QDate start = ui->dateEdit->date();
    QDate end = ui->dateEdit_2->date();
    g_QuestionSearch.clear();

    for (int i = 0; i < g_Question.size(); i++)
    {
        if (g_Question.at(i)->getCreateTime().date() >= start && g_Question.at(i)->getCreateTime().date() <= end)
        {
            g_QuestionSearch.push_back(g_Question.at(i));//先找到跟关键字匹配的项，然后添加到问题搜索集合里
        }
    }
    refresh_list_search();
}

/*
1. 采用先保存用户，再保存用户下问题信息，再保存问题对应的答案信息
*/
void Widget::save()
{
    try {
        QFile file("save.txt");
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            //qDebug()<<"Can't open the file!"<<endl;
            return;
        }
        QTextStream stream(&file);

        int user_size = g_User.size();
        stream<<user_size<<"\n";
        for (int user_i = 0; user_i < user_size; user_i++)
        {
            CUser* pUser = g_User.at(user_i);//先保存用户
            //在这里保存用户信息
            stream<<*pUser;

            QVector<Question*> pVecQuestion = pUser->m_vecQuestion;
            int q_size = pVecQuestion.size();
            //再保存问题的大小
            stream<<q_size<<"\n";
            for (int q_i = 0; q_i < q_size; q_i++)
            {
                Question* pQuestion = pVecQuestion.at(q_i);
                //在这里保存问题信息
                stream<<*pQuestion;

                QVector<Answer*> pVecAns = pQuestion->m_vecAns;
                int a_size = pVecAns.size();
                //再保存答案的大小
                stream<<a_size<<"\n";
                for (int a_i = 0; a_i < a_size; a_i++)
                {
                    Answer* pAns = pVecAns.at(a_i);
                    stream<<*pAns;
                }
            }
        }
    } catch(QString exception)
    {
        QMessageBox::about(this,"Error",exception);
    }
}

void Widget::on_pushButton_19_clicked()
{
    save();
}

//加载文件里面的信息
void Widget::load_file()
{
    try {
        QFile file("save.txt");
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            //qDebug()<<"Can't open the file!"<<endl;
            return;
        }
        QTextStream stream(&file);

        g_User.clear();
        int user_size = 0;
        //stream>>user_size;
        user_size = stream.readLine().toInt();
        for (int user_i = 0; user_i < user_size; user_i++)
        {
            CUser *pUser = new CUser();
            g_User.push_back(pUser);
            stream>>*pUser;
            int q_size = 0;
            //stream>>q_size;
            q_size = stream.readLine().toInt();
            for (int q_i = 0; q_i < q_size; q_i++)
            {
                Question *pQuestion = new Question();
                pQuestion->setUser(pUser);
                pUser->m_vecQuestion.push_back(pQuestion);
                addAnsGlobal(pQuestion);
                stream>>*pQuestion;
                int a_size = 0;
                //stream>>a_size;
                a_size = stream.readLine().toInt();
                for (int a_i = 0; a_i < a_size; a_i++)
                {
                    Answer *pAns = new Answer();
                    pAns->setUser(pUser);
                    pQuestion->m_vecAns.push_back(pAns);
                    stream>>*pAns;
                    g_Answer.push_back(pAns);
                }
            }
        }

        //
        int a_size = g_Answer.size();
        for (int i = 0; i < a_size; i++)
        {
            Answer *pans = g_Answer.at(i);
            QStringList list = pans->getStr().split(',');
            for (int list_i = 0; list_i < list.size(); list_i++)
            {
                user_size = g_User.size();
                for (int user_i = 0; user_i < user_size; user_i++)
                {
                    CUser *pUser = g_User.at(user_i);
                    if (pUser->getId().compare(list.at(list_i)) == 0)
                    {
                        pans->m_vecPrise.push_back(pUser);
                    }
                }
            }
        }
    } catch(QString exception)
    {
        QMessageBox::about(this,"Error",exception);
    }
}

//将问题添加到全局的集合里面
void Widget::addAnsGlobal(Question *q)
{
    if (g_Question.size() == 0)
    {
        g_Question.push_front(q);
    }
    else if (q->getCreateTime() > g_Question.at(0)->getCreateTime())
    {
        g_Question.push_front(q);
    }
    else
    {
        int size = g_Question.size();
        int i = 0;
        for (i = 0; i < size - 1; i++)
        {
            if (q->getCreateTime() < g_Question.at(i)->getCreateTime()
                    && q->getCreateTime() > g_Question.at(i+1)->getCreateTime())
            {
                g_Question.insert(i+1, q);
                break;
            }
        }
        if (i == size - 1)
        {
            g_Question.push_back(q);
        }
    }
}

void Widget::on_pushButton_20_clicked()
{
    load_file();
}

