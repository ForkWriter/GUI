#include "rectgraphicitem.h"
#include <QPainter>
#include <QDebug>
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <math.h>
#include "figures/cornerdots.h"
#include "redactdialog.h"

RectGraphicItem::RectGraphicItem(QObject *parent) :
    QObject(parent),
    entity_name(new QGraphicsSimpleTextItem(this)),
    prim_rect(new QGraphicsRectItem(this)),
    max_width(0)
{
    setFlags(ItemIsSelectable|ItemSendsGeometryChanges);
    for(int i = 0; i < 8; i++){
        cornerGrabber[i] = new CornerDots(this);
    }
    setPositionGrabbers();
    hideGrabbers();
}

RectGraphicItem::~RectGraphicItem()
{
    for(int i = 0; i < 8; i++){
        delete cornerGrabber[i];
    }
}

QPointF RectGraphicItem::previousPosition() const
{
    return m_previousPosition;
}

void RectGraphicItem::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged();
}

void RectGraphicItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
  max_width = w;
  max_height = h;
  setRect(QRectF(x,y,w,h));
}

void RectGraphicItem::setRect(const QRectF &rect)
{
  QPointF sp = rect.topLeft();
  sp.setY(rect.topLeft().y() - 20);
  entity_name->setPos(sp);
  sp.setX(sp.x() + 2);

  double prim_height = 20;
  for (int i = 0, num = 0; i < attributes.size(); i++) {
    if (attributes[i]->text().toStdString().find("PK") != std::variant_npos) {
      prim_height = 20 + 15 * num;
      sp.setY(rect.topLeft().y() + 15 * num++);
      attributes[i]->setPos(sp);
    }
  }
  for (int i = 0, num = 0; i < attributes.size(); i++) {
    if (attributes[i]->text().toStdString().find("PK") == std::variant_npos) {
      int width = attributes[i]->boundingRect().width() + 5;
      if (max_width < width)
        max_width = width;
      sp.setY(rect.y() + prim_height + 15 * num++);
      attributes[i]->setPos(sp);
      if (sp.y() >= (rect.y() + rect.height())) {
        max_height += 15;
      }
    }
  }
  prim_rect->setRect(QRectF(rect.x(), rect.y(), max_width, prim_height));
  QGraphicsRectItem::setRect(QRectF(rect.x(), rect.y(), max_width, max_height));
  scenePos = rect.topLeft();
}

void RectGraphicItem::setAttributes()
{
  attributes.clear();
  std::vector<std::string> attrs_name = entity.second->attributes();
  for (int i = 0; i < attrs_name.size(); i++) {
    std::string attr_value = attrs_name[i] + "(" +
                              entity.second->attribute(attrs_name[i])->type() +
                              "): " + entity.second->attribute(attrs_name[i])->parameters();
    if (entity.second->attribute(attrs_name[i])->primaryKey()) {
      attr_value += " PK";
    }
    QGraphicsSimpleTextItem *attr_text = new QGraphicsSimpleTextItem(this);
    attr_text->setText(attr_value.c_str());
    attributes.push_back(attr_text);
    if (max_width < attr_text->boundingRect().width())
      max_width = attr_text->boundingRect().width() + 5;
  }
}

bool RectGraphicItem::checkPos(std::string params)
{
  std::string old_params = entity.second->additionalModelParameters().toString().toStdString();
  return params == old_params;
}

void RectGraphicItem::saveAdditionalParams(QPointF point)
{
  std::string x = QString::number(point.x()).toStdString();
  std::string y = QString::number(point.y()).toStdString();
  std::string w = QString::number(max_width).toStdString();
  std::string h = QString::number(max_height).toStdString();
  std::string add_params = x + ";" + y + ";" + w + ";" + h + ";";
  if (!checkPos(add_params))
    entity.second->setAdditionalModelParameters(add_params.c_str());
}

std::vector<qreal> RectGraphicItem::restoreCoords()
{
  QString params = entity.second->additionalModelParameters().toString();
  std::vector<qreal> coord;
  QRegExp re("-?[0-9]+");
  int lastPos = 0;
  while((lastPos = re.indexIn(params, lastPos)) != -1) {
    lastPos += re.matchedLength();
    coord.push_back(re.cap(0).toDouble());
  }
  return coord;
}

void RectGraphicItem::restoreFromAdditionalParams()
{
  std::vector<qreal> coord = restoreCoords();
  setRect(coord[0], coord[1], coord[2], coord[3]);
}

void RectGraphicItem::setName(std::string name)
{
  entity_name->setText(name.c_str());
}

void RectGraphicItem::redactAttributes()
{
  RedactDialog *dialog = new RedactDialog(nullptr, entity.second, entity_name->text().toStdString());
  connect(dialog, &RedactDialog::attrChange, this, &RectGraphicItem::attribute_parameters);
  //connect(dialog, &RedactDialog::nameChange, this, &RectGraphicItem::attribute_parameters);
  dialog->show();
}

void RectGraphicItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_leftMouseButtonPressed) {
        setCursor(Qt::ClosedHandCursor);
        auto dx = event->scenePos().x() - m_previousPosition.x();
        auto dy = event->scenePos().y() - m_previousPosition.y();
        moveBy(dx,dy);
        scenePos.setX(scenePos.x() + dx);
        scenePos.setY(scenePos.y() + dy);
        setPreviousPosition(event->scenePos());
        emit signalMove(this, dx, dy);
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void RectGraphicItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->scenePos());
        emit clicked(this);
    }
    QGraphicsItem::mousePressEvent(event);
}

void RectGraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        m_leftMouseButtonPressed = false;
        saveAdditionalParams(scenePos);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}

void RectGraphicItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    AddAttributeDialog *dialog = new AddAttributeDialog(nullptr, DB);
    connect(dialog, &AddAttributeDialog::AttrAdd, this, &RectGraphicItem::attribute_parameters);
    dialog->show();
    QGraphicsItem::mouseDoubleClickEvent(event);
}

const std::pair<std::string, Entity *> &RectGraphicItem::getEntity() const
{
  return entity;
}

QGraphicsSimpleTextItem *RectGraphicItem::getEntity_name() const
{
  return entity_name;
}

void RectGraphicItem::attribute_parameters(Attribute *new_attr, std::string name)
{
    ParametersDialog *dialog = new ParametersDialog(nullptr, new_attr, name);
    connect(dialog, &ParametersDialog::AttrName, this, &RectGraphicItem::attribute_add);
    dialog->show();
}

void RectGraphicItem::attribute_add(std::string name, Attribute *new_attr)
{
  if (entity.second->attributeName(new_attr).empty()) {
    entity.second->addAttribute(new_attr, name);
  } else {
    setAttributes();
  }
  if (name.empty())
    name = entity.second->attributeName(new_attr);
  std::string attr_value = name + "(" + new_attr->type() + "): " + new_attr->parameters();
  if (new_attr->primaryKey()) {
    attr_value += " PK";
  }
  QGraphicsSimpleTextItem *attr_text = new QGraphicsSimpleTextItem(this);
  attr_text->setText(attr_value.c_str());
  attributes.push_back(attr_text);
  if (max_width < attr_text->boundingRect().width())
    max_width = attr_text->boundingRect().width() + 5;
  setRect(rect());
}

void RectGraphicItem::setDB(const std::string &newDB)
{
  DB = newDB;
}


void RectGraphicItem::setEntity(const std::pair<std::string, Entity *> &newEntity, bool restore)
{
  entity = newEntity;
  entity_name->setText(newEntity.first.c_str());
  if (!restore) {
    saveAdditionalParams(scenePos);
  }
  setAttributes();
}

void RectGraphicItem::addConnector(ConnectorGraphicItem *item)
{
  connectors.append(item);
}

void RectGraphicItem::setPositionGrabbers()
{
    QRectF tmpRect = rect();
    cornerGrabber[GrabberTop]->setPos(tmpRect.left() + tmpRect.width()/2, tmpRect.top());
    cornerGrabber[GrabberBottom]->setPos(tmpRect.left() + tmpRect.width()/2, tmpRect.bottom());
    cornerGrabber[GrabberLeft]->setPos(tmpRect.left(), tmpRect.top() + tmpRect.height()/2);
    cornerGrabber[GrabberRight]->setPos(tmpRect.right(), tmpRect.top() + tmpRect.height()/2);
    cornerGrabber[GrabberTopLeft]->setPos(tmpRect.topLeft().x(), tmpRect.topLeft().y());
    cornerGrabber[GrabberTopRight]->setPos(tmpRect.topRight().x(), tmpRect.topRight().y());
    cornerGrabber[GrabberBottomLeft]->setPos(tmpRect.bottomLeft().x(), tmpRect.bottomLeft().y());
    cornerGrabber[GrabberBottomRight]->setPos(tmpRect.bottomRight().x(), tmpRect.bottomRight().y());
}

void RectGraphicItem::hideGrabbers()
{
  for(int i = 0; i < 8; i++){
      cornerGrabber[i]->hide();
    }
}

