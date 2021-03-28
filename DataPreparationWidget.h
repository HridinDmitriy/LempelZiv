#pragma once
#include <QWidget>
#include <QPushButton>
#include <QLabel>

class DataPreparationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataPreparationWidget(QWidget *parent = nullptr);

public slots:
    void selectFileSlot();
    void compressButtonPressedSlot();
    void decompressButtonPressedSlot();

private:
    bool isFilePathValid();

private:
    const QString   labelDefaultText{"Select file..."};
    QPushButton*    selectFileButton;
    QLabel*         fileNameLabel;

    QString         filePath;

signals:
    void compressionSelectedSignal(const QString& fileName);
    void decompressionSelectedSignal(const QString& fileName);
};
