#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QWidget>
#include <QGraphicsView>

class SceneView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:

public slots:
};

#endif // SCENEVIEW_H
