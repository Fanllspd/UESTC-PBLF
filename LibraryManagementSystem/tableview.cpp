#include "tableview.h"

TableView::TableView(QWidget *parent):
    QTableView(parent)
{

}

TableView::~TableView()
{

}

void TableView::mousePressEvent(QMouseEvent *event)
{
    setCurrentIndex(QModelIndex()); //否则点击空白处，currentIndex仍为上一次点击的记录值
    QTableView::mousePressEvent(event);

}
