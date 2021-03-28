#include "Decompressor.h"
#include <QFileInfo>
#include <QDataStream>

Decompressor::Decompressor(QObject *parent)
    : QObject(parent)
{

}

void Decompressor::decompress(const QString &fileName)
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

    decompressLoop();

    inputFile.close();
}

void Decompressor::initialize()
{
    vector.clear();
    vector.append(QByteArray());

    outputBuffer.clear();
    outputFile.close();

    outputFilePos = 0;
    inputFilePos = 0;
}

void Decompressor::decompressLoop()
{
    initialize();

    QString outputFileName = decompressedFileName();
    if (outputFileName.isEmpty())
    {
        emit errorOccuredSignal("The compressed file's header is damaged");
        return;
    }

    outputFile.setFileName(outputFileName);
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::NewOnly))
    {
        emit errorOccuredSignal("An error occured while creating output file");
        return;
    }

    while (!inputFile.atEnd())
    {
        inputFile.seek(inputFilePos);
        QByteArray inputBuffer = inputFile.read(MAX_BLOCK_SIZE);
        inputFilePos += inputBuffer.size();

        auto size = inputBuffer.size() - 2;
        for (int i = 0; i < size; i += 3)
        {
            auto matchingIndex = readUInt16(inputBuffer[i], inputBuffer[i + 1]);
            char symbol = inputBuffer[i + 2];

            checkVectorSize();

            if (matchingIndex >= vector.size())
            {
                emit errorOccuredSignal("The compressed file is damaged");
                return;
            }

            QByteArray newEntry = vector[matchingIndex] + symbol;
            vector.append(newEntry);

            writeOutputData(newEntry);
        }

        processLastBytesIfAvailable(inputBuffer);
    }

    if (!outputBuffer.isNull())
    {
        flushBuffer();
    }

    emitResults();

    outputFile.close();
}

void Decompressor::checkVectorSize()
{
    if (vector.size() >= MAX_VECTOR_SIZE)
    {
        vector.clear();
        vector.append(QByteArray());
    }
}

void Decompressor::writeOutputData(const QByteArray& data)
{
    outputBuffer.append(data);

    if (outputBuffer.size() >= MAX_BLOCK_SIZE)
    {
        flushBuffer();
    }
}

quint16 Decompressor::readUInt16(char highByte, char lowByte)
{
    quint16 highPart = static_cast<quint8>(highByte);
    quint16 lowPart = static_cast<quint8>(lowByte);

    return ((highPart << 8) + lowPart);
}

void Decompressor::flushBuffer()
{
    outputFile.seek(outputFilePos);
    outputFile.write(outputBuffer);

    outputFilePos += outputBuffer.size();

    outputBuffer.clear();
}

void Decompressor::emitResults()
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

void Decompressor::processLastBytesIfAvailable(const QByteArray &data)
{
    auto unprocessedSymbolsCount = data.size() % 3;

    if (unprocessedSymbolsCount == 1)
    {
        emit errorOccuredSignal("The compressed file is damaged.\n\nThe output file will not be correctly uncompressed");
        return;
    }

    if (unprocessedSymbolsCount == 2)
    {
        auto dataSize = data.size();

        auto matchingIndex = readUInt16(
                data[dataSize - 2],
                data[dataSize - 1]
        );

        QByteArray matchingData = vector[matchingIndex];

        writeOutputData(matchingData);
    }
}

QString Decompressor::readHeader()
{
    if (!inputFile.isOpen())
    {
        return QString();
    }

    char headerLength[1] = {0};
    char header[300] = {0};

    inputFile.read(headerLength, 1);
    inputFilePos += 1;

    auto expectedlength = (int)headerLength[0];

    auto actualLength = inputFile.read(header, expectedlength);
    inputFilePos += actualLength;

    if (actualLength < expectedlength)
    {
        return QString();
    }

    return QString(header);
}

QString Decompressor::decompressedFileName()
{
    QString inputFileName = QFileInfo(inputFile).baseName();
    QString extension = readHeader();

    if (extension.isEmpty())
    {
        return QString();
    }

    return inputFileName + "." + extension;
}


