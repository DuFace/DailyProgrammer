#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTextEdit>

#include "controlsdockwidget.h"
#include "ui_mainwindow.h"

#include "graph.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

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
    void postMessage(const QString& htmlColour, const QString& leader,
        const QString& msg);

private:
    Ui::MainWindowClass m_ui;

    QGraphicsScene*     m_graphScene;
    QGraphicsView*      m_graphView;

    ControlsDockWidget* m_controlsDock;
    QTextEdit*          m_transcript;

    Digraph             m_graph;

    bool                m_highlightStartNode;
    bool                m_highlightEndNode;
    bool                m_highlightPath;
};

#endif // MAINWINDOW_H
