#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QtWidgets/QGraphicsObject>

class NodeItem;

class EdgeItem : public QGraphicsObject
{
    Q_OBJECT

public:
    Q_INVOKABLE EdgeItem (NodeItem* start = NULL, NodeItem* end = NULL);
    virtual ~EdgeItem ();

    static const float ArrowBase;
    static const float ArrowHeight;
    static const float LineWidth;

    enum { Type = UserType + 2 };

    // properties
    NodeItem* startNode () const { return m_startNode; }
    void setStartNode (NodeItem* node);

    NodeItem* endNode () const { return m_endNode; }
    void setEndNode (NodeItem* node);

    virtual int type () const { return Type; }

    virtual QRectF boundingRect () const;

    // methods
    virtual void adjust ();

protected:
    virtual void paint (QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget = NULL);

private:
    QLineF      m_line;
    NodeItem*   m_startNode;
    NodeItem*   m_endNode;
};

#endif // EDGEITEM_H
