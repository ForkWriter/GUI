#include "parametersdialog.h"
#include "ui_parametersdialog.h"

ParametersDialog::ParametersDialog(QWidget *parent, Attribute *new_attr, std::string name) :
  QDialog(parent),
  ui(new Ui::ParametersDialog),
  attr(new_attr)
{
  ui->setupUi(this);
  setWindowFlag(Qt::FramelessWindowHint);

  ui->nameLine->setText(name.c_str());
  ui->valueLine->setText(new_attr->parameters().c_str());
  ui->templateLabel->setText(attr->parametersTemplate().c_str());
}

ParametersDialog::~ParametersDialog()
{
  delete ui;
}

void ParametersDialog::accept()
{
  std::string name = ui->nameLine->text().toStdString();
  std::string value = ui->valueLine->text().toStdString();
  try {
    attr->setPrimaryKey(ui->radioButton->isChecked());
    attr->setNullable(ui->nullButton->isChecked());
    attr->setParameters(value);
    emit AttrName(name, attr);
    QDialog::accept();
  }  catch (const std::invalid_argument &error_massage) {
    ui->errorLabel->setText(error_massage.what());
    return;
  }
}

void ParametersDialog::on_buttonBox_rejected()
{
  delete attr;
}

void ParametersDialog::on_radioButton_clicked(bool checked)
{
  ui->nullButton->setEnabled(!checked);
  if (checked) {
    ui->nullButton->setChecked(false);
  }
}

