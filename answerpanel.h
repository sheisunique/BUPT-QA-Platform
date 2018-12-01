#ifndef ANSWERPANEL_H
#define ANSWERPANEL_H

#include <QDialog>
#include <QString>

namespace Ui {
class AnswerPanel;
}

class AnswerPanel : public QDialog
{
    Q_OBJECT

public:
    explicit AnswerPanel(QWidget *parent = 0);
    ~AnswerPanel();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::AnswerPanel *ui;
    QString content;

public:
    QString getContent();
    int ret;
};

#endif // ANSWERPANEL_H
