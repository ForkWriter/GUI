#ifndef REDACTDIALOG_H
#define REDACTDIALOG_H

#include <QDialog>
//#include "parametersdialog.h"
//#include "entitynamedialog.h"
#include "datamodeler/model/attribute.hpp"
#include "datamodeler/model/entity.hpp"
#include "datamodeler/model/model.hpp"

namespace Ui {
  class RedactDialog;
}

class RedactDialog : public QDialog
{
  Q_OBJECT

public:
  explicit RedactDialog(QWidget *parent, Entity *new_ent,std::string ent_name);
  ~RedactDialog();

signals:
  void attrChange(Attribute *changed_attr, std::string name);

  void nameChange();

private slots:
  void on_attrList_doubleClicked(const QModelIndex &index);

  void on_nameList_doubleClicked(const QModelIndex &index);

private:
  Ui::RedactDialog *ui;
  Entity *ent;
  std::string ent_name;

  void setForm(std::string name);
};

#endif // REDACTDIALOG_H
