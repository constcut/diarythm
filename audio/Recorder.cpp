#include "Recorder.hpp"

#include <QAudioEncoderSettings>

#include <QUrl>
#include <QDir>
#include <QFile>

#include <QDate>
#include <QTime>

#include <QDebug>


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
void Recorder::processBuffer(const QAudioBuffer& buffer)
{
    _durationMicroSeconds += buffer.duration();
    //qDebug() << "Time ms: " << _durationMicroSeconds / 1000;
}
