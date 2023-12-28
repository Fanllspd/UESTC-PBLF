#ifndef DIALOGREGISTER_H
#define DIALOGREGISTER_H

#include "basicfunction.h"
#include <QDialog>


namespace Ui {
class DialogRegister;
}

class DialogRegister : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRegister(QWidget *parent = nullptr, UsersList *userList = nullptr);
    ~DialogRegister();

    bool eventFilter(QObject *obj, QEvent *event);

    QString GetRegisterUname();

private slots:

    // QPixmap PixmapToRound(const QPixmap &src, int radius);

    void on_toolButton_close_clicked();

    void SetAvatar();

    void on_toolButton_Comfirm_clicked();

    void on_lineEdit_Username_textEdited(const QString &arg1);

    void on_lineEdit_Password_textEdited(const QString &arg1);

    void on_lineEdit_RePasswd_textEdited(const QString &arg1);

    // bool SaveUsersInfo();

private:
    Ui::DialogRegister *ui;
    UsersList *userList;
    // DialogLogin *login;

    bool mousePressed;
    QPoint dragPosition;
    int UserId;
    QString username;
    QString password;
    QString rePasswd;
    QString salt;
    QString Quote;
    QString AvatarUrl;
    // bool ifRepeat;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
};

#endif // DIALOGREGISTER_H
