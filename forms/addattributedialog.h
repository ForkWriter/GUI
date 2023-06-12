#ifndef ADDATTRIBUTEDIALOG_H
#define ADDATTRIBUTEDIALOG_H

#include <QDialog>

#include "datamodeler/config.hpp"
#include "datamodeler/model/attribute.hpp"

namespace Ui {
  class AddAttributeDialog;
}

class AddAttributeDialog : public QDialog
{
  Q_OBJECT

public:
  explicit AddAttributeDialog(QWidget *parent, std::string cur_DB);
  ~AddAttributeDialog();

signals:
  void AttrAdd(Attribute* new_attr, std::string name);

private slots:
  void on_domainList_clicked(const QModelIndex &index);

  void on_typesList_clicked(const QModelIndex &index);

private:
  Ui::AddAttributeDialog *ui;
  std::string DB;
  std::string domain;
  std::string type;

  void accept() override;
};

#endif // ADDATTRIBUTEDIALOG_H
