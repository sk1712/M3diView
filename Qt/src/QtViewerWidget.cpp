#include <QtViewerWidget.h>

#include <QGridLayout>
#include <QHBoxLayout>


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
}

void QtViewerWidget::setInvertedAxes(bool *) {

}

void QtViewerWidget::paintEvent(QPaintEvent *) {
    toolWidget->raise();
}

void QtViewerWidget::createToolButtons() {
    expandToolButton = new QToolButton();
    expandToolButton->setIcon(QIcon(":/icons/expand_window.png"));
    expandToolButton->setToolTip("Expand/collapse window");

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
    settingsToolButton->setToolTip("Settings");
}

void QtViewerWidget::connectSignals() {
    connect(expandToolButton, SIGNAL(clicked()), this, SLOT(expandWindow()));
    connect(linkToolButton, SIGNAL(toggled(bool)), this, SLOT(changeLinked(bool)));
    connect(deleteToolButton, SIGNAL(clicked()), this, SLOT(deleteWindow()));
}

void QtViewerWidget::expandWindow() {
    emit windowExpanded();
}

void QtViewerWidget::deleteWindow() {
    emit windowDeleted();
}

void QtViewerWidget::changeLinked(bool checked) {
    linked = checked;
}
