#include <QObject>
#include <QTimer>
#include <QCommandLineParser>
#include "commandparser.h"
#include "tcphandler.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption filePath("f", "path to the command file", "filePath");
    parser.addOption(filePath);

    QCommandLineOption ipAdr("i", "ip address", "ip address");
    parser.addOption(ipAdr);

    QCommandLineOption portNumb("p", "port Number", "port Number");
    parser.addOption(portNumb);

    parser.process(a);

    QString strCmdFile = parser.value(filePath);
    if(strCmdFile.isEmpty())
        parser.showHelp(-1);

    QString ipadr = parser.value(ipAdr);
    if(ipadr.isEmpty())
        parser.showHelp(-1);

    QString port = parser.value(portNumb);
    if(port.isEmpty())
        parser.showHelp(-1);

    //HandleTelnetConnection conn;
    //conn.setConnection()

    CommandParser cmdParser;
    cmdParser.parseCmdFile(strCmdFile);

    exit(0);
}
