#include <QObject>
#include <QTimer>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption filePath("f", "path to the file", "filePath");
    parser.addOption(filePath);

    QCommandLineOption ipAdr("i", "ip address", "ip address");
    parser.addOption(ipAdr);

    QCommandLineOption portNumb("p", "port Number", "port Number");
    parser.addOption(portNumb);

    parser.process(a);

    QString strExecFile = parser.value(filePath);
    if(strExecFile.isEmpty())
        parser.showHelp(-1);

    QString ipadr = parser.value(ipAdr);
    if(ipadr.isEmpty())
        parser.showHelp(-1);

    QString port = parser.value(portNumb);
    if(port.isEmpty())
        parser.showHelp(-1);
}
