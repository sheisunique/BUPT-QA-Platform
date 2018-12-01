#ifndef FOCUSUSER_H
#define FOCUSUSER_H

#include <QDialog>
#include <QStandardItemModel>
#include "user.h"

namespace Ui {
class FocusUser;
}

class FocusUser : public QDialog
{
    Q_OBJECT

public:
    explicit FocusUser(QWidget *parent = 0);
    ~FocusUser();
    void postUser(CUser* pUser);
    void refresh_list();
    void refresh_list2();

private slots:
    void on_pushButton_clicked();


    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();

    void on_tableView_4_doubleClicked(const QModelIndex &index);

private:
    Ui::FocusUser *ui;
    QStandardItemModel  *model;
    QStandardItemModel  *model2;
    QStandardItemModel  *model4;

    CUser *mpUser;
    int q_pager;//问题分页
    int a_pager;//回答分页
};

#endif // FOCUSUSER_H
