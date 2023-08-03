#include <QObject>
#include <QCommandLineParser>
#include "commandparser.h"
#include "tcphandler.h"
#include "logging.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Define command line arguments
    QCommandLineParser parser;
    parser.addHelpOption();

    QCommandLineOption cmdFileOption("f", "Path to the command file.", "FILE_PATH");
    parser.addOption(cmdFileOption);

    QCommandLineOption ipAddressOption("i", "IP-address.", "IP_ADDRESS");
    parser.addOption(ipAddressOption);

    QCommandLineOption portNumberOption("p", "Port number.", "PORT_NUMBER");
    parser.addOption(portNumberOption);

    parser.process(a);

    // Read and check command line arguments
    QString cmdFile = parser.value(cmdFileOption);
    if(cmdFile.isEmpty())
    {
        Logging::LogMsg(QString("Please specify a command file!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    QString ipAddress = parser.value(ipAddressOption);
    if(ipAddress.isEmpty())
    {
        Logging::LogMsg(QString("Please specify an IP-address!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    quint16 portNumber = parser.value(portNumberOption).toUInt();
    if(portNumber <= 0 || portNumber > 65535)
    {
        Logging::LogMsg(QString("The port number needs to be within range 1..65535!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    // Prepare for and perform the task itself
    TcpHandler tcpHandler;
    if (tcpHandler.connectTCP(ipAddress, portNumber))
    {
        Logging::LogMsg(QString("TCP connection to %1:%2 was opened successfully.").arg(ipAddress, QString::number(portNumber)));
    }
    else
    {
        Logging::LogMsg(QString("TCP connection to %1:%2 could not be opened!").arg(ipAddress, QString::number(portNumber)), LoggingColor::RED);
        exit(1);
    }

    CommandParser cmdParser(tcpHandler);
    cmdParser.parseCmdFile(cmdFile);

    return a.exec();
}
