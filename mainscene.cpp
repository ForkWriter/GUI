#include "mainscene.h"
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QDebug>

MainScene::MainScene(QObject *parent) :
    QGraphicsScene(parent),
    currentItem(nullptr),
    m_currentAction(DefaultType),
    m_previousAction(0),
    m_leftMouseButtonPressed(false)
{

}

MainScene::~MainScene()
{
    delete currentItem;
}

int MainScene::currentAction() const
{
    return m_currentAction;
}

QPointF MainScene::previousPosition() const
{
    return m_previousPosition;
}

void MainScene::setCurrentAction(const int type)
{
    m_currentAction = type;
    emit currentActionChanged(m_currentAction);
}

void MainScene::setPreviousPosition(const QPointF previousPosition)
{
    if (m_previousPosition == previousPosition)
        return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged();
}

void MainScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        m_leftMouseButtonPressed = true;
        setPreviousPosition(event->scenePos());
        if(QApplication::keyboardModifiers() & Qt::ShiftModifier){
            m_previousAction = m_currentAction;
            setCurrentAction(SelectionType);
        }
    }
    switch (m_currentAction) {
      case One_to_Many:
      case Many_to_Many:
      case One_to_One: {
        if (m_leftMouseButtonPressed && !(event->button() & Qt::RightButton) && !(event->button() & Qt::MiddleButton)) {
            deselectItems();
            ConnectorGraphicItem *polyline = new ConnectorGraphicItem(this, m_currentAction);
            currentItem = polyline;
            addItem(currentItem);
            connect(polyline, &ConnectorGraphicItem::clicked, this, &MainScene::signalSelectItem);
            connect(polyline, &ConnectorGraphicItem::signalMove, this, &MainScene::slotMove);
            QPainterPath path;
            path.moveTo(m_previousPosition);
            polyline->setPath(path);
            QList<QGraphicsItem *> colliding = collidingItems(polyline);
            polyline->processCollidings(colliding, &path, true);
            emit signalNewSelectItem(polyline);
        }
        break;
    }
    case RectangleType: {
        if (m_leftMouseButtonPressed && !(event->button() & Qt::RightButton) && !(event->button() & Qt::MiddleButton)) {
            deselectItems();
            RectGraphicItem *rectangle = new RectGraphicItem();
            currentItem = rectangle;
            addItem(currentItem);
            connect(rectangle, &RectGraphicItem::clicked, this, &MainScene::signalSelectItem);
            connect(rectangle, &RectGraphicItem::signalMove, this, &MainScene::slotMove);
            emit signalNewSelectItem(rectangle);
        }
        break;
    }
    case RedactType: {
        QList<QGraphicsItem *> colliding = items(event->scenePos());
        if (!colliding.empty())
          currentItem = colliding.last();
        break;
    }
    case SelectionType: {
        if (m_leftMouseButtonPressed && !(event->button() & Qt::RightButton) && !(event->button() & Qt::MiddleButton)) {
            deselectItems();
            SelectionRect *selection = new SelectionRect();
            currentItem = selection;
            addItem(currentItem);
        }
        break;
    }
    default: {
        QGraphicsScene::mousePressEvent(event);
        break;
    }
    }
}

void MainScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    switch (m_currentAction) {
      case One_to_Many:
      case Many_to_Many:
      case One_to_One: {
        if (m_leftMouseButtonPressed) {
            ConnectorGraphicItem * polyline = qgraphicsitem_cast<ConnectorGraphicItem *>(currentItem);
            QPainterPath path;
            path.moveTo(m_previousPosition);
            path.lineTo(event->scenePos());
            polyline->setPath(path);
        }
        break;
    }
    case RectangleType:
    case RedactType: {
      break;
    }
    case SelectionType: {
        if (m_leftMouseButtonPressed) {
            auto dx = event->scenePos().x() - m_previousPosition.x();
            auto dy = event->scenePos().y() - m_previousPosition.y();
            SelectionRect * selection = qgraphicsitem_cast<SelectionRect *>(currentItem);
            selection->setRect((dx > 0) ? m_previousPosition.x() : event->scenePos().x(),
                                   (dy > 0) ? m_previousPosition.y() : event->scenePos().y(),
                                   qAbs(dx), qAbs(dy));
        }
        break;
    }
    default: {
        QGraphicsScene::mouseMoveEvent(event);
        break;
    }
    }
}

void MainScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) m_leftMouseButtonPressed = false;
    switch (m_currentAction) {
    case One_to_Many:
    case Many_to_Many:
    case One_to_One: {
        if (!m_leftMouseButtonPressed &&
                !(event->button() & Qt::RightButton) &&
                !(event->button() & Qt::MiddleButton)) {
          ConnectorGraphicItem *polyline = dynamic_cast<ConnectorGraphicItem*>(currentItem);
          QPainterPath path = polyline->path();
          QList<QGraphicsItem *> colliding = collidingItems(polyline);
          polyline->processCollidings(colliding, &path, false);
          currentItem = nullptr;
          if (!polyline->checkRects()) {
            removeItem(polyline);
            delete polyline;
          }
        }
        break;
      }
    case RectangleType: {
        if (!m_leftMouseButtonPressed &&
                !(event->button() & Qt::RightButton) &&
                !(event->button() & Qt::MiddleButton)) {
            RectGraphicItem *rectangle = qgraphicsitem_cast<RectGraphicItem *>(currentItem);
            rectangle->setRect(event->scenePos().x(), event->scenePos().y(), 150, 150);
            rectangle->setDB(current_model->dbms());
            EntityNameDialog *dialog = new EntityNameDialog(nullptr, rectangle, current_model);
            dialog->show();
            currentItem = nullptr;
        }
        break;
    }
    case RedactType: {
        if (!m_leftMouseButtonPressed &&
                !(event->button() & Qt::RightButton) &&
                !(event->button() & Qt::MiddleButton)) {
            RectGraphicItem *rectangle = qgraphicsitem_cast<RectGraphicItem *>(currentItem);
            if (rectangle) {
              rectangle->redactAttributes();
            }
            currentItem = nullptr;
        }
        break;
    }
    case SelectionType: {
        if (!m_leftMouseButtonPressed &&
                !(event->button() & Qt::RightButton) &&
                !(event->button() & Qt::MiddleButton)) {
            SelectionRect * selection = qgraphicsitem_cast<SelectionRect *>(currentItem);
            if(!selection->collidingItems().isEmpty()){
                foreach (QGraphicsItem *item, selection->collidingItems()) {
                    item->setSelected(true);
                }
            }
            selection->deleteLater();
            if(selectedItems().length() == 1){
                signalSelectItem(selectedItems().at(0));
            }
            setCurrentAction(m_previousAction);
            currentItem = nullptr;
        }
        break;
    }
    default: {
        QGraphicsScene::mouseReleaseEvent(event);
        break;
    }
    }
}

void MainScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    switch (m_currentAction) {
    case One_to_Many:
    case Many_to_Many:
    case One_to_One:
    case RectangleType:
    case RedactType:
    case SelectionType:
        break;
    default:
        QGraphicsScene::mouseDoubleClickEvent(event);
        break;
    }
}

void MainScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Delete: {
        foreach (QGraphicsItem *item, selectedItems()) {
            removeItem(item);
            delete item;
        }
        deselectItems();
        break;
    }
    case Qt::Key_A: {
        if(QApplication::keyboardModifiers() & Qt::ControlModifier){
            foreach (QGraphicsItem *item, items()) {
                item->setSelected(true);
            }
            if(selectedItems().length() == 1) signalSelectItem(selectedItems().at(0));
        }
        break;
    }
    default:
        break;
    }
    QGraphicsScene::keyPressEvent(event);
}

void MainScene::deselectItems()
{
    foreach (QGraphicsItem *item, selectedItems()) {
        item->setSelected(false);
    }
    selectedItems().clear();
}

void MainScene::slotMove(QGraphicsItem *signalOwner, qreal dx, qreal dy)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if(item != signalOwner) item->moveBy(dx,dy);
      }
}

void MainScene::updateModel()
{
  clear();
  std::vector<std::string> entities =  current_model->entities();
  for (int i = 0; i< entities.size(); i++) {
    deselectItems();
    RectGraphicItem *rectangle = new RectGraphicItem();
    addItem(rectangle);
    connect(rectangle, &RectGraphicItem::clicked, this, &MainScene::signalSelectItem);
    connect(rectangle, &RectGraphicItem::signalMove, this, &MainScene::slotMove);
    emit signalNewSelectItem(rectangle);
    rectangle->setDB(current_model->dbms());
    rectangle->setEntity(std::make_pair(entities[i], current_model->entity(entities[i])), true);
    rectangle->restoreFromAdditionalParams();
  }
}

Model *MainScene::getCurrent_model() const
{
  return current_model;
}

void MainScene::setCurrent_model(Model *newCurrent_model)
{
  current_model = newCurrent_model;
}
