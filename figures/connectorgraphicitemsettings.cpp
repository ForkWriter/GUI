#include "connectorgraphicitemsettings.h"
#include "ui_connectoritemsettings.h"
#include <QColorDialog>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "connectorgraphicitem.h"
#include "colorlabel.h"

ConnectorGraphicItemSettings::ConnectorGraphicItemSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectorItemSettings),
    currentPolyline(nullptr)
{
    ui->setupUi(this);
    setLineColor(QColor(Qt::black));
    setLineWidth(1);

    connect(ui->lineColor, &ColorLabel::clicked,
            [=](){
        QColorDialog dialog;
        connect(&dialog, &QColorDialog::colorSelected, this, &ConnectorGraphicItemSettings::setLineColor);
        dialog.exec();
    });

    connect(ui->lineWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &ConnectorGraphicItemSettings::setLineWidth);
}

ConnectorGraphicItemSettings::~ConnectorGraphicItemSettings()
{
    delete ui;
    delete currentPolyline;
}

QColor ConnectorGraphicItemSettings::lineColor() const
{
    return m_lineColor;
}

int ConnectorGraphicItemSettings::lineWidth() const
{
    return m_lineWidth;
}

void ConnectorGraphicItemSettings::setLineColor(const QColor &color)
{
    m_lineColor = color;
    ui->lineColor->setColor(m_lineColor);
    if(currentPolyline != nullptr){
        QPen pen(color,currentPolyline->pen().width());
        currentPolyline->setPen(pen);
    }
    emit lineColorChanged(m_lineColor);
}

void ConnectorGraphicItemSettings::setLineWidth(const int &width)
{
    m_lineWidth = width;
    ui->lineWidth->setValue(m_lineWidth);
    if(currentPolyline != nullptr){
        QPen pen(currentPolyline->pen().color(),width);
        currentPolyline->setPen(pen);
    }
    emit lineWidthChanged(m_lineWidth);
}

void ConnectorGraphicItemSettings::newPolyline(ConnectorGraphicItem *polyline)
{
    polyline->setBrush(QBrush(Qt::transparent));
    polyline->setPen(QPen(m_lineColor, m_lineWidth));
}

void ConnectorGraphicItemSettings::loadPolyline(ConnectorGraphicItem *polyline)
{
    currentPolyline = polyline;
    m_lineColor = currentPolyline->pen().color();
    m_lineWidth = currentPolyline->pen().width();
    ui->lineColor->setColor(m_lineColor);
    ui->lineWidth->setValue(m_lineWidth);
}

void ConnectorGraphicItemSettings::deselect()
{
    currentPolyline = nullptr;
}

void ConnectorGraphicItemSettings::setVisible(bool visible)
{
    if(!visible && currentPolyline != nullptr){
        QWidget::setVisible(true);
    } else {
        QWidget::setVisible(visible);
    }
}
