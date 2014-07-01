#include <QtCore/QMimeData>
#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>

#include "controlsdockwidget.h"
#include "mainwindow.h"

ControlsDockWidget::ControlsDockWidget(MainWindow* parent) :
    QDockWidget(parent),
    m_graphDisplayEnabled(false),
    m_graphLayoutEnabled(false)
{
    m_ui.setupUi(this);

    // Map properties
    connect(m_ui.highlightStartCheck, &QCheckBox::stateChanged,
        [parent] (int state) {
            parent->setHighlightStartNode(state == Qt::Checked);
        });
    connect(m_ui.highlightEndCheck, &QCheckBox::stateChanged,
        [parent] (int state) {
            parent->setHighlightEndNode(state == Qt::Checked);
        });
    connect(m_ui.highlightPathCheck, &QCheckBox::stateChanged,
        [parent] (int state) {
            parent->setHighlightPath(state == Qt::Checked);
        });

    // Map commands
    connect(m_ui.randomLayoutButton, &QPushButton::clicked, parent,
        &MainWindow::applyRandomLayout);
    connect(m_ui.springLayoutButton, &QPushButton::clicked, parent,
        &MainWindow::applySpringLayout);
    connect(m_ui.pasteAndRouteButton, &QPushButton::clicked, this,
        &ControlsDockWidget::pasteAndRouteClicked);
    connect(m_ui.routeButton, &QPushButton::clicked, this,
        &ControlsDockWidget::routeClicked);
    connect(m_ui.clearSpecButton, &QPushButton::clicked, [this] () {
        m_ui.problemSpecEdit->clear();
    });
    connect(m_ui.clearNetworkButton, &QPushButton::clicked, parent,
        &MainWindow::clearNetwork);
    connect(m_ui.generateReportButton, &QPushButton::clicked, parent,
        &MainWindow::generateRouteReport);
    connect(m_ui.exitButton, &QPushButton::clicked, qApp, &QApplication::quit);

    // Monitor the clipboard so we can only enable paste when there's text data
    connect(QApplication::clipboard(), &QClipboard::dataChanged, this,
        &ControlsDockWidget::clipboardDataChanged);
    connect(m_ui.problemSpecEdit, &QPlainTextEdit::textChanged, this,
        &ControlsDockWidget::problemSpecChanged);
    connect(this, &ControlsDockWidget::parseRequested, parent,
        &MainWindow::parseAndRouteNetwork);

    // Finally, check the clipboard for data
    clipboardDataChanged();
}

ControlsDockWidget::~ControlsDockWidget()
{
}

// Properties
bool ControlsDockWidget::areGraphDisplayOptionsEnabled() const
{
    return m_graphDisplayEnabled;
}

void ControlsDockWidget::enableGraphDisplayOptions(bool enable)
{
    m_graphDisplayEnabled = enable;
    m_ui.highlightEndCheck->setEnabled(enable);
    m_ui.highlightPathCheck->setEnabled(enable);
    m_ui.highlightStartCheck->setEnabled(enable);
}

bool ControlsDockWidget::areGraphLayoutOptionsEnabled() const
{
    return m_graphLayoutEnabled;
}

void ControlsDockWidget::enableGraphLayoutOptions(bool enable)
{
    m_graphDisplayEnabled = enable;
    m_ui.randomLayoutButton->setEnabled(enable);
    m_ui.springLayoutButton->setEnabled(enable);
}

bool ControlsDockWidget::isClearNetworkEnabled() const
{
    return m_ui.clearNetworkButton->isEnabled();
}

void ControlsDockWidget::enableClearNetwork(bool enable)
{
    m_ui.clearNetworkButton->setEnabled(enable);
}

bool ControlsDockWidget::isGenerateReportEnabled() const
{
    return m_ui.generateReportButton->isEnabled();
}

void ControlsDockWidget::enableGenerateReport(bool enable)
{
    m_ui.generateReportButton->setEnabled(enable);
}

bool ControlsDockWidget::highlightStartNode() const
{
    return m_ui.highlightStartCheck->isChecked();
}

bool ControlsDockWidget::highlightEndNode() const
{
    return m_ui.highlightEndCheck->isChecked();
}

bool ControlsDockWidget::highlightPath() const
{
    return m_ui.highlightPathCheck->isChecked();
}

int ControlsDockWidget::maxIterations() const
{
    return m_ui.maxIterationsSpinner->value();
}

double ControlsDockWidget::layoutTolerance() const
{
    return m_ui.layoutToleranceSpinner->value();
}

// Event handlers
void ControlsDockWidget::problemSpecChanged()
{
    bool enable = !m_ui.problemSpecEdit->document()->isEmpty();

    m_ui.routeButton->setEnabled(enable);
    m_ui.clearSpecButton->setEnabled(enable);
}

void ControlsDockWidget::pasteAndRouteClicked()
{
    QClipboard* clipboard = QApplication::clipboard();
    const QMimeData* mimeData = clipboard->mimeData();
    
    if (mimeData->hasText()) {
        QString description = mimeData->text().trimmed();

        // Make sure there is data remaining after the leading/trailing
        // whitespace has been removed
        if (!description.isEmpty()) {
            QTextDocument* doc = m_ui.problemSpecEdit->document();

            // Replace all the text of the description box
            doc->clear();
            doc->setPlainText(description);

            // Now issue the signal
            emit parseRequested(description);
        }
    }
}

void ControlsDockWidget::routeClicked()
{
    QString description = m_ui.problemSpecEdit->toPlainText().trimmed();
    if (!description.isEmpty()) {
        emit parseRequested(description);
    }
}

void ControlsDockWidget::clipboardDataChanged()
{
    QClipboard* clipboard = QApplication::clipboard();

    // Only enable the paste button when there's text data in the clipboard
    const QMimeData* mimeData = clipboard->mimeData();
    m_ui.pasteAndRouteButton->setEnabled(mimeData->hasText());
}

