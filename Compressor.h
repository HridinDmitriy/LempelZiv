#pragma once
#include <QObject>
#include <QFile>
#include <QHash>

class Compressor : public QObject
{
    Q_OBJECT
public:
    explicit Compressor(QObject *parent = nullptr);

public slots:
    void    compress(const QString& fileName);

private:
    void    initialize();
    void    compressLoop();
    void    checkHashTableSize();
    void    writeOutputData(quint16 index, char lastSymbol);
    void    writeOutputData(quint16 index);
    void    flushBuffer();
    void    emitResults();
    void    writeHeader();
    QString compressedFileName();

private:
    using HashType = QHash<QByteArray, quint16>;

    const quint16   NO_MATCHING         =   0;
    const qint32    MAX_BLOCK_SIZE      =   3 * 1024 * 1024;
    const qint32    MAX_HASH_TABLE_SIZE =   65535;

    QFile           inputFile;
    QFile           outputFile;

    HashType        hashTable;
    QByteArray      outputBuffer;
    qint64          outputFilePos{};

signals:
    void    errorOccuredSignal(const QString& errorMsg);
    void    inputFileSizeSignal(const QString& fileSize);
    void    outputFileSizeSignal(const QString& fileSize);
    void    efficiencySignal(const QString& efficiency);
};

