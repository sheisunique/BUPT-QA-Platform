#ifndef QUESTIONPANEL_H
#define QUESTIONPANEL_H

#include <QDialog>
#include <QString>

namespace Ui {
class QuestionPanel;
}

class QuestionPanel : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionPanel(QWidget *parent = 0);
    ~QuestionPanel();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::QuestionPanel *ui;

public:
    QString title;
    QString content;

    int ret;
};

#endif // QUESTIONPANEL_H
