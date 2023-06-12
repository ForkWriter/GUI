#include "addattributedialog.h"
#include "ui_addattributedialog.h"

AddAttributeDialog::AddAttributeDialog(QWidget *parent, std::string cur_DB) :
  QDialog(parent),
  ui(new Ui::AddAttributeDialog)
{
  ui->setupUi(this);
  setWindowFlag(Qt::FramelessWindowHint);

  DB = cur_DB;
  std::vector<std::string> listDomains = Config::availableDomains(DB);
  for (auto iter = listDomains.begin(); iter != listDomains.end(); ++iter)
  {
    new QListWidgetItem(iter->c_str(), ui->domainList);
  }
}

AddAttributeDialog::~AddAttributeDialog()
{
  delete ui;
}

void AddAttributeDialog::on_domainList_clicked(const QModelIndex &index)
{
  ui->typesList->clear();
  domain = index.data().toString().toStdString();
  std::vector<std::string> listTypes = Config::availableTypes(DB, domain);
  for (auto iter = listTypes.begin(); iter != listTypes.end(); ++iter)
  {
    new QListWidgetItem(iter->c_str(), ui->typesList);
  }
}

void AddAttributeDialog::on_typesList_clicked(const QModelIndex &index)
{
  type = index.data().toString().toStdString();
}

void AddAttributeDialog::accept()
{
  if (!type.empty()) {
    std::pair<std::string, std::string> param(type, Config::typeParmetersTemplate(DB, domain, type));
    Attribute* new_attr = new Attribute(domain, param);
    emit AttrAdd(new_attr, "");
    QDialog::accept();
  } else {
    return;
  }
}

