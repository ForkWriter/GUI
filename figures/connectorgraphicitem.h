#ifndef CONNECTORGRAPHICITEM_H
#define CONNECTORGRAPHICITEM_H

#include <QObject>
#include <QGraphicsPathItem>
#include "figures/cornerdots.h"
#include "figures/rectgraphicitem.h"

class CornerDots;
class RectGraphicItem;
class QGraphicsSceneMouseEvent;

class ConnectorGraphicItem : public QObject, public QGraphicsPathItem
{
  Q_OBJECT
  Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
  explicit ConnectorGraphicItem(QObject *parent = 0, int type = 0);
  ~ConnectorGraphicItem();

  QPointF previousPosition() const;
  void setPreviousPosition(const QPointF previousPosition);
  void setPath(const QPainterPath &path);

  void processCollidings(QList<QGraphicsItem *> collidins, QPainterPath *path, bool line_start);
  bool checkRects();

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
  int type;
  RectGraphicItem *input_rect;
  RectGraphicItem *output_rect;

  void updateDots();
};

#endif // CONNECTORGRAPHICITEM_H
