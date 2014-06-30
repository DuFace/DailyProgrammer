#ifndef NODEITEM_H
#define NODEITEM_H

#include <QtGui/QBrush>
#include <QtGui/QFont>
#include <QtGui/QPen>
#include <QtWidgets/QGraphicsObject>

class EdgeItem;

class NodeItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QBrush background READ background WRITE setBackground 
        RESET resetBackground)
    Q_PROPERTY(QBrush emphasisBrush READ emphasisBrush WRITE setEmphasisBrush
        RESET resetEmphasisBrush)
    Q_PROPERTY(QPen borderPen READ borderPen WRITE setBorderPen
        RESET resetBorderPen)
    Q_PROPERTY(QPen selectionPen READ selectionPen WRITE setSelectionPen
        RESET resetSelectionPen)
    Q_PROPERTY(QPen textPen READ textPen WRITE setTextPen RESET resetTextPen)
    Q_PROPERTY(QFont font READ font WRITE setFont RESET resetFont)
    Q_PROPERTY(QString text READ text WRITE setText)

public:
    Q_INVOKABLE NodeItem(QGraphicsItem* parent = nullptr);
    virtual ~NodeItem();

    static const float Radius;
    static const float Border;
    static const float EmphWidth;

    enum { Type = UserType + 1 };

    // properties
    QList<EdgeItem*> edges() const { return m_edges; }

    QBrush background() const { return m_brush; }
    void setBackground(QBrush brush);
    void resetBackground();

    QPen borderPen() const { return m_penBorder; }
    void setBorderPen(QPen pen);
    void resetBorderPen();

    QPen selectionPen() const { return m_penSelect; }
    void setSelectionPen(QPen pen);
    void resetSelectionPen();

    QPen textPen() const { return m_penText; }
    void setTextPen(QPen pen);
    void resetTextPen();

    QFont font() const { return m_font; }
    void setFont(QFont font);
    void resetFont();

    QString text() const { return m_text; }
    void setText(const QString& text);

    QBrush emphasisBrush() const { return m_emphBrush; }
    void setEmphasisBrush(QBrush brush);
    void resetEmphasisBrush();

    bool isEmphasised() const { return m_emphasised; }
    void setEmphasised(bool emph);

    virtual int type() const { return Type; }
    virtual QRectF boundingRect() const;

    // methods
    void addEdge(EdgeItem* edge);
    void removeEdge(EdgeItem* edge);

signals:
    void nodeItemDoubleClicked(NodeItem* node);

protected:
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget = nullptr);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);

    virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change,
        const QVariant& value);

    void invalidate();

private:
    QList<EdgeItem*>    m_edges;
    QBrush              m_brush;
    QBrush              m_emphBrush;
    QPen                m_penBorder;
    QPen                m_penSelect;
    QPen                m_penText;
    QFont               m_font;
    bool                m_emphasised;
    QString             m_text;
};

#endif // NODEITEM_H
