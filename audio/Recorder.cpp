#include "Recorder.h"

#include <QAudioEncoderSettings>
#include <QUrl>

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



void Recorder::record()
{
    _audioRecorder->setOutputLocation(QUrl::fromLocalFile("test.wav"));
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
