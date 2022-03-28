#include "Init.hpp"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTextCodec>
#include <QFontDatabase>
#include <QDir>
#include <QTemporaryDir>
#include <QStandardPaths>
#include <QDateTime>

#include <fstream>
#include <signal.h>

#include "app/log.hpp"

#include "app/LogHandler.hpp"
#include "app/Config.hpp"
#include "app/StretchImage.hpp"
#include "app/SQLBase.hpp"

#include "audio/wave/AudioHandler.hpp"
#include "audio/wave/WaveShape.hpp"
#include "audio/spectrum/Spectrograph.hpp"
#include "audio/spectrum/Cepstrumgraph.hpp"
#include "audio/features/ACFgraph.hpp"

#include "audio/Recorder.hpp"



using namespace std;
using namespace diaryth;


void posix_death_signal(int signum)
{
    qDebug() << "Crash happend signal:"<<signum;
    signal(signum, SIG_DFL);
    //TODO
    exit(3);
}


void setLibPath() {
#ifdef WIN32
    QStringList libPath = QCoreApplication::libraryPaths();

    libPath.append(".");
    libPath.append("platforms");
    libPath.append("imageformats");

    QCoreApplication::setLibraryPaths(libPath);
#endif
}


void loadConfig() {
    std::string currentPath = Config::getInst().testsLocation;
    Config& configuration = Config::getInst();
    configuration.checkConfig();
    std::string confFileName = currentPath + "g.config";
    if (QFile::exists(confFileName.c_str())) {
        std::ifstream confFile(confFileName);
        if (confFile.is_open())
            configuration.load(confFile);
    }
    configuration.printValues();
}


void setPosixSignals() {
    signal(SIGSEGV, posix_death_signal);
    signal(SIGILL, posix_death_signal);
    signal(SIGFPE, posix_death_signal);
}




int mainInit(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qmlRegisterType<diaryth::ConsoleLogQML>("diaryth",1,0,"ConsoleLogView");

    QGuiApplication app(argc, argv); //QApplication if widgets are used

    app.setApplicationName("diaryth");
    app.setOrganizationName("accumerite");
    app.setOrganizationDomain("accumerite.ru");

    Q_INIT_RESOURCE(fonts);

    qDebug() << "Current working path "<<QDir::currentPath();
    int fontId = QFontDatabase::addApplicationFont(":/fonts/prefont.ttf");

    QStringList famList = QFontDatabase::applicationFontFamilies(fontId) ;
    qDebug() << famList << " font families for id "<<fontId;
    if (famList.isEmpty() == false)
        app.setFont(QFont(famList[0], 11, QFont::Normal, false));
    else
        qWarning() << "Failed to load font";

    Config::getInst().checkConfig();

    qmlRegisterType<diaryth::ConfigTableModel>("diaryth", 1, 0, "ConfigTableModel");

    qmlRegisterType<diaryth::WaveshapeQML>("diaryth", 1, 0, "Waveshape");
    qmlRegisterType<diaryth::SpectrographQML>("diaryth", 1, 0,"Spectrograph");
    qmlRegisterType<diaryth::ACGraphQML>("diaryth", 1, 0,"ACgraph");
    qmlRegisterType<diaryth::StretchImageQML>("diaryth", 1, 0,"StretchImage");
    qmlRegisterType<diaryth::CepstrumgraphQML>("diaryth", 1, 0,"Cepstrumgraph");


    QDir dir;
    if (dir.exists("records") == false) { //AudioHandler records, later they would be removed from current project
        if (dir.mkdir("records") == false)
            qDebug() << "Failed to create records directory";
    }
    if (dir.exists("recorder") == false) {
        if (dir.mkdir("recorder") == false)
            qDebug() << "Failed to create records directory";
    }


    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251")); //Настройки //KOI8-R //ISO 8859-5 //UTF-8 //Windows-1251
    QQmlApplicationEngine engine;

    diaryth::AudioHandler audio;
    diaryth::Recorder recorder;
    diaryth::ConfigQML config;
    diaryth::SQLBase sqlBase;

    engine.rootContext()->setContextProperty("audio", &audio);
    engine.rootContext()->setContextProperty("recorder", &recorder);
    engine.rootContext()->setContextProperty("aconfig", &config);
    engine.rootContext()->setContextProperty("sqlBase", &sqlBase);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int res = 0;
    try {
        res = app.exec();
    }
    catch(std::exception& e) {
        qDebug() << "Catched exception " << e.what();
        res = -1;
    }

    return res;
}
