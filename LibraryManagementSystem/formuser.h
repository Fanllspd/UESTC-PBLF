#ifndef FORMUSER_H
#define FORMUSER_H

#include "basicfunction.h"
#include <QWidget>

namespace Ui {
class FormUser;
}

class FormUser : public QWidget
{
    Q_OBJECT

public:
    explicit FormUser(QWidget *parent = nullptr, BooksList *booksList = nullptr, UserInfo userInfo = {},UsersList *uList = nullptr);
    ~FormUser();

    void RecvUserInfo(UserInfo userInfo);

    void SetBorrowBook(QString bookName);

    void onSetBorrowedBook(QString BookName);

private slots:

    void SetAvatar();

    int colorvalue(QTimer *timer);

    void animateButton(QToolButton *toolButton, QTimer *timer);

    void on_toolButton_Change_clicked();

    void on_toolButton_Cancel_clicked();

    void on_toolButton_Logout_clicked();

    void on_toolButton_Confirm_clicked();

    void on_toolButton_Delete_clicked();

    void SetUsersList();

    void on_toolButton_Search_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_customContextMenuRequested(const QPoint &pos);

    void onDeleteUser(QString uname);

    void on_lineEdit_Search_textChanged(const QString &arg1);

private:
    Ui::FormUser *ui;
    UserInfo userInfo;
    UsersList *userList;
    BooksList *booksList;

    int x;
    char *Username;
    char *AvatarUrl;

signals:
    void logoutRequested();
};

#endif // FORMUSER_H
