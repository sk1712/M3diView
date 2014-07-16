#include <QtViewerWidget.h>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>


QtViewerWidget::QtViewerWidget(QWidget *parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout();

    toolWidget = new QWidget();
    createToolButtons();
    connectSignals();

    QHBoxLayout *toolLayout = new QHBoxLayout();
    toolLayout->addWidget(expandToolButton);
    toolLayout->addWidget(linkToolButton);
    toolLayout->addWidget(deleteToolButton);
    toolLayout->addWidget(settingsToolButton);

    toolWidget->setLayout(toolLayout);
    layout->addWidget(toolWidget, 1, 0, 1, 2, Qt::AlignLeft);

    setLayout(layout);

    // By default link viewer to the rest of the viewers
    linked = true;
    expanded = false;
}

void QtViewerWidget::setInvertedAxes(bool *) {

}

void QtViewerWidget::paintEvent(QPaintEvent *) {
    toolWidget->raise();
}

void QtViewerWidget::createToolButtons() {
    expandToolButton = new QToolButton();
    expandToolButton->setIcon(QIcon(":/icons/expand.png"));
    expandToolButton->setToolTip("Expand window");

    linkToolButton = new QToolButton();
    linkToolButton->setIcon(QIcon(":/icons/link.png"));
    linkToolButton->setToolTip("Link to other viewers");
    linkToolButton->setCheckable(true);
    linkToolButton->setChecked(true);

    deleteToolButton = new QToolButton();
    deleteToolButton->setIcon(QIcon(":/icons/delete.png"));
    deleteToolButton->setToolTip("Delete view");

    settingsToolButton = new QToolButton();
    settingsToolButton->setIcon(QIcon(":/icons/settings.png"));
    settingsToolButton->setToolTip("Options");

    settingsMenu = new QMenu();
    settingsToolButton->setMenu(settingsMenu);
    settingsToolButton->setPopupMode(QToolButton::InstantPopup);

    saveScreenshotAction = new QAction(tr("Save image as..."), this);
    saveScreenshotAction->setIcon(QIcon(":/icons/save.png"));
    settingsMenu->addAction(saveScreenshotAction);
}

void QtViewerWidget::connectSignals() {
    connect(expandToolButton, SIGNAL(clicked()), this, SLOT(expandWindow()));
    connect(linkToolButton, SIGNAL(toggled(bool)), this, SLOT(changeLinked(bool)));
    connect(deleteToolButton, SIGNAL(clicked()), this, SLOT(deleteWindow()));

    connect(saveScreenshotAction, SIGNAL(triggered()), this, SLOT(saveScreenshot()));
}

void QtViewerWidget::expandWindow() {
    expanded = !expanded;

    if (expanded) {
        expandToolButton->setIcon(QIcon(":/icons/collapse.png"));
        expandToolButton->setToolTip("Collapse window");
    }
    else {
        expandToolButton->setIcon(QIcon(":/icons/expand.png"));
        expandToolButton->setToolTip("Expand window");
    }

    emit windowExpanded();
}

void QtViewerWidget::deleteWindow() {
    emit windowDeleted();
}

void QtViewerWidget::changeLinked(bool checked) {
    linked = checked;
}

void QtViewerWidget::saveScreenshot() {
    QString fileName = QFileDialog::getSaveFileName(
                this,tr("Save screenshot as"),
                "", tr("Images (*.png *.xpm *.jpg)"));
    if (getGlWidget()->saveScreenshotInFile(fileName)) {
        QMessageBox message;
        message.setText("Image successfully saved in " + fileName);
        message.exec();
    }
    else {
        qCritical("Could not save screenshot in %s", qPrintable(fileName));
    }
}
