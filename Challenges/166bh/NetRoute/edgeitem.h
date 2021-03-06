/*
 * Copyright � 2014 Kier Dugan.  All Rights Reserved.
 *
 * This source code is covered by the MIT licence as set out in the LICENSE
 * file at the root of this repository.
 */

#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QtGui/QFont>
#include <QtGui/QPen>
#include <QtWidgets/QGraphicsObject>

class NodeItem;

class EdgeItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QFont font READ font WRITE setFont RESET resetFont)

public:
    Q_INVOKABLE EdgeItem(NodeItem* start = nullptr, NodeItem* end = nullptr);
    virtual ~EdgeItem();

    static const float ArrowBase;
    static const float ArrowHeight;
    static const float LineWidth;
    static const float LabelOffset;

    enum { Type = UserType + 2 };

    // properties
    NodeItem* startNode() const { return m_startNode; }
    void setStartNode(NodeItem* node);

    NodeItem* endNode() const { return m_endNode; }
    void setEndNode(NodeItem* node);

    int weight() const { return m_weight; }
    void setWeight(int weight);

    QFont font() const { return m_font; }
    void setFont(QFont font);
    void resetFont();

    QPen emphasisPen() const { return m_emphPen; }
    void setEmphasisPen(QPen emphPen);
    void resetEmphasisPen();

    bool isArrowheadVisible() const { return m_arrowhead; }
    void setArrowheadVisible(bool visible);

    bool isEmphasised() const { return m_emphasised; }
    void setEmphasised(bool emph);

    virtual int type() const { return Type; }

    virtual QRectF boundingRect() const;

    // methods
    virtual void adjust();

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget = nullptr);

private:
    void recalculateLabelRect();

private:
    QFont       m_font;
    QPen        m_emphPen;
    QLineF      m_line;
    QRectF      m_labelRect;
    NodeItem*   m_startNode;
    NodeItem*   m_endNode;
    int         m_weight;
    bool        m_arrowhead;
    bool        m_emphasised;
};

#endif // EDGEITEM_H
