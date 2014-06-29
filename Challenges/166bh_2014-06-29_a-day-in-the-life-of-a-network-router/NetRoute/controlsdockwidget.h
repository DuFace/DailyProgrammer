#ifndef CONTROLSDOCKWIDGET_H
#define CONTROLSDOCKWIDGET_H

#include <QtWidgets/QDockWidget>

#include "ui_controlsdockwidget.h"

class MainWindow;


class ControlsDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    ControlsDockWidget(MainWindow* parent);
    virtual ~ControlsDockWidget();

    // Properties
    bool areGraphDisplayOptionsEnabled() const;
    void enableGraphDisplayOptions(bool enable);

    bool isClearNetworkEnabled() const;
    void enableClearNetwork(bool enable);

    bool isGenerateReportEnabled() const;
    void enableGenerateReport(bool enable);

protected slots:
    void problemSpecChanged();
    void pasteAndRouteClicked();
    void routeClicked();
    void clipboardDataChanged();

signals:
    void parseRequested(const QString& description);

private:
    Ui::ControlsDockWidget m_ui;
};


#endif  /* !CONTROLSDOCKWIDGET_H */
