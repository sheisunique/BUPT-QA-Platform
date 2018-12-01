#include "widget.h"
#include <QApplication>

#include "login.h"
#include "base_info.h"
#include "dialog.h"

QVector<CUser*> g_User;
QVector<Question*> g_Question;
QVector<Question*> g_QuestionSearch;
QVector<Answer*> g_Answer;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    //模拟创建初始用户
    CUser* pUser1 = new CUser("a", "嘻嘻", "a");g_User.push_back(pUser1);
    CUser* pUser2 = new CUser("b", "哈哈", "b");g_User.push_back(pUser2);
    CUser* pUser3 = new CUser("c", "吼吼", "c");g_User.push_back(pUser3);
    //三个字符串分别是，ID,用户昵称，密码

    Dialog dlg;
    dlg.show();
    int ret = dlg.exec();
    if(ret == QDialog::Accepted)
    {
        Login login;
        login.show();
        ret = login.exec();
        if(ret == QDialog::Accepted)
        {
            if (login.ret == 1)
            {
                w.setUser(login.m_username);
                w.show();
                return a.exec();
            }
        }

    }

    return 0;
}
