#include "focususer.h"
#include "ui_focususer.h"

#include "base_info.h"
#include <QMessageBox>

#define PAGER_PER 2

FocusUser::FocusUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FocusUser)
{
    ui->setupUi(this);

    model4 = new QStandardItemModel();
    model4->setColumnCount(1);
    int index = 0;
    model4->setHeaderData(index++,Qt::Horizontal,("我关注的用户"));

    ui->tableView_4->setSelectionMode( QAbstractItemView::SingleSelection);
    ui->tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_4->setModel(model4);
    //表头信息显示居左
    ui->tableView_4->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->tableView_4->show();

    model = new QStandardItemModel();
    model->setColumnCount(5);
    index = 0;
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

    mpUser = NULL;

}

FocusUser::~FocusUser()
{
    delete ui;
}

void FocusUser::on_pushButton_clicked()
{
    accept();
}

void FocusUser::postUser(CUser* pUser)
{
    model4->removeRows(0,model4->rowCount());

    int size = pUser->focuslist.size();
    int index = 0;
    for (int i = 0; i < size; i++)
    {
        index = 0;
        CUser* tmp = pUser->focuslist.at(i);
        model4->setItem(i,index++,new QStandardItem(tmp->getId()));
    }
}

void FocusUser::on_tableView_4_doubleClicked(const QModelIndex &index)
{
    //
    int curRow=ui->tableView_4->currentIndex().row();

    if (curRow == -1)
    {
        QMessageBox::information(this, "告警", "请选择要查看的用户", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    QModelIndex idx3 = model4->index(curRow, 0);
    QString user_id = model4->data(idx3).toString();

    int size = g_User.size();
    for (int i = 0; i < size; i++)
    {
        CUser *pUser = g_User.at(i);
        if (pUser->getId().compare(user_id) == 0)
        {
            mpUser = pUser;
        }
    }

    if (mpUser == NULL) return;

    q_pager = 1;
    a_pager = 1;

    refresh_list();
    refresh_list2();

}

void FocusUser::refresh_list()
{
    model->removeRows(0,model->rowCount());

    int size = mpUser->m_vecQuestion.size();
    int index = 0;
    int count = 0;
    int mm = 0;
    for(int i = (q_pager-1) * PAGER_PER; i < size && count < PAGER_PER; i++, count++)
    {
        index = 0;
        Question *pq = (Question *)mpUser->m_vecQuestion.at(i);
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

    int total = mpUser->m_vecQuestion.size() % PAGER_PER == 0 ? mpUser->m_vecQuestion.size() / PAGER_PER : mpUser->m_vecQuestion.size() / PAGER_PER + 1;
    QString display = QString("%1/%2").arg(total > 0 ? q_pager : 0).arg(total);
    ui->label_3->setText(display);
}

void FocusUser::refresh_list2()
{
    model2->removeRows(0,model2->rowCount());

    int index = 0;
    int count = 0;
    int mm = 0;
    for (int m = (a_pager - 1) * PAGER_PER; m < mpUser->m_vecAnswer.size() && count < PAGER_PER; m++, count++)
    {
        index = 0;
        Answer* a = (Answer*)mpUser->m_vecAnswer.at(m);
        model2->setItem(mm,index++,new QStandardItem(a->getId()));
        model2->setItem(mm,index++,new QStandardItem(a->getContent()));
        model2->setItem(mm,index++,new QStandardItem(QString("%1").arg(a->getPraiseNum())));
        model2->setItem(mm,index++,new QStandardItem(a->getCreateId()));
        model2->setItem(mm,index++,new QStandardItem(a->getCreateTime().toString("yyyy-MM-dd hh:mm:ss")));
        mm++;
    }
    int k = index - 1;
    model2->sort(k, Qt::DescendingOrder);

    int total = mpUser->m_vecAnswer.size() % PAGER_PER == 0 ? mpUser->m_vecAnswer.size() / PAGER_PER : mpUser->m_vecAnswer.size() / PAGER_PER + 1;
    QString display = QString("%1/%2").arg(total > 0 ? a_pager : 0).arg(total);
    ui->label_4->setText(display);
}

//1
void FocusUser::on_pushButton_3_clicked()
{
    //

    if (mpUser == NULL) return;
    q_pager = 1;
    this->refresh_list();
}

void FocusUser::on_pushButton_4_clicked()
{
    //
    if (mpUser == NULL) return;
    if (q_pager == 1)
    {
        return;
    }
    q_pager--;
    this->refresh_list();
}

void FocusUser::on_pushButton_5_clicked()
{
    //
    if (mpUser == NULL) return;
    int total = mpUser->m_vecQuestion.size() % PAGER_PER == 0 ? mpUser->m_vecQuestion.size() / PAGER_PER : mpUser->m_vecQuestion.size() / PAGER_PER + 1;
    if (q_pager == total)
    {
        return;
    }
    q_pager++;
    this->refresh_list();
}

void FocusUser::on_pushButton_6_clicked()
{
    //
    if (mpUser == NULL) return;
    q_pager = mpUser->m_vecQuestion.size() % PAGER_PER == 0 ? mpUser->m_vecQuestion.size() / PAGER_PER : mpUser->m_vecQuestion.size() / PAGER_PER + 1;
    this->refresh_list();
}

//1
void FocusUser::on_pushButton_10_clicked()
{
    //
    if (mpUser == NULL) return;
    a_pager = 1;
    this->refresh_list2();
}

void FocusUser::on_pushButton_8_clicked()
{
    //
    if (mpUser == NULL) return;
    if (a_pager == 1)
    {
        return;
    }
    a_pager--;
    this->refresh_list2();
}

void FocusUser::on_pushButton_9_clicked()
{
    //
    if (mpUser == NULL) return;
    int total = mpUser->m_vecAnswer.size() % PAGER_PER == 0 ? mpUser->m_vecAnswer.size() / PAGER_PER : mpUser->m_vecAnswer.size() / PAGER_PER + 1;
    if (a_pager == total)
    {
        return;
    }
    a_pager++;
    this->refresh_list2();
}

void FocusUser::on_pushButton_7_clicked()
{
    //
    if (mpUser == NULL) return;
    a_pager = mpUser->m_vecAnswer.size() % PAGER_PER == 0 ? mpUser->m_vecAnswer.size() / PAGER_PER : mpUser->m_vecAnswer.size() / PAGER_PER + 1;
    this->refresh_list2();
}


