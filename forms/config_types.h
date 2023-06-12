#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <QDialog>

#include "datamodeler/config.hpp"

namespace Ui {
  class Config_Types;
}

class Config_Types : public QDialog
{
  Q_OBJECT

public:
  explicit Config_Types(QWidget *parent = nullptr);
  ~Config_Types();

signals:
  void DBSelected(std::string DB);

private slots:
  void on_DB_list_clicked(const QModelIndex &index);

  void on_Available_list_clicked(const QModelIndex &index);

  void on_buttonBox_accepted();

private:
  Ui::Config_Types *ui;
  std::string domain;
  std::string type;
};

#endif // CONFIG_TYPES_H
