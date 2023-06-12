#ifndef RECTGRAPHICITEM_H
#define RECTGRAPHICITEM_H

#include <QObject>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include "addattributedialog.h"
#include "parametersdialog.h"
#include "DataModeler/include/datamodeler/model/entity.hpp"

class CornerDots;
class ConnectorGraphicItem;
class QGraphicsSceneMouseEvent;

class RectGraphicItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
    explicit RectGraphicItem(QObject * parent = 0);
    ~RectGraphicItem();

    enum CornerGrabbers {
        GrabberTop,
        GrabberBottom,
        GrabberLeft,
        GrabberRight,
        GrabberTopLeft,
        GrabberTopRight,
        GrabberBottomLeft,
        GrabberBottomRight
    };

    QPointF previousPosition() const;
    void setPreviousPosition(const QPointF previousPosition);

    void setRect(qreal x, qreal y, qreal w, qreal h);
    void setRect(const QRectF &rect);

    void addConnector(ConnectorGraphicItem *item);

    void setEntity(const std::pair<std::string, Entity *> &newEntity, bool restore = false);

    void setDB(const std::string &newDB);

    void restoreFromAdditionalParams();

    void setName(std::string name);

    void redactAttributes();

    QGraphicsSimpleTextItem *getEntity_name() const;

    const std::pair<std::string, Entity *> &getEntity() const;

signals:
    void rectChanged(RectGraphicItem *rect);
    void previousPositionChanged();
    void clicked(RectGraphicItem *rect);
    void signalMove(QGraphicsItem *item, qreal dx, qreal dy);

private slots:
    void attribute_parameters(Attribute* new_attr, std::string name = "");
    void attribute_add(std::string name, Attribute *new_attr);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF m_previousPosition, scenePos;
    bool m_leftMouseButtonPressed;
    double max_width, max_height;
    CornerDots *cornerGrabber[8];
    std::string DB;
    std::pair<std::string, Entity*> entity;
    QGraphicsSimpleTextItem *entity_name;
    std::vector<QGraphicsSimpleTextItem*> attributes;
    QGraphicsRectItem *prim_rect;
    QList<ConnectorGraphicItem*> connectors;

    void setPositionGrabbers();
    void hideGrabbers();
    void setAttributes();
    bool checkPos(std::string params);
    void saveAdditionalParams(QPointF point);
    std::vector<qreal> restoreCoords();

};

#endif // RECTGRAPHICITEM_H
