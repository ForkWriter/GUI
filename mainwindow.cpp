#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "figures/connectorgraphicitem.h"
#include "figures/rectgraphicitem.h"
#include "datamodeler/scriptgenerator.hpp"
#include "datamodeler/model/relationship.hpp"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , current_model(nullptr)
{
  ui->setupUi(this);

  enable_tools(false);

  aDirList = new QFileInfoList();
  model = new QListViewExplorerModel();
  model->getFolderList(".",aDirList);
  ui->folderTree->setModel(model);

  count = 0;
  ui->modelTree->setColumnCount(1);
  QStringList headers;
  headers << "Model";
  ui->modelTree->setHeaderLabels(headers);
  currentItem = NULL;
  currentColumn = 0;

  ui->graphicsView->setAlignment(Qt::AlignTop|Qt::AlignLeft);
  scene = new MainScene(this);
  scene->setSceneRect(0,0,1100,1100);
  ui->graphicsView->setScene(scene);
  ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
  ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
  ui->graphicsView->setCursor(QCursor());
  ui->connectorSettings->setVisible(false);
  ui->rectSettings->setVisible(false);

  connect(this, &MainWindow::modelUpdate, this, &MainWindow::updateModel);

  connect(ui->selectButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::DefaultType);});

  connect(ui->entityButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::RectangleType);});

  connect(ui->redactButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::RedactType);});

  connect(ui->one_to_manyButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::One_to_Many);});
  connect(ui->many_to_manyButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::Many_to_Many);});
  connect(ui->one_to_oneButton, &QPushButton::clicked, [=](){scene->setCurrentAction(MainScene::One_to_One);});

  ui->graphicsView->setDragMode(QGraphicsView::NoDrag);

  Config::initTypes("./types.json");
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

void MainWindow::modelCreate(std::string DB)
{
  if(current_model) {
    delete current_model;
  }
  current_model = new Model(DB);
  scene->setCurrent_model(current_model);
  connect(this, &MainWindow::modelUpdate, scene, &MainScene::updateModel);
  connect(current_model, &Model::_changed, this, &MainWindow::updateModel);
  enable_tools(true);
  emit modelUpdate();
}

void MainWindow::updateModel()
{
  ui->modelTree->clear();
  QList<QTreeWidgetItem *> items;
  std::vector<std::string> entities =  current_model->entities();
  for (int i = 0; i < entities.size(); i++) {
    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->modelTree, ui->modelTree->currentItem());
    newItem->setText (currentColumn, "" + QString(entities[i].c_str()));
    newItem->setExpanded(true);

    std::vector<std::string> attributes = current_model->entity(entities[i])->attributes();
    for (int j = 0; j < attributes.size(); j++) {
      std::string attr_value = attributes[j] + "(" +
                                current_model->entity(entities[i])->attribute(attributes[j])->type() +
                                "): " + current_model->entity(entities[i])->attribute(attributes[j])->parameters();
      if (current_model->entity(entities[i])->attribute(attributes[j])->primaryKey()) {
        attr_value += " PK";
      }
      InsertItem(newItem, "" + QString(attr_value.c_str()));
    }
  }
  showAll();
}

void MainWindow::on_actionNew_model_triggered()
{
  Config_Types *conf_dialog = new Config_Types();
  connect(conf_dialog, &Config_Types::DBSelected, this, &MainWindow::modelCreate);
  conf_dialog->setWindowFlag(Qt::FramelessWindowHint);
  conf_dialog->show();
}

void MainWindow::enable_tools(bool state)
{
  ui->selectButton->setEnabled(state);
  ui->entityButton->setEnabled(state);
  ui->redactButton->setEnabled(state);
  ui->undoButton->setEnabled(state);
  ui->redoButton->setEnabled(state);
  ui->many_to_manyButton->setEnabled(state);
  ui->one_to_manyButton->setEnabled(state);
  ui->one_to_oneButton->setEnabled(state);
}

Model *MainWindow::getCurrent_model() const
{
  return current_model;
}


void MainWindow::on_undoButton_clicked()
{
  current_model->undo();
  emit modelUpdate();
}


void MainWindow::on_redoButton_clicked()
{
  current_model->redo();
  emit modelUpdate();
}


void MainWindow::on_actionOpen_file_triggered()
{
  std::string file_name = QFileDialog::getOpenFileName(0, "Открыть", "", "*.json").toStdString();
  if (file_name.find(".json") != std::string::npos) {
    if(current_model) {
      delete current_model;
    }
    current_model = Model::load(file_name);
    scene->setCurrent_model(current_model);
    connect(this, &MainWindow::modelUpdate, scene, &MainScene::updateModel);
    connect(current_model, &Model::_changed, this, &MainWindow::updateModel);
    enable_tools(true);
    QRegExp re(".+(?=[\\\\|\\/])");
    re.indexIn(file_name.c_str());
    model->getFolderList(re.cap(0), aDirList);
    emit modelUpdate();
  }
}


void MainWindow::on_actionSave_as_triggered()
{
  std::string str = QFileDialog::getSaveFileName(0, "Сохранить", "", "*.json").toStdString();
  current_model->save(str);
}


void MainWindow::on_generate_scriptButton_clicked()
{
  if(current_model) {
      //current_model->addRelationship(new Relationship(Relationship::RELATION_TYPE::ManyToMany, {"E_2", "E_3"}), "R2");
      try {
        ui->scriptEdit->setText(ScriptGenerator::generateScript(*current_model));
      } catch (std::invalid_argument error) {
        QString problems = error.what();
        problems += "\n";
        auto problems_list = ScriptGenerator::problemsReadyList(*current_model);
        for (const auto &i : problems_list)
          problems += i;
        ui->scriptEdit->setText(problems);
      }
  }
}

QListViewExplorerModel::QListViewExplorerModel(QObject *parent)
    :QAbstractListModel(parent)
{

}

void QListViewExplorerModel::getFolderList(QString folderPath, QFileInfoList *dirList)
{
    QDir dir = QDir(folderPath);

    *dirList = dir.entryInfoList(QDir::NoDot | QDir::Files | QDir::Dirs, QDir::DirsFirst);

    beginResetModel();
    aDirList = dirList;
    endResetModel();
}

int QListViewExplorerModel::rowCount(const QModelIndex &) const
{
    return aDirList->count();
}

QVariant QListViewExplorerModel::data( const QModelIndex &index, int role ) const
{
    QVariant value;

        switch ( role )
        {
            case Qt::DisplayRole: //string
            {
                value = aDirList->at(index.row()).fileName();
            }
            break;

            case Qt::DecorationRole: //icon
            {
                if (aDirList->at(index.row()).isDir()) {
                    QPixmap icon = QPixmap(":/icons/QListViewExplorer/folder.png");
                    QPixmap tmp = icon.scaled(30, 30, Qt::KeepAspectRatio);
                    value = tmp;
                    break;
                }

                if (aDirList->at(index.row()).isFile()) {
                    QPixmap icon = QPixmap(":/icons/QListViewExplorer/file.png");
                    QPixmap tmp = icon.scaled(30, 30, Qt::KeepAspectRatio);
                    value = tmp;
                    break;
                }

                value = aDirList->at(index.row()).fileName();
            }
            break;

            case Qt::UserRole: //data
            {
                value = aDirList->at(index.row()).fileName();
            }
            break;

            default:
                break;
        }

    return value;
}

void MainWindow::on_folderTree_doubleClicked(const QModelIndex &index)
{
  if (aDirList->at(index.row()).isDir())
  {
      QString tmp = aDirList->at(index.row()).absoluteFilePath();
      model->getFolderList(aDirList->at(index.row()).absoluteFilePath(), aDirList);
  } else {
    std::string file_name = aDirList->at(index.row()).absoluteFilePath().toStdString();
    if (file_name.find(".json") != std::string::npos) {
      if (current_model) {
        delete current_model;
      }
      current_model = Model::load(aDirList->at(index.row()).absoluteFilePath().toStdString());
      scene->setCurrent_model(current_model);
      connect(this, &MainWindow::modelUpdate, scene, &MainScene::updateModel);
      connect(current_model, &Model::_changed, this, &MainWindow::updateModel);
      enable_tools(true);
      emit modelUpdate();
    }
  }
}

int MainWindow::treeCount(QTreeWidget *tree, QTreeWidgetItem *parent = 0) {
 tree->expandAll(); //а это "костыль"
 int count = 0;
 if (parent == 0) {
  int topCount = tree->topLevelItemCount();
  for (int i = 0; i < topCount; i++) {
   QTreeWidgetItem *item = tree->topLevelItem(i);
   if (item->isExpanded()) {
    count += treeCount(tree, item);
   }
  }
  count += topCount;
 }
 else {
  int childCount = parent->childCount();
  for (int i = 0; i < childCount; i++) {
   QTreeWidgetItem *item = parent->child(i);
   if (item->isExpanded()) {
    count += treeCount(tree, item);
   }
  }
  count += childCount;
 }
 return count;
}

void MainWindow::InsertItem (QTreeWidgetItem *parent, QString text) {
 if (parent->isExpanded()==false) parent->setExpanded(true);
 QTreeWidgetItem *newItem = new QTreeWidgetItem(parent, ui->modelTree->currentItem());
 newItem->setText (currentColumn, text);
 newItem->setExpanded(true);
}

void MainWindow::showAll(void) {
 int cnt = treeCount(ui->modelTree);
 QString str(tr("Всего: ")+QString("%1").arg(cnt));
}
