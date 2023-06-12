#include "config_types.h"
#include "ui_config_types.h"

Config_Types::Config_Types(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Config_Types)
{
  ui->setupUi(this);
  setWindowFlag(Qt::FramelessWindowHint);

  std::vector<std::string> listDBMS = Config::availableDBMS();
  for (auto iter = listDBMS.begin(); iter != listDBMS.end(); ++iter)
  {
    new QListWidgetItem(iter->c_str(), ui->DB_list);
  }
}

Config_Types::~Config_Types()
{
  delete ui;
}


void Config_Types::on_DB_list_clicked(const QModelIndex &index)
{
  ui->Available_list->clear();
  domain = index.data().toString().toStdString();
  std::vector<std::string> listDomains = Config::availableDomains(domain);
  for (auto iter = listDomains.begin(); iter != listDomains.end(); ++iter)
  {
    new QListWidgetItem(iter->c_str(), ui->Available_list);
  }
}


void Config_Types::on_Available_list_clicked(const QModelIndex &index)
{
  ui->Types_list->clear();
  type = index.data().toString().toStdString();
  std::vector<std::string> listTypes = Config::availableTypes(domain, type);
  for (auto iter = listTypes.begin(); iter != listTypes.end(); ++iter)
  {
    new QListWidgetItem(iter->c_str(), ui->Types_list);
  }
}


void Config_Types::on_buttonBox_accepted()
{
  if (!domain.empty())
    emit DBSelected(domain);
}

