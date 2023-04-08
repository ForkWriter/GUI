#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  scene = new QGraphicsScene(this);
  scene->setItemIndexMethod(QGraphicsScene::NoIndex);
  scene->setSceneRect(0,0,850,1100);
  ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
  delete ui;
}
