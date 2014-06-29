#include <QtGui/QPainter>
#include <QtWidgets/QApplication>

#include "EdgeItem.h"
#include "NodeItem.h"

const float EdgeItem::ArrowBase   =  6.0f;
const float EdgeItem::ArrowHeight = 10.0f;
const float EdgeItem::LineWidth   =  2.0f;
const float EdgeItem::LabelOffset =  5.0f;

EdgeItem::EdgeItem (NodeItem* start /* = NULL */, NodeItem* end /* = NULL */)
    : QGraphicsObject ()
    , m_startNode (NULL)
    , m_endNode (NULL)
    , m_weight (0)
    , m_arrowhead (false)
    , m_emphasised (false)
{
    resetFont();
    resetEmphasisPen();

    setStartNode (start);
    setEndNode (end);
    setZValue(100.0f);

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

void EdgeItem::setWeight(int weight)
{
    if (m_weight != weight) {
        m_weight = weight;
        recalculateLabelRect();
    }
}

void EdgeItem::setArrowheadVisible(bool visible)
{
    m_arrowhead = visible;
    update();
}

void EdgeItem::setEmphasised(bool emph)
{
    m_emphasised = emph;
    update();
}

void EdgeItem::setEmphasisPen(QPen emphPen)
{
    m_emphPen = emphPen;

    if (m_emphasised) { 
        update();
    }
}

void EdgeItem::resetEmphasisPen()
{
    QPen p(QBrush(QColor(Qt::yellow)), 12.0f, Qt::SolidLine, Qt::RoundCap);
    m_emphPen = p;
}

void EdgeItem::setFont (QFont font)
{
    m_font = font;
    update();
}

void EdgeItem::resetFont ()
{
    m_font      = qApp->font ();
    m_font.setBold (true);
    m_font.setPixelSize(14);
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

        float newLength = m_line.length () - 2 * NodeItem::Radius;
        if (m_arrowhead) {
            newLength -= ArrowHeight / 2;
        }
        m_line.setLength(newLength);
    }
}

void EdgeItem::paint (QPainter* painter, const QStyleOptionGraphicsItem* option,
    QWidget* widget /* = NULL */)
{
    // Draw the emphasis line
    if (m_emphasised) {
        painter->setPen(m_emphPen);
        painter->drawLine(m_line);
    }

    // draw the line
    QPen p (Qt::blue, LineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    painter->setPen (p);
    painter->drawLine (m_line);

    // Draw the label
    QRectF rcLabel(m_labelRect);
    QPointF ptLabel(m_line.pointAt(0.5f));
    if ((m_line.dx() * m_line.dy()) < 0) {
        ptLabel += QPointF(-EdgeItem::LabelOffset,
            -EdgeItem::LabelOffset);
        rcLabel.moveBottomRight(ptLabel);
    } else {
        ptLabel += QPointF(EdgeItem::LabelOffset,
            -EdgeItem::LabelOffset);
        rcLabel.moveBottomLeft(ptLabel);
    }
    painter->setFont(m_font);
    painter->drawText(rcLabel, QString("%1").arg(m_weight));

    // draw the arrow head
    if (m_arrowhead) {
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
}

void EdgeItem::recalculateLabelRect()
{
    QFontMetrics fm (font());
    m_labelRect = fm.boundingRect(QString("%1").arg(m_weight));
}
