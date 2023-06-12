#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QDir>
#include <QModelIndex>
#include <QtWidgets>

#include "mainscene.h"
#include "connectorgraphicitemsettings.h"
#include "rectgraphicitemsettings.h"
#include "config_types.h"

#include "datamodeler/config.hpp"
#include "datamodeler/model/model.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QListViewExplorerModel: public QAbstractListModel
{
public:
    QListViewExplorerModel(QObject *parent=nullptr);
    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;

    void getFolderList(QString folderPath, QFileInfoList *dirList);

private:
    QFileInfoList *aDirList;

};

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  Model *getCurrent_model() const;

signals:
  void modelUpdate();

private slots:
    void modelCreate(std::string DB);
    void updateModel();

    void on_actionNew_model_triggered();

    void on_undoButton_clicked();

    void on_redoButton_clicked();

    void on_actionOpen_file_triggered();

    void on_actionSave_as_triggered();

    void on_generate_scriptButton_clicked();

    void on_folderTree_doubleClicked(const QModelIndex &index);

private:
  Ui::MainWindow *ui;
  MainScene *scene;
  QString path;
  Model *current_model;
  QFileInfoList *aDirList;
  QListViewExplorerModel *model;

  void enable_tools(bool state);

  int treeCount(QTreeWidget *, QTreeWidgetItem *); //подсчёт количества элементов в QTreeWidget
  void DeleteItem (QTreeWidgetItem *currentItem); //удаление элемента из QTreeWidget
  void InsertItem (QTreeWidgetItem *, QString); //добавление элемента в QTreeWidget
  void showAll(void); //вывод информации о QTreeWidget
  int count; //переменная для хранения номера очередного узла
  QTreeWidgetItem *currentItem; //текущий элемент, запоминается при клике в QTreeWidget
  int currentColumn; //номер столбца, на самом деле будет = 0, т.к. у нас 1 столбец
};

#endif // MAINWINDOW_H
