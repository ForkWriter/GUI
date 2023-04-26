#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "figures/connectorgraphicitem.h"
#include "figures/rectgraphicitem.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  scene = new MainScene(this);
  scene->setSceneRect(0,0,850,1100);
  ui->graphicsView->setScene(scene);
  ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
  ui->graphicsView->setCursor(QCursor());
  ui->connectorSettings->setVisible(false);
  ui->rectSettings->setVisible(false);

  connect(ui->selectButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::DefaultType);});

  connect(ui->entityButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::RectangleType);});

  connect(ui->one_to_manyButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::LineType);});
  connect(ui->many_to_manyButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::LineType);});
  connect(ui->one_to_oneButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::LineType);});

  connect(scene, &MainScene::selectionChanged, this, &MainWindow::checkSelection);
  connect(scene, &MainScene::currentActionChanged, this, &MainWindow::checkActionStates);
  connect(scene, &MainScene::signalSelectItem, this, &MainWindow::selectItem);
  connect(scene, &MainScene::signalNewSelectItem, this, &MainWindow::selectNewItem);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

//void MainWindow::on_butSave_clicked()
//{
//    QString newPath = QFileDialog::getSaveFileName(this, trUtf8("Save SVG"),
//        path, tr("SVG files (*.svg)"));

//    if (newPath.isEmpty())
//        return;

//    path = newPath;

//    QSvgGenerator generator;
//    generator.setFileName(path);
//    generator.setSize(QSize(scene->width(), scene->height()));
//    generator.setViewBox(QRect(0, 0, scene->width(), scene->height()));
//    generator.setTitle(trUtf8("Vector Editor"));
//    generator.setDescription(trUtf8("File created by Vector Editor."));

//    QPainter painter;
//    painter.begin(&generator);
//    scene->render(&painter);
//    painter.end();
//}

//void MainWindow::on_butOpen_clicked()
//{
//    QString newPath = QFileDialog::getOpenFileName(this, trUtf8("Open SVG"),
//                                                   path, tr("SVG files (*.svg)"));
//    if (newPath.isEmpty())
//        return;

//    path = newPath;
//    scene->clear();

//    scene->setSceneRect(SvgReader::getSizes(path));

//    foreach (QGraphicsItem *item, SvgReader::getElements(path)) {
//        switch (item->type()) {
//        case QGraphicsPathItem::Type: {
//            ConnectorGraphicItem *polyline = qgraphicsitem_cast<ConnectorGraphicItem*>(item);
//            scene->addItem(polyline);
//            connect(polyline, &ConnectorGraphicItem::clicked, scene, &MainScene::signalSelectItem);
//            connect(polyline, &ConnectorGraphicItem::signalMove, scene, &MainScene::slotMove);
//            break;
//        }
//        case QGraphicsRectItem::Type: {
//            RectGraphicItem *rect = qgraphicsitem_cast<RectGraphicItem*>(item);
//            scene->addItem(rect);
//            connect(rect, &RectGraphicItem::clicked, scene, &MainScene::signalSelectItem);
//            connect(rect, &RectGraphicItem::signalMove, scene, &MainScene::slotMove);
//            break;
//        }
//        default:
//            break;
//        }
//    }
//}

void MainWindow::checkSelection()
{
    checkActionStates();
    switch (scene->selectedItems().length()) {
    case 0:
        ui->rectSettings->deselect();
        ui->connectorSettings->deselect();
        checkActionStates();
        //ui->toolsWidget->setEnabled(true);
        break;
    case 1:
        switch (scene->selectedItems().at(0)->type()) {
        case QGraphicsRectItem::Type:{
            ui->rectSettings->setVisible(true);
            ui->connectorSettings->deselect();
            ui->connectorSettings->setVisible(false);
            break;
        }
        default: {
            ui->rectSettings->deselect();
            ui->rectSettings->setVisible(false);
            ui->connectorSettings->setVisible(true);
            break;
        }
        }
        checkActionStates();
        //ui->toolsWidget->setEnabled(true);
        break;
    default:
        ui->rectSettings->deselect();
        ui->connectorSettings->deselect();
        //ui->toolsWidget->setEnabled(false);
        break;
    }
}

void MainWindow::checkActionStates()
{
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    ui->rectSettings->setVisible(false);
    ui->connectorSettings->setVisible(false);
    switch (scene->currentAction()){
    case MainScene::LineType:
        ui->connectorSettings->setVisible(true);
        break;
    case MainScene::RectangleType:
        ui->rectSettings->setVisible(true);
        break;
    case MainScene::SelectionType:
        break;
    default:
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
        break;
    }
}

void MainWindow::selectItem(QGraphicsItem *item)
{
    switch (item->type()) {
    case QGraphicsRectItem::Type: {
        RectGraphicItem *rect = qgraphicsitem_cast<RectGraphicItem *>(item);
        ui->rectSettings->loadRectangle(rect);
        break;
    }
    case QGraphicsPathItem::Type: {
        ConnectorGraphicItem *polyline = qgraphicsitem_cast<ConnectorGraphicItem *>(item);
        ui->connectorSettings->loadPolyline(polyline);
        break;
    }
    default:
        break;
    }
}

void MainWindow::selectNewItem(QGraphicsItem *item)
{
    switch (item->type()) {
    case QGraphicsRectItem::Type: {
        RectGraphicItem *rect = qgraphicsitem_cast<RectGraphicItem *>(item);
        ui->rectSettings->newRectangle(rect);
        break;
    }
    case QGraphicsPathItem::Type: {
        ConnectorGraphicItem *polyline = qgraphicsitem_cast<ConnectorGraphicItem *>(item);
        ui->connectorSettings->newPolyline(polyline);
        break;
    }
    default:
        break;
    }
}
