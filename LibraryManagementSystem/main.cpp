#include "dialoglogin.h"
#include "dialoglogin.h"
#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <Windows.h>   // For Windows API functions and types
#include <QGuiApplication>
#include <QString>
#include <QStandardItemModel>
#include <QSharedMemory>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough); // 屏幕自适应
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication app(argc, argv);
    QFile styleFile(":/qdarkstyle/dark/darkstyle.qss"); // 注意路径的设置
    styleFile.open(QFile::ReadOnly);

    // 读取QSS文件内容
    QString style = styleFile.readAll();

    // 设置QApplication的样式表
    app.setStyleSheet(style);



    // w.show();

    // DialogLogin login;// = new DialogLogin();
    // MainWindow *w;

    // w.setLayout(layout);

    QSharedMemory shared("apa");
    if(shared.attach()) //共享内存被占用则直接返回
    {
        QMessageBox::information(NULL,QStringLiteral("警告"),QStringLiteral("程序已在运行中！"));
        return 0;
    }
    shared.create(1);//共享内存没有被占用则创建UI
    // if (login.exec() == QDialog::Accepted){// || login.CheckAutoSave()) {
    //     // qDebug() << "login END";
    //     w = new MainWindow(nullptr, login.getUserInfo());
    //     w->setWindowTitle("图书馆管理系统");
    //     w->show();	// 主界面显示
    //     return app.exec();
    // }
    while (true) {
        DialogLogin *login = new DialogLogin;
        login->setWindowTitle("登录");
        if (login->exec() == QDialog::Accepted) {
            MainWindow *w = new MainWindow(nullptr, login->getUserInfo(), login->getUsersList());
            w->setWindowTitle("图书馆管理系统");
            w->show();
            app.exec();
            // qDebug() << "whw0";
            if(!w->shouldRelogin())
                break;
        } else {
            break;
            // return app.exec();
        }
    }
    return 0;
}
