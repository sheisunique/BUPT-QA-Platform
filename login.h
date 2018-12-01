#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QString>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Login *ui;

public:
    int ret;
    QString m_username;
};

#endif // LOGIN_H
