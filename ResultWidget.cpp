#include "ResultWidget.h"
#include <QBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>

ResultWidget::ResultWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QHBoxLayout* footerLayout = new QHBoxLayout();

    QGridLayout* grid = new QGridLayout();
    grid->setAlignment(Qt::AlignmentFlag::AlignCenter);
    grid->setHorizontalSpacing(100);

    QFont font;
    font.setPointSize(14);
    font.setFamily(QString("Verdana"));

    QFont boldFont;
    boldFont.setPointSize(14);
    boldFont.setFamily(QString("Verdana"));
    boldFont.setBold(true);

    beforeProcessingLabel = new QLabel();
    beforeProcessingFileSize = new QLabel();

    afterProcessingLabel = new QLabel();
    afterProcessingFileSize = new QLabel();

    QLabel* efficiencyLabel = new QLabel("Efficiency: ");
    efficiencyValue = new QLabel();

    beforeProcessingLabel->setFont(font);
    beforeProcessingFileSize->setFont(font);

    afterProcessingLabel->setFont(font);
    afterProcessingFileSize->setFont(font);

    efficiencyLabel->setFont(font);
    efficiencyValue->setFont(boldFont);

    QPushButton* backButton = new QPushButton("Back");
    QPushButton* closeButton = new QPushButton("Close");

    grid->addWidget(beforeProcessingLabel, 1, 1);
    grid->addWidget(beforeProcessingFileSize, 1, 2);
    grid->addWidget(afterProcessingLabel, 2, 1);
    grid->addWidget(afterProcessingFileSize, 2, 2);
    grid->addWidget(efficiencyLabel, 3, 1);
    grid->addWidget(efficiencyValue, 3, 2);

    footerLayout->addStretch(1);
    footerLayout->addWidget(backButton);
    footerLayout->addWidget(closeButton);

    mainLayout->addLayout(grid);
    mainLayout->addLayout(footerLayout);

    setLayout(mainLayout);

    connect(backButton, &QPushButton::clicked, this, &ResultWidget::backButtonPressedSignal);
    connect(closeButton, &QPushButton::clicked, this, &ResultWidget::closeButtonPressedSignal);
}

void ResultWidget::setBeforeProcessingLabel(const QString &text)
{
    beforeProcessingLabel->setText(text);
}

void ResultWidget::setAfterProcessingLabel(const QString &text)
{
    afterProcessingLabel->setText(text);
}

void ResultWidget::setBeforeProcessingFileSize(const QString &text)
{
    beforeProcessingFileSize->setText(text);
}

void ResultWidget::setAfterProcessingFileSize(const QString &text)
{
    afterProcessingFileSize->setText(text);
}

void ResultWidget::setEfficiency(const QString &text)
{
    efficiencyValue->setText(text);
}

void ResultWidget::showErrorMessage(const QString &text)
{
    QMessageBox::critical(nullptr, "Error", text);
}
