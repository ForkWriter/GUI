#include "entitynamedialog.h"
#include "ui_entitynamedialog.h"

EntityNameDialog::EntityNameDialog(QWidget *parent, RectGraphicItem *item, Model *model) :
  QDialog(parent),
  ui(new Ui::EntityNameDialog),
  entity(item),
  model(model)
{
  ui->setupUi(this);
  setWindowFlag(Qt::FramelessWindowHint);
}

EntityNameDialog::~EntityNameDialog()
{
  delete ui;
}

void EntityNameDialog::on_buttonBox_rejected()
{
  delete entity;
}

void EntityNameDialog::accept()
{
  try {
    std::string ent_name = ui->name_line->text().toStdString();
    Entity *ent = new Entity();
    entity->setEntity(std::make_pair(ent_name, ent));
    model->addEntity(ent, ent_name);
    entity->setName(model->entityName(ent));
    QDialog::accept();
  }  catch (const std::invalid_argument &error_massage) {
    ui->error_label->setText(error_massage.what());
    return;
  }
}

