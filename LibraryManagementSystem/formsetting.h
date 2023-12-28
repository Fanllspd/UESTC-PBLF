#ifndef FORMSETTING_H
#define FORMSETTING_H

#include "basicfunction.h"
#include "formhome.h"
#include <QWidget>
class SwitchButton;
class MainWindow;

namespace Ui {
class FormSetting;
}

class FormSetting : public QWidget
{
    Q_OBJECT

public:
    explicit FormSetting(QWidget *parent = nullptr, BooksList *booksList = nullptr, FormHome *formHome = nullptr, bool isAdmin = false);
    ~FormSetting();

signals:
    void sendCheck(bool checked, float opacity);

private slots:
    void onSwitchButtonOpacityClicked(bool checked);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_valueChanged(int value);

    void on_toolButton_Reload_clicked();

    void on_toolButton_Save_clicked();

    void on_toolButton_DeleteAll_clicked();

    int colorvalue(QTimer *timer);

    void animateButton(QToolButton *toolButton, QTimer *timer);

private:
    Ui::FormSetting *ui;
    BooksList *booksList;
    FormHome *formHome;
    bool IsAdmin;

    int x;
    bool ifChecked;
    float opacity;
    float opacityLength;

};

#endif // FORMSETTING_H
