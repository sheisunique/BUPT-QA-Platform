#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QStandardItemModel>
#include <QFile>

#include "user.h"
#include "base_info.h"
#include "answer.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void setUser(QString username);
    void refresh_list();
    void refresh_list2(Question* q);
    void refresh_list_search();
    void save();
    void load_file();
    void addAnsGlobal(Question *q);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

private:
    Ui::Widget *ui;
    QString m_username;
    CUser* m_pUser;
    QStandardItemModel  *model;
    QStandardItemModel  *model2;

    Question* pQuestion;
    QString question_id;

    int q_pager;//问题分页
    int a_pager;//回答分页
};

#endif // WIDGET_H
