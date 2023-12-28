#ifndef DIALOGADD_H
#define DIALOGADD_H

#include "basicfunction.h"
#include <QDialog>
#include <QLineEdit>

namespace Ui {
class DialogAdd;
}

class DialogAdd : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAdd(QWidget *parent = nullptr, BooksList *BooksList = nullptr, QStandardItemModel *model = nullptr);
    ~DialogAdd();

private slots:
    void on_toolButton_close_clicked();

    void isLineEditTextChanged(const QString &text, QLineEdit *lineEdit);

    void on_lineEdit_Title_textChanged(const QString &arg1);

    void on_lineEdit_InsertPos_textChanged(const QString &arg1);

    void on_lineEdit_Author_textChanged(const QString &arg1);

    void on_lineEdit_Publisher_textChanged(const QString &arg1);

    void on_lineEdit_Keywords_textChanged(const QString &arg1);

    void on_lineEdit_PublishDate_textChanged(const QString &arg1);

    void on_lineEdit_Language_textChanged(const QString &arg1);

    void on_lineEdit_CopyAmount_textChanged(const QString &arg1);

    void on_lineEdit_ISBN_textChanged(const QString &arg1);

    void on_lineEdit_Category_textChanged(const QString &arg1);

    void accept();

    void reject();

    bool areAllLineEditsFilled(const QList<QLineEdit *> &lineEdits);

    bool isInputNoSpace(const QString &input);

    void on_SwitchButton_Available_clicked(bool checked);

    void on_SwitchButton_eBook_clicked(bool checked);

    void startShakeAnimation(QLineEdit *lineEdit);

private:
    Ui::DialogAdd *ui;
    BooksList *booksList;
    QStandardItemModel *model;

    bool mousePressed;
    QPoint dragPosition;

    int InsertPos;
    QString Title;
    QString Author;
    QString Publisher;
    QString PublishDate;
    QString Language;
    bool Available;
    int CopyAmount;
    QString ISBN;
    QStringList Keywords;
    QString Category;
    bool eBook;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // DIALOGADD_H
