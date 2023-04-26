#include "rectgraphicitemsettings.h"
#include "ui_rectitemsettings.h"
#include <QColorDialog>
#include <QBrush>
#include <QPen>
#include <QDebug>
#include "rectgraphicitem.h"

RectGraphicItemSettings::RectGraphicItemSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RectGraphicItemSettings),
    currentRectangle(nullptr)
{
    ui->setupUi(this);

    setBorderColor(Qt::black);
    setBorderWidth(1);

    connect(ui->borderColor, &ColorLabel::clicked,
            [=](){
        QColorDialog dialog;
        connect(&dialog, &QColorDialog::colorSelected, this, &RectGraphicItemSettings::setBorderColor);
        dialog.exec();
    });

    connect(ui->borderWidth, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, &RectGraphicItemSettings::setBorderWidth);
}

RectGraphicItemSettings::~RectGraphicItemSettings()
{
    delete ui;
    delete currentRectangle;
}

QColor RectGraphicItemSettings::borderColor() const
{
    return m_borderColor;
}

int RectGraphicItemSettings::borderWidth() const
{
    return m_borderWidth;
}

void RectGraphicItemSettings::setBorderColor(const QColor &color)
{
    m_borderColor = color;
    ui->borderColor->setColor(color);
    if(currentRectangle != nullptr){
        QPen pen(color,currentRectangle->pen().width());
        currentRectangle->setPen(pen);
    }
    emit borderColorChanged(m_borderColor);
}

void RectGraphicItemSettings::setBorderWidth(const int &width)
{
    m_borderWidth = width;
    if(currentRectangle != nullptr){
        if(width == 0){
            currentRectangle->setPen(Qt::NoPen);
        } else {
            QPen pen(currentRectangle->pen().color(), width);
            currentRectangle->setPen(pen);
        }
    }
    emit borderWidthChanged(m_borderWidth);
}

void RectGraphicItemSettings::newRectangle(RectGraphicItem *rect)
{
    if(m_borderWidth == 0){
        rect->setPen(Qt::NoPen);
    } else {
        rect->setPen(QPen(m_borderColor, m_borderWidth));
    }
}

void RectGraphicItemSettings::loadRectangle(RectGraphicItem *rect)
{
    currentRectangle = rect;

    m_borderColor = currentRectangle->pen().color();
    if(currentRectangle->pen().style() == Qt::NoPen){
        m_borderWidth = 0;
    } else {
        m_borderWidth = currentRectangle->pen().width();
    }
    ui->borderColor->setColor(m_borderColor);
    ui->borderWidth->setValue(m_borderWidth);
}

void RectGraphicItemSettings::deselect()
{
    currentRectangle = nullptr;
}

void RectGraphicItemSettings::setVisible(bool visible)
{
    if(!visible && currentRectangle != nullptr){
        QWidget::setVisible(true);
    } else {
        QWidget::setVisible(visible);
    }
}
