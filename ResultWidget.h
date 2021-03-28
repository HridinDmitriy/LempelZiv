#pragma once

#include <QWidget>
#include <QLabel>

class ResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWidget(QWidget *parent = nullptr);

public slots:
    void setBeforeProcessingLabel(const QString& text);
    void setAfterProcessingLabel(const QString& text);
    void setBeforeProcessingFileSize(const QString& text);
    void setAfterProcessingFileSize(const QString& text);
    void setEfficiency(const QString& text);

private:
    QLabel* beforeProcessingLabel;
    QLabel* afterProcessingLabel;

    QLabel* beforeProcessingFileSize;
    QLabel* afterProcessingFileSize;

    QLabel* efficiencyValue;

signals:
    void backButtonPressedSignal();
    void closeButtonPressedSignal();
};

