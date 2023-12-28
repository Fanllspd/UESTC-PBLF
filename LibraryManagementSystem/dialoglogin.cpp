#include "dialoglogin.h"
#include "basicfunction.h"
#include "dialogregister.h"
#include "qpainterpath.h"
#include "ui_dialoglogin.h"
#include <QAction>
#include "qevent.h"
#include "qpainter.h"
#include <QMessageBox>
#include <QtMath>
#include "qgraphicseffect.h"
#include <QtMath>
#include <QPushButton>
#include <QSequentialAnimationGroup>
#include <QDebug>
#include <QValidator>
#include "cJSON.h"
#include <QDateTime>

DialogLogin::DialogLogin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogLogin)
{
    ui->setupUi(this);
    InitUsersList(&userList);

    this->setWindowIcon(QIcon(":/source/logo.ico"));
    this->setWindowFlags(Qt::FramelessWindowHint); // 自定义界面
    this->setAttribute(Qt::WA_TranslucentBackground); // 界面背景透明
    // setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_QuitOnClose, true);
    this->ui->label_Avatar->setAttribute(Qt::WA_TranslucentBackground, true); // 头像背景透明
    SetAvatar(); // 圆形头像
    this->setFocusPolicy(Qt::ClickFocus); // 点击空白处使LineEdit失去焦点
    this->ui->toolButton_Comfirm->setEnabled(false);
    this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
            background-color: rgba(246, 210, 133, 150);\
            border: 1px solid  rgba(246, 210, 133, 50);\
            border-radius: 10px;\
            color: #ecf0f1;\
        }");
    ui->label_FindPwd->installEventFilter(this);
    ui->label_Register->installEventFilter(this);

    loginSuccess = false;
    AutoLogin = false;
    BorrowId = 0;
    CheckAutoSave();

    // DialogRegister Reg(this);
    // Reg.setWindowTitle("注册");
    // Reg.exec();

    /*
    自定义LineEdit的一键删除（太丑了有点，懒得改
    */
    // QAction *clearAction = new QAction(this->ui->lineEdit_Password);
    // clearAction->setIcon(QIcon(":/qss_icons/dark/rc/window_close.png")); // 设置你的自定义图标
    // this->ui->lineEdit_Password->addAction(clearAction, QLineEdit::TrailingPosition); // 将动作添加到文本框尾部
    // connect(clearAction, &QAction::triggered, this->ui->lineEdit_Password, &QLineEdit::clear); // 连接信号和槽
    // connect(this->ui->toolButton_Comfirm, &QToolButton::clicked, this, &DialogLogin::SendUserInfo);

}

DialogLogin::~DialogLogin()
{
    delete ui;
}

void DialogLogin::SetAvatar()
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




void DialogLogin::paintEvent(QPaintEvent *event)
{
    //this->setContentsMargins(1,1,1,1);
     // QPainterPath path;
     // path.setFillRule(Qt::WindingFill);
     // path.addRect(10, 10, this->width()-20, this->height()-20);

    if(AutoLogin)
        QDialog::accept();
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

void DialogLogin::on_toolButton_close_clicked()
{
    //QApplication::quit();
    DialogLogin::close();
}

void DialogLogin::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {

        mousePressed = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
    }
}

void DialogLogin::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && mousePressed)
    {
        //当窗口最大化或最小化时也不进行触发

        if(DialogLogin::isMaximized() || DialogLogin::isMinimized())
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
                DialogLogin::move(event->globalPos() - dragPosition);//移动
            }

        }
    }
    event->accept();
}

void DialogLogin::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        mousePressed = false;
}

UserInfo DialogLogin::getUserInfo()
{
    // qDebug() << "1:" << username;
    UserInfo info;
    UsersList *Cur = userList->next;

    info.UserId = UserId;

    while(Cur){
        if(Cur->data.UserId == UserId){
            strcpy_s(info.Username, Cur->data.Username);
            strcpy_s(info.Password, Cur->data.Password);
            strcpy_s(info.Salt, Cur->data.Salt);
            strcpy_s(info.AvatarUrl, Cur->data.AvatarUrl);
            strcpy_s(info.Quote, Cur->data.Quote);
            info.isAdmin = Cur->data.isAdmin;
            info.BorrowId = Cur->data.BorrowId;
            break;
        }
        Cur = Cur->next;
    }


    // qDebug() << info.BorrowId;
    return info;
}

UsersList* DialogLogin::getUsersList()
{
    return userList;
}


bool DialogLogin::CheckAutoSave()
{
    AutoSave = this->ui->checkBox->isChecked();

    cJSON *root = NULL,
        *item = ReadAutoSave(&root);

    if (!item)
    {
        // printf("JSON文件有误！\n");
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("AutoSaveInfo.json文件有误！"));
        cJSON_Delete(root);
        return false;
    }

    UserId = cJSON_GetObjectItem(item,"UserId")->valueint;
    Username = cJSON_GetObjectItem(item,"Username")->valuestring; //登录信息
    Password = cJSON_GetObjectItem(item,"Password")->valuestring;
    Salt = cJSON_GetObjectItem(item,"Salt")->valuestring;
    Quote = cJSON_GetObjectItem(item,"Quote")->valuestring;
    bool AutoSaveCheck = cJSON_GetObjectItem(item,"Check")->valueint;
    qint64 AutoSaveDeadline = std::atoll(cJSON_GetObjectItem(item,"Deadline")->valuestring);
    isAdmin = cJSON_GetObjectItem(item,"isAdmin")->valueint;
    BorrowId = cJSON_GetObjectItem(item,"BorrowId")->valueint;
    // qDebug() << "AutoSaveCheck";

    if(AutoSaveCheck){
        qint64 now = QDateTime::currentDateTime().toSecsSinceEpoch();
        // qDebug() << AutoSaveDeadline << (now) << (AutoSaveDeadline > now);
        if(AutoSaveDeadline < now){
            this->ui->checkBox->setChecked(false);

            ClearAutoSave(); // 设置AutoSave.json的数据为空
            return false;
        }else{

            AutoLogin = true;
            // 方便PaintEvent检测，（为什么不直接accept，因为在这里无效，无法直接关闭界面，进行跳转，可能是因为窗口层级问题？
            cJSON_SetValuestring(cJSON_GetObjectItem(item, "Deadline"), QString::number(QDateTime::currentDateTime().toSecsSinceEpoch() + 300 /*五分钟*/).toUtf8());

            // QDialog::accept();
        }
        return SaveJsonToFile(root, SAVE_DEFAULT_AUTOSAVE_JSON);
    }
    return false;
}

void DialogLogin::on_toolButton_Comfirm_clicked()
{
    Username = this->ui->lineEdit_Username->text();
    Password = this->ui->lineEdit_Password->text();

    // bool usernameUndefined = true;
    //AutoSave = this->ui->checkBox->isChecked();
    bool loginSuccess = false;

    cJSON *root = NULL,
        *item = ReadUsers(&root);

    if (!item)
    {
        // printf("JSON文件有误！\n");
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("UsersInfo.json文件有误！"));
        cJSON_Delete(root);
        return;
    }


    // 检索JSON比对密码
    for (int i = 0; i < cJSON_GetArraySize(item); i++) {
        cJSON *arr = cJSON_GetArrayItem(item, i);
        if(arr){
            // qDebug() << username;// << cJSON_GetObjectItem(arr,"Username")->valuestring;
            // qDebug() << 22;
            if(!strcmp(Username.toUtf8().constData(), cJSON_GetObjectItem(arr,"Username")->valuestring)) {
                if(verifyPassword(Password.toUtf8().constData(), cJSON_GetObjectItem(arr,"Salt")->valuestring, cJSON_GetObjectItem(arr,"Password")->valuestring)){
                    // qDebug() << "aaa";
                    // emit SendUserInfo(info);
                    UserId = cJSON_GetObjectItem(arr,"UserId")->valueint;
                    Username = cJSON_GetObjectItem(arr,"Username")->valuestring; //登录信息
                    Password = cJSON_GetObjectItem(arr,"Password")->valuestring;
                    Salt = cJSON_GetObjectItem(arr,"Salt")->valuestring;
                    isAdmin = cJSON_GetObjectItem(arr, "isAdmin")->valueint;
                    AvatarUrl = cJSON_GetObjectItem(arr, "AvatarUrl")->valuestring;
                    Quote = cJSON_GetObjectItem(arr,"Quote")->valuestring;
                    BorrowId = cJSON_GetObjectItem(arr, "BorrowId")->valueint;
                    loginSuccess = true;
                    // usernameUndefined = false;

                    break;
                }
            }
        }
    }


    AutoSave = this->ui->checkBox->isChecked();
    if(loginSuccess){
        cJSON *root = NULL;
        item = ReadAutoSave(&root);
        if (!item)
        {
            // printf("JSON文件有误！\n");
            QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("AutoSaveInfo.json文件有误！"));
            cJSON_Delete(root);
            return;
        }

        cJSON_SetIntValue(cJSON_GetObjectItem(item,"UserId"), UserId);
        cJSON_SetValuestring(cJSON_GetObjectItem(item, "Username"), Username.toUtf8().constData());
        cJSON_SetValuestring(cJSON_GetObjectItem(item, "Password"), Password.toUtf8().constData());
        cJSON_SetValuestring(cJSON_GetObjectItem(item, "Salt"), Salt.toUtf8().constData());
        cJSON_SetBoolValue(cJSON_GetObjectItem(item, "Check"), AutoSave);
        cJSON_SetBoolValue(cJSON_GetObjectItem(item, "isAdmin"), isAdmin);
        cJSON_SetValuestring(cJSON_GetObjectItem(item, "Deadline"), QString::number(QDateTime::currentDateTime().toSecsSinceEpoch() + 300 /*五分钟*/).toUtf8());
        cJSON_SetValuestring(cJSON_GetObjectItem(item, "Quote"), Quote.toUtf8().constData());
        cJSON_SetIntValue(cJSON_GetObjectItem(item, "BorrowId"), BorrowId);

        SaveJsonToFile(root, QString::fromUtf8(SAVE_DEFAULT_AUTOSAVE_JSON));

        QDialog::accept();

    }else{
        QMessageBox::warning(NULL,QStringLiteral("错误"),QStringLiteral("用户名或密码错误！"));
    }

    cJSON_Delete(root); // 记得删
}



void DialogLogin::on_lineEdit_Username_textEdited(const QString &arg1)
{
    if(arg1.isEmpty() || this->ui->lineEdit_Password->text().isEmpty()){
        this->ui->toolButton_Comfirm->setEnabled(false);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgba(246, 210, 133, 150);\
                border: 1px solid  rgba(246, 210, 133, 50);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }");
    }else{
        this->ui->toolButton_Comfirm->setEnabled(true);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgb(246, 210, 133);\
                border: 1px solid  rgba(246, 210, 133, 50);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }\
            \
            /* 鼠标悬停 */\
            QToolButton:hover{\
                background-color: rgba(246, 210, 133, 150);\
            }\
        \
            /* 点击和按下 */\
            QToolButton:pressed,QToolButton:checked{\
                /*border-left: 1px solid #34495e80; \
                border-top: 1px solid #34495e80;\
                border-right: none;\
                border-bottom:none;*/\
                background-color: rgba(246, 210, 133, 200);\
                color: rgba(236, 240, 241, 60);\
            }\
        ");
    }
}


void DialogLogin::on_lineEdit_Password_textEdited(const QString &arg1)
{
    if(arg1.isEmpty() || this->ui->lineEdit_Username->text().isEmpty()){
        this->ui->toolButton_Comfirm->setEnabled(false);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgba(246, 210, 133, 150);\
                border: 1px solid  rgba(246, 210, 133, 50);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }");
    }else{
        this->ui->toolButton_Comfirm->setEnabled(true);
        this->ui->toolButton_Comfirm->setStyleSheet("QToolButton{\
                background-color: rgb(246, 210, 133);\
                border: 1px solid  rgba(246, 210, 133, 50);\
                border-radius: 10px;\
                color: #ecf0f1;\
            }\
            \
            /* 鼠标悬停 */\
            QToolButton:hover{\
                background-color: rgba(246, 210, 133, 150);\
            }\
        \
            /* 点击和按下 */\
            QToolButton:pressed,QToolButton:checked{\
                /*border-left: 1px solid #34495e80; \
                border-top: 1px solid #34495e80;\
                border-right: none;\
                border-bottom:none;*/\
                background-color: rgba(246, 210, 133, 200);\
                color: rgba(236, 240, 241, 60);\
            }\
        ");
    }
}

bool DialogLogin::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->label_FindPwd)//指定某个QLabel
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

            if(mouseEvent->button() == Qt::LeftButton)
            {
                QMessageBox::information(NULL,QStringLiteral("抱歉"),QStringLiteral("暂不支持~"));
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
    else if(obj == ui->label_Register)
    {
        if (event->type() == QEvent::MouseButtonPress) //鼠标点击
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event); // 事件转换

            if(mouseEvent->button() == Qt::LeftButton)
            {
                DialogRegister Reg(this, userList);// = new DialogRegister(this);
                Reg.setWindowTitle("注册");
                if(Reg.exec() == QDialog::Accepted){
                    this->ui->lineEdit_Username->setText(Reg.GetRegisterUname());
                    this->ui->lineEdit_Password->setText(""); //清空密码输入
                }
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
        return QWidget::eventFilter(obj, event);
    }
}

void DialogLogin::on_checkBox_stateChanged(int arg1)
{
    AutoSave = arg1;
}









