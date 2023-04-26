#ifndef CONNECTORGRAPHICITEM_H
#define CONNECTORGRAPHICITEM_H

#include <QObject>
#include <QGraphicsPathItem>
#include "figures/cornerdots.h"

class CornerDots;
class QGraphicsSceneMouseEvent;

class ConnectorGraphicItem : public QObject, public QGraphicsPathItem
{
  Q_OBJECT
  Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
  explicit ConnectorGraphicItem(QObject *parent = 0);
  ~ConnectorGraphicItem();

  QPointF previousPosition() const;
  void setPreviousPosition(const QPointF previousPosition);
  void setPath(const QPainterPath &path);

signals:
  void previousPositionChanged();
  void clicked(ConnectorGraphicItem *rect);
  void signalMove(QGraphicsItem *item, qreal dx, qreal dy);

protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

public slots:

private slots:
  void slotMove(QGraphicsItem *signalOwner, qreal dx, qreal dy);
  void checkForDeletePoints();

private:
  QPointF m_previousPosition;
  bool m_leftMouseButtonPressed;
  QList<CornerDots *> listDotes;
  int m_pointForCheck;

  void updateDots();
};

#endif // CONNECTORGRAPHICITEM_H
