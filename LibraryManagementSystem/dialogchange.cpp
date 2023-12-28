#include "dialogchange.h"
#include "ui_dialogchange.h"
#include "qevent.h"
#include "qpropertyanimation.h"
#include "qgraphicseffect.h"
#include "qpainter.h"
#include <QtMath>
#include <QPushButton>
#include <QSequentialAnimationGroup>
#include <QDebug>
#include <QValidator>
#include "basicfunction.h"

DialogChange::DialogChange(QWidget *parent, BooksList *BL, int rowToSelect, QStandardItemModel *model)
    : QDialog(parent), booksList(BL), rowToSelect(rowToSelect), model(model)
    , ui(new Ui::DialogChange)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(windowFlags()  | Qt::FramelessWindowHint);

    // this->ui->lineEdit_InsertPos->setValidator(new QIntValidator());
    this->ui->SwitchButton_Available->setEnabled(false);
    this->ui->lineEdit_CopyAmount->setValidator(new QIntValidator());
    DisplayData();
}

DialogChange::~DialogChange()
{
    delete ui;
}

void DialogChange::DisplayData(){
    this->ui->lineEdit_Title->setText(model->data(model->index(rowToSelect, 1)).toString());
    this->ui->lineEdit_Author->setText(model->data(model->index(rowToSelect, 2)).toString());
    this->ui->lineEdit_Publisher->setText(model->data(model->index(rowToSelect, 3)).toString());
    this->ui->lineEdit_PublishDate->setText(model->data(model->index(rowToSelect, 4)).toString());
    this->ui->lineEdit_Language->setText(model->data(model->index(rowToSelect, 5)).toString());

    // this->ui->SwitchButton_Available->setChecked(model->data(model->index(rowToSelect, 6)).toBool());

    this->ui->lineEdit_CopyAmount->setText(model->data(model->index(rowToSelect, 6)).toString());
    this->ui->lineEdit_ISBN->setText(model->data(model->index(rowToSelect, 7)).toString());
    this->ui->lineEdit_Category->setText(model->data(model->index(rowToSelect, 8)).toString());

    this->ui->SwitchButton_eBook->setChecked(model->data(model->index(rowToSelect, 9)).toBool());

}

void DialogChange::paintEvent(QPaintEvent *event)
{
    //this->setContentsMargins(1,1,1,1);
    //  QPainterPath path;
    //  path.setFillRule(Qt::WindingFill);
    //  path.addRect(10, 10, this->width()-20, this->height()-20);

    //  QPainter painter(this);
    //  painter.setRenderHint(QPainter::Antialiasing, true);
    // // painter.fillPath(path, QBrush(Qt::white));

    //  QColor color(0, 0, 0, 100);
    //  for(int i=0; i<10; i++)
    //  {
    //      QPainterPath path;
    //      path.setFillRule(Qt::WindingFill);
    //      path.addRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
    //      color.setAlpha(150 - qSqrt(i)*50);
    //      painter.setPen(color);
    //      painter.drawPath(path);
    //  }

    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor color(0, 0, 0, 50);
    for(int i=0; i<10; i++)
    {
        color.setAlpha(150 - qSqrt(i)*50);
        painter.setPen(color);
        // drawRect 绘制矩形
        //        painter.drawRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2);
        // drawRoundedRect 绘制带原角
        painter.drawRoundedRect(10-i, 10-i, this->width()-(10-i)*2, this->height()-(10-i)*2, 15, 15);
    }
}


void DialogChange::on_toolButton_close_clicked()
{
    DialogChange::close();
}


void DialogChange::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {

        mousePressed = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
    }
}

void DialogChange::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton && mousePressed)
    {
        //当窗口最大化或最小化时也不进行触发

        if(DialogChange::isMaximized() || DialogChange::isMinimized())
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
                DialogChange::move(event->globalPos() - dragPosition);//移动
            }

        }
    }
    event->accept();
}

void DialogChange::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        mousePressed = false;
}

void DialogChange::startShakeAnimation(QLineEdit *lineEdit) {
    // 如果已经有动画在运行，不启动新动画
    if (lineEdit->property("isShaking").toBool()) {
        return;
    }

    QPropertyAnimation *animationX = new QPropertyAnimation(lineEdit, "pos");
    animationX->setEasingCurve(QEasingCurve::InOutQuad);
    animationX->setDuration(100);
    animationX->setKeyValueAt(0, lineEdit->pos());
    animationX->setKeyValueAt(0.25, lineEdit->pos() + QPoint(5, 0));
    animationX->setKeyValueAt(0.75, lineEdit->pos() - QPoint(5, 0));
    animationX->setEndValue(lineEdit->pos());

    QPropertyAnimation *animationY = new QPropertyAnimation(lineEdit, "pos");
    animationY->setEasingCurve(QEasingCurve::InOutQuad);
    animationY->setDuration(100);
    animationY->setKeyValueAt(0, lineEdit->pos());
    animationY->setKeyValueAt(0.25, lineEdit->pos() + QPoint(0, 5));
    animationY->setKeyValueAt(0.75, lineEdit->pos() - QPoint(0, 5));
    animationY->setEndValue(lineEdit->pos());

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(lineEdit);
    group->addAnimation(animationX);
    group->addAnimation(animationY);

    // 连接 finished 信号，以便在动画完成时重置状态
    QObject::connect(group, &QSequentialAnimationGroup::finished, [=]() {
        lineEdit->setProperty("isShaking", false);
    });

    // 启动动画前设置状态
    lineEdit->setProperty("isShaking", true);

    group->start(QAbstractAnimation::DeleteWhenStopped);
}


bool DialogChange::isInputNoSpace(const QString &input) {
    return !input.trimmed().isEmpty();
}

bool DialogChange::areAllLineEditsFilled(const QList<QLineEdit *> &lineEdits)
{
    for (QLineEdit *lineEdit : lineEdits) {
        if (lineEdit->isEnabled() && (lineEdit->text().isEmpty() || !isInputNoSpace(lineEdit->text()))) {
            lineEdit->setText("");
            lineEdit->setPlaceholderText("不能全为空哦~");
            lineEdit->setStyleSheet("border:1px solid #e74c3c;");
            startShakeAnimation(lineEdit);
            return false; // 如果任何一个为空，则返回 false
        }
    }
    return true; // 如果所有都不为空，则返回 true
}


void DialogChange::accept()
{
    QList<QLineEdit *> lineEdits = {
                                    this->ui->lineEdit_Title,
                                    this->ui->lineEdit_Author,
                                    this->ui->lineEdit_Publisher,
                                    this->ui->lineEdit_PublishDate,
                                    this->ui->lineEdit_Language,
                                    this->ui->lineEdit_CopyAmount,
                                    this->ui->lineEdit_ISBN,
                                    this->ui->lineEdit_Keywords,
                                    this->ui->lineEdit_Category
    };
    if(areAllLineEditsFilled(lineEdits)) {
        QModelIndex index = model->index(rowToSelect, 0);
        QVariant data = model->data(index);
        if(ChangeBook(booksList,lineEdits, data.toInt(), // 修改位置比较对象应为BookId
                       this->ui->SwitchButton_Available->getChecked(),
                       this->ui->SwitchButton_eBook->getChecked())){
            OutputAllBooks(booksList, model);
            SaveBooksInfo(booksList,SAVE_DEFAULT_NAME_JSON);
            QDialog::accept();
        }
    } else {
        qDebug() << "Insert Failed";
        return;

    }
}

void DialogChange::reject()
{
    QDialog::reject();
}


void DialogChange::isLineEditTextChanged(const QString &text, QLineEdit *lineEdit)
{
    if (text.isEmpty()) {
        lineEdit->setStyleSheet("border:1px solid #e74c3c;");
    } else {
        lineEdit->setStyleSheet("border:1px solid #2ecc71;");
    }
}

// void DialogChange::on_lineEdit_InsertPos_textChanged(const QString &arg1)
// {
//     isLineEditTextChanged(arg1, this->ui->lineEdit_InsertPos);
//     InsertPos = arg1.toInt();
// }


void DialogChange::on_lineEdit_Title_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_Title);
    Title = arg1;
}



void DialogChange::on_lineEdit_Author_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_Author);
    Author = arg1;
}


void DialogChange::on_lineEdit_Publisher_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_Publisher);
    Publisher = arg1;
}


void DialogChange::on_lineEdit_PublishDate_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_PublishDate);
    PublishDate = arg1;
}


void DialogChange::on_lineEdit_Language_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_Language);
    Language = arg1;
}


void DialogChange::on_lineEdit_CopyAmount_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_CopyAmount);
    CopyAmount  = arg1.toInt();
}


void DialogChange::on_lineEdit_ISBN_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_ISBN);
    ISBN = arg1;
}


void DialogChange::on_lineEdit_Keywords_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_Keywords);
}


void DialogChange::on_lineEdit_Category_textChanged(const QString &arg1)
{
    isLineEditTextChanged(arg1, this->ui->lineEdit_Category);
    Category = arg1;
}



void DialogChange::on_SwitchButton_Available_clicked(bool checked)
{
    Available = checked;
}

void DialogChange::on_SwitchButton_eBook_clicked(bool checked)
{
    eBook = checked;
}
