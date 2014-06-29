#include <QtGui/QPainter>

#include "EdgeItem.h"
#include "NodeItem.h"

const float EdgeItem::ArrowBase   =  6.0f;
const float EdgeItem::ArrowHeight = 10.0f;
const float EdgeItem::LineWidth   =  2.0f;

EdgeItem::EdgeItem (NodeItem* start /* = NULL */, NodeItem* end /* = NULL */)
    : QGraphicsObject ()
    , m_startNode (NULL)
    , m_endNode (NULL)
{
    //m_startNode->addEdge (this);
    //m_endNode->addEdge (this);
    setStartNode (start);
    setEndNode (end);

    setCacheMode (QGraphicsItem::NoCache);
    setAcceptedMouseButtons (Qt::NoButton);
    adjust ();
}

EdgeItem::~EdgeItem()
{
}

void EdgeItem::setStartNode (NodeItem* node)
{
    if (m_startNode != NULL)
        m_startNode->removeEdge (this);

    m_startNode = node;

    if (m_startNode != NULL)
        m_startNode->addEdge (this);
}

void EdgeItem::setEndNode (NodeItem* node)
{
    if (m_endNode != NULL)
        m_endNode->removeEdge (this);

    m_endNode = node;

    if (m_endNode != NULL)
        m_endNode->addEdge (this);
}

QRectF EdgeItem::boundingRect () const
{
    return QRectF (-m_line.dx () / 2.0f, -m_line.dy () / 2.0f,
        m_line.dx (), m_line.dy ());
}

void EdgeItem::adjust ()
{
    if (m_startNode == NULL || m_endNode == NULL)
        return;

    prepareGeometryChange ();

    // update the line object
    m_line.setP1 (m_startNode->pos ());
    m_line.setP2 (m_endNode->pos ());

    // recalculate the start and end positions
    if (m_line.length () > 0.0) {
        QLineF trans = m_line.unitVector ();
        trans.setLength (NodeItem::Radius);
        m_line.translate (trans.dx (), trans.dy ());
        m_line.setLength (m_line.length () - 2*NodeItem::Radius - 
            ArrowHeight/2);
    }
}

void EdgeItem::paint (QPainter* painter, const QStyleOptionGraphicsItem* option,
    QWidget* widget /* = NULL */)
{
    // draw the line
    QPen p (Qt::blue, LineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    painter->setPen (p);
    painter->drawLine (m_line);

    // draw the arrow head
    QPolygonF arrowHead;
    arrowHead.append (QPointF (-ArrowHeight / 2,  ArrowBase / 2));
    arrowHead.append (QPointF ( ArrowHeight / 2,  0.0f));
    arrowHead.append (QPointF (-ArrowHeight / 2, -ArrowBase / 2));
    QTransform trans;
    trans.translate (m_line.x2 (), m_line.y2 ());
    trans.rotate (-m_line.angle ());
    painter->setPen (Qt::NoPen);
    painter->setBrush (QBrush (Qt::blue));
    painter->setTransform (trans, true);
    painter->drawPolygon (arrowHead);
}

