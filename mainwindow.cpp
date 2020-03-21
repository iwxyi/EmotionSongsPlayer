#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new NeteasePlayer(this);
    player->setDataDir("musics/");
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
    player->randomPlay(ui->lineEdit->text());
}

/**
 * 停止播放音乐
 */
void MainWindow::on_pushButton_2_clicked()
{
    player->stop();
}

/**
 * 下一首
 */
void MainWindow::on_pushButton_3_clicked()
{
    player->next();
}

/**
 * 拉黑当前歌曲
 */
void MainWindow::on_pushButton_5_clicked()
{
    player->block();
}

/**
 * 歌曲信息
 */
void MainWindow::on_pushButton_4_clicked()
{
    qDebug() << player->information();
}

void MainWindow::on_pushButton_6_clicked()
{
    player->play();
}

void MainWindow::on_pushButton_7_clicked()
{
    player->setType(ui->lineEdit->text());
}
