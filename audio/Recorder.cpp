#include "Recorder.hpp"

#include <QAudioEncoderSettings>
#include <QUrl>
#include <QDebug>
#include <QDir>

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



void Recorder::start()
{
    QString directory = QDir::currentPath() + "/recorder/";

    _audioRecorder->setOutputLocation(QUrl::fromLocalFile( directory + "test.wav"));

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
