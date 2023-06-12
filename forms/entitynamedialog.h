#ifndef ENTITYNAMEDIALOG_H
#define ENTITYNAMEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>

#include "figures/rectgraphicitem.h"
#include "datamodeler/model/model.hpp"

namespace Ui {
  class EntityNameDialog;
}

class EntityNameDialog : public QDialog
{
  Q_OBJECT

public:
  explicit EntityNameDialog(QWidget *parent, RectGraphicItem *item, Model *model);
  ~EntityNameDialog();

private slots:
  void on_buttonBox_rejected();

private:
  Ui::EntityNameDialog *ui;
  RectGraphicItem *entity;
  Model *model;

  void accept() override;
};

#endif // ENTITYNAMEDIALOG_H
