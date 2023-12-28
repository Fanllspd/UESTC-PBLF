#include "formuser.h"
// #include "dialoglogin.h"
// #include "mainwindow.h"
#include "cJSON.h"
#include "qinputdialog.h"
#include "qmainwindow.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "qtimer.h"
#include "ui_formuser.h"
#include <QDebug>
#include <QMessageBox>
#include <QMenuBar>

FormUser::FormUser(QWidget *parent, BooksList *bList, UserInfo info, UsersList *uList)
    : QWidget(parent), booksList(bList), userInfo(info), userList(uList)
    , ui(new Ui::FormUser)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->ui->label_Avatar->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    x = 2;
    qDebug() << "userName:" << userInfo.Username;
    SetAvatar();
    // SetUsersList();
    OutputAllUsers(userList, this->ui->listWidget, userInfo.UserId);
    this->ui->lineEdit_Username->setText(QString(userInfo.Username));
    this->ui->lineEdit_Quote->setText(userInfo.Quote);
    this->ui->toolButton_Cancel->setVisible(false);
    this->ui->toolButton_Confirm->setVisible(false);
    // this->ui->lineEdit_Quote->setEnabled(false);

    // 使用 customContextMenuRequested 信号则需要设置
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);


    if(userInfo.BorrowId){
        BooksList *Cur = booksList->next;
        while(Cur){
            if(Cur->data.Book_Id == userInfo.BorrowId){
                onSetBorrowedBook(Cur->data.Book_Title);
                break;
            }
            Cur = Cur->next;
        }
    }else
        onSetBorrowedBook("无");
}

FormUser::~FormUser()
{
    delete ui;
}


void FormUser::SetAvatar()
{
    QPixmap img1(":/source/avatar.jpg");
    QSize size(90, 90);
    QPixmap image = img1.scaled(size.width(), size.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // 缩放图片

    // 创建一个新的QPixmap用于绘制边框
    QPixmap borderPixmap(size);
    borderPixmap.fill(Qt::transparent); // 透明背景

    QPainter painter(&borderPixmap);
    painter.setPen(Qt::NoPen);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 白色Border
    int borderSize = 2; // Border粗细
    QPainterPath borderPath;
    borderPath.addEllipse(0, 0, size.width(), size.height());
    painter.fillPath(borderPath, QColor(255,255,255,150));

    // Set the path for the image (smaller ellipse inside the border)
    QPainterPath imagePath;
    imagePath.addEllipse(borderSize, borderSize, size.width() - (borderSize * 2), size.height() - (borderSize * 2));

    // Clip the painter to the image path and draw the image
    painter.setClipPath(imagePath);
    painter.drawPixmap(borderSize, borderSize, image.scaled(size.width() - (borderSize * 2), size.height() - (borderSize * 2), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    painter.end(); // Finish painting

    this->ui->label_Avatar->setPixmap(borderPixmap);
}

int FormUser::colorvalue(QTimer *timer)
{
    int y = 0;
    if(x <= 255)
    {
        y = x;
    }
    else
    {
        y = 511 - x;
    }
    x++;
    if(x == 510)
    {
        x = 2;


        timer->stop();
    }
    return y;
}

void FormUser::animateButton(QToolButton *toolButton, QTimer *timer)
{
    QString colorValueString = QString::number(this->colorvalue(timer));
    toolButton->setStyleSheet(QString("QToolButton{\
                background-color: #2c3e50;\
                border: 2px solid rgba(46, 204, 113, %1);\
                border-radius: 5px;\
                font-size: 12px;\
            }\
            QToolButton:hover{/*\
                border: 2px solid rgba(46, 204, 113, %1);\
                color: #ecf0f1;\
                font-weight: bold;\
            */}\
            QToolButton:pressed,QToolButton:checked{\
                /*border-left: 2px outset #34495e80;\
                border-top: 2px outset #34495e80;\
                border-right: none;\
                border-bottom:none;*/\
                background-color: #34495e;\
            }").arg(colorValueString));


    if (!timer->isActive()) {
        toolButton->setStyleSheet("QToolButton{\
                background-color: #2c3e50;\
                border: 4px solid #2c3e5080;\
                border-radius: 5px;\
                font-size: 12px;\
            }\
            QToolButton:hover{\
                border: 1px solid #bdc3c7;\
                color: #ecf0f1;\
                font-weight: bold;\
            }\
            QToolButton:pressed,QToolButton:checked{\
                border-left: 2px outset #34495e80;\
                border-top: 2px outset #34495e80;\
                border-right: none;\
                border-bottom:none;\
                background-color: #34495e;\
            }");
    }
}

void FormUser::on_toolButton_Change_clicked()
{
    ////// 点击边框变色
    QTimer *ChangeTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(ChangeTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Change, ChangeTimer);});
    ChangeTimer->start(1);
    ////// END

    this->ui->toolButton_Change->setVisible(false);
    this->ui->toolButton_Cancel->setVisible(true);
    this->ui->toolButton_Confirm->setVisible(true);

    this->ui->lineEdit_Username->setReadOnly(false);
    this->ui->lineEdit_Quote->setReadOnly(false);


}


void FormUser::on_toolButton_Cancel_clicked()
{
    ////// 点击边框变色
    QTimer *CancelTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(CancelTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Cancel, CancelTimer);});
    CancelTimer->start(1);
    ////// END

    this->ui->toolButton_Cancel->setVisible(false);
    this->ui->toolButton_Confirm->setVisible(false);
    this->ui->toolButton_Change->setVisible(true);

    this->ui->lineEdit_Username->setReadOnly(true);
    this->ui->lineEdit_Quote->setReadOnly(true);
}


void FormUser::on_toolButton_Logout_clicked()
{
    ////// 点击边框变色
    QTimer *LogoutTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(LogoutTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Logout, LogoutTimer);});
    LogoutTimer->start(1);
    ////// END

    ClearAutoSave(); // 设置AutoSave.json的数据为空
    emit logoutRequested(); // 登出
}


void FormUser::on_toolButton_Confirm_clicked()
{
    ////// 点击边框变色
    QTimer *ConfirmTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(ConfirmTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Confirm, ConfirmTimer);});
    ConfirmTimer->start(1);
    ////// END

    this->ui->lineEdit_Username->setReadOnly(true);
    this->ui->lineEdit_Quote->setReadOnly(true);

    this->ui->toolButton_Cancel->setVisible(false);
    this->ui->toolButton_Confirm->setVisible(false);
    this->ui->toolButton_Change->setVisible(true);

    UserInfo info;
    info = userInfo;
    strcpy_s(info.Username, this->ui->lineEdit_Username->text().toUtf8().constData());
    strcpy_s(info.Quote, this->ui->lineEdit_Quote->text().toUtf8().constData());
    // qDebug() << info.Password;

    if(ChangeUser(userList, info)){ ////ChangeUserinfo(userInfo, this->ui->lineEdit_Username->text(), this->ui->lineEdit_Quote->text())){
        QMessageBox::information(NULL,QStringLiteral("恭喜"),QStringLiteral("修改成功！"));
        strcpy_s(userInfo.Username, info.Username);
        strcpy_s(userInfo.Quote, info.Quote);
    }else{
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("修改失败！"));
        this->ui->lineEdit_Username->setText(userInfo.Username);
        this->ui->lineEdit_Quote->setText(userInfo.Quote);
    }
}


void FormUser::on_toolButton_Delete_clicked()
{
    ////// 点击边框变色
    QTimer *DeleteTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(DeleteTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Delete, DeleteTimer);});
    DeleteTimer->start(1);
    ////// END

    int check1 = QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("确定要删除吗！"), QMessageBox::Ok | QMessageBox::Cancel);
    if(check1 == QMessageBox::Ok){
        // QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("修改失败！"));
        if(DeleteUser(userList, userInfo.UserId)){
            ClearAutoSave();
            emit logoutRequested();
            QMessageBox::information(NULL,QStringLiteral("恭喜"),QStringLiteral("注销成功！"));
        }else
            QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("注销失败！"));
    }
}

void FormUser::SetUsersList()
{
    this->ui->listWidget->clear();
    OutputAllUsers(userList, this->ui->listWidget, userInfo.UserId);
    cJSON *root = NULL,
        *item = ReadUsers(&root);
    for (int i = 0; i < cJSON_GetArraySize(item); ++i) {
        cJSON *arr = cJSON_GetArrayItem(item, i);
        if(arr){
            QString name = cJSON_GetObjectItem(arr, "Username")->valuestring;
            if(!strcmp(userInfo.Username, name.toUtf8().constData()))
                name += /*QString("<font color = #2ecc71 >%1</font>").arg*/("<--[ME]");

            // this->ui->listWidget->addItem(item);
            // QString text = QString("<font color='red'>红色</font>和<font color='blue'>蓝色</font>的文字");
            // QListWidgetItem *item = new QListWidgetItem();
            // item->setText(text);
            QLabel* label=new QLabel(name);
            label->setStyleSheet(QString("QLabel {\
                            height: 45px;\
                            border-bottom: 1px solid rgba(255,255,255,10);\
                        }\
\
                        QLabel:hover {\
                            background-color: transparent;\
                            padding: 10px;\
                            border-left: 3px solid rgb(130, 130, 130);\
                        }\
                        QLabel:selected {\
                            background-color: transparent;\
                            color: black;\
                            padding: 10px;\
                            border-left: 3px solid black;\
                        }\
\
                        QLabel{\
                            height: 45px;\
                            border-bottom: 1px solid rgba(255,255,255,10);\
                        }"));
            // QListWidgetItem* item = new QListWidgetItem(this->ui->listWidget);
            this->ui->listWidget->addItem(name);

        }}
}

void FormUser::onSetBorrowedBook(QString BookName)
{
    this->ui->label_Book->setText(QString("当前借阅书籍：" + BookName));
}

void FormUser::on_toolButton_Search_clicked()
{
    ////// 点击边框变色
    QTimer *SearchTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(SearchTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Search, SearchTimer);});
    SearchTimer->start(1);
    ////// END

    UsersList *Result = SearchUser(userList, ui->lineEdit_Search->text(),this->ui->checkBox->isChecked());
    OutputAllUsers(Result, this->ui->listWidget, userInfo.UserId);

}


void FormUser::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if(!userInfo.isAdmin){
        QMessageBox::warning(NULL,QStringLiteral("错误"),QStringLiteral("只有管理员能修改!"));
        return;
    }
    bool ok;
    QString newUname = QInputDialog::getText(this, tr("设置"), tr("新用户名："),QLineEdit::Normal,item->text(), &ok);
    if(ok && !newUname.isEmpty()){
        UsersList *Cur = userList->next, *found = NULL;
        while (Cur) {
            if(!strcmp(item->text().toUtf8().constData(), Cur->data.Username))
                found = Cur;
            if(!strcmp(newUname.toUtf8().constData(), Cur->data.Username)){
                QMessageBox::warning(NULL,QStringLiteral("错误"),QStringLiteral("用户名重复!"));
                return;
            }
            Cur = Cur->next;
        }
        strcpy_s(found->data.Username, newUname.toUtf8().constData());
        SaveUsersInfo(userList);
        item->setText(found->data.Username);
        QMessageBox::warning(NULL,QStringLiteral("恭喜"),QStringLiteral("修改成功!"));
    }
    QString newPasswd = QInputDialog::getText(this, tr("设置"), tr("新密码："),QLineEdit::Normal,"", &ok);
    if(ok && !newPasswd.isEmpty()){
        UsersList *Cur = userList->next;
        while (Cur) {
            if(!strcmp(item->text().toUtf8().constData(), Cur->data.Username))
                break;
            Cur = Cur->next;
        }
        QString Salt = generateSalt(8);
        strcpy_s(Cur->data.Salt, Salt.toUtf8().constData());
        strcpy_s(Cur->data.Password, calculateSaltedMD5(newPasswd.toUtf8().constData(), Salt).toUtf8().constData());
        SaveUsersInfo(userList);
        // item->setText(found->data.Username);
        QMessageBox::warning(NULL,QStringLiteral("恭喜"),QStringLiteral("修改成功!"));
    }
}


void FormUser::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    // qDebug() << 1;        qDebug() << 1;
    QMenu* pMenu = new QMenu(ui->listWidget);
    QAction* pActionProperties = new QAction(tr("删除用户"), pMenu);


    QListWidgetItem* currentItem = ui->listWidget->itemAt(pos);
    //只有当item不为空时才添加右键菜单
    if (currentItem != nullptr)
    {
        QString itemText = currentItem->text(); // 获取当前item的文本
        connect(pActionProperties, &QAction::triggered, this, [=] {
            onDeleteUser(itemText);//点击右键菜单需要执行的函数
        });
        pMenu->addAction(pActionProperties);
        // 确保菜单和其动作在隐藏后被删除
        connect(pMenu, &QMenu::aboutToHide, pMenu, &QMenu::deleteLater);
        pMenu->popup(ui->listWidget->mapToGlobal(pos));
        if(!userInfo.isAdmin || !strcmp(itemText.toUtf8().constData(), (QString(userInfo.Username) + "[ME]").toUtf8().constData()))
            pMenu->setEnabled(false);
    }
}

void FormUser::onDeleteUser(QString uname)
{
    int check = QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("确定要删除吗！"), QMessageBox::Ok | QMessageBox::Cancel);
    if(check == QMessageBox::Ok){
        UsersList *uCur = userList, *Del = userList->next;
        while(Del){
            if(!strcmp(Del->data.Username, uname.toUtf8().constData())){
                uCur->next = Del->next;
                free(Del);
                SaveUsersInfo(userList);
                QMessageBox::warning(NULL,QStringLiteral("恭喜"),QStringLiteral("删除成功!"));
                OutputAllUsers(userList, this->ui->listWidget, userInfo.UserId);
                break;
            }
            Del = Del->next;
            uCur = uCur->next;
        }
    }
}


void FormUser::on_lineEdit_Search_textChanged(const QString &arg1)
{
    UsersList *Result = SearchUser(userList, arg1, this->ui->checkBox->isChecked());
    OutputAllUsers(Result, this->ui->listWidget, userInfo.UserId);
}

