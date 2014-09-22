#include <QtViewerWidget.h>

#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QToolButton>

QtViewerWidget::QtViewerWidget(QWidget *parent) : QWidget(parent) {
    createToolButtons();
    connectSignals();

    QGridLayout *layout = new QGridLayout();
    layout->addWidget(toolWidget, 1, 0, 1, 2, Qt::AlignLeft);
    layout->setContentsMargins(5, 0, 5, 0);
    setLayout(layout);

    // By default link viewer to the rest of the viewers
    linked = true;
    expanded = false;
}

void QtViewerWidget::setLinked(bool link) {
    linkToolButton->setChecked(link);
}

void QtViewerWidget::setInvertedAxes(bool *) {

}

bool QtViewerWidget::imageOriginVisible() const {
    return showImageOriginAction->isChecked();
}

void QtViewerWidget::setImageOriginVisible(const bool visible) {
    showImageOriginAction->setChecked(visible);
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

void QtViewerWidget::paintEvent(QPaintEvent *) {
    // Tool widget should not be hidden by other widgets
    toolWidget->raise();
}

void QtViewerWidget::createToolButtons() {
    toolWidget = new QWidget();
    QHBoxLayout *toolLayout = new QHBoxLayout();
    toolWidget->setLayout(toolLayout);

    expandToolButton = new QToolButton();
    expandToolButton->setIcon(QIcon(":/icons/expand.png"));
    expandToolButton->setToolTip("Expand window");
    toolLayout->addWidget(expandToolButton);

    linkToolButton = new QToolButton();
    linkToolButton->setIcon(QIcon(":/icons/link.png"));
    linkToolButton->setToolTip("Link to other viewers");
    linkToolButton->setCheckable(true);
    linkToolButton->setChecked(true);
    toolLayout->addWidget(linkToolButton);

    deleteToolButton = new QToolButton();
    deleteToolButton->setIcon(QIcon(":/icons/delete.png"));
    deleteToolButton->setToolTip("Delete view");
    toolLayout->addWidget(deleteToolButton);

    settingsToolButton = new QToolButton();
    settingsToolButton->setIcon(QIcon(":/icons/settings.png"));
    settingsToolButton->setToolTip("Options");
    toolLayout->addWidget(settingsToolButton);

    settingsMenu = new QMenu();
    settingsToolButton->setMenu(settingsMenu);
    settingsToolButton->setPopupMode(QToolButton::InstantPopup);

    saveScreenshotAction = new QAction(tr("Save Image As..."), this);
    saveScreenshotAction->setIcon(QIcon(":/icons/save_as.png"));
    settingsMenu->addAction(saveScreenshotAction);

    showImageOriginAction = new QAction(tr("Show Image Origin"), this);
    showImageOriginAction->setCheckable(true);
    showImageOriginAction->setChecked(true);
    settingsMenu->addAction(showImageOriginAction);
}

void QtViewerWidget::connectSignals() {
    connect(expandToolButton, SIGNAL(clicked()), this, SLOT(expandWindow()));
    connect(linkToolButton, SIGNAL(toggled(bool)), this, SLOT(changeLinked(bool)));
    connect(deleteToolButton, SIGNAL(clicked()), this, SLOT(deleteWindow()));

    connect(saveScreenshotAction, SIGNAL(triggered()), this, SLOT(saveScreenshot()));
    connect(showImageOriginAction, SIGNAL(toggled(bool)), this, SLOT(toggleImageOriginVisible(bool)));
}

void QtViewerWidget::changeLinked(bool checked) {
    linked = checked;
}

void QtViewerWidget::deleteWindow() {
    emit windowDeleted();
}

void QtViewerWidget::saveScreenshot() {
    QString fileName = QFileDialog::getSaveFileName(
                this,tr("Save screenshot as"),
                "", tr("Images (*.png *.xpm *.jpg)"));
    if (getGlWidget()->saveScreenshotInFile(fileName)) {
        QMessageBox message;
        message.setText("Image successfully saved in " + fileName);
        message.setIcon(QMessageBox::Information);
        message.exec();
    }
    else {
        qCritical("Could not save screenshot in %s", qPrintable(fileName));
    }
}

void QtViewerWidget::toggleImageOriginVisible(bool checked) {
    getGlWidget()->setImageOriginVisible(checked);
}
