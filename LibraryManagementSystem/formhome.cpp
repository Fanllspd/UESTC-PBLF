#include "formhome.h"
#include "cJSON.h"
#include "qtimer.h"
#include "ui_formhome.h"
#include <QStandardItemModel>
#include "dialogadd.h"
#include "dialogchange.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QToolButton>
#include <QGraphicsRotation>
#include <QMessageBox>

FormHome::FormHome(QWidget *parent, BooksList *booksList, UsersList *uList, UserInfo uInfo) :
    QWidget(parent), booksList(booksList), usersList(uList), userInfo(uInfo)
    ,ui(new Ui::FormHome)
{
    ui->setupUi(this);

    // int cmd, id;
    x = 2; // 按钮点击变色

    this->ui->lineEdit_Search->setPlaceholderText("请输入相关信息~~");
    this->ui->comboBox->setItemData(3, QVariant(0), Qt::UserRole - 1);
    this->ui->comboBox->setItemData(4, QVariant(0), Qt::UserRole - 1);

    model = new QStandardItemModel(this);   //创建一个标准的条目模型
    this->ui->tableView->setModel(model);   //将tableview设置成model这个标准条目模型的模板, model设置的内容都将显示在tableview上
    this->ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 禁止直接编辑
    // this->ui->tableView->setSelectionMode(QAbstractItemView::MultiSelection);
    this->ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    this->ui->tableView->setSortingEnabled(true);
    this->ui->tableView->sortByColumn(0,Qt::AscendingOrder); // 排序
    // 获取选择模型
    //设置tableView水平滚动和自适应宽度
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(false);

    // 设置选择模式为 SingleSelection（单选）
    //selectionModel->setCurrentIndex(model->index(0, 0), QItemSelectionModel::Select);

    // model->setHorizontalHeaderItem(0, new QStandardItem("序号"));
    // this->ui->tableView->setColumnWidth(0, 50);
    // model->setHorizontalHeaderItem(1, new QStandardItem("书名"));
    // this->ui->tableView->setColumnWidth(1, 150);
    // model->setHorizontalHeaderItem(2, new QStandardItem("作者"));
    // this->ui->tableView->setColumnWidth(2, 75);
    // model->setHorizontalHeaderItem(3, new QStandardItem("出版社"));
    // this->ui->tableView->setColumnWidth(3, 150);
    // model->setHorizontalHeaderItem(4, new QStandardItem("出版日期"));
    // this->ui->tableView->setColumnWidth(4, 150);
    // model->setHorizontalHeaderItem(5, new QStandardItem("语言"));
    // this->ui->tableView->setColumnWidth(5, 75);
    // model->setHorizontalHeaderItem(6, new QStandardItem("种类"));
    // this->ui->tableView->setColumnWidth(6, 75);
    // model->setHorizontalHeaderItem(7, new QStandardItem("副本数量"));
    // this->ui->tableView->setColumnWidth(7, 75);
    // model->setHorizontalHeaderItem(8, new QStandardItem("借阅状态"));
    // this->ui->tableView->setColumnWidth(8, 75);
    // model->setHorizontalHeaderItem(9, new QStandardItem("ISBN"));
    // this->ui->tableView->setColumnWidth(9, 150);

    OutputAllBooks(booksList, model);
    // 书本ID
    // 书名
    // 作者
    // 出版社
    // 首次出版日期
    // 语言
    // 借阅状态
    // 借阅历史，包括借阅日期和归还日期
    // 副本数量
    // 全球唯一的标识书籍的编号
    // 关键词
    // 种类
    // 是否有电子书版本
    // 设置列的宽度

    if(!userInfo.isAdmin){
        this->ui->toolButton_add->setEnabled(false);
        this->ui->toolButton_change->setEnabled(false);
        this->ui->toolButton_remove->setEnabled(false);
    }

    BorrowId = userInfo.BorrowId;

    if(!BorrowId){
        this->ui->toolButton_BorrowOrReturn->setText("借书");
    }else {
        this->ui->toolButton_BorrowOrReturn->setText("还书");
    }
}

FormHome::~FormHome()
{
    delete ui;
}

// void FormHome::RefreshBooks(){
//     OutputAllBooks(booksList, model);
// }

int FormHome::colorvalue(QTimer *timer)
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

void FormHome::animateButton(QToolButton *toolButton, QTimer *timer)
{
    QString colorValueString = QString::number(this->colorvalue(timer));
    toolButton->setStyleSheet(QString("QToolButton{\
                background-color: #2c3e50;\
                border: 2px solid rgba(46, 204, 113, %1);\
                border-radius: 5px;\
                font-size: 18px;\
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
                font-size: 18px;\
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

void FormHome::on_toolButton_refresh_clicked()
{
    ////// 点击边框变色
    QTimer *RefreshTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(RefreshTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_refresh, RefreshTimer);});
    RefreshTimer->start(1);
    ////// END

    RefreshFile();
}


void FormHome::on_toolButton_add_clicked()
{
    ////// 点击边框变色
    QTimer *AddTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(AddTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_add, AddTimer);});
    AddTimer->start(1);
    ////// END

    DialogAdd *add = new DialogAdd(this, booksList, model);
    //add->setParent(this);
    //add->setWindowFlags(Qt::Window);
    add->exec();
    add->raise();
}


void FormHome::on_toolButton_change_clicked()
{
    ////// 点击边框变色
    QTimer *ChangeTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(ChangeTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_change, ChangeTimer);});
    ChangeTimer->start(1);
    ////// END

    // qDebug() << data;

    int rowToSelect = this->ui->tableView->selectionModel()->currentIndex().row(); // 获取鼠标焦点所在行
    QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    if(selectionModel->selectedIndexes().size() > model->columnCount()){
        // qDebug() << ;
        QModelIndex topLeft = model->index(rowToSelect, 0);
        QModelIndex bottomRight = model->index(rowToSelect, model->columnCount() - 1);
        QItemSelection selection(topLeft, bottomRight);
        selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
    }
    QModelIndex index = model->index(rowToSelect, 0);
    QVariant data = model->data(index);
    // qDebug() << rowToSelect;



    if(data.isValid()){
        // 打开修改界面
        qDebug() << data;
        DialogChange *change = new DialogChange(this, booksList, rowToSelect, model);
        //add->setParent(this);
        //add->setWindowFlags(Qt::Window);
        change->exec();
        change->raise();
    }else{
        // 如果没选中则跳弹窗
        QMessageBox::information(NULL,QStringLiteral("警告"),QStringLiteral("请选中一项！"));

    }
}


void FormHome::on_toolButton_remove_clicked()
{
    ////// 点击边框变色
    QTimer *RemoveTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(RemoveTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_remove, RemoveTimer);});
    RemoveTimer->start(1);
    ////// END


    //获取选择模型的指针
    QItemSelectionModel *selectionModel =  ui->tableView->selectionModel();
    //获取被选中的指针列表
    // QModelIndexList selected = selectionModel->selectedIndexes();
    if(selectionModel->selectedIndexes().size() >= model->columnCount()){
        //qDebug() << selectionModel->selectedRows()[1].data();

        if(DeleteBooks(booksList, selectionModel)){
            OutputAllBooks(booksList, model);
            // SaveBooksInfo(booksList,SAVE_DEFAULT_NAME_JSON);
        }else {
            QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("删除失败"));
        }
    }else {
        QMessageBox::information(NULL,QStringLiteral("警告"),QStringLiteral("请选中一项！"));
    }
}


void FormHome::on_toolButton_Search_clicked()
{
    ////// 点击边框变色
    QTimer *SearchTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(SearchTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_Search, SearchTimer);});
    SearchTimer->start(1);
    ////// END

    // qDebug() << ui->comboBox->currentIndex();
    BooksList *Result = SearchBook(booksList, ui->lineEdit_Search->text().toUtf8().data(), ui->comboBox->currentIndex(), ui->checkBox->isChecked());
    OutputAllBooks(Result, model);
}
//////// PUBLIC ////////
void FormHome::ReloadFile()
{
    //if(booksList)
    DeleteAllBooks(&booksList);
    // BooksList *booksList;
    InitBooksList(&booksList);
    OutputAllBooks(booksList, model);
    // return true;
}

void FormHome::RefreshFile()
{
    OutputAllBooks(booksList, model);
}

bool FormHome::SaveFile(QString path)
{
    return SaveBooksInfo(booksList, path);
    // return true;
}
////////////////////////

void FormHome::on_toolButton_BorrowOrReturn_clicked()
{

    ////// 点击边框变色
    QTimer *BorrowOrReturnTimer = new QTimer(this); // 点击边框变色按钮Timer
    connect(BorrowOrReturnTimer, &QTimer::timeout, this, [=](){animateButton(this->ui->toolButton_BorrowOrReturn, BorrowOrReturnTimer);});
    BorrowOrReturnTimer->start(1);
    ////// END

    if(!BorrowId){
        this->ui->toolButton_BorrowOrReturn->setText("借书");
    }else {
        this->ui->toolButton_BorrowOrReturn->setText("还书");
        this->ui->toolButton_BorrowOrReturn->setStyleSheet(this->ui->toolButton_BorrowOrReturn->styleSheet());
    }

    int rowToSelect = this->ui->tableView->selectionModel()->currentIndex().row(); // 获取鼠标焦点所在行
    QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
    if(selectionModel->selectedIndexes().size() > model->columnCount()){
        // qDebug() << ;
        QModelIndex topLeft = model->index(rowToSelect, 0);
        QModelIndex bottomRight = model->index(rowToSelect, model->columnCount() - 1);
        QItemSelection selection(topLeft, bottomRight);
        selectionModel->select(selection, QItemSelectionModel::ClearAndSelect);
    }
    QModelIndex index = model->index(rowToSelect, 0);
    QVariant data = model->data(index); // 得到id列的值
    if(data.isValid()){
        if(!BorrowId){
            qDebug() << data;
            if(BorrowBook(booksList, usersList, data.toInt(), userInfo.UserId)){
                QMessageBox::information(NULL,QStringLiteral("恭喜"),QStringLiteral("借阅成功！"));
                RefreshFile();
                BorrowId = data.toInt();
                this->ui->toolButton_BorrowOrReturn->setText("还书");
                emit setBorrowedBook(model->index(rowToSelect, 1).data().toString());
            } else{
                QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("借阅失败！"));
            }
        } else{
            if(ReturnBook(booksList, usersList, data.toInt(), userInfo.UserId)){
                QMessageBox::information(NULL,QStringLiteral("恭喜"),QStringLiteral("退还成功！"));
                RefreshFile();
                BorrowId = 0;
                this->ui->toolButton_BorrowOrReturn->setText("借书");
                emit setBorrowedBook("无");
            } else{
                QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("退还失败！"));
            }
        }
    } else{
        // 如果没选中则跳弹窗
        QMessageBox::information(NULL,QStringLiteral("警告"),QStringLiteral("请选中一项！"));
    }
}


void FormHome::on_lineEdit_Search_textChanged(const QString &arg1)
{
    // qDebug() << ui->comboBox->currentIndex();
    BooksList *Result = SearchBook(booksList, arg1.toUtf8().data(), ui->comboBox->currentIndex(), ui->checkBox->isChecked());
    // qDebug() << Result->data.Book_Title;
    OutputAllBooks(Result, model);
}

