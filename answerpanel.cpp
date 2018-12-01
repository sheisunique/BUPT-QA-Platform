#include "answerpanel.h"
#include "ui_answerpanel.h"

#include <QMessageBox>

AnswerPanel::AnswerPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AnswerPanel)
{
    ui->setupUi(this);
}

AnswerPanel::~AnswerPanel()
{
    delete ui;
}

void AnswerPanel::on_pushButton_clicked()
{
    ret = 0;
    accept();
}

void AnswerPanel::on_pushButton_2_clicked()
{
    //
    ret = 0;

    content = ui->textEdit->toPlainText();
    if (content.isEmpty())
    {
        QMessageBox::information(this, "哎呀", "提问内容不能为空的！", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    ret = 1;

    accept();
}

QString AnswerPanel::getContent()
{
    return content;
}
