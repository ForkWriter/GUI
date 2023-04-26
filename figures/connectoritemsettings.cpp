#include "connectoritemsettings.h"
#include "ui_connectoritemsettings.h"

ConnectorItemSettings::ConnectorItemSettings(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ConnectorItemSettings)
{
  ui->setupUi(this);
}

ConnectorItemSettings::~ConnectorItemSettings()
{
  delete ui;
}
