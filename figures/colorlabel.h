#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>

class QMouseEvent;

class ColorLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ColorLabel(QWidget *parent = 0);
    ~ColorLabel();

protected:
    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked();

public slots:
    void setColor(const QColor &color);
};

#endif // COLORLABEL_H
