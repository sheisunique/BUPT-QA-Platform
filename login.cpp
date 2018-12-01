#include "login.h"
#include "ui_login.h"

#include <QMessageBox>
#include "base_info.h"
#include "register.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

//登录按钮
void Login::on_pushButton_2_clicked()
{
    //分别获得用户名和密码
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    if (password.isEmpty() || username.isEmpty())
    {
        QMessageBox box(QMessageBox::NoIcon,"哎呀","袋鼠提示，用户名或密码不能为空呀！");
                box.setStandardButtons (QMessageBox::Ok);
                box.setStyleSheet("background-color:rgb(255,240,245)");
                box.setButtonText (QMessageBox::Ok,QString("我知道啦"));
                box.setIconPixmap(QPixmap("icon1.jpg"));
                box.exec ();
        return;
    }
    int size = g_User.size();
    ret = 0;  //标识，检测是否找到这个用户
    for (int i = 0; i < size; i++)
    {
        CUser* pu = g_User.at(i);
        if (pu->login(username, password) == 0)
        {
            m_username = username;
            ret = 1;
            break;
        }
    }
    if (ret == 1)
    {
        accept();
    }
    else
    {
        QMessageBox box(QMessageBox::NoIcon,"哎呀","用户名或密码错误！");
                box.setStandardButtons (QMessageBox::Ok);
                box.setStyleSheet("background-color:rgb(255,240,245)");
                box.setButtonText (QMessageBox::Ok,QString("我知道啦"));
                box.setIconPixmap(QPixmap("icon1.jpg"));
                box.exec ();
        return;
    }
}

void Login::on_pushButton_clicked()
{
    ret = 0;
    accept();
}

void Login::on_pushButton_3_clicked()
{
    Register dlg;
    dlg.setModal(true);
    dlg.show();
    int ret = dlg.exec();
    if(ret == QDialog::Accepted)
    {
        if (dlg.getRet() == 1)
        {
            QStringList list = dlg.getRegisterInfo().split('#');
            CUser *pUser = new CUser(list.at(0), list.at(1), list.at(2));
            g_User.push_back(pUser);
            QMessageBox box(QMessageBox::NoIcon,"成功！","快开始你的袋鼠问答之旅吧！");
                    box.setStandardButtons (QMessageBox::Ok);
                    box.setStyleSheet("background-color:rgb(255,240,245)");
                    box.setButtonText (QMessageBox::Ok,QString("我知道啦"));
                    box.setIconPixmap(QPixmap("icon1.jpg"));
                    box.exec ();
            return;
        }
    }
}
