#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtOpenGL/QGLWidget>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMessageBox>

#include "mainwindow.h"

#include "nodeitem.h"
#include "edgeitem.h"

typedef boost::detail::constant_value_property_map<double>
    ConstantValueDoubleMap;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_highlightStartNode(true)
    , m_highlightEndNode(true)
    , m_highlightPath(true)
{
    // Set up the general UI stuff
    m_ui.setupUi(this);

    // Create the graph explorer
    m_graphScene = new QGraphicsScene(this);
    m_graphScene->setItemIndexMethod(QGraphicsScene::NoIndex);

    m_graphView = new QGraphicsView;
    m_graphView->setScene(m_graphScene);
    m_graphView->setRenderHints(QPainter::Antialiasing |
        QPainter::HighQualityAntialiasing);
    m_graphView->setCacheMode(QGraphicsView::CacheNone);
    m_graphView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_graphView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    m_graphView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    m_graphView->ensureVisible(-150.0f, -150.0f, 300.0f, 300.0f);

    // Set as central widget
    setCentralWidget(m_graphView);

    // Create the transcript
    QDockWidget* transcriptDock = new QDockWidget(
        QLatin1String("Transcript"), this);

    m_transcript = new QTextEdit(transcriptDock);
    m_transcript->setFont(QFont("Meslo LG S DZ", 8));

    transcriptDock->setFeatures(QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetVerticalTitleBar);
    transcriptDock->setAllowedAreas(Qt::TopDockWidgetArea |
        Qt::BottomDockWidgetArea);
    transcriptDock->setWidget(m_transcript);

    addDockWidget(Qt::BottomDockWidgetArea, transcriptDock);

    // Create the control button panel
    m_controlsDock = new ControlsDockWidget(this);
    m_controlsDock->setFeatures(QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable);
    m_controlsDock->setAllowedAreas(Qt::LeftDockWidgetArea |
        Qt::RightDockWidgetArea);

    m_controlsDock->enableGraphDisplayOptions(true);

    addDockWidget(Qt::RightDockWidgetArea, m_controlsDock);

    // Initialisation finished
    postInfoMessage("Ready; paste network description into the text edit.");
}

MainWindow::~MainWindow()
{
}

// Properties
bool MainWindow::highlightStartNode() const
{
    return m_highlightStartNode;
}

void MainWindow::setHighlightStartNode(bool highlight)
{
    m_highlightStartNode = highlight;
}

bool MainWindow::highlightEndNode() const
{
    return m_highlightEndNode;
}

void MainWindow::setHighlightEndNode(bool highlight)
{
    m_highlightEndNode = highlight;
}

bool MainWindow::highlightPath() const
{
    return m_highlightPath;
}

void MainWindow::setHighlightPath(bool highlight)
{
    m_highlightPath = highlight;
}

// Logging functions
void MainWindow::postInfoMessage(const QString& msg)
{
    static const QString colour = QLatin1String("#3A87AD");
    static const QString leader = QLatin1String("Info");

    postMessage(colour, leader, msg);
}

void MainWindow::postSuccessMessage(const QString& msg)
{
    static const QString colour = QLatin1String("#468847");
    static const QString leader = QLatin1String("Success");

    postMessage(colour, leader, msg);
}

void MainWindow::postWarningMessage(const QString& msg)
{
    static const QString colour = QLatin1String("#C09853");
    static const QString leader = QLatin1String("Warning");

    postMessage(colour, leader, msg);
}

void MainWindow::postErrorMessage(const QString& msg)
{
    static const QString colour = QLatin1String("#B94A48");
    static const QString leader = QLatin1String("Error");

    postMessage(colour, leader, msg);
}

void MainWindow::postMessage(const QString& htmlColour, const QString& leader,
    const QString& msg)
{
    static const QString format = QLatin1String(
        "<strong><span style=\"color:%1\">%2:</span></strong> %3");

    m_transcript->append(format.arg(htmlColour).arg(leader).arg(msg));
}

// Graph layout stuff
void MainWindow::applyRandomLayout()
{
    RandomNumberGenerator rng(QDateTime::currentMSecsSinceEpoch());

    // Define the topology to work in
    QRectF rc(m_graphView->visibleRegion().boundingRect());
    rc.adjust(-10.0f, -10.0f, -10.0f, -10.0f);
    rc.moveTo(-rc.width() / 2.0f, -rc.height() / 2.0f);
    Topology top(rng, rc.left(), rc.top(), rc.right(), rc.bottom());

    // Apply the layout
    boost::random_graph_layout(m_graph,
        get(&NodeProperties::pos, m_graph), top);

    // Forward the calculated values into the graphics objects
    std::pair<DigraphVertexIterator, DigraphVertexIterator> iterators =
        boost::vertices(m_graph);
    DigraphVertexIterator it = iterators.first;
    for (; it != iterators.second; ++it) {
        TopologicalPoint pt   = m_graph[*it].pos;
        NodeItem*        node = m_graph[*it].item;

        node->setPos(pt[0], pt[1]);
    }
}

void MainWindow::applySpringLayout()
{
    ConstantValueDoubleMap weights(1.0f);
    RandomNumberGenerator rng(QDateTime::currentMSecsSinceEpoch());

    // Create an undirected copy of the graph
    QMap<GraphVertex, DigraphVertex> mapDtoU;
    QMap<DigraphVertex, GraphVertex> mapUtoD;
    Graph g;

    {
        // Copy the nodes
        std::pair<DigraphVertexIterator, DigraphVertexIterator> digraphVertices
            = boost::vertices(m_graph);
        DigraphVertexIterator vit = digraphVertices.first;
        for (; vit != digraphVertices.second; vit++) {
            // Add the vertex
            GraphVertex v = boost::add_vertex(m_graph[*vit], g);

            // Create the mapping
            mapDtoU[*vit] = v;
            mapUtoD[v]    = *vit;
        }

        // Copy the edges
        std::pair<Digraph::edge_iterator, Digraph::edge_iterator> digraphEdges
            = boost::edges(m_graph);
        Digraph::edge_iterator eit = digraphEdges.first;
        for (; eit != digraphEdges.second; eit++) {
            // Get the nodes in terms of the undirected graph
            DigraphVertex src = boost::source(*eit, m_graph);
            DigraphVertex tgt = boost::target(*eit, m_graph);

            // Add an edge into the undirected graph
            boost::add_edge(mapDtoU[src], mapDtoU[tgt], m_graph[*eit], g);
        }
    }

    // Define the topology to work in
    QRectF rc(m_graphView->visibleRegion().boundingRect());
    float adjH = rc.width()  * 0.1f;
    float adjV = rc.height() * 0.1f;
    rc.adjust(adjH, adjV, -adjH, -adjV);
    rc.moveTo(-rc.width() / 2.0f, -rc.height() / 2.0f);

    Topology top(rng, rc.left(), rc.top(), rc.right(), rc.bottom());

    // Seed with a random layout
    boost::random_graph_layout(g, get(&NodeProperties::pos, g), top);

    // Now apply the force-directed layout
    boost::kamada_kawai_spring_layout(g, 
        get(&NodeProperties::pos, g), weights, top,
        boost::side_length(rc.width()));

    // Forward the calculated values into the graphics objects
    std::pair<GraphVertexIterator, GraphVertexIterator> iterators =
        boost::vertices(g);
    GraphVertexIterator it = iterators.first;
    for (; it != iterators.second; ++it) {
        TopologicalPoint pt   = g[*it].pos;
        NodeItem*        node = g[*it].item;

        // Set the scene item
        node->setPos(pt[0], pt[1]);

        // Update the digraph
        m_graph[mapUtoD[*it]].pos = pt;
    }
}

// Network specification stuff
void MainWindow::parseAndRouteNetwork(const QString& description)
{
    // Split into lines
    QStringList lines = description.split(QRegExp("[\\n|\\r]"),
        QString::SkipEmptyParts);
    if (lines.isEmpty()) {
        postErrorMessage("Problem specification is empty after whitespace "
            "removed!");
        return;
    }

    // Validate the length of the specification
    int nodeCount = lines[0].toInt();
    postInfoMessage(QString("Expecting %1x%1 adjacency matrix...")
        .arg(nodeCount));

    if (lines.length() != (nodeCount + 2)) {
        postErrorMessage(QString("Expecting %1 lines in specification; read %2")
            .arg(nodeCount + 2)
            .arg(lines.length()));
        return;
    }

    // Clear the existing graph and scene
    if (boost::num_vertices(m_graph) != 0) {
        postWarningMessage("Existing network already loaded; must be cleared "
            "in order to continue.");

        int response = QMessageBox::question(this, "NetRoute", "There is "
            "already a graph in the explorer; the current data will have to "
            "be discared.  Continue?");
        if (response == QMessageBox::No) {
            postErrorMessage("Aborted by user.");
            return;
        }

        postInfoMessage("Discarding network.");
        clearNetwork();
    }

    // Create the nodes
    postInfoMessage("Creating nodes...");
    for (int i = 0; i < nodeCount; ++i) {
        QString name = QString("%1").arg(QChar('A' + i));

        NodeItem* node = new NodeItem;
        node->setText(name);
        node->setObjectName(name);
        
        boost::add_vertex(NodeProperties(node), m_graph);

        m_graphScene->addItem(node);
    }

    // Create the edges
    postInfoMessage("Creating edges from adjacency matrix...");
    for (int i = 0; i < nodeCount; ++i) {
        QString     line    = lines[i + 1].trimmed();
        QStringList weights = line.split(',', QString::SkipEmptyParts);

        // Sanity check
        if (weights.length() != nodeCount) {
            postErrorMessage(
                QString("Matrix row %1 has %2 columns; expecting %3.")
                    .arg(i)
                    .arg(weights.length())
                    .arg(nodeCount));
            return;
        }

        // Actually create the edges
        postInfoMessage(QString("Creating edges for node %1")
            .arg(QChar('A' + i)));
        DigraphVertex vStart = boost::vertex(i, m_graph);
        for (int j = 0; j < nodeCount; ++j) {
            bool ok;
            int weight = weights[j].trimmed().toInt(&ok);

            if (ok && weight >= 0) {
                DigraphVertex vEnd = boost::vertex(j, m_graph);

                // Create the new edge item
                EdgeItem* edge = new EdgeItem;
                edge->setStartNode(m_graph[vStart].item);
                edge->setEndNode(m_graph[vEnd].item);
                edge->setWeight(weight);
                m_graphScene->addItem(edge);

                // Add it to the graph
                boost::add_edge(vStart, vEnd, EdgeProperties(edge), m_graph);
            } else if (!ok) {
                postWarningMessage(QString("Weight (%1,%2) is malformed: %3.")
                    .arg(i)
                    .arg(j)
                    .arg(weights[j]));
            }
        }
    }

    // Load complete, lay it out
    postInfoMessage("Laying out graph...");
    applySpringLayout();
    postSuccessMessage("Graph loaded successfully!");

    // Extract the start and end points
    postInfoMessage("Beginning routing step...");
    QStringList nodes = lines[lines.length() - 1].split(QRegExp("\\s+"),
        QString::SkipEmptyParts);
    if (nodes.length() != 2) {
        postErrorMessage("Start and end nodes line is malformed.");
        return;
    }

    QString startNodeName = nodes[0];
    QString endNodeName   = nodes[1];

    // Complete
    postSuccessMessage(QString("Load complete! "
        "Routing not performed between nodes %1 and %2.")
            .arg(startNodeName)
            .arg(endNodeName));
}

void MainWindow::clearNetwork()
{
    m_graph.clear();
    m_graphScene->clear();
}

void MainWindow::generateRouteReport()
{
}

