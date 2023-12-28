#include <stdio.h>
#include "basicfunction.h"
#include "cJSON.h"
// #include "qdebug.h"
#include <string.h>
#include <QMessageBox>
#include "basicfunction.h"
/*
  1.显示所有书本的信息
  2.修改某个书本的信息
  3.插入某个书本的信息
  4.删除某个书本的信息
  5.查找某个书本的信息
  6.书本存盘并退出系统
  7.对书本某一属性（价格/出版时间/副本数量/语言/借阅状态）进行排序
  8.(慎用)删除所有内容
  其他.不存盘并退出系统
*/


void InitBooksList(BooksList **pL)
{

    *pL = (BooksList *)malloc(sizeof(BooksList));
    // if (*pL != NULL) {
    //     (*pL)->next = NULL;
    //     // 这里可以初始化头节点的其他成员，如果有的话
    //     (*pL)->counts = 0;
    // }
    (*pL)->next = NULL;
    (*pL)->counts = 0;
    BooksList *Cur = *pL; // 定义指向pL链表初始位置的指针，用来遍历数据

    FILE *fp = fopen(SAVE_DEFAULT_NAME_JSON, "r");
    if (!fp)
    {
        // perror("open file for reading");
        fclose(fp);
        return;
        //return false;
    }


    cJSON *root = NULL,
        *item = cJSON_GetObjectItem(ReadJsonFile(fp), "Books");
    if (!item)
    {
        // printf("JSON文件有误！\n");
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("JSON文件有误！"));
        free(*pL); // 释放链表头的内存
        fclose(fp);
        return;
        // return NULL;
    }

    for (int i = 0; i < cJSON_GetArraySize(item); i++)
    {
        cJSON *arr = cJSON_GetArrayItem(item, i); // 根据索引获得数组中的值

        if (arr != NULL)
        {
            BooksInfo info;
            InitBooksInfo(&info, arr);
            // 赋值结束
            // 将准备好的数据插入链表
            BooksList *newList = (BooksList *)malloc(sizeof(BooksList));
            newList->data = info;
            newList->next = Cur->next;
            Cur->next = newList;
            Cur = Cur->next;
            (*pL)->counts++;
            // free(newList);
            // qDebug() << Cur->data.Book_Id;
        }
    }
    //return true;
}

void InitBooksInfo(BooksInfo *info, cJSON *arr)
{
    // 缺少错误处理
    // 丑陋的给结构体赋值
    info->Book_Id = cJSON_GetObjectItem(arr, "Id")->valueint;
    strcpy_s(info->Book_Title, cJSON_GetObjectItem(arr, "Title")->valuestring);
    strcpy_s(info->Book_Author, cJSON_GetObjectItem(arr, "Author")->valuestring);
    strcpy_s(info->Book_Publisher, cJSON_GetObjectItem(arr, "Publisher")->valuestring);
    strcpy_s(info->Book_Publication_Date, cJSON_GetObjectItem(arr, "Publication_Date")->valuestring);
    strcpy_s(info->Book_Language, cJSON_GetObjectItem(arr, "Lang")->valuestring);
    // info->Availability_Status = cJSON_GetObjectItem(arr, "Availability_status")->valueint;
    //strcpy_s(info->Borrowing_History[0], cJSON_GetArrayItem(cJSON_GetObjectItem(arr, "Borrowing_history"), 0)->valuestring);
    //strcpy_s(info->Borrowing_History[1], cJSON_GetArrayItem(cJSON_GetObjectItem(arr, "Borrowing_history"), 1)->valuestring);
    info->Book_Copies_Amount = cJSON_GetObjectItem(arr, "Copies_amount")->valueint;
    strcpy_s(info->Book_ISBN, cJSON_GetObjectItem(arr, "ISBN")->valuestring);
    strcpy_s(info->Book_Category, cJSON_GetObjectItem(arr, "Category")->valuestring);
    info->Is_eBook = cJSON_GetObjectItem(arr, "Is_eBook")->valueint;

    // for (int j = 0; j < cJSON_GetArraySize(cJSON_GetObjectItem(arr, "Keywords")); j++)
    // {
    //     cJSON *item = cJSON_GetArrayItem(cJSON_GetObjectItem(arr, "Keywords"), j); // 根据索引获得数组中的值
    //     if (item != NULL)
    //     {
    //         strcpy_s(info->Book_Keywords[j], item->valuestring);
    //     }
    // }
}

void InitUsersList(UsersList **pL)
{
    *pL = (UsersList *)malloc(sizeof(UsersList));
    (*pL)->next = NULL;
    (*pL)->counts = 0;
    UsersList *Cur = *pL; // 定义指向pL链表初始位置的指针，用来遍历数据

    cJSON *root = NULL,
        *item = ReadUsers(&root);
    if (!item)
    {
        // printf("JSON文件有误！\n");
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("JSON文件有误！"));
        free(*pL); // 释放链表头的内存
        cJSON_Delete(root);
        return;
        // return NULL;
    }

    for (int i = 0; i < cJSON_GetArraySize(item); i++)
    {
        cJSON *arr = cJSON_GetArrayItem(item, i); // 根据索引获得数组中的值

        if (arr != NULL)
        {
            UserInfo info;
            InitUsersInfo(&info, arr);
            // 赋值结束
            // 将准备好的数据插入链表
            UsersList *newList = (UsersList *)malloc(sizeof(UsersList));
            newList->data = info;
            newList->next = Cur->next;
            Cur->next = newList;
            Cur = Cur->next;
            (*pL)->counts++;
            // free(newList);
            // qDebug() << Cur->data.Book_Id;
        }
    }
    //return true;
}



void InitUsersInfo(UserInfo *info, cJSON *arr)
{
    // 缺少错误处理
    // 丑陋的给结构体赋值
    info->UserId = cJSON_GetObjectItem(arr, "UserId")->valueint;
    strcpy_s(info->Username, cJSON_GetObjectItem(arr, "Username")->valuestring);
    strcpy_s(info->Password, cJSON_GetObjectItem(arr, "Password")->valuestring);
    strcpy_s(info->Salt, cJSON_GetObjectItem(arr, "Salt")->valuestring);
    info->isAdmin = cJSON_GetObjectItem(arr, "isAdmin")->valueint;
    strcpy_s(info->AvatarUrl, cJSON_GetObjectItem(arr, "AvatarUrl")->valuestring);
    strcpy_s(info->Quote, cJSON_GetObjectItem(arr, "Quote")->valuestring);
    info->BorrowId = cJSON_GetObjectItem(arr, "BorrowId")->valueint;
}

void OutputMenu()
{
    printf("\n===== 图书管理系统 =====\n");
    printf("1. 显示所有书本的信息\n");
    printf("2. 修改某个书本的信息\n");
    printf("3. 插入某个书本的信息\n");
    printf("4. 删除某个书本的信息\n");
    printf("5. 查找某个书本的信息\n");
    printf("6. 书本存盘并退出系统\n");
    printf("7. 对书本特定属性排序\n");
    printf("8. (慎用) 删除所有内容\n");
    printf("其他. 不存盘并退出系统\n");
    printf("========================\n");
}

void PrintMsg(char *msg)
{
    printf("[system]: %s", msg);
}

void OutputOneBook(BooksList *L,QStandardItemModel *model,int row)
{
    QStandardItem *item = new QStandardItem;
    item->setData(L->data.Book_Id, Qt::DisplayRole);
    model->setItem(row, 0, item);
    model->setItem(row, 1, new QStandardItem(L->data.Book_Title));
    model->setItem(row, 2, new QStandardItem(L->data.Book_Author));
    model->setItem(row, 3, new QStandardItem(L->data.Book_Publisher));
    model->setItem(row, 4, new QStandardItem(L->data.Book_Publication_Date));
    model->setItem(row, 5, new QStandardItem(L->data.Book_Language));
    // model->setItem(row, 6, new QStandardItem(L->data.Availability_Status==1? "true":"false"));

    QStandardItem *item2 = new QStandardItem;
    item2->setData(L->data.Book_Copies_Amount, Qt::DisplayRole);
    model->setItem(row, 6, item2);
    model->setItem(row, 7, new QStandardItem(L->data.Book_ISBN));
    model->setItem(row, 8, new QStandardItem(L->data.Book_Category));

    model->setItem(row, 9, new QStandardItem(L->data.Is_eBook==1? "true":"false"));
    // printf("Book Id: %d\t", L->data.Book_id);
    // printf("Book Title: %s\t", L->data.Book_title);
    // printf("Book Author: %s\t", L->data.Book_author);
    // printf("Book Publisher: %s\t", L->data.Book_Publisher);
    // printf("Book Publication Date: %s\t", L->data.Book_Publication_Date);
    // printf("Book Language: %s\t", L->data.Book_language);
    // printf("Book Availability Status: %s\t", L->data.Availability_status ? "true" : "false");
    // printf("Book Borrowing History: %s\t", L->data.Borrowing_history[0]);
    // printf("Book Copies Amount: %d\t", L->data.Book_copies_amount);
    // printf("Book ISBN: %s\t", L->data.Book_ISBN);
    // printf("Book Keywords: %s\t", L->data.Book_keywords[0]);
    // printf("Book Category: %s\t", L->data.Book_category);
    // printf("Is_eBook: %s\t\n", L->data.Is_eBook ? "true" : "false");
}
void OutputAllBooks(BooksList *L,QStandardItemModel *model)
{
    model->clear();
    model->setHorizontalHeaderItem(0, new QStandardItem("序号"));
    // this->ui->tableView->setColumnWidth(0, 50);
    model->setHorizontalHeaderItem(1, new QStandardItem("书名"));
    // this->ui->tableView->setColumnWidth(1, 150);
    model->setHorizontalHeaderItem(2, new QStandardItem("作者"));
    // this->ui->tableView->setColumnWidth(2, 75);
    model->setHorizontalHeaderItem(3, new QStandardItem("出版社"));

    model->setHorizontalHeaderItem(4, new QStandardItem("出版日期"));
    // this->ui->tableView->setColumnWidth(3, 150);
    model->setHorizontalHeaderItem(5, new QStandardItem("语言"));
    // this->ui->tableView->setColumnWidth(4, 75);
    // model->setHorizontalHeaderItem(6, new QStandardItem("借阅状态"));
    // this->ui->tableView->setColumnWidth(5, 75);
    model->setHorizontalHeaderItem(6, new QStandardItem("副本数量"));
    // this->ui->tableView->setColumnWidth(6, 75);
    // this->ui->tableView->setColumnWidth(7, 75);
    model->setHorizontalHeaderItem(7, new QStandardItem("ISBN"));
    // this->ui->tableView->setColumnWidth(5, 150);
    model->setHorizontalHeaderItem(8, new QStandardItem("种类"));
    model->setHorizontalHeaderItem(9, new QStandardItem("有电子书"));
    BooksList *Cur = L->next;
    int counts = 0;
    while (Cur)
    {
        OutputOneBook(Cur, model, counts);
        Cur = Cur->next;
        counts++;
    }
    //printf("Books Count: %d", counts);
}
