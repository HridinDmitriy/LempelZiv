#include "DataPreparationWidget.h"
#include <QBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

DataPreparationWidget::DataPreparationWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *mainHBoxLayout = new QHBoxLayout();
    QVBoxLayout *mainVBoxLayout = new QVBoxLayout();

    QHBoxLayout *selectFileButtonWrapper = new QHBoxLayout();
    QHBoxLayout *fileNameLabelWrapper = new QHBoxLayout();

    QFont font;
    font.setPointSize(20);
    font.setFamily(QString("Verdana"));

    selectFileButton = new QPushButton(QIcon(QPixmap(":/resources/nonOpenedFile.png")), "");
    selectFileButton->setIconSize(QSize(90, 100));
    selectFileButton->setFixedSize(100, 100);

    fileNameLabel = new QLabel(labelDefaultText);
    fileNameLabel->setWordWrap(true);

    QPushButton *compressionButton = new QPushButton(QString("Compression"));
    compressionButton->setFixedSize(500, 70);
    compressionButton->setFont(font);

    QPushButton *decompressionButton = new QPushButton(QString("Decompression"));
    decompressionButton->setFixedSize(500, 70);
    decompressionButton->setFont(font);

    // set layouts
    selectFileButtonWrapper->addStretch(1);
    selectFileButtonWrapper->addWidget(selectFileButton);
    selectFileButtonWrapper->addStretch(1);

    fileNameLabelWrapper->addStretch(1);
    fileNameLabelWrapper->addWidget(fileNameLabel);
    fileNameLabelWrapper->addStretch(1);

    mainVBoxLayout->addStretch(1);
    mainVBoxLayout->addLayout(selectFileButtonWrapper);
    mainVBoxLayout->addLayout(fileNameLabelWrapper);
    mainVBoxLayout->addSpacing(40);
    mainVBoxLayout->addWidget(compressionButton);
    mainVBoxLayout->addWidget(decompressionButton);
    mainVBoxLayout->addStretch(1);

    mainHBoxLayout->addStretch(1);
    mainHBoxLayout->addLayout(mainVBoxLayout);
    mainHBoxLayout->addStretch(1);

    setLayout(mainHBoxLayout);

    // set connections
    connect(compressionButton, &QPushButton::clicked, this, &DataPreparationWidget::compressButtonPressedSlot);
    connect(decompressionButton, &QPushButton::clicked, this, &DataPreparationWidget::decompressButtonPressedSlot);
    connect(selectFileButton, &QPushButton::clicked, this, &DataPreparationWidget::selectFileSlot);
}

void DataPreparationWidget::selectFileSlot()
{
    QString path = QFileDialog::getOpenFileName();

    if (!path.isEmpty())
    {
        if (fileNameLabel->text() == labelDefaultText)
        {
            selectFileButton->setIcon(QIcon(QPixmap(":/resources/openedFile.png")));
        }

        filePath = path;

        QFileInfo fileInfo(filePath);
        QString fileName = fileInfo.fileName();
        fileNameLabel->setText(fileName);
    }
}

void DataPreparationWidget::compressButtonPressedSlot()
{
    if (isFilePathValid())
    {
        emit compressionSelectedSignal(filePath);
    }
}

void DataPreparationWidget::decompressButtonPressedSlot()
{
    if (isFilePathValid())
    {
        emit decompressionSelectedSignal(filePath);
    }
}


bool DataPreparationWidget::isFilePathValid()
{
    if (filePath.isEmpty())
    {
        QMessageBox::information(nullptr, "Warning", "\nNo file selected!\t\t\n");
        return false;
    }
    return true;
}
