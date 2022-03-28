#include "Recorder.hpp"

#include <QAudioEncoderSettings>

#include <QUrl>
#include <QDir>
#include <QFile>

#include <QDate>
#include <QTime>

#include <QDebug>

#include "audio/features/FeatureExtractor.hpp"

using namespace diaryth;


Recorder::Recorder()
{
    _audioProbe = std::make_unique<QAudioProbe>();
    _audioRecorder = std::make_unique<QAudioRecorder>();
    //TODO audio probe connection for recorder

    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/pcm");
    audioSettings.setQuality(QMultimedia::HighQuality);

    _audioRecorder->setEncodingSettings(audioSettings);
    //Set volume?
    _audioProbe->setSource(_audioRecorder.get());

    connect(_audioProbe.get(), SIGNAL(audioBufferProbed(QAudioBuffer)),
            this, SLOT(processBuffer(QAudioBuffer)));


    _defaultInput = getInputDevice();
    _defaultCodec = getAudioCodec();
    _defaultContainer = getFileContainer();
    _defaultSampleRate = getSampleRate();
}



void Recorder::start()
{
    auto dateString = QDate::currentDate().toString("yy_MM_dd");
    QString directory = QDir::currentPath() + "/recorder/" + dateString + "/";

    QDir dir;
    if (dir.exists(directory) == false) {
        if (dir.mkdir(directory) == false)
            qDebug() << "Failed to create recorder directory: " << directory;
    }

    auto currentDirectory = QDir::currentPath();

    dir.setCurrent(directory);
    auto listOfFiles = dir.entryInfoList(QDir::Files);

    auto timeString = QTime::currentTime().toString("HH_mm_ss");
    QString recordName = "record" + QString::number(listOfFiles.size() + 1) + "_" + timeString;

    dir.setCurrent(currentDirectory);

    _durationMicroSeconds = 0;

    _audioRecorder->setOutputLocation(QUrl::fromLocalFile(directory + recordName));
    _audioRecorder->record();
}


QString Recorder::lastFilename() {
    return _audioRecorder->actualLocation().toString();
}


void Recorder::pause() {
    _audioRecorder->pause();
}


void Recorder::stop() {
    _audioRecorder->stop();
}


void Recorder::cancel()
{
    _audioRecorder->stop();

    QString fn = lastFilename().mid(8); //Remove file:///
    QFile file(fn);

    if (file.remove(fn) == false)
        qDebug() << "Failed to remove: " << fn;

}


//TODO нужно отлавливать состояние, когда началась запись и сигнализировать, чтобы пользователь избегал проглоченных первых слов
//P.s. можно реализовать на QML стороне, тк после нажатия на кнопку - первый сигнал пришедший - маркер начала

void Recorder::processBuffer(const QAudioBuffer& buffer)
{
    _durationMicroSeconds += buffer.duration();

    //Возможно стоит уменьшить частоту обновления, и оптарвлять сигналы не каждый раз

    emit timeUpdate(_durationMicroSeconds / 1000);

    const int16_t* samples = reinterpret_cast<const int16_t*>(buffer.data());
    const int bitsInByte = 8;
    const int length = buffer.byteCount() / ( buffer.format().sampleSize() / bitsInByte);

    const double dbs = calc_dB(samples, length);
    emit dbsUpdate(dbs);
}



QStringList Recorder::inputDevices()
{
    QStringList list = {"Default"};
    list.append(_audioRecorder->audioInputs());
    return list;
}


QStringList Recorder::audioCodecs()
{
    QStringList list = {"Default"};
    list.append(_audioRecorder->supportedAudioCodecs());
    return list;
}


QStringList Recorder::fileContainers()
{
    QStringList list = {"Default"};
    list.append(_audioRecorder->supportedContainers());
    return list;
}


QStringList Recorder::sampleRates()
{
    QStringList list = {"Default"};

    for (int sampleRate: _audioRecorder->supportedAudioSampleRates())
       list.append(QString::number(sampleRate));

    return list;
}


QString Recorder::getInputDevice() {
    return _audioRecorder->audioInput();
}


QString Recorder::getAudioCodec() {
    return _audioRecorder->audioSettings().codec();
}


QString Recorder::getFileContainer() {
    return _audioRecorder->containerFormat();
}


int Recorder::getSampleRate() {
    return _audioRecorder->audioSettings().sampleRate();
}
