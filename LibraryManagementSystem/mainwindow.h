#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "formhome.h"
#include "formuser.h"
#include "formsetting.h"
#include "formabout.h"
#include <QPropertyAnimation>
// #include "framelesswindow.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, UserInfo userInfo = {}, UsersList *uList = nullptr);
    ~MainWindow();

    void recvCheck(bool checked, float opacity);

    bool shouldRelogin() const { return m_shouldRelogin; }

public slots:

private slots:

    void on_toolButton_close_clicked();

    void on_toolButton_maximize_clicked();

    void on_toolButton_minimize_clicked();



    void on_toolButton_about_clicked();

    void on_toolButton_setting_clicked();

    void on_toolButton_home_clicked();

    void on_toolButton_user_clicked();

    void onLogoutRequested();

    void handleData(const QString &data);

private:


    Ui::MainWindow *ui;
    BooksList *booksList;
    UserInfo userInfo;
    UsersList *usersList;

    bool m_shouldRelogin = false;
    bool mousePressed = false;
    QPoint dragPosition;
    FormHome *formHome;
    FormUser *formUser;
    FormSetting *formSetting;
    FormAbout *formAbout;
    QPainter *painter;
    QColor shadowColor{0, 0, 0, 150};
    int shadowOffset = 50;
    int shadowBlurRadius = 200;
    int cornerRadius = 50;
    bool shouldPaint = true;

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void sendBorrowedBook(const QString &data);
};

#endif // MAINWINDOW_H
