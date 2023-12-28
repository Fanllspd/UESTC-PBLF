#ifndef LIBRARYMANAGEMENTSYSTEM_H
#define LIBRARYMANAGEMENTSYSTEM_H


// #include "qitemselectionmodel.h"
// #include "qlineedit.h"
// // #include <string.h>
// #include <stdbool.h>
// #include <QStandardItemModel>
// #include <sys/stat.h>

// #define SAVE_DEFAULT_NAME_JSON "LibrarySystemInfo.json" // （json好看一点

// // #define SAVE_DEFAULT_NAME_CSV "LibrarySystemInfo.csv" // 二选一吧（数据要求有点严苛，暂时弃用

// #define SAVE_DEFAULT_USERS_JSON "UsersInfo.json"

// #define SAVE_DEFAULT_AUTOSAVE_JSON "AutoSaveInfo.json"

// // #define MAX_ID_LEN 30

// #define MAX_TITLE_LEN 30

// #define MAX_AUTHOR_NAME_LEN 30

// #define MAX_PUBLISHER_LEN 30

// #define MAX_CATEGORY_LEN 30

// #define MAX_PUBLICATIONDATE_LEN 20

// #define MAX_LANGUAGE_LEN 6

// #define MAX_ISBN_LEN 14

// #define MAX_KEYWORDS_LEN 10

// struct cJSON;

// typedef struct
// {
//     int Book_Id;                            // 书本ID 理论唯一
//     char Book_Title[MAX_TITLE_LEN];         // 书名
//     char Book_Author[MAX_AUTHOR_NAME_LEN];  // 作者
//     char Book_Publisher[MAX_PUBLISHER_LEN]; // 出版社
//     char Book_Publication_Date[MAX_PUBLICATIONDATE_LEN];         // 首次出版日期
//     char Book_Language[MAX_LANGUAGE_LEN];                  // 语言
//     bool Availability_Status;               // 借阅状态
//     // char Borrowing_History[2][10];          // 借阅历史，包括借阅日期和归还日期
//     int Book_Copies_Amount;                 // 副本数量
//     char Book_ISBN[MAX_ISBN_LEN];                     // 全球唯一的标识书籍的编号
//     // char Book_Keywords[10][MAX_KEYWORDS_LEN];             // 关键词
//     char Book_Category[MAX_CATEGORY_LEN];   // 种类
//     bool Is_eBook;                          // 是否有电子书版本
// } BooksInfo;

// typedef struct
// {
//     char Username[20];
//     char Password[129];
//     char AvatarUrl[200];
//     char Salt[9];

// } UserInfo;

// typedef struct node
// {
//     BooksInfo data; // 数据域
//     struct node *next; // 指针域
//     int counts; // 含有的数据量
// } BooksList;

// // BooksInfo read_goods_info();
// void InitList(BooksList **pL);
// void InitBooksInfo(BooksInfo *info, cJSON *arr);
// void destroy_list(BooksList **pL);
// void destroy_list_and_file(BooksList **pL);
// // bool SaveToFile(BooksList *L, QString path);
// // void OutputOneBook(BooksList *L, QStandardItemModel *model,int row);
// // void OutputAllBooks(BooksList *L,QStandardItemModel *model);
// // bool InsertBook(BooksList *L, QList<QLineEdit *> lineEdits, int InsertPos, bool isAvailable, bool iseBook);
// // bool DeleteBooks(BooksList *L, QItemSelectionModel *selectionModel);
// // BooksList *SearchBook(BooksList *L, char *books_id, int type, bool isStrict);
// // bool ChangeBook(BooksList *L, QList<QLineEdit *> lineEdits, int BookId, bool isAvailable, bool iseBook);
// void bubble_sort(BooksList *L);
// int read_line(char str[], int n);
// // void OutputMenu();
// // void PrintMsg(char *msg);
// // cJSON* ReadJsonFile(FILE *fp, BooksList **pL);
// // void ReloadFile(BooksList **pL);

#endif // LIBRARYMANAGEMENTSYSTEM_H
