#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>
#include "user.h"
extern QVector<CUser*> g_User;

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();
    int getRet();
    QString getRegisterInfo() { return register_info; }

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Register *ui;

    int ret;
    QString register_info;
};

#endif // REGISTER_H
