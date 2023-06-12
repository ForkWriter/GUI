#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include <QDialog>

#include "datamodeler/config.hpp"
#include "datamodeler/model/attribute.hpp"

namespace Ui {
  class ParametersDialog;
}

class ParametersDialog : public QDialog
{
  Q_OBJECT

public:
  explicit ParametersDialog(QWidget *parent, Attribute *new_attr, std::string name);
  ~ParametersDialog();

signals:
  void AttrName(std::string name, Attribute *new_attr);

signals:
  void params_changed(std::string name, std::string value);

private slots:
  void on_buttonBox_rejected();

  void on_radioButton_clicked(bool checked);

private:
  Ui::ParametersDialog *ui;
  Attribute *attr;

  void accept() override;
};

#endif // PARAMETERSDIALOG_H
