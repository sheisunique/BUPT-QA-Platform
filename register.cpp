#include "register.h"
#include "ui_register.h"

#include <QMessageBox>

//注册
Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    ret = 0;
}

Register::~Register()
{
    delete ui;
}

//取消按钮
void Register::on_pushButton_clicked()
{
    ret = 0;
    this->accept();
}

//注册按钮
void Register::on_pushButton_2_clicked()
{
    ret = 0;
    QString username = ui->lineEdit->text();
    QString name = ui->lineEdit_2->text();
    QString password = ui->lineEdit_3->text();
    if (username.isEmpty() || name.isEmpty() || password.isEmpty())
    {
        QMessageBox box(QMessageBox::NoIcon,"调皮","袋鼠提示，注册信息要完全哦！");
                box.setStandardButtons (QMessageBox::Ok);
                box.setStyleSheet("background-color:rgb(209,238,238)");
                box.setButtonText (QMessageBox::Ok,QString("我知道啦"));
                box.setIconPixmap(QPixmap("icon1.jpg"));
                box.exec ();
        return;
    }
    int size = g_User.size();
    for (int i = 0; i < size; i++)
    {
        CUser* pu = g_User.at(i);
        QString id = pu->getId();
        if (id.compare(username) == 0)
        {
            QMessageBox box(QMessageBox::NoIcon,"调皮","袋鼠提示，你想设置的ID已经被别人用了呢，重新选择吧~");
                    box.setStandardButtons (QMessageBox::Ok);
                    box.setStyleSheet("background-color:rgb(209,238,238)");
                    box.setButtonText (QMessageBox::Ok,QString("我知道啦"));
                    box.setIconPixmap(QPixmap("icon1.jpg"));
                    box.exec ();
            return;
        }
    }
    register_info = QString("%1#%2#%3").arg(username).arg(name).arg(password);
    ret = 1;
    this->accept();
}

int Register::getRet()
{
    return ret;
}
