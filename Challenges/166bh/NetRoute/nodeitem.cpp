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

const float NodeItem::Radius    = 20.0f;
const float NodeItem::Border    =  2.0f;
const float NodeItem::EmphWidth = 10.0f;


NodeItem::NodeItem(QGraphicsItem* parent /* = nullptr */)
    : QGraphicsObject(parent)
    , m_emphasised(false)
{
    resetBackground();
    resetEmphasisBrush();
    resetBorderPen();
    resetSelectionPen();
    resetTextPen();
    resetFont();

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPos(0, 0);
    setZValue(200.0f);
}

NodeItem::~NodeItem()
{
}

void NodeItem::resetBackground()
{
    QLinearGradient g(-Radius, -Radius, Radius / 2, Radius / 2);
    g.setColorAt(0, QColor(  0, 135, 255));
    g.setColorAt(1, QColor(255, 255, 255));
    m_brush = g;
}

void NodeItem::setBackground(QBrush brush)
{
    m_brush = brush;
    invalidate();
}

void NodeItem::setEmphasisBrush(QBrush brush)
{
    m_emphBrush = brush;

    if (m_emphasised) {
        invalidate();
    }
}

void NodeItem::resetEmphasisBrush()
{
    QRadialGradient g(0, 0, Radius + 2*EmphWidth);
    g.setColorAt(0.50f, QColor(Qt::gray));
    g.setColorAt(0.75f, QColor(255, 255, 255, 0));
    m_emphBrush = g;
}

void NodeItem::setEmphasised(bool emph)
{
    m_emphasised = emph;
    update();
}

void NodeItem::setBorderPen(QPen pen)
{
    m_penBorder = pen;
    invalidate();
}

void NodeItem::resetBorderPen()
{
    QBrush b(QColor(0, 85, 255));
    m_penBorder = QPen(b, Border);
}

void NodeItem::setSelectionPen(QPen pen)
{
    m_penSelect = pen;
    invalidate();
}

void NodeItem::resetSelectionPen()
{
    m_penSelect = QPen(QBrush(Qt::gray), 1, Qt::DashLine);
}

void NodeItem::setTextPen(QPen pen)
{
    m_penText = pen;
    invalidate();
}

void NodeItem::resetTextPen()
{
    m_penText   = QPen(Qt::black);
}

void NodeItem::setFont(QFont font)
{
    m_font = font;
    invalidate();
}

void NodeItem::resetFont()
{
    m_font      = qApp->font();
    m_font.setBold(true);
    m_font.setPixelSize(14);
}

void NodeItem::setText(const QString& text)
{
    m_text = text;
    invalidate();
}

QRectF NodeItem::boundingRect() const
{
    QRectF rect(-Radius, -Radius, 2*Radius, 2*Radius);
    return rect.adjusted(-10, -10, 10, 10);
}

void NodeItem::addEdge(EdgeItem* edge)
{
    m_edges.append(edge);
}

void NodeItem::removeEdge(EdgeItem* edge)
{
    m_edges.removeAll(edge);
}

void NodeItem::invalidate()
{
    QRectF rc = boundingRect();
    rc.moveTo(pos());
    update(rc);
}

void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
    QWidget* widget /* = nullptr */)
{
    QRectF rect(-Radius, -Radius, 2*Radius, 2*Radius);

    // Draw the emphasis
    if (m_emphasised) {
        QRectF rcEmph = rect.adjusted(-EmphWidth, -EmphWidth,
            EmphWidth, EmphWidth);

        // Draw the emphasis background
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_emphBrush);
        painter->drawEllipse(rcEmph);
    }

    // Draw the background and border
    painter->setBrush(m_brush);
    painter->setPen(m_penBorder);
    painter->drawEllipse(rect);

    // Draw the text, if there is any
    if (!m_text.isEmpty()) {
        painter->setPen(m_penText);
        painter->setFont(m_font);
        painter->drawText(rect, Qt::AlignCenter | Qt::AlignVCenter |
            Qt::TextSingleLine, m_text);
    }

    // Draw the selection rectangle
    if (isSelected()) {
        rect.adjust(-10, -10, 10, 10);
        painter->setPen(m_penSelect);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(rect, 10, 10);
    }
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    emit nodeItemDoubleClicked(this);
    QGraphicsObject::mouseDoubleClickEvent(event);
}

QVariant NodeItem::itemChange(QGraphicsItem::GraphicsItemChange change,
    const QVariant& value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        QListIterator<EdgeItem*> i(m_edges);
        while (i.hasNext()) {
            EdgeItem* edge = i.next();
            edge->adjust();
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

