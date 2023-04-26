#ifndef CONNECTORGRAPHICITEMSETTINGS_H
#define CONNECTORGRAPHICITEMSETTINGS_H

#include <QWidget>

class ConnectorGraphicItem;

namespace Ui {
class ConnectorItemSettings;
}

class ConnectorGraphicItemSettings : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor lineColor READ lineColor WRITE setLineColor NOTIFY lineColorChanged)
    Q_PROPERTY(int lineWidth READ lineWidth WRITE setLineWidth NOTIFY lineWidthChanged)

public:
    explicit ConnectorGraphicItemSettings(QWidget *parent = 0);
    ~ConnectorGraphicItemSettings();

    QColor lineColor() const;
    int lineWidth() const;

public slots:
    void setLineColor(const QColor &color);
    void setLineWidth(const int &width);

signals:
    void lineColorChanged(const QColor &color);
    void lineWidthChanged(const int &width);

private:
    Ui::ConnectorItemSettings *ui;
    ConnectorGraphicItem *currentPolyline;

public slots:
    void newPolyline(ConnectorGraphicItem *polyline);
    void loadPolyline(ConnectorGraphicItem *polyline);
    void deselect();
    void setVisible(bool visible);

private slots:
    //void on_lineWidth_textChanged(const QString &arg1);

private:
    QColor m_lineColor;
    int m_lineWidth;
};

#endif // CONNECTORGRAPHICITEMSETTINGS_H
