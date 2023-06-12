#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QObject>
#include <QGraphicsScene>

#include "forms/entitynamedialog.h"
#include "figures/connectorgraphicitem.h"
#include "figures/selectionrect.h"
#include "figures/rectgraphicitem.h"
#include "datamodeler/model/model.hpp"

class QGraphicsSceneMouseEvent;
class QKeyEvent;

class MainScene : public QGraphicsScene
{
    Q_OBJECT

    Q_PROPERTY(int currentAction READ currentAction WRITE setCurrentAction NOTIFY currentActionChanged)
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
    explicit MainScene(QObject *parent = 0);
    ~MainScene();

    enum ActionTypes {
        DefaultType,
        One_to_Many,
        Many_to_Many,
        One_to_One,
        RectangleType,
        SelectionType,
        RedactType
    };

    int currentAction() const;
    QPointF previousPosition() const;

    void setCurrentAction(const int type);
    void setPreviousPosition(const QPointF previousPosition);

    void setCurrent_model(Model *newCurrent_model);

    Model *getCurrent_model() const;

signals:
    void previousPositionChanged();
    void currentActionChanged(int);
    void signalSelectItem(QGraphicsItem *item);
    void signalNewSelectItem(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void deselectItems();

public slots:
    void slotMove(QGraphicsItem *signalOwner, qreal dx, qreal dy);
    void updateModel();

private:
    QGraphicsItem *currentItem;
    int m_currentAction;
    int m_previousAction;
    QPointF m_previousPosition;
    bool m_leftMouseButtonPressed;
    Model *current_model;
};

#endif // MAINSCENE_H
