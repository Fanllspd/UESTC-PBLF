#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "dialoglogin.h"
#include "qdebug.h"
#include <QMouseEvent>
#include <QEvent>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QBitmap>
#include <QtMath>

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif


MainWindow::MainWindow(QWidget *parent, UserInfo uInfo, UsersList *uList)
    : QMainWindow(parent), userInfo(uInfo), usersList(uList)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    InitBooksList(&booksList); // 初始化链表

    this->setWindowIcon(QIcon(":/source/logo.ico")); // Logo
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint); // 自定义界面
    this->setAttribute(Qt::WA_TranslucentBackground); // 界面背景透明
    // this->setAttribute(Qt::WA_DeleteOnClose,true);//使用时记得加头文件<Qt>
    // setAttribute(Qt::WA_QuitOnClose, true);
    this->setProperty("canMove", true);
    this->setContentsMargins(1,1,1,1);    // 给垂直布局器设置边距(此步很重要, 设置宽度为阴影的宽度)

    shouldPaint = true; // 根据是否最大化来判断要不要渲染阴影

    formHome = new FormHome(this, booksList, usersList, userInfo);
    formUser = new FormUser(this, booksList, userInfo, usersList);
    formSetting = new FormSetting(this, booksList, formHome, userInfo.isAdmin);
    formAbout = new FormAbout();
    ui->stackedWidget->addWidget(formHome);
    ui->stackedWidget->addWidget(formUser);
    ui->stackedWidget->addWidget(formSetting);
    ui->stackedWidget->addWidget(formAbout);

    ui->stackedWidget->setCurrentIndex(0);
    ui->toolButton_home->setChecked(true);

    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(this->ui->label_2);
    opacityEffect->setOpacity(0.5);  // 设置透明度（范围：0.0到1.0）

    // 将透明度效果应用到 QLabel 上
    this->ui->label_2->setGraphicsEffect(opacityEffect);

    connect(formSetting, &FormSetting::sendCheck, this, &MainWindow::recvCheck);
    connect(formUser, &FormUser::logoutRequested, this, &MainWindow::onLogoutRequested);

    connect(formHome, &FormHome::setBorrowedBook, this, &MainWindow::handleData);
    connect(this, &MainWindow::sendBorrowedBook, formUser, &FormUser::onSetBorrowedBook);

    // if(userInfo.BorrowId){
    //     BooksList *Cur = booksList->next;
    //     while(Cur){
    //         if(Cur->data.Book_Id == userInfo.BorrowId){
    //             emit sendBorrowedBook(Cur->data.Book_Title);
    //         }
    //     }
    // }else
    //     emit sendBorrowedBook("无");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleData(const QString &data) {
    emit sendBorrowedBook(data);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << usersList->next->data.Username ;
    SaveUsersInfo(usersList);
    SaveBooksInfo(booksList, SAVE_DEFAULT_NAME_JSON);
    // ClearAutoSave();

    event->accept();
}

void MainWindow::onLogoutRequested() {
    m_shouldRelogin = true;
    this->close();
    QApplication::quit();
    // DialogLogin login;
    // login.exec();
}

void MainWindow::recvCheck(bool checked, float opacity)
{
    if(checked)
        setWindowOpacity(opacity + 0.1);
    else
        setWindowOpacity(1.0);
}

void MainWindow::paintEvent(QPaintEvent *event)
{


    // QBitmap bmp(this->size());
    // bmp.fill();
    // QPainter p(&bmp);
    // p.setPen(Qt::NoPen);
    // p.setBrush(Qt::black);
    // p.drawRoundedRect(bmp.rect(),15,15);//像素为10的圆角
    // p.setRenderHint(QPainter::Antialiasing, true);
    // p.setRenderHint(QPainter::SmoothPixmapTransform);
    // setMask(bmp);

    // QPainterPath path;
    // path.setFillRule(Qt::WindingFill);
    // path.addRoundedRect(0, 0, bmp.width(), bmp.height(), 2, 2);

    // QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing, true);
    // painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    // painter.fillPath(path, QBrush(QColor(230,230,230,30)));

    if(shouldPaint){
        QPainter painter2(this);
        painter2.setRenderHint(QPainter::Antialiasing, true);

        QColor color(0, 0, 0, 50);
        for(int i=0; i<10; i++)
        {
            color.setAlpha(150 - qSqrt(i)*50);
            painter2.setPen(color);
            // drawRect 绘制矩形
            //        painter.drawRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
            // drawRoundedRect 绘制带原角
            painter2.drawRoundedRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2, 15, 15);
        }
    }


    // QPainterPath path;
    // path.setFillRule(Qt::WindingFill);
    // path.addRect(10, 10, this->width()-20, this->height()-20);

    // QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing, true);
    // // painter.fillPath(path, QBrush(Qt::white));

    // QColor color(0, 0, 0, 20);
    // for(int i=0; i<10; i++)
    // {
    //     QPainterPath path;
    //     //path.setFillRule(Qt::WindingFill);
    //     path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
    //     color.setAlpha(150 - qSqrt(i)*50);
    //     painter.setPen(color);
    //     painter.drawPath(path);
    // }
}


void MainWindow::on_toolButton_close_clicked()
{
    MainWindow::close();
    QApplication::quit();
}


void MainWindow::on_toolButton_maximize_clicked()
{
    if (MainWindow::isMaximized())
    {
        shouldPaint = true;
        ui->centralwidget->setStyleSheet("QWidget{\
                        background-color:rgb(44, 42, 56);\
                    }\
                    #centralwidget{\
                        margin: 9 9 9 9;\
                        border-radius: 15px;\
            }");
        MainWindow::showNormal();//还原事件
        ui->toolButton_maximize->setIcon(QIcon(QPixmap(":/qss_icons/dark/rc/checkbox_unchecked@2x.png")));
    }
    else
    {
        shouldPaint = false;
        ui->centralwidget->setStyleSheet("background-color:rgb(44, 42, 56);");
        MainWindow::showMaximized();//最大化事件
        ui->toolButton_maximize->setIcon(QIcon(QPixmap(":/qss_icons/dark/rc/window_undock@2x.png")));
    }
}


void MainWindow::on_toolButton_minimize_clicked()
{
    MainWindow::showMinimized();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {

        mousePressed = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && mousePressed)
    {
        //当窗口最大化或最小化时也不进行触发

        if(MainWindow::isMaximized() || MainWindow::isMinimized())
            return;
        else
        {
            //当在按钮之类需要鼠标操作的地方不进行触发(防误触)
                if (!ui->toolButton_close->underMouse()||!ui->toolButton_maximize->underMouse()||!ui->toolButton_minimize->underMouse())
                {
                    int maxWidth = this->width();
                    int maxHeight = 80;
                    if(event->pos().x()<=0 ||event->pos().x()>maxWidth)
                        return;
                    if(event->pos().y()<=0 || event->pos().y()>maxHeight)
                        return;
                    MainWindow::move(event->globalPos() - dragPosition);//移动
                }

        }
    }
    event->accept();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        mousePressed = false;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 在标题栏区域双击时，移动窗口

        int maxWidth = this->width();
        int maxHeight = 80;
        if(event->pos().x()<=0 ||event->pos().x()>=maxWidth)
            return;
        if(event->pos().y()<=0 || event->pos().y()>=maxHeight)
            return;
        if (MainWindow::isMaximized()) {
            showNormal();
            shouldPaint = true;
            ui->centralwidget->setStyleSheet("QWidget{\
                        background-color:rgb(44, 42, 56);\
                    }\
                    #centralwidget{\
                        margin: 9 9 9 9;\
                        border-radius: 15px;\
            }");
            ui->toolButton_maximize->setIcon(QIcon(QPixmap(":/qss_icons/dark/rc/checkbox_unchecked@2x.png")));
        } else {
            showMaximized();
            shouldPaint = false;
            ui->centralwidget->setStyleSheet("background-color:rgb(44, 42, 56);");
            ui->toolButton_maximize->setIcon(QIcon(QPixmap(":/qss_icons/dark/rc/window_undock@2x.png")));
        }
    }
    event->accept();
}



void MainWindow::on_toolButton_home_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_toolButton_user_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_toolButton_setting_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_toolButton_about_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}
