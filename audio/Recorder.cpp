#include "Recorder.hpp"

#include <QAudioEncoderSettings>
#include <QUrl>
#include <QDebug>
#include <QDir>
#include <QDate>

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
}


//TODO нужно отлавливать состояние, когда началась запись и сигнализировать, чтобы пользователь избегал проглоченных первых слов
void Recorder::start()
{
    auto dateString = QDate::currentDate().toString("yy_MM_dd");
    QString directory = QDir::currentPath() + "/recorder/" + dateString + "/";

    QDir dir;
    if (dir.exists(directory) == false) {
        if (dir.mkdir(directory) == false)
            qDebug() << "Failed to create recorder directory: " << directory;
    }

    _audioRecorder->setOutputLocation(QUrl::fromLocalFile(directory + "test.wav"));

    qDebug() << _audioRecorder->actualLocation().toString() << " : audio location";

    _audioRecorder->record();
}


void Recorder::pause()
{
    _audioRecorder->pause();
}


void Recorder::stop()
{
    _audioRecorder->stop();
}


void Recorder::clear()
{

}
