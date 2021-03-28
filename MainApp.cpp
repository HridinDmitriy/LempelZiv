#include "MainApp.h"

MainApp::MainApp(QWidget *parent)
    : QWidget(parent)
{
    stackLayout = new QStackedLayout();

    dataPreparationWidget = new DataPreparationWidget();
    stackLayout->addWidget(dataPreparationWidget);

    resultWidget = new ResultWidget();
    stackLayout->addWidget(resultWidget);

    stackLayout->setCurrentIndex(0);

    connect(dataPreparationWidget, &DataPreparationWidget::compressionSelectedSignal,
            this, &MainApp::startCompressionSlot);

    connect(dataPreparationWidget, &DataPreparationWidget::decompressionSelectedSignal,
            this, &MainApp::startDecompressionSlot);

    connect(resultWidget, &ResultWidget::backButtonPressedSignal, this, &MainApp::setDataPreparationWidgetActiveSlot);

    connect(resultWidget, &ResultWidget::closeButtonPressedSignal, this, &MainApp::closeAppSignal);

    connect(&compressor, &Compressor::errorOccuredSignal, resultWidget, &ResultWidget::showErrorMessage);
    connect(&compressor, &Compressor::inputFileSizeSignal, resultWidget, &ResultWidget::setBeforeProcessingFileSize);
    connect(&compressor, &Compressor::outputFileSizeSignal, resultWidget, &ResultWidget::setAfterProcessingFileSize);
    connect(&compressor, &Compressor::efficiencySignal, resultWidget, &ResultWidget::setEfficiency);

    connect(&decompressor, &Decompressor::errorOccuredSignal, resultWidget, &ResultWidget::showErrorMessage);
    connect(&decompressor, &Decompressor::inputFileSizeSignal, resultWidget, &ResultWidget::setBeforeProcessingFileSize);
    connect(&decompressor, &Decompressor::outputFileSizeSignal, resultWidget, &ResultWidget::setAfterProcessingFileSize);
    connect(&decompressor, &Decompressor::efficiencySignal, resultWidget, &ResultWidget::setEfficiency);

    setLayout(stackLayout);
}

void MainApp::startCompressionSlot(const QString& fileName)
{
    setResultWidgetActiveSlot();
    resultWidget->setBeforeProcessingLabel("Uncompressed file size:");
    resultWidget->setAfterProcessingLabel("Compressed file size:");

    compressor.compress(fileName);
}

void MainApp::startDecompressionSlot(const QString& fileName)
{
    setResultWidgetActiveSlot();
    resultWidget->setBeforeProcessingLabel("Compressed file size:");
    resultWidget->setAfterProcessingLabel("Uncompressed file size:");

    decompressor.decompress(fileName);
}

void MainApp::setDataPreparationWidgetActiveSlot()
{
    stackLayout->setCurrentWidget(dataPreparationWidget);
}

void MainApp::setResultWidgetActiveSlot()
{
    stackLayout->setCurrentWidget(resultWidget);
}




