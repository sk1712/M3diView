#include <QtViewerWidget.h>

#include <QGridLayout>
#include <QHBoxLayout>


QtViewerWidget::QtViewerWidget(QWidget *parent) : QWidget(parent) {
    linked = true;

    QGridLayout *layout = new QGridLayout();

    QWidget *toolWidget = new QWidget();
    createToolButtons();
    connectSignals();

    QHBoxLayout *toolLayout = new QHBoxLayout();
    toolLayout->addWidget(expandToolButton);
    toolLayout->addWidget(linkToolButton);
    toolLayout->addWidget(deleteToolButton);

    toolWidget->setLayout(toolLayout);
    layout->addWidget(toolWidget, 1, 0, Qt::AlignLeft);

    setLayout(layout);
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
