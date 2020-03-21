#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * 开始播放这一类型的音乐
 */
void MainWindow::on_pushButton_clicked()
{

}

/**
 * 停止播放音乐
 */
void MainWindow::on_pushButton_2_clicked()
{

}

/**
 * 下一首
 */
void MainWindow::on_pushButton_3_clicked()
{

}

/**
 * 拉黑当前歌曲
 */
void MainWindow::on_pushButton_5_clicked()
{

}

/**
 * 歌曲信息
 */
void MainWindow::on_pushButton_4_clicked()
{

}
