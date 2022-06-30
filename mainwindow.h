#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "crawler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    crawler *craw;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void getMess();
    void setImgType();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
