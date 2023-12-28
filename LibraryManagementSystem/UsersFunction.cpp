#include <stdio.h>
#include "basicfunction.h"
#include "qcryptographichash.h"
#include "qdebug.h"
#include <QString>
#include "cJSON.h"
#include "qlistwidget.h"
#include "qregularexpression.h"
#include <QMessageBox>
#include <QRandomGenerator>

// 只有尾插
bool InsertUser(UsersList *L, UserInfo info)
{
    // assert(pc);
    // if (0 == CheckCapacity(pc))
    // {
    // 	printf("图书馆已满，不能添加\n");
    // 	return;
    // }
    // qDebug() << "Insert" << L->counts;
    // UserInfo info;
    // info.UserId = L->counts + 1;
    // qstrncpy(info.Username, lineEdits.at(1)->text().toUtf8().constData(), MAX_TITLE_LEN);
    // qstrncpy(info.Password, lineEdits.at(2)->text().toUtf8().constData(), MAX_AUTHOR_NAME_LEN);
    // qstrncpy(info.Salt, lineEdits.at(3)->text().toUtf8().constData(), MAX_PUBLISHER_LEN);
    // qstrncpy(info.AvatarUrl, lineEdits.at(4)->text().toUtf8().constData(), MAX_PUBLICATIONDATE_LEN);
    // info.isAdmin = lineEdits.at(6)->text().toInt();
    // qstrncpy(info.Quote, lineEdits.at(7)->text().toUtf8().constData(), MAX_ISBN_LEN);
    // info.BorrowId = isAvailable;

    UsersList *Cur = L;

    UsersList *pL = (UsersList *)malloc(sizeof(UsersList));
    pL->data = info;

    while (Cur->next)
        Cur = Cur->next;

    pL->next = NULL;
    Cur->next = pL;


    L->counts++;
    // qDebug() << "添加成功！！";
    // printf("添加成功！！\n");
    // free(pL);
    return true;
}

bool ChangeUser(UsersList *L, UserInfo info)
{

    UsersList *found = NULL, *Cur = L->next;

    while (Cur != NULL)
    {
        if(info.UserId == Cur->data.UserId)
            found = Cur;
        // qDebug() << BookId;
        if(!strcmp(info.Username, Cur->data.Username)){
            QMessageBox::warning(NULL,QStringLiteral("错误"),QStringLiteral("用户名重复!"));
            return false;
        }
        Cur = Cur->next;
    }

    if (found == NULL)
    {
        // qDebug() << "未找到该用户";
        QMessageBox::warning(NULL,QStringLiteral("错误"),QStringLiteral("未找到该用户!"));
        return false;
    }
    else
    {
        qDebug() << found->data.Username ;
        found->data = info;
        return SaveUsersInfo(L);
    }
}

bool SaveUsersInfo(UsersList *L){

    if (!L)
        return false;

    cJSON* root = cJSON_CreateObject();  // 创建 JSON 根对象
    cJSON* UsersArray = cJSON_AddArrayToObject(root, "Users");  // 添加名为 "Books" 的数组

    UsersList *Cur = L->next;
    while (Cur)
    {


        //counts++;

        cJSON *newUser = cJSON_CreateObject();

        cJSON_AddNumberToObject(newUser, "UserId", Cur->data.UserId);
        cJSON_AddStringToObject(newUser, "Username", Cur->data.Username);
        cJSON_AddStringToObject(newUser, "Password", Cur->data.Password);
        cJSON_AddStringToObject(newUser, "Salt", Cur->data.Salt);
        cJSON_AddBoolToObject(newUser, "isAdmin", Cur->data.isAdmin);
        cJSON_AddStringToObject(newUser, "AvatarUrl", Cur->data.AvatarUrl);
        cJSON_AddStringToObject(newUser, "Quote", Cur->data.Quote);
        cJSON_AddNumberToObject(newUser, "BorrowId", Cur->data.BorrowId);

        cJSON_AddItemToArray(UsersArray, newUser);  // 将用户对象添加到数组中

        Cur = Cur->next;
    }

    return SaveJsonToFile(root, SAVE_DEFAULT_USERS_JSON);
}


cJSON* ReadUsers(cJSON **root)
{
    FILE *fp = fopen(SAVE_DEFAULT_USERS_JSON,"r");

    if (!fp)
    {
        QMessageBox::information(NULL, QStringLiteral("错误"),QStringLiteral("读取UsersInfo.json文件失败！"));
        return NULL;
    }

    *root = ReadJsonFile(fp);

    return cJSON_GetObjectItem(*root, "Users");
}

QString generateSalt(int length) {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    QString randomString;
    for(int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        randomString.append(possibleCharacters.at(index));
    }
    return randomString;
}

QString calculateSaltedMD5(const QString &data, const QString &salt) {
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData((data + salt).toUtf8());
    return hash.result().toHex();
}

bool verifyPassword(const QString &inputPassword, const QString &storedSalt, const QString &storedHashedPassword) {
    QString newHashedPassword = calculateSaltedMD5(inputPassword, storedSalt);
    qDebug() << newHashedPassword;
    return newHashedPassword == storedHashedPassword;
}


bool DeleteUser(UsersList *L, int UserId)
{
    UsersList *nownode = L, *Del = nownode->next;
    bool ifDelete = false;
    while (Del)//&& UserId != nownode->data.UserId)
    {
        if(UserId == Del->data.UserId){
            nownode->next = Del->next;
            free(Del);
            ifDelete = true;
            break;
        }
        nownode = nownode->next;
        Del = Del->next;
    }

    if (!ifDelete)
    {
        // qDebug() << "未找到该用户";
        QMessageBox::warning(NULL,QStringLiteral("错误"),QStringLiteral("未找到该用户!"));
        return false;
    }
    return SaveUsersInfo(L);

}
void ClearAutoSave()
{
    cJSON *root = NULL,
        *item = ReadAutoSave(&root);
    if (!item)
    {
        // printf("JSON文件有误！\n");
        QMessageBox::information(NULL,QStringLiteral("错误"),QStringLiteral("AutoSave.json文件有误！"));
        cJSON_Delete(root);
        return;
    }
    cJSON_SetBoolValue(cJSON_GetObjectItem(item, "Check"), false);
    cJSON_SetBoolValue(cJSON_GetObjectItem(item, "isAdmin"), false);
    cJSON_SetValuestring(cJSON_GetObjectItem(item, "Username"), "");
    cJSON_SetValuestring(cJSON_GetObjectItem(item, "Password"), "");
    cJSON_SetValuestring(cJSON_GetObjectItem(item, "Salt"), "");
    cJSON_SetValuestring(cJSON_GetObjectItem(item, "Deadline"), "");
    cJSON_SetValuestring(cJSON_GetObjectItem(item, "Quote"), "");
    cJSON_SetIntValue(cJSON_GetObjectItem(item, "BorrowId"), 0);
    cJSON_SetIntValue(cJSON_GetObjectItem(item, "UserId"), 0);
    SaveJsonToFile(root, QString(SAVE_DEFAULT_AUTOSAVE_JSON));
}

UsersList* SearchUser(UsersList *uL, QString uname, bool isStrict)
{
    if(uname.isEmpty())
        return uL;


    UsersList *nownode = uL->next;
    UsersList *Result = (UsersList *)malloc(sizeof(UsersList));
    // QString compare;
    Result->next = NULL;
    Result->counts = 0;
    UsersList *ResultCur;// = (BooksList *)malloc(sizeof(BooksList));
    ResultCur = Result;

    if(isStrict){
        while (nownode) {
            if(!strcmp(uname.toUtf8().constData(), nownode->data.Username)){
                UsersList *items = (UsersList *)malloc(sizeof(UsersList));

                items->data = nownode->data;
                items->next = NULL; //ResultCur->next;
                items->counts = 1;

                // qDebug() << items->data.Book_Title;
                Result->counts++;
                ResultCur->next = items;
                ResultCur = ResultCur->next;
            }
            nownode = nownode->next;
        }
        // return Result;
    }else{
        while(nownode){
            // Qt自带的正则识别，应该是C++实现的，这里选用吧
            QString pattern = uname;

            // 创建 QRegularExpression 对象
            QRegularExpression regex(pattern);

            // 进行匹配
            QRegularExpressionMatch match = regex.match(nownode->data.Username);

            // 判断是否匹配成功
            // qDebug() << match.hasMatch() <<
            if (match.hasMatch()) {
                UsersList *items = (UsersList *)malloc(sizeof(UsersList));

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
    }
    return Result;
}

void OutputAllUsers(UsersList *uL, QListWidget *listWidget, int nowUserId)
{
    listWidget->clear();
    UsersList *Cur = uL->next;
    while (Cur) {
        if(nowUserId == Cur->data.UserId)
            listWidget->addItem(QString(Cur->data.Username) + "[ME]");
        else
            listWidget->addItem(Cur->data.Username);


        Cur = Cur->next;
    }
}
