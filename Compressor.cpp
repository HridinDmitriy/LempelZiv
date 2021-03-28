#include "Compressor.h"
#include <QIODevice>
#include <QDataStream>
#include <QFileInfo>

Compressor::Compressor(QObject *parent)
    : QObject(parent)
{

}

void Compressor::compress(const QString& fileName)
{
    inputFile.setFileName(fileName);

    if (!inputFile.open(QIODevice::ReadOnly))
    {
        emit errorOccuredSignal("An error occured while opening input file");
        return;
    }

    if (inputFile.size() == 0)
    {
        emit errorOccuredSignal("The opened file is empty");
        inputFile.close();
        return;
    }

    compressLoop();

    inputFile.close();
}

void Compressor::compressLoop()
{    
    initialize();

    outputFile.setFileName(compressedFileName());
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::NewOnly))
    {
        emit errorOccuredSignal("An error occured while creating output file");
        return;
    }

    writeHeader();

    QByteArray longestMatching;
    quint16 previousMatchingIndex = NO_MATCHING;
    qint64 inputFilePos = 0;

    while (!inputFile.atEnd())
    {
        inputFile.seek(inputFilePos);
        QByteArray inputBuffer = inputFile.read(MAX_BLOCK_SIZE);
        inputFilePos += inputBuffer.size();

        for (char currentChar : inputBuffer)
        {
            checkHashTableSize();

            longestMatching.append(currentChar);
            auto matchingIndex = hashTable.value(longestMatching, NO_MATCHING);

            if (matchingIndex == NO_MATCHING)
            {
                hashTable.insert(longestMatching, hashTable.size() + 1);

                writeOutputData(previousMatchingIndex, currentChar);

                longestMatching.clear();
            }

            previousMatchingIndex = matchingIndex;
        }
    }

    if (!longestMatching.isNull())
    {
        writeOutputData(previousMatchingIndex);
    }
    if (!outputBuffer.isNull())
    {
        flushBuffer();
    }

    emitResults();

    outputFile.close();
}

void Compressor::initialize()
{
    hashTable.clear();
    outputBuffer.clear();
    outputFile.close();
    outputFilePos = 0;
}

QString Compressor::compressedFileName()
{
    QString inputFileName = QFileInfo(inputFile).baseName();
    return inputFileName + ".compressed";
}

void Compressor::checkHashTableSize()
{
    if (hashTable.size() >= MAX_HASH_TABLE_SIZE)
    {
        hashTable.clear();
    }
}

void Compressor::writeOutputData(quint16 index, char lastSymbol)
{
    outputBuffer.append(static_cast<quint8>(index >> 8));
    outputBuffer.append(static_cast<quint8>((index << 8) >> 8));
    outputBuffer.append(lastSymbol);

    if (outputBuffer.size() >= MAX_BLOCK_SIZE)
    {
        flushBuffer();
    }
}

void Compressor::writeOutputData(quint16 index)
{
    outputBuffer.append(static_cast<quint8>(index >> 8));
    outputBuffer.append(static_cast<quint8>((index << 8) >> 8));

    flushBuffer();
}

void Compressor::flushBuffer()
{
    outputFile.seek(outputFilePos);
    outputFile.write(outputBuffer);

    outputFilePos += outputBuffer.size();

    outputBuffer.clear();
}

void Compressor::emitResults()
{
    if (!outputFile.fileName().isEmpty() && !inputFile.fileName().isEmpty())
    {
        auto inputFileSize = inputFile.size();
        auto outputFileSize = outputFile.size();

        emit inputFileSizeSignal(QString::number(inputFileSize) + " bytes");
        emit outputFileSizeSignal(QString::number(outputFileSize) + " bytes");

        qint32 efficiency = 100 - (qint32)(((double)outputFileSize / inputFileSize) * 100);
        emit efficiencySignal(QString::number(efficiency) + " %");
    }
}

void Compressor::writeHeader()
{
    if (!outputFile.isOpen())
    {
        return;
    }

    QFileInfo inputFileInfo(inputFile);
    auto inputFileExtension = inputFileInfo.completeSuffix();

    outputBuffer.append(inputFileExtension.length());
    outputBuffer.append(inputFileExtension.toStdString().c_str());

    outputFile.write(outputBuffer);
    outputFilePos += outputBuffer.size();

    outputBuffer.clear();
}
