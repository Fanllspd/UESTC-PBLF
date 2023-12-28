#ifndef DIALOGLOGIN_H
#define DIALOGLOGIN_H

#include "basicfunction.h"
#include <QDialog>
#include <QWidget>

namespace Ui {
class DialogLogin;
}

class DialogLogin : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogin(QWidget *parent = nullptr);
    ~DialogLogin();

    bool eventFilter(QObject *obj, QEvent *event);	// 添加时间过滤器声明

    UserInfo getUserInfo();

    bool CheckAutoSave();

    UsersList* getUsersList();

private slots:

    // QPixmap PixmapToRound(const QPixmap &src, int radius);

    void on_toolButton_close_clicked();

    void SetAvatar();

    void on_toolButton_Comfirm_clicked();

    void on_lineEdit_Username_textEdited(const QString &arg1);

    void on_lineEdit_Password_textEdited(const QString &arg1);

    void on_checkBox_stateChanged(int arg1);


private:
    Ui::DialogLogin *ui;
    UsersList *userList;


    bool mousePressed;
    QPoint dragPosition;

    int UserId;
    QString Username;
    QString Password;
    QString Salt;
    QString Quote;
    bool isAdmin;
    QString AvatarUrl;
    int BorrowId;

    bool AutoSave;
    bool loginSuccess;
    bool AutoLogin;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);


};

#endif // DIGLOGLOGIN_H
