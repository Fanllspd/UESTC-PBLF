#include "formsetting.h"
#include "qtimer.h"
#include "ui_formsetting.h"
#include <QMessageBox>
#include <QFileDialog>
#include "basicfunction.h"
#include <QInputDialog>

FormSetting::FormSetting(QWidget *parent, BooksList *booksList, FormHome *formHome, bool isAdmin)
    : QWidget(parent), booksList(booksList), formHome(formHome), IsAdmin(isAdmin)
    , ui(new Ui::FormSetting)
{
    ifChecked = false;
    ui->setupUi(this);
    opacity = 1.0;
    x = 2;
    opacityLength = 100.0;
    ui->horizontalSlider->setMaximum(opacityLength);
    connect(this->ui->switchButton_opacity, &SwitchButton::checkedChanged, this, &FormSetting::onSwitchButtonOpacityClicked);
    if(!IsAdmin){
        this->ui->toolButton_DeleteAll->setEnabled(false);
        this->ui->toolButton_Save->setEnabled(false);
    }

    this->ui->widget_CMD->setEnabled(false);

}

FormSetting::~FormSetting()
{
    delete ui;
}

void FormSetting::onSwitchButtonOpacityClicked(bool checked)
{
    ifChecked = checked;
    emit sendCheck(checked, opacity);
}

void FormSetting::on_horizontalSlider_sliderMoved(int position)
{
    opacity = position / opacityLength;
    if(ifChecked){
        emit sendCheck(true, opacity);
    }
}


void FormSetting::on_horizontalSlider_valueChanged(int value)
{
    opacity = value / opacityLength;
    if(ifChecked){
        emit sendCheck(true, opacity);
    }
}

int FormSetting::colorvalue(QTimer *timer)
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

void FormSetting::animateButton(QToolButton *toolButton, QTimer *timer)
{
    QString colorValueString = QString::number(this->colorvalue(timer));
    toolButton->setStyleSheet(QString("QToolButton{\
                background-color: #2c3e50;\
                border: 2px solid rgba(46, 204, 113, %1);\
                border-radius: 5px;\
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

void FormSetting::on_toolButton_Reload_clicked()
{
    ////// 点击边框变色
    QTimer *ReloadTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(ReloadTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Reload, ReloadTimer);});
    ReloadTimer->start(1);
    ////// END

    formHome->ReloadFile();

}


void FormSetting::on_toolButton_Save_clicked()
{
    ////// 点击边框变色
    QTimer *SaveTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(SaveTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Save, SaveTimer);});
    SaveTimer->start(1);
    ////// END

    QString dirpath = QFileDialog::getSaveFileName(this, "选择目录", SAVE_DEFAULT_NAME_JSON, tr("*.json"));
    if(formHome->SaveFile(dirpath))
        QMessageBox::information(NULL,QStringLiteral("恭喜"),(QString("文件成功保存在" + dirpath)), QMessageBox::Ok);
    else
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("文件保存失败！"), QMessageBox::Ok);
}




void FormSetting::on_toolButton_DeleteAll_clicked()
{
    ////// 点击边框变色
    QTimer *DeleteAllTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(DeleteAllTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_DeleteAll, DeleteAllTimer);});
    DeleteAllTimer->start(1);
    ////// END

    int check1 = QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("确定要全部删除吗！"), QMessageBox::Ok | QMessageBox::Cancel);
    if(check1 == QMessageBox::Ok){
        int check2 = QMessageBox::warning(NULL,QStringLiteral("警告"),QStringLiteral("再次确定要全部删除吗！"), QMessageBox::Ok| QMessageBox::Cancel);
        if(check2 == QMessageBox::Ok){
            DeleteAllBooks(&booksList);
            formHome->RefreshFile();
            QMessageBox::information(NULL,QStringLiteral("恭喜"),QStringLiteral("已经删除完毕，请手动保存！"), QMessageBox::Ok);
        }
    }
}

