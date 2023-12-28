#include <stdio.h>
#include "basicfunction.h"
// #include "qcryptographichash.h"
#include "qdebug.h"
#include "qregularexpression.h"
#include <QString>
#include "cJSON.h"
#include <QMessageBox>
#include <QRandomGenerator>

// 实现书本插入链表（头，中，尾）
bool InsertBook(BooksList *L, QList<QLineEdit *> lineEdits, int InsertPos, bool isAvailable, bool iseBook)
{
    // assert(pc);
    // if (0 == CheckCapacity(pc))
    // {
    // 	printf("图书馆已满，不能添加\n");
    // 	return;
    // }
    // qDebug() << "Insert" << L->counts;
    BooksInfo info;
    info.Book_Id = L->counts + 1;
    qstrncpy(info.Book_Title, lineEdits.at(1)->text().toUtf8().constData(), MAX_TITLE_LEN);
    qstrncpy(info.Book_Author, lineEdits.at(2)->text().toUtf8().constData(), MAX_AUTHOR_NAME_LEN);
    qstrncpy(info.Book_Publisher, lineEdits.at(3)->text().toUtf8().constData(), MAX_PUBLISHER_LEN);
    qstrncpy(info.Book_Publication_Date, lineEdits.at(4)->text().toUtf8().constData(), MAX_PUBLICATIONDATE_LEN);
    qstrncpy(info.Book_Language, lineEdits.at(5)->text().toUtf8().constData(), MAX_LANGUAGE_LEN);
    info.Book_Copies_Amount = lineEdits.at(6)->text().toInt();
    qstrncpy(info.Book_ISBN, lineEdits.at(7)->text().toUtf8().constData(), MAX_ISBN_LEN);
    //qstrncpy(info.Book_Keywords, lineEdits.at(8)->text().toUtf8().constData(), MAX_Keywords_LEN);
    qstrncpy(info.Book_Category, lineEdits.at(8)->text().toUtf8().constData(), MAX_CATEGORY_LEN);
    // info.Availability_Status = isAvailable;
    info.Is_eBook = iseBook;

    BooksList *Cur = L;
    InsertPos = lineEdits.at(0)->text().toInt();
    // int counts = 0;

    // printf("请输入插入位置（0尾插，1头插，中插[输入想要的位置]）：>");
    // scanf("%d", &InsertPos);
    if (InsertPos < 0) // TODO:这里应该直接比较是否插入过大，需要传入参数
    {
         // qDebug() << "InsertPos < 0";

        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("插入位置应为正数！"));
        // printf("请输入正确位置！");
        return false;
    }

    //////////////////////////////////////////////////////

    // printf("请输入书本ID：>");
    // scanf("%d", &info.Book_id);
    // printf("请输入书名：>");
    // scanf("%s", info.Book_title);
    // printf("请输入作者：>");
    // scanf("%s", info.Book_author);
    // printf("请输入出版社：>");
    // scanf("%s", info.Book_Publisher);
    // printf("请输入首次出版日期：>");
    // scanf("%s", info.Book_Publication_Date);
    // printf("请输入语言：>");
    // scanf("%s", info.Book_language);
    // printf("请输入借阅状态：>");
    // scanf("%d", &info.Availability_status);
    // printf("请输入副本数量：>");
    // scanf("%d", &info.Book_copies_amount);
    // printf("请输入ISBN号：>");
    // scanf("%s", info.Book_ISBN);
    // // printf("请输入关键词：>");
    // // scanf("%d", info.Book_keywords); // 后期考虑增加
    // printf("请输入种类：>");
    // scanf("%s", info.Book_category);
    // printf("请输入是否电子书：>");
    // scanf("%d", &info.Is_eBook);

    BooksList *pL = (BooksList *)malloc(sizeof(BooksList));
    pL->data = info;

    //////////////////////////////////////////////////////

    switch (InsertPos)
    {
    case 0:
    {
        while (Cur->next)
            Cur = Cur->next;

        pL->next = NULL; // 很重要，不然输出无法即时停止循环，导致报错
        Cur->next = pL;
        break;
    }
    case 1:
        pL->next = L->next;
        L->next = pL;
        break;
    default:
    {
        int k = 1;
        while (k++ - InsertPos)
        {
            if (!Cur->next)
            {
                // printf("插入位置过大！");
                QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("插入位置过大！"));

                free(pL);
                return false;
            }

            Cur = Cur->next;
        }

        pL->next = Cur->next;
        Cur->next = pL;
        break;
    }
    }

    // Cur->data = info;
    Cur->next = pL;
    L->counts++;
    // qDebug() << "添加成功！！";
    QMessageBox::information(NULL,QStringLiteral("恭喜"),QStringLiteral("添加成功！！"));
    // printf("添加成功！！\n");
    // free(pL);
    return true;
}

// 查询节点
BooksList* SearchBook(BooksList *L, char *books_id, int type, bool isStrict)
{
    if(!strlen(books_id))
        return L;
    // char books_id[20];
    BooksList *nownode = L->next;
    // printf("请输入要查询的书名：\n");
    // scanf("%s", books_id);
    char *compare;

    BooksList *Result = (BooksList *)malloc(sizeof(BooksList));
    Result->next = NULL;
    Result->counts = 0;
    BooksList *ResultCur;// = (BooksList *)malloc(sizeof(BooksList));
    ResultCur = Result;
    if(isStrict){ // 严格模式
        while (nownode) //  && strcmp(books_id, compare) != 0)
        {
            switch (type) {
            case 0:
                compare = nownode->data.Book_Title;
                break;
            case 1:
                compare = nownode->data.Book_Author;
                break;
            case 2:
                compare = nownode->data.Book_Category;
                break;
                // case 3:
                //     compare = nownode->data.Book_Title;
                //     break;
            // case 4:
            //     itoa(nownode->data.Book_Id, compare, 10);
            //     qDebug() << compare;
                // compare = (char*) nownode->data.Book_Id;
                break;
            default:
                compare = nownode->data.Book_Title;
                break;
            }
            // qDebug() << books_id << compare;
            if(!strcmp(books_id, compare)){
                BooksList *items = (BooksList *)malloc(sizeof(BooksList));

                items->data = nownode->data;
                items->next = ResultCur->next;
                items->counts = 1;

                // qDebug() << items->data.Book_Title;
                Result->counts++;
                ResultCur->next = items;
                ResultCur = ResultCur->next;
            }
            nownode = nownode->next;
        }

        // if (nownode == NULL)
        // {
        //     printf("未找到该书籍\n");
        //     return NULL;
        // }
        // else
        // {
            // printf("书名：%s\n", nownode->data.Book_Title);
            // printf("作者：%s\n", nownode->data.Book_Author);
            // printf("出版社：%s\n", nownode->data.Book_Publisher);
            // // printf("出版日期：%d\n", nownode->data.Book_Publication_Date);
            // printf("语言：%s\n", nownode->data.Book_Language);
            // printf("可用状态：%d\n", nownode->data.Availability_Status);
            // printf("库存数量：%d\n", nownode->data.Book_Copies_Amount);
            // printf("ISBN：%s\n", nownode->data.Book_ISBN);
            // printf("价格：%lf\n", nownode->data.);
            // Result->data = nownode->data;
            // Result->next = NULL;
            // Result->counts = 1;
            // qDebug() << Result->next->data.Book_Title;
        //qDebug() << Result->next->next->data.Book_Title;
            return Result;
        //}
    }else{ // 模糊搜索
        while (nownode) //  && strcmp(books_id, compare) != 0)
        {
            switch (type) {
            case 0:
                compare = nownode->data.Book_Title;
                break;
            case 1:
                compare = nownode->data.Book_Author;
                break;
            case 2:
                compare = nownode->data.Book_Category;
                break;
                // case 3:
                //     compare = nownode->data.Book_Title;
                //     break;
            // case 4:
            //     itoa(nownode->data.Book_Id, compare, 10);

            default:
                compare = nownode->data.Book_Title;
                break;
            }


            // Qt自带的正则识别，应该是C++实现的，这里选用吧
            QString pattern = books_id;

            // 创建 QRegularExpression 对象
            QRegularExpression regex(pattern);

            // 进行匹配
            QRegularExpressionMatch match = regex.match(compare);

            // 判断是否匹配成功
            if (match.hasMatch()) {
                BooksList *items = (BooksList *)malloc(sizeof(BooksList));

                items->data = nownode->data;
                items->next = ResultCur->next;
                items->counts = 1;

                // qDebug() << items->data.Book_Title;
                Result->counts++;
                ResultCur->next = items;
                ResultCur = ResultCur->next;
            }

            nownode = nownode->next;




            //qDebug() << books_id << compare;
            // int len = strlen(books_id);
            // int counts = strlen(compare) - len + 1;
            // for (int i = 0; i < counts; ++i) {
            //     // 使用 strncpy 复制子字符串
            //     char subStr[len + 1];  // +1 用于存储 null 终止符
            //     strncpy(subStr, compare + i, len);
            //     subStr[len] = '\0';
            //     qDebug() << books_id << compare << strncmp(books_id, compare, len);
            //     if(!strncmp(books_id, compare, len)){
            //         BooksList *items = (BooksList *)malloc(sizeof(BooksList));

            //         items->data = nownode->data;
            //         items->next = ResultCur->next;
            //         items->counts = 1;

            //         // qDebug() << items->data.Book_Title;
            //         Result->counts++;
            //         ResultCur->next = items;
            //         ResultCur = ResultCur->next;
            //         continue;
            //     }
            // }
            // if(!strcmp(books_id, compare)){
            //     BooksList *items = (BooksList *)malloc(sizeof(BooksList));

            //     items->data = nownode->data;
            //     items->next = ResultCur->next;
            //     items->counts = 1;

            //     // qDebug() << items->data.Book_Title;
            //     Result->counts++;
            //     ResultCur->next = items;
            //     ResultCur = ResultCur->next;
            // }
            // nownode = nownode->next;
        }

        return Result;

        //return NULL;
    }

    // return NULL;
}


bool ChangeBook(BooksList *L, QList<QLineEdit *> lineEdits, int BookId, bool isAvailable, bool iseBook)
{
    // char chan[20];
    BooksList *nownode = L->next;
    // printf("请输入要修改的书名：");
    // scanf("%s", chan);

    while (nownode != NULL && BookId != nownode->data.Book_Id)
    {
        nownode = nownode->next;
        qDebug() << BookId;
    }

    if (nownode == NULL)
    {
        // printf("未找到该书籍");
        qDebug() << "未找到该书籍";
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("未找到该书籍!"));

        return false;
    }
    else
    {

        qstrncpy(nownode->data.Book_Title, lineEdits.at(0)->text().toUtf8().constData(), MAX_TITLE_LEN);
        qstrncpy(nownode->data.Book_Author, lineEdits.at(1)->text().toUtf8().constData(), MAX_AUTHOR_NAME_LEN);
        qstrncpy(nownode->data.Book_Publisher, lineEdits.at(2)->text().toUtf8().constData(), MAX_PUBLISHER_LEN);
        qstrncpy(nownode->data.Book_Publication_Date, lineEdits.at(3)->text().toUtf8().constData(), MAX_PUBLICATIONDATE_LEN);
        qstrncpy(nownode->data.Book_Language, lineEdits.at(4)->text().toUtf8().constData(), MAX_LANGUAGE_LEN);
        nownode->data.Book_Copies_Amount = lineEdits.at(5)->text().toInt();
        qstrncpy(nownode->data.Book_ISBN, lineEdits.at(6)->text().toUtf8().constData(), MAX_ISBN_LEN);
        //qstrncpy(nownode->data.Book_Keywords, lineEdits.at(8)->text().toUtf8().constData(), MAX_Keywords_LEN);
        qstrncpy(nownode->data.Book_Category, lineEdits.at(8)->text().toUtf8().constData(), MAX_CATEGORY_LEN);
        // nownode->data.Availability_Status = isAvailable;
        nownode->data.Is_eBook = iseBook;
        return true;
        // int c;
        // printf("_________________________________________________________________\n");
        // printf("\t1.作者\n");
        // printf("\t2.出版社\n");
        // printf("\t3.出版日期\n");
        // printf("\t4.语言\n");
        // printf("\t5.可用状态\n");
        // printf("\t6.库存数量\n");
        // printf("\t7.ISBN\n");
        // printf("\t8.价格\n");
        // printf("_________________________________________________________________\n");
        // printf("请选择要修改的信息：");
        // scanf("%d", &c);

        // switch (c)
        // {
        // case 1:
        //     printf("请输入新的作者：\n");
        //     scanf("%s", nownode->data.Book_Author);
        //     printf("修改成功\n");
        //     break;
        // case 2:
        //     printf("请输入新的出版社：\n");
        //     scanf("%s", nownode->data.Book_Publisher);
        //     printf("修改成功\n");
        //     break;
        // // case 3:
        // //     printf("请输入新的出版日期：\n");
        // //     scanf("%d", &nownode->data.Book_Publication_Date);
        // //     printf("修改成功\n");
        // //     break;
        // case 4:
        //     printf("请输入新的语言：\n");
        //     scanf("%s", nownode->data.Book_Language);
        //     printf("修改成功\n");
        //     break;
        // case 5:
        //     printf("请输入新的可用状态：\n");
        //     scanf("%d", nownode->data.Availability_Status);
        //     printf("修改成功\n");
        //     break;
        // case 6:
        //     printf("请输入新的库存数量：\n");
        //     scanf("%d", &nownode->data.Book_Copies_Amount);
        //     printf("修改成功\n");
        //     break;
        // case 7:
        //     printf("请输入新的ISBN：\n");
        //     scanf("%d", &nownode->data.Book_ISBN);
        //     printf("修改成功\n");
        //     break;
        // // case 8:
        // //     printf("请输入新的价格：\n");
        // //     scanf("%lf", &nownode->Price);
        // //     printf("修改成功\n");
        // //     break;
        // default:
        //     printf("Error!请选择1~8的数字\n");
        // }
    }
}



// bool ChangeUserinfo(UserInfo info, QString newUname, QString newQuote)
// {
//     cJSON *root = NULL,
//         *item = ReadUsers(&root);

//     if (!item)
//     {
//         // printf("JSON文件有误！\n");
//         QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("UsersInfo.json文件有误！"));
//         cJSON_Delete(root);
//         return false;
//     }

//     for (int i = 0; i < cJSON_GetArraySize(item); i++) {
//         cJSON *arr = cJSON_GetArrayItem(item, i);
//         if(arr != NULL){
//             if(!strcmp(info.Username, cJSON_GetObjectItem(arr,"Username")->valuestring)){
//                 cJSON_SetValuestring(cJSON_GetObjectItem(arr, "Username"), newUname.toUtf8().constData());
//                 cJSON_SetValuestring(cJSON_GetObjectItem(arr, "Quote"), newQuote.toUtf8().constData());


//                 return SaveJsonToFile(root, SAVE_DEFAULT_USERS_JSON);
//             }
//         }
//     }

//     return false;
// }

bool DeleteBooks(BooksList *L, QItemSelectionModel *selectionModel){
    // char del[20];
    // scanf("%s", del);
    // int i = 0;
    int counts = 0;
    qDebug() << selectionModel->selectedRows().count();
    for (int i = 0; i < selectionModel->selectedRows().count(); i++) {
        BooksList *deleteleftnode = L;
        BooksList *deletenode = L->next;
        while (deletenode != NULL)// && strcmp(del, deletenode->data.Book_Title) != 0)
        {
            if(selectionModel->selectedRows().at(i).data().toInt() == deletenode->data.Book_Id){
                deleteleftnode->next = deletenode->next;
                free(deletenode);
                counts++;
                // L->counts--;
                break;
            }

            deleteleftnode = deletenode;
            deletenode = deleteleftnode->next;
        }
    }

    // BooksList *deleteleftnode = L;
    // BooksList *deletenode = L->next;
    // while (deletenode != NULL)// && strcmp(del, deletenode->data.Book_Title) != 0)
    //     {
    //     for (int i = selectionModel->selectedRows().count() - 1; i >= 0; --i) {
    //         if(selectionModel->selectedRows().at(i).data().toInt() == deletenode->data.Book_Id){
    //             deleteleftnode->next = deletenode->next;
    //             free(deletenode);
    //             // selectionModel->selectedRows().removeAt(i);
    //             break;
    //         }

    //         deleteleftnode = deletenode;
    //         deletenode = deleteleftnode->next;
    //     }
    // }

    // 判断是否还有数据没删除
    // qDebug() << selectionModel->selectedRows().count();
    // qDebug() << selectionModel->selectedRows().isEmpty();
    if(counts == selectionModel->selectedRows().count()){
        return true;
    }
    return false;
    // if (deletenode == NULL)
    // {
    //     qDebug() << "未找到该书籍";
    //     // printf("未找到该书籍\n");
    //     return false;
    // }
    // else
    // {
    //     deleteleftnode->next = deletenode->next;
    //     free(deletenode);
    //     deletenode = NULL;
    // }

    // qDebug() << "删除成功\n";
    // printf("删除成功\n");

}

void DeleteAllBooks(BooksList **pL)
{
    if (!pL || !*pL) {
        // 检查空指针
        qDebug() << 1;
        return;
    }

    BooksList *Cur = (*pL), *Des;
    while (Cur->next)
    {
        Des = Cur->next;
        Cur->next = Des->next;
        // qDebug() << Cur->next->data.Book_Title;
        free(Des);
    }

    //free(*pL);
    (*pL) = NULL;
}

cJSON* ReadJsonFile(FILE *fp){

    // 移动到文件末尾
    fseek(fp, 0, SEEK_END);
    // 获取文件大小
    long fileSize = ftell(fp);
    // 重置到文件开始
    fseek(fp, 0, SEEK_SET);
    // 根据文件大小分配内存
    char *jsonStr = (char *)malloc(fileSize + 1);
    if (jsonStr == NULL) {
        // 错误处理：内存分配失败
        QMessageBox::warning(NULL,QStringLiteral("错误"),QStringLiteral("读取JSON内存分配失败！"));
    }

    int size = fread(jsonStr, sizeof(char), fileSize, fp);
    if (size == 0)
    {
        // printf("读取文件失败！\n");
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("读取文件失败！"));
        fclose(fp);
        free(jsonStr);
        return NULL;
    }
    fclose(fp);

    // 把读取到的jsonStr数据转换为cJson结构
    cJSON *item = NULL;
    cJSON *root = cJSON_Parse(jsonStr);
    if (!root)
    {
        // printf("Error before: [%s]\n", cJSON_GetErrorPtr());

        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("JSON文件有误！"));
        free(jsonStr);
        return NULL;
    }
    free(jsonStr);


    return root;
}

bool SaveBooksInfo(BooksList *L, QString path){

    if (!L)
        return false;

    cJSON* root = cJSON_CreateObject();  // 创建 JSON 根对象
    cJSON* booksArray = cJSON_AddArrayToObject(root, "Books");  // 添加名为 "Books" 的数组

    BooksList *Cur = L->next;
    while (Cur)
    {


        cJSON* bookObject = cJSON_CreateObject();  // 为每本书创建一个对象

        cJSON_AddNumberToObject(bookObject, "Id", Cur->data.Book_Id);
        cJSON_AddStringToObject(bookObject, "Title", Cur->data.Book_Title);
        cJSON_AddStringToObject(bookObject, "Author", Cur->data.Book_Author);
        cJSON_AddStringToObject(bookObject, "Publisher", Cur->data.Book_Publisher);
        cJSON_AddStringToObject(bookObject, "Publication_Date", Cur->data.Book_Publication_Date);
        cJSON_AddStringToObject(bookObject, "Lang", Cur->data.Book_Language);
        // cJSON_AddBoolToObject(bookObject, "Availability_status", Cur->data.Availability_Status);

        cJSON_AddNumberToObject(bookObject, "Copies_amount", Cur->data.Book_Copies_Amount);
        cJSON_AddStringToObject(bookObject, "ISBN", Cur->data.Book_ISBN);
        cJSON_AddStringToObject(bookObject, "Category", Cur->data.Book_Category);
        cJSON_AddBoolToObject(bookObject, "Is_eBook", Cur->data.Is_eBook);


        cJSON_AddItemToArray(booksArray, bookObject);  // 将书本对象添加到数组中

        Cur = Cur->next;
    }



    return SaveJsonToFile(root,path);
}






cJSON* ReadAutoSave(cJSON **root)
{
    FILE *fp = fopen(SAVE_DEFAULT_AUTOSAVE_JSON,"r");

    if (!fp)
    {
        QMessageBox::information(NULL, QStringLiteral("错误"),QStringLiteral("读取AutoSaveInfo.json文件失败！"));
        return NULL;
    }

    *root = ReadJsonFile(fp);

    return cJSON_GetObjectItem(*root, "AutoSave");;
}

bool SaveJsonToFile(cJSON *root, QString filename)
{
    char* jsonString = cJSON_Print(root);  // 将 JSON 根对象转换为字符串
    // qDebug() << jsonString;

    FILE* file = fopen(filename.toUtf8(), "w");
    if (file != NULL) {
        fprintf(file, "%s", jsonString);  // 将字符串写入文件
        fclose(file);
    }else {
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("文件写入失败！"));
        cJSON_Delete(root);
        fclose(file);
        free(jsonString);
        return false;
    }

    cJSON_Delete(root);  // 释放 cJSON 根对象
    free(jsonString);  // 释放字符串
    return true;
}



bool BorrowBook(BooksList *L, UsersList *uL, int bookId, int UserId)
{
    BooksList *Cur = L->next;
    UsersList *uCur = uL->next;

    while(Cur){
        if(Cur->data.Book_Id == bookId){
            if(Cur->data.Book_Copies_Amount < 1){
                QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("书没啦！"));
                return false;
            }
            Cur->data.Book_Copies_Amount--;
            while (uCur) {
                if(uCur->data.UserId == UserId){
                    uCur->data.BorrowId = bookId;

                    return SaveBooksInfo(L, SAVE_DEFAULT_NAME_JSON) && SaveUsersInfo(uL);
                }
                uCur = uCur->next;
            }
        }
        Cur = Cur->next;
    }


    return false;
}

bool ReturnBook(BooksList *L, UsersList *uL, int bookId, int UserId)
{
    BooksList *Cur = L->next;
    UsersList *uCur = uL->next;
    while (uCur) {
        if(uCur->data.UserId == UserId){
            if(uCur->data.BorrowId == bookId){
                while(Cur){
                    if(Cur->data.Book_Id == bookId){
                        Cur->data.Book_Copies_Amount++;

                        uCur->data.BorrowId = 0;
                        return SaveBooksInfo(L, SAVE_DEFAULT_NAME_JSON) && SaveUsersInfo(uL);
                    }
                    Cur = Cur->next;
                }
            }else{
                QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("你没有借这本书！"));
            }
        }
        uCur = uCur->next;
    }

    return false;
}
