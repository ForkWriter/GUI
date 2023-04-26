#ifndef RECTGRAPHICITEMSETTINGS_H
#define RECTGRAPHICITEMSETTINGS_H

#include <QWidget>

class RectGraphicItem;

namespace Ui {
class RectGraphicItemSettings;
}

class RectGraphicItemSettings : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)

public:
    explicit RectGraphicItemSettings(QWidget *parent = 0);
    ~RectGraphicItemSettings();

    QColor borderColor() const;
    int borderWidth() const;

public slots:
    void setBorderColor(const QColor &color);
    void setBorderWidth(const int &width);

signals:
    void borderColorChanged(const QColor &color);
    void borderWidthChanged(const int &width);

private:
    Ui::RectGraphicItemSettings *ui;
    RectGraphicItem *currentRectangle;

public slots:
    void newRectangle(RectGraphicItem *rect);
    void loadRectangle(RectGraphicItem *rect);
    void deselect();
    void setVisible(bool visible) override;

private:
    QColor m_borderColor;
    int m_borderWidth;
};

#endif // RECTGRAPHICITEMSETTINGS_H
