#include "formabout.h"
#include "ui_formabout.h"

FormAbout::FormAbout(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormAbout)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

FormAbout::~FormAbout()
{
    delete ui;
}
