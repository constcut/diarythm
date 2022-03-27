#ifndef RECORDER_H
#define RECORDER_H

#include <memory>

#include <QObject>
#include <QAudioProbe>
#include <QAudioRecorder>


namespace diaryth {


    class Recorder : public QObject
    {
        Q_OBJECT

    public:
        Recorder();

        Q_INVOKABLE void start();
        Q_INVOKABLE void pause();
        Q_INVOKABLE void stop();
        Q_INVOKABLE void clear();

        Q_INVOKABLE QString lastFilename();


    private:

        std::unique_ptr<QAudioProbe> _audioProbe;
        std::unique_ptr<QAudioRecorder> _audioRecorder;

    };


}

#endif // RECORDER_H
