#include "redactdialog.h"
#include "ui_redactdialog.h"

RedactDialog::RedactDialog(QWidget *parent, Entity *new_ent, std::string ent_name) :
  QDialog(parent),
  ui(new Ui::RedactDialog),
  ent(new_ent),
  ent_name(ent_name)
{
  ui->setupUi(this);
  setWindowFlag(Qt::FramelessWindowHint);
  setForm(ent_name);
}

RedactDialog::~RedactDialog()
{
  delete ui;
}

void RedactDialog::on_attrList_doubleClicked(const QModelIndex &index)
{
  std::string name = index.data().toString().toStdString();
  QRegExp re(".+(?=\\(.+\\):)");
  re.indexIn(name.c_str());
  name = re.cap(0).toStdString();
  emit attrChange(ent->attribute(name), name);
}

void RedactDialog::on_nameList_doubleClicked(const QModelIndex &index)
{
  emit nameChange();
}

void RedactDialog::setForm(std::string name)
{
  ui->nameList->clear();
  ui->attrList->clear();

  new QListWidgetItem(name.c_str(), ui->nameList);

  std::vector<std::string> attrs_name = ent->attributes();
  for (int i = 0; i < attrs_name.size(); i++) {
    std::string attr_value = attrs_name[i] + "(" +
                              ent->attribute(attrs_name[i])->type() +
                              "): " + ent->attribute(attrs_name[i])->parameters();
    if (ent->attribute(attrs_name[i])->primaryKey()) {
      attr_value += " PK";
    }
    new QListWidgetItem(attr_value.c_str(), ui->attrList);
  }
}

