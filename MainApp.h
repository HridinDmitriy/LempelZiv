#pragma once
#include <QWidget>
#include <QStackedLayout>
#include "Compressor.h"
#include "Decompressor.h"
#include "DataPreparationWidget.h"
#include "ResultWidget.h"

class MainApp : public QWidget
{
    Q_OBJECT

public:
    MainApp(QWidget *parent = nullptr);

public slots:
    void startCompressionSlot(const QString& fileName);
    void startDecompressionSlot(const QString& fileName);
    void setDataPreparationWidgetActiveSlot();
    void setResultWidgetActiveSlot();
    void showMessage(const QString& text);

private:
    Compressor              compressor;
    Decompressor            decompressor;

    QStackedLayout*         stackLayout;
    DataPreparationWidget*  dataPreparationWidget;
    ResultWidget*           resultWidget;

signals:
    void closeAppSignal();
};
