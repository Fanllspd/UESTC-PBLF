#include "dialogregister.h"
#include "qmath.h"
#include "qpainter.h"
#include "qpainterpath.h"
#include "ui_dialogregister.h"
#include "qevent.h"
#include "basicfunction.h"
#include <QDebug>
#include <QMessageBox>
#include "cJSON.h"
// #include "dialoglogin.h"
#include <QCryptographicHash>
#include <QString>
#include <QByteArray>
#include <QRandomGenerator>
// #include "basicfunction.h"

DialogRegister::DialogRegister(QWidget *parent, UsersList *uList)
    : QDialog(parent), userList(uList)
    , ui(new Ui::DialogRegister)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/source/logo.ico"));
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint); // 自定义界面
    this->setAttribute(Qt::WA_TranslucentBackground); // 界面背景透明
    this->setAttribute(Qt::WA_QuitOnClose, true);
    this->ui->label_Avatar->setAttribute(Qt::WA_TranslucentBackground, true); // 头像背景透明
    SetAvatar(); // 圆形头像
    this->setFocusPolicy(Qt::ClickFocus); // 点击空白处使LineEdit失去焦点
    this->ui->toolButton_Comfirm->setEnabled(false);
    this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
            background-color: rgba(0, 170, 255, 80);\
            border-radius: 10px;\
            color: #ecf0f1;\
        }");

    ui->label_Register->installEventFilter(this);

}

DialogRegister::~DialogRegister()
{
    delete ui;
}

void DialogRegister::SetAvatar()
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

void DialogRegister::paintEvent(QPaintEvent *event)
{

     QPainter painter(this);
     painter.setRenderHint(QPainter::Antialiasing, true);
    // painter.fillPath(path, QBrush(Qt::white));

     QColor color(0, 0, 0, 150);
     for(int i=0; i<10; i++)
     {
         QPainterPath path;
         path.setFillRule(Qt::WindingFill);
         path.addRoundedRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2, 10, 10);
         color.setAlpha(150 - qSqrt(i)*50);
         painter.setPen(color);
         painter.drawPath(path);
     }

    // Q_UNUSED(event)

    // QPainter painter(this);
    // painter.setRenderHint(QPainter::Antialiasing, true);

    // QColor color(0, 0, 0, 50);
    // for(int i=0; i<10; i++)
    // {
    //     color.setAlpha(150 - qSqrt(i)*50);
    //     painter.setPen(color);
    //     // drawRect 绘制矩形
    //     //        painter.drawRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
    //     // drawRoundedRect 绘制带原角
    //     painter.drawRoundedRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2, 15, 15);
    // }


}

void DialogRegister::on_toolButton_close_clicked()
{
    //QApplication::quit();
    DialogRegister::close();
}

void DialogRegister::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {

        mousePressed = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
    }
}

void DialogRegister::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && mousePressed)
    {
        //当窗口最大化或最小化时也不进行触发

        if(DialogRegister::isMaximized() || DialogRegister::isMinimized())
            return;
        else
        {
            //当在按钮之类需要鼠标操作的地方不进行触发(防误触)
            if (!ui->toolButton_close->underMouse())
            {
                int maxWidth = this->width();
                int maxHeight = 80;
                if(event->pos().x()<=0 ||event->pos().x()>maxWidth)
                    return;
                if(event->pos().y()<=0 || event->pos().y()>maxHeight)
                    return;
                DialogRegister::move(event->globalPos() - dragPosition);//移动
            }

        }
    }
    event->accept();
}

void DialogRegister::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        mousePressed = false;
}



void DialogRegister::on_toolButton_Comfirm_clicked()
{
    username = this->ui->lineEdit_Username->text();
    password = this->ui->lineEdit_Password->text();
    rePasswd = this->ui->lineEdit_RePasswd->text();
    AvatarUrl = "";

    qDebug() << userList->counts;

    QString prefix = "This is ";
    Quote = prefix + username;
    qDebug() << Quote;

    if(strcmp(password.toUtf8().constData(), rePasswd.toUtf8().constData())){
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("两次密码不一致！"));
        return;
    }
    bool ifRepeat = true;

    cJSON *root = NULL,
        *item = ReadUsers(&root);

    if (!item)
    {
        // printf("JSON文件有误！\n");
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("UsersInfo.json文件有误！"));
        cJSON_Delete(root);
        return;
    }

    for (int i = 0; i < cJSON_GetArraySize(item); i++) {
        cJSON *arr = cJSON_GetArrayItem(item, i);
        if(arr != NULL){
            qDebug() << username << cJSON_GetObjectItem(arr,"Username")->valuestring << strcmp(username.toUtf8().constData(), cJSON_GetObjectItem(arr,"Username")->valuestring);
            if(!strcmp(username.toUtf8().constData(), cJSON_GetObjectItem(arr,"Username")->valuestring)) {
                ifRepeat = true;
                AvatarUrl = cJSON_GetObjectItem(arr, "AvatarUrl")->valuestring;
                break;
            }else{
                ifRepeat = false;
            }
        }
    }
    if(!ifRepeat){


            UserInfo info;
            salt = generateSalt(8);
            password = calculateSaltedMD5(password.toUtf8().constData(), salt);

            info.UserId = userList->counts + 1;
            strcpy_s(info.Username, username.toUtf8().constData());
            strcpy_s(info.Password, password.toUtf8().constData());
            strcpy_s(info.Salt, salt.toUtf8().constData());
            strcpy_s(info.AvatarUrl, AvatarUrl.toUtf8().constData());
            info.isAdmin = false;
            strcpy_s(info.Quote, Quote.toUtf8().constData());
            info.BorrowId = 0;

            if(InsertUser(userList, info) && SaveUsersInfo(userList)){
                QMessageBox::information(NULL,QStringLiteral("恭喜"),QStringLiteral("注册成功！"));
                QDialog::accept();
                return;
            } else
                QMessageBox::warning(NULL,QStringLiteral("错误"),QStringLiteral("注册失败！"));

    } else
        QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("用户名重复！"));
    // this->ui->lineEdit_Username->setText(,"Name")->valuestring);
    // fclose(fp);
}

// bool DialogRegister::SaveUsersInfo(){
//     cJSON *root = NULL,
//         *item = ReadUsers(&root);
//     if (!item)
//     {
//         // printf("JSON文件有误！\n");
//         QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("UsersInfo.json文件有误！"));
//         cJSON_Delete(root);
//         return false;
//     }


//             // cJSON_ReplaceItemInObject(item, "Username", cJSON_Parse(username));
//     cJSON *newUser = cJSON_CreateObject();
//     cJSON_AddItemToObject(newUser, "Username", cJSON_CreateString(username.toUtf8().constData()));
//     cJSON_AddItemToObject(newUser, "Password", cJSON_CreateString(password.toUtf8().constData()));
//     cJSON_AddItemToObject(newUser, "Salt", cJSON_CreateString(salt.toUtf8().constData()));
//     cJSON_AddBoolToObject(newUser, "isAdmin", false);
//     cJSON_AddItemToObject(newUser, "AvatarUrl", cJSON_CreateString(""));
//     // char* prefix = "This is ";

//     cJSON_AddItemToObject(newUser, "Quote", cJSON_CreateString(Quote.toUtf8().constData()));
//             // username = cJSON_GetObjectItem(item,"Username")->valuestring; //登录信息
//             // bool AutoSaveCheck = cJSON_GetObjectItem(item,"Check")->valueint;
//             // double AutoSaveDeadline = cJSON_GetObjectItem(item,"Deadline")->valuedouble;
//     cJSON_AddItemToArray(item, newUser);
//     return SaveJsonToFile(root, SAVE_DEFAULT_USERS_JSON);;
// }

QString DialogRegister::GetRegisterUname()
{
    return username;
}

void DialogRegister::on_lineEdit_Username_textEdited(const QString &arg1)
{
    if(arg1.isEmpty() || this->ui->lineEdit_Password->text().isEmpty() || this->ui->lineEdit_RePasswd->text().isEmpty()){
        this->ui->toolButton_Comfirm->setEnabled(false);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgba(0, 170, 255, 80);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }");
    }else{
        this->ui->toolButton_Comfirm->setEnabled(true);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgba(0, 170, 255, 150);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }\
            \
            /* 鼠标悬停 */\
            QToolButton:hover{\
                background-color: rgba(0, 170, 255, 190);\
            }\
        \
            /* 点击和按下 */\
            QToolButton:pressed,QToolButton:checked{\
                /*border-left: 1px solid #34495e80; \
                border-top: 1px solid #34495e80;\
                border-right: none;\
                border-bottom:none;*/\
                background-color: rgba(0, 170, 255, 240);\
                color: rgba(236, 240, 241, 60);\
            }\
        ");
    }
}


void DialogRegister::on_lineEdit_Password_textEdited(const QString &arg1)
{
    if(arg1.isEmpty() || this->ui->lineEdit_Username->text().isEmpty() || this->ui->lineEdit_RePasswd->text().isEmpty()){
        this->ui->toolButton_Comfirm->setEnabled(false);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgba(0, 170, 255, 100);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }");
    }else{
        this->ui->toolButton_Comfirm->setEnabled(true);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgba(0, 170, 255, 150);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }\
            \
            /* 鼠标悬停 */\
            QToolButton:hover{\
                background-color: rgba(0, 170, 255, 190);\
            }\
        \
            /* 点击和按下 */\
            QToolButton:pressed,QToolButton:checked{\
                /*border-left: 1px solid #34495e80; \
                border-top: 1px solid #34495e80;\
                border-right: none;\
                border-bottom:none;*/\
                background-color: rgba(0, 170, 255, 240);\
                color: rgba(236, 240, 241, 60);\
            }\
        ");
    }
}

void DialogRegister::on_lineEdit_RePasswd_textEdited(const QString &arg1)
{
    if(arg1.isEmpty() || this->ui->lineEdit_Username->text().isEmpty() || this->ui->lineEdit_Password->text().isEmpty()){
        this->ui->toolButton_Comfirm->setEnabled(false);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgba(0, 170, 255, 100);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }");
    }else{
        this->ui->toolButton_Comfirm->setEnabled(true);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgba(0, 170, 255, 150);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }\
            \
            /* 鼠标悬停 */\
            QToolButton:hover{\
                background-color: rgba(0, 170, 255, 190);\
            }\
        \
            /* 点击和按下 */\
            QToolButton:pressed,QToolButton:checked{\
                /*border-left: 1px solid #34495e80; \
                border-top: 1px solid #34495e80;\
                border-right: none;\
                border-bottom:none;*/\
                background-color: rgba(0, 170, 255, 240);\
                color: rgba(236, 240, 241, 60);\
            }\
        ");
    }
}

bool DialogRegister::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->label_Register)
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

            if(mouseEvent->button() == Qt::LeftButton)
            {
                QDialog::reject();
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else{
        // pass the event on to the parent class
        return QDialog::eventFilter(obj, event);
    }
}



