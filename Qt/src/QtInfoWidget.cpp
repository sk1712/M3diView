#include <QtInfoWidget.h>

#include <QHeaderView>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextStream>


QtInfoWidget::QtInfoWidget(QWidget *parent) : QWidget(parent) {
    _image = NULL;

    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    QFormLayout *formLayout = new QFormLayout;
    formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    imageSizeEdit = new QLineEdit;
    imageSizeEdit->setReadOnly(true);
    imageSizeEdit->setFont(font);
    imageSizeEdit->setAlignment(Qt::AlignRight);
    formLayout->addRow(tr("Image size :"), imageSizeEdit);

    voxelSizeEdit = new QLineEdit;
    voxelSizeEdit->setReadOnly(true);
    voxelSizeEdit->setFont(font);
    voxelSizeEdit->setAlignment(Qt::AlignRight);
    formLayout->addRow(tr("Voxel size :"), voxelSizeEdit);

    imageOriginEdit = new QLineEdit;
    imageOriginEdit->setReadOnly(true);
    imageOriginEdit->setFont(font);
    imageOriginEdit->setAlignment(Qt::AlignRight);
    formLayout->addRow(tr("Image origin :"), imageOriginEdit);

    xAxisEdit = new QLineEdit;
    xAxisEdit->setReadOnly(true);
    xAxisEdit->setFont(font);
    xAxisEdit->setAlignment(Qt::AlignRight);
    formLayout->addRow(tr("X-axis :"), xAxisEdit);

    yAxisEdit = new QLineEdit;
    yAxisEdit->setReadOnly(true);
    yAxisEdit->setFont(font);
    yAxisEdit->setAlignment(Qt::AlignRight);
    formLayout->addRow(tr("Y-axis :"), yAxisEdit);

    zAxisEdit = new QLineEdit;
    zAxisEdit->setReadOnly(true);
    zAxisEdit->setFont(font);
    zAxisEdit->setAlignment(Qt::AlignRight);
    formLayout->addRow(tr("Z-axis :"), zAxisEdit);

    QWidget *basicInfoWidget = new QWidget;
    basicInfoWidget->setLayout(formLayout);
    verticalLayout->addWidget(basicInfoWidget);

    QLabel *imagetoWorldLabel = new QLabel("Image to world matrix");
    verticalLayout->addWidget(imagetoWorldLabel);
    imageToWorldMatrix = new QTableWidget;
    imageToWorldMatrix->setFont(font);
    imageToWorldMatrix->verticalHeader()->hide();
    imageToWorldMatrix->horizontalHeader()->hide();
    verticalLayout->addWidget(imageToWorldMatrix);

    QLabel *worldToImageLabel = new QLabel("World to image matrix");
    verticalLayout->addWidget(worldToImageLabel);
    worldToImageMatrix = new QTableWidget;
    worldToImageMatrix->setFont(font);
    worldToImageMatrix->verticalHeader()->hide();
    worldToImageMatrix->horizontalHeader()->hide();
    verticalLayout->addWidget(worldToImageMatrix);

    QSpacerItem* spacer = new QSpacerItem( 20, 20, QSizePolicy::Minimum,
    QSizePolicy::Expanding );
    verticalLayout->addItem( spacer );

    setLayout(verticalLayout);
}

QSize QtInfoWidget::sizeHint() const {
    return QSize(350, 500);
}

void QtInfoWidget::setImage(irtkImage *image) {
    _image = image;
}

void QtInfoWidget::update() {
    if (_image) {
        updateImageInfo();
        updateImageToWorldMatrix();
        updateWorldToImageMatrix();
    }
    else {
        imageSizeEdit->setText("");
        voxelSizeEdit->setText("");
        imageOriginEdit->setText("");

        xAxisEdit->setText("");
        yAxisEdit->setText("");
        zAxisEdit->setText("");

        imageToWorldMatrix->clear();
        worldToImageMatrix->clear();
    }
}

void QtInfoWidget::updateImageInfo() {
    QString imageSize, voxelSize, imageOrigin;
    QString xAxis, yAxis, zAxis;
    QTextStream textStream;

    irtkImageAttributes attr = _image->GetImageAttributes();

    // Print image dimensions
    textStream.setString(&imageSize);
    textStream.setIntegerBase(10);
    textStream.setFieldAlignment(QTextStream::AlignRight);
    textStream.setFieldWidth(4);
    textStream << attr._x << " " << attr._y << " " << attr._z;// << " " << attr._t;
    imageSizeEdit->setText(imageSize);

    // Print voxel dimensions
    textStream.setString(&voxelSize);
    textStream.setIntegerBase(10);
    textStream.setRealNumberNotation(QTextStream::FixedNotation);
    textStream.setRealNumberPrecision(4);
    textStream.setFieldAlignment(QTextStream::AlignRight);
    textStream << attr._dx << " " << attr._dy << " " << attr._dz;// << " " << attr._dt;
    voxelSizeEdit->setText(voxelSize);

    // Print origin
    textStream.setString(&imageOrigin);
    textStream.setIntegerBase(10);
    textStream.setRealNumberNotation(QTextStream::FixedNotation);
    textStream.setRealNumberPrecision(4);
    textStream.setFieldAlignment(QTextStream::AlignRight);
    textStream << attr._xorigin << " " << attr._yorigin << " "
               << attr._zorigin;// << " " << attr._torigin;
    imageOriginEdit->setText(imageOrigin);

    // Print x-axis
    textStream.setString(&xAxis);
    textStream.setIntegerBase(10);
    textStream.setRealNumberNotation(QTextStream::FixedNotation);
    textStream.setRealNumberPrecision(2);
    textStream.setFieldAlignment(QTextStream::AlignRight);
    textStream << attr._xaxis[0] << " " << attr._xaxis[1] << " " << attr._xaxis[2];
    xAxisEdit->setText(xAxis);

    // Print y-axis
    textStream.setString(&yAxis);
    textStream.setIntegerBase(10);
    textStream.setRealNumberNotation(QTextStream::FixedNotation);
    textStream.setRealNumberPrecision(2);
    textStream.setFieldAlignment(QTextStream::AlignRight);
    textStream << attr._yaxis[0] << " " << attr._yaxis[1] << " " << attr._yaxis[2];
    yAxisEdit->setText(yAxis);

    // Print z-axis
    textStream.setString(&zAxis);
    textStream.setIntegerBase(10);
    textStream.setRealNumberNotation(QTextStream::FixedNotation);
    textStream.setRealNumberPrecision(2);
    textStream.setFieldAlignment(QTextStream::AlignRight);
    textStream << attr._zaxis[0] << " " << attr._zaxis[1] << " " << attr._zaxis[2];
    zAxisEdit->setText(zAxis);
}

void QtInfoWidget::updateImageToWorldMatrix() {
    irtkMatrix matrix = _image->GetImageToWorldMatrix();

    imageToWorldMatrix->setRowCount(matrix.Rows());
    imageToWorldMatrix->setColumnCount(matrix.Cols());

    for (int i = 0; i < matrix.Rows(); i++) {
        for (int j = 0; j < matrix.Cols(); j++) {
            QTableWidgetItem *newItem =
                    new QTableWidgetItem(tr("%1").arg(
                                             QString::number(matrix.Get(i, j), 'f', 4)));
            newItem->setTextAlignment(Qt::AlignHCenter);
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
            imageToWorldMatrix->setItem(i, j, newItem);
        }
    }

    imageToWorldMatrix->resizeColumnsToContents();
    imageToWorldMatrix->resizeRowsToContents();
}

void QtInfoWidget::updateWorldToImageMatrix() {
    irtkMatrix matrix = _image->GetWorldToImageMatrix();

    worldToImageMatrix->setRowCount(matrix.Rows());
    worldToImageMatrix->setColumnCount(matrix.Cols());

    for (int i = 0; i < matrix.Rows(); i++) {
        for (int j = 0; j < matrix.Cols(); j++) {
            QTableWidgetItem *newItem =
                    new QTableWidgetItem(tr("%1").arg(
                                             QString::number(matrix.Get(i, j), 'f', 4)));
            newItem->setTextAlignment(Qt::AlignHCenter);
            newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
            worldToImageMatrix->setItem(i, j, newItem);
        }
    }

    worldToImageMatrix->resizeColumnsToContents();
    worldToImageMatrix->resizeRowsToContents();
}
