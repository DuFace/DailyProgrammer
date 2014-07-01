/*
 * Copyright © 2014 Kier Dugan.  All Rights Reserved.
 *
 * This source code is covered by the MIT licence as set out in the LICENSE
 * file at the root of this repository.
 */

#include <QtGui/QPainter>
#include <QtWidgets/QApplication>

#include "edgeitem.h"
#include "nodeitem.h"

const float EdgeItem::ArrowBase   =  6.0f;
const float EdgeItem::ArrowHeight = 10.0f;
const float EdgeItem::LineWidth   =  2.0f;
const float EdgeItem::LabelOffset =  5.0f;

EdgeItem::EdgeItem(NodeItem* start /* = nullptr */,
                   NodeItem* end /* = nullptr */)
    : QGraphicsObject()
    , m_startNode(nullptr)
    , m_endNode(nullptr)
    , m_weight(0)
    , m_arrowhead(false)
    , m_emphasised(false)
{
    resetFont();
    resetEmphasisPen();

    setStartNode(start);
    setEndNode(end);
    setZValue(100.0f);

    setCacheMode(QGraphicsItem::NoCache);
    setAcceptedMouseButtons(Qt::NoButton);
    adjust();
}

EdgeItem::~EdgeItem()
{
}

void EdgeItem::setStartNode(NodeItem* node)
{
    if (m_startNode != nullptr)
        m_startNode->removeEdge(this);

    m_startNode = node;

    if (m_startNode != nullptr)
        m_startNode->addEdge(this);
}

void EdgeItem::setEndNode(NodeItem* node)
{
    if (m_endNode != nullptr)
        m_endNode->removeEdge(this);

    m_endNode = node;

    if (m_endNode != nullptr)
        m_endNode->addEdge(this);
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
    QBrush b(QColor(104, 222, 255, 64));
    QPen p(b, 12.0f, Qt::SolidLine, Qt::RoundCap);
    m_emphPen = p;
}

void EdgeItem::setFont(QFont font)
{
    m_font = font;
    update();
}

void EdgeItem::resetFont()
{
    m_font = qApp->font();
    m_font.setBold(true);
    m_font.setPixelSize(14);
}


QRectF EdgeItem::boundingRect() const
{
    // Calculate the bounding box for the line itself
    float dx = qAbs(m_line.dx());
    float dy = qAbs(m_line.dy());
    QRectF rcLine(-dx / 2.0f, -dy / 2.0f, dx, dy);

    // Factor in the label
    return QRectF(
        qMin(rcLine.left(), m_labelRect.left() - LabelOffset),
        qMin(rcLine.top(), m_labelRect.top() - LabelOffset),
        qMax(rcLine.width(), m_labelRect.width() + 2*LabelOffset),
        qMax(rcLine.height(), m_labelRect.height() + 2*LabelOffset));
}

void EdgeItem::adjust()
{
    if (m_startNode == nullptr || m_endNode == nullptr)
        return;

    prepareGeometryChange();

    // update the line object
    m_line.setP1(m_startNode->pos());
    m_line.setP2(m_endNode->pos());

    // recalculate the start and end positions
    if (m_line.length() > 0.0) {
        QLineF trans = m_line.unitVector();
        trans.setLength(NodeItem::Radius);
        m_line.translate(trans.dx(), trans.dy());

        float newLength = m_line.length() - 2 * NodeItem::Radius;
        if (m_arrowhead) {
            newLength -= ArrowHeight / 2;
        }
        m_line.setLength(newLength);
    }
}

void EdgeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
    QWidget* widget /* = nullptr */)
{
    // Draw the emphasis line
    if (m_emphasised) {
        painter->setPen(m_emphPen);
        painter->drawLine(m_line);
    }

    // draw the line
    //QBrush b(QColor(18, 228, 18));
    QBrush b(QColor(0, 199, 255));
    QPen p(b, LineWidth, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
    painter->setPen(p);
    painter->drawLine(m_line);

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
        arrowHead.append(QPointF(-ArrowHeight / 2,  ArrowBase / 2));
        arrowHead.append(QPointF( ArrowHeight / 2,  0.0f));
        arrowHead.append(QPointF(-ArrowHeight / 2, -ArrowBase / 2));
        QTransform trans;
        trans.translate(m_line.x2 (), m_line.y2 ());
        trans.rotate(-m_line.angle ());
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(Qt::blue));
        painter->setTransform(trans, true);
        painter->drawPolygon(arrowHead);
    }
}

void EdgeItem::recalculateLabelRect()
{
    QFontMetrics fm (font());
    m_labelRect = fm.boundingRect(QString("%1").arg(m_weight));
}
