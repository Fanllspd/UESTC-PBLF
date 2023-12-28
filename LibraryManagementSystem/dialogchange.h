#ifndef DIALOGCHANGE_H
#define DIALOGCHANGE_H

#include "basicfunction.h"
#include <QDialog>
#include <QLineEdit>

namespace Ui {
class DialogChange;
}

class DialogChange : public QDialog
{
    Q_OBJECT

public:
    explicit DialogChange(QWidget *parent = nullptr, BooksList *BooksList = nullptr, int rowToSelect = -1, QStandardItemModel *model = nullptr);
    ~DialogChange();

private slots:
    void on_toolButton_close_clicked();

    void isLineEditTextChanged(const QString &text, QLineEdit *lineEdit);

    void on_lineEdit_Title_textChanged(const QString &arg1);

    // void on_lineEdit_InsertPos_textChanged(const QString &arg1);

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

    void DisplayData();

private:
    Ui::DialogChange *ui;
    BooksList *booksList;
    int rowToSelect;
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

#endif // DIALOGCHANGE_H
