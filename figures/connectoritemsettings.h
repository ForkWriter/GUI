#ifndef CONNECTORITEMSETTINGS_H
#define CONNECTORITEMSETTINGS_H

#include <QWidget>

namespace Ui {
  class ConnectorItemSettings;
}

class ConnectorItemSettings : public QWidget
{
  Q_OBJECT

public:
  explicit ConnectorItemSettings(QWidget *parent = nullptr);
  ~ConnectorItemSettings();

private:
  Ui::ConnectorItemSettings *ui;
};

#endif // CONNECTORITEMSETTINGS_H
