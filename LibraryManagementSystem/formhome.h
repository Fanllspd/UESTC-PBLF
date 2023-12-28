#ifndef FORMHOME_H
#define FORMHOME_H

#include <QWidget>
#include <QStandardItemModel>
#include "basicfunction.h"
#include "qtoolbutton.h"

namespace Ui {
class FormHome;
}

class FormHome : public QWidget
{
    Q_OBJECT

public:
    explicit FormHome(QWidget *parent = nullptr, BooksList *booksList = nullptr, UsersList *uList = nullptr, UserInfo uInfo = {});
    ~FormHome();

    void ReloadFile();

    bool SaveFile(QString path);

    void RefreshFile();

private slots:

    void on_toolButton_Search_clicked();

    int colorvalue(QTimer *timer);

    void animateButton(QToolButton *toolButton, QTimer *timer);

    void on_toolButton_refresh_clicked();

    void on_toolButton_add_clicked();

    void on_toolButton_change_clicked();

    void on_toolButton_remove_clicked();

    // void on_tableView_clicked(const QModelIndex &index);

    // void on_tableView_activated(const QModelIndex &index);

    // void on_tableView_pressed(const QModelIndex &index);

    // void RefreshBooks();


    void on_toolButton_BorrowOrReturn_clicked();

    void on_lineEdit_Search_textChanged(const QString &arg1);

private:
    Ui::FormHome *ui;
    QStandardItemModel *model;
    BooksList *booksList;
    UsersList *usersList;
    UserInfo userInfo;

    int BorrowId;
    // QTimer *timer;
    QTimer *AddTimer;
    QTimer *RefreshTimer;
    QTimer *ChangeTimer;
    QTimer *RemoveTimer;
    QTimer *SearchTimer;
    int x;
    int rowToSelect;



protected:

signals:
    void setBorrowedBook(QString BookName);
    // void mousePressEvent(QMouseEvent *event);
};

#endif // FORMHOME_H
