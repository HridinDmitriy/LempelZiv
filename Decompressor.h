#pragma once
#include <QObject>
#include <QFile>
#include <QVector>

class Decompressor : public QObject
{
    Q_OBJECT
public:
    explicit Decompressor(QObject *parent = nullptr);

public slots:
    void    decompress(const QString& fileName);

private:
    void    initialize();
    void    decompressLoop();
    void    checkVectorSize();
    void    writeOutputData(const QByteArray& data);
    quint16 readUInt16(char highByte, char lowByte);
    void    flushBuffer();
    void    emitResults();
    void    processLastBytesIfAvailable(const QByteArray& data);
    QString readHeader();
    QString decompressedFileName();

private:
    using VectorType = QVector<QByteArray>;

    const quint16   NO_MATCHING         =   0;
    const qint32    MAX_BLOCK_SIZE      =   3 * 1024 * 1024;
    const qint32    MAX_VECTOR_SIZE     =   65536;

    QFile           inputFile;
    QFile           outputFile;

    VectorType      vector;
    QByteArray      outputBuffer;

    qint64          outputFilePos{};
    qint64          inputFilePos{};

signals:
    void    errorOccuredSignal(const QString& errorMsg);
    void    inputFileSizeSignal(const QString& fileSize);
    void    outputFileSizeSignal(const QString& fileSize);
    void    efficiencySignal(const QString& efficiency);
};

