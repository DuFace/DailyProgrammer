/*
 * Copyright © 2014 Kier Dugan.  All Rights Reserved.
 *
 * This source code is covered by the MIT licence as set out in the LICENSE
 * file at the root of this repository.
 */

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

    bool areGraphLayoutOptionsEnabled() const;
    void enableGraphLayoutOptions(bool enable);

    bool isClearNetworkEnabled() const;
    void enableClearNetwork(bool enable);

    bool isGenerateReportEnabled() const;
    void enableGenerateReport(bool enable);

    bool highlightStartNode() const;
    bool highlightEndNode() const;
    bool highlightPath() const;

    int  maxIterations() const;
    double layoutTolerance() const;

protected slots:
    void problemSpecChanged();
    void pasteAndRouteClicked();
    void routeClicked();
    void clipboardDataChanged();

signals:
    void parseRequested(const QString& description);

private:
    Ui::ControlsDockWidget  m_ui;

    bool                    m_graphDisplayEnabled;
    bool                    m_graphLayoutEnabled;
};


#endif  /* !CONTROLSDOCKWIDGET_H */
