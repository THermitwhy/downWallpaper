#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QNetworkAccessManager *pManager=new QNetworkAccessManager(this);
    craw=new crawler(pManager);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::setImgType);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::getMess);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getMess(){
    craw->getReq();
}

void MainWindow::setImgType(){

    QString typeImg=ui->lineEdit1->text();
    if(!typeImg.isNull()){
        craw->imgType = typeImg;
    }
    else{
        craw->imgType = "fire";
    }

}
