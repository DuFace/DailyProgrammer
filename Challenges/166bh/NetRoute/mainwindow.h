#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>

#include "controlsdockwidget.h"
#include "ui_mainwindow.h"

#include "graph.h"

class NodeItem;
class EdgeItem;


class MainWindow : public QMainWindow
{
    Q_OBJECT

    typedef QMap<QString, NodeItem*>    NamedNodeMap;
    typedef QList<EdgeItem*>            EdgeList;

    enum ReturnCodes {
        Success             =  0,

        ErrorEmpty          = -1,
        ErrorSpecTooSmall   = -2,
        ErrorRowTooShort    = -3,

        WarningAbort        = 0x01,
        WarningBadCell      = 0x02,
        WarningBadStartEnd  = 0x04,
        WarningNoStartNode  = 0x08,
        WarningNoEndNode    = 0x10,
    };

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    // Properties
    bool highlightStartNode() const;
    void setHighlightStartNode(bool highlight);

    bool highlightEndNode() const;
    void setHighlightEndNode(bool highlight);

    bool highlightPath() const;
    void setHighlightPath(bool highlight);

    // Logging functions
    void postInfoMessage(const QString& msg);
    void postSuccessMessage(const QString& msg);
    void postWarningMessage(const QString& msg);
    void postErrorMessage(const QString& msg);

    // Graph layout stuff
    void applyRandomLayout();
    void applySpringLayout();

    // Network specification stuff
    void parseAndRouteNetwork(const QString& description);
    void clearNetwork();
    void generateRouteReport();

private:
    int  buildNetwork(const QString& description);
    void routeNetwork();
    void postMessage(const QString& htmlColour, const QString& leader,
        const QString& msg);

private:
    Ui::MainWindowClass m_ui;

    Digraph             m_graph;
    QGraphicsScene*     m_graphScene;
    QGraphicsView*      m_graphView;

    ControlsDockWidget* m_controlsDock;
    QTextEdit*          m_transcript;

    NamedNodeMap        m_graphNodes;
    EdgeList            m_route;
    NodeItem*           m_routeStart;
    NodeItem*           m_routeEnd;

    bool                m_highlightPath;
};

#endif // MAINWINDOW_H
