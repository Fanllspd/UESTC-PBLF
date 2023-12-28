#ifndef BASICFUNCTION_H
#define BASICFUNCTION_H


#include "qlistwidget.h"
#include <QItemSelectionModel>
#include <QLineEdit>
// #include <string.h>
#include <stdbool.h>
#include <QStandardItemModel>
#include <sys/stat.h>

#define SAVE_DEFAULT_NAME_JSON "json/LibrarySystemInfo.json" // （json好看一点
// #define SAVE_DEFAULT_NAME_CSV "LibrarySystemInfo.csv" // 二选一吧（数据要求有点严苛，暂时弃用
#define SAVE_DEFAULT_USERS_JSON "json/UsersInfo.json"
#define SAVE_DEFAULT_AUTOSAVE_JSON "json/AutoSaveInfo.json"
#define MAX_TITLE_LEN 30
#define MAX_AUTHOR_NAME_LEN 30
#define MAX_PUBLISHER_LEN 30
#define MAX_CATEGORY_LEN 30
#define MAX_PUBLICATIONDATE_LEN 20
#define MAX_LANGUAGE_LEN 6
#define MAX_ISBN_LEN 14
#define MAX_KEYWORDS_LEN 10

struct cJSON;

typedef struct
{
    int Book_Id;                            // 书本ID 理论唯一
    char Book_Title[MAX_TITLE_LEN];         // 书名
    char Book_Author[MAX_AUTHOR_NAME_LEN];  // 作者
    char Book_Publisher[MAX_PUBLISHER_LEN]; // 出版社
    char Book_Publication_Date[MAX_PUBLICATIONDATE_LEN];         // 首次出版日期
    char Book_Language[MAX_LANGUAGE_LEN];                  // 语言
    // bool Availability_Status;               // 借阅状态
    // char Borrowing_History[2][10];          // 借阅历史，包括借阅日期和归还日期
    int Book_Copies_Amount;                 // 副本数量
    char Book_ISBN[MAX_ISBN_LEN];                     // 全球唯一的标识书籍的编号
    // char Book_Keywords[10][MAX_KEYWORDS_LEN];             // 关键词
    char Book_Category[MAX_CATEGORY_LEN];   // 种类
    bool Is_eBook;                          // 是否有电子书版本
} BooksInfo;

typedef struct
{
    int UserId;
    char Username[100];
    char Password[33];
    char Salt[9];
    char AvatarUrl[200];
    bool isAdmin;
    char Quote[100];
    int BorrowId;

} UserInfo;

typedef struct Node
{
    UserInfo data; // 数据域
    struct Node *next; // 指针域
    int counts; // 含有的数据量
} UsersList;

typedef struct node
{
    BooksInfo data; // 数据域
    struct node *next; // 指针域
    int counts; // 含有的数据量
} BooksList;

/////////////////////
///     Books     ///
/////////////////////
void InitBooksList(BooksList **pL); // 初始化书本链表
void InitBooksInfo(BooksInfo *info, cJSON *arr);
void OutputOneBook(BooksList *L, QStandardItemModel *model,int row);
void OutputAllBooks(BooksList *L,QStandardItemModel *model); // 展示所有数据
bool InsertBook(BooksList *L, QList<QLineEdit *> lineEdits, int InsertPos, bool isAvailable, bool iseBook);
// 往链表插入指定数据
bool DeleteBooks(BooksList *L, QItemSelectionModel *selectionModel); // 删除链表某一数据
void DeleteAllBooks(BooksList **L); // 删除链表所有数据
BooksList *SearchBook(BooksList *L, char *books_id, int type, bool isStrict); //查找书籍信息，模糊/精准
bool ChangeBook(BooksList *L, QList<QLineEdit *> lineEdits, int BookId, bool isAvailable, bool iseBook);
// 往链表插入指定数据
bool BorrowBook(BooksList *L, UsersList *uL, int bookId, int UserId);
// 借阅书籍
bool ReturnBook(BooksList *L, UsersList *uL, int bookId, int UserId);
// 归还书籍
bool SaveBooksInfo(BooksList *L, QString path);
// void bubble_sort(BooksList *L);
// int read_line(char str[], int n);
/////////////////////



/////////////////////
///     Users     ///
/////////////////////
void InitUsersList(UsersList **pL); // 初始化用户链表
void InitUsersInfo(UserInfo *info, cJSON *arr);
bool InsertUser(UsersList *L, UserInfo info); // 往链表插入指定数据
bool ChangeUser(UsersList *L, UserInfo info); // 往链表插入指定数据
bool DeleteUser(UsersList *L, int UserId); // 删除链表某一数据
bool SaveUsersInfo(UsersList *L); //储存用户信息
UsersList* SearchUser(UsersList *uL, QString uname, bool isStrict); //查找用户信息，模糊/精准
void OutputAllUsers(UsersList *uList, QListWidget *listWidget, int nowUserId);  // 展示所有数据
/////////////////////



/////////////////////
///     Utils     ///
/////////////////////
QString generateSalt(int length); //生成用来md5加密的盐值
QString calculateSaltedMD5(const QString &data, const QString &salt); //计算md5返回hex
bool verifyPassword(const QString &inputPassword, const QString &storedSalt, const QString &storedHashedPassword);
bool ChangeUserinfo(UserInfo info, QString newUname, QString newQuote);
void ClearAutoSave(); // 清除储存自动保存信息的配置文件
// void OutputMenu(); // 弃用
// void PrintMsg(char *msg); // 弃用
cJSON* ReadJsonFile(FILE *fp); // 封装了读取json操作的函数,返回root
cJSON* ReadAutoSave(cJSON **root); // 调用ReadJsonFile，便于读取AutoSaveInfo.json，返回AutoSave对象
cJSON* ReadUsers(cJSON **root); // 调用ReadJsonFile，便于读取UsersInfo.json，返回Users对象
bool SaveJsonToFile(cJSON *root, QString filename); // 主要是储存书本信息，可以自定义路径储存
void ReloadFile(BooksList **pL); // 重新初始化链表，并重新OutputAllBooks
/////////////////////



#endif // BASICFUNCTION_H
