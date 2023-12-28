#ifndef FORMSEARCH_H
#define FORMSEARCH_H

#include <QWidget>

namespace Ui {
class FormSearch;
}

class FormSearch : public QWidget
{
    Q_OBJECT

public:
    explicit FormSearch(QWidget *parent = nullptr);
    ~FormSearch();

private:
    Ui::FormSearch *ui;
};

#endif // FORMSEARCH_H
