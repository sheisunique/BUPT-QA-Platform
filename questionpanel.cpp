#include "questionpanel.h"
#include "ui_questionpanel.h"

#include <QMessageBox>

QuestionPanel::QuestionPanel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionPanel)
{
    ui->setupUi(this);
}

QuestionPanel::~QuestionPanel()
{
    delete ui;
}

void QuestionPanel::on_pushButton_clicked()
{
    //
    ret = 0;
    accept();
}

void QuestionPanel::on_pushButton_2_clicked()
{
    //
    ret = 0;

    title = ui->lineEdit->text();
    content = ui->textEdit->toPlainText();
    if (title.isEmpty())
    {
        QMessageBox::information(this, "哎呀", "标题不能为空的！", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }
    if (content.isEmpty())
    {
        QMessageBox::information(this, "哎呀", "内容不能为空的！", QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    ret = 1;

    accept();
}
