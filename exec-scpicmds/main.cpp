#include <QObject>
#include <QCommandLineParser>
#include "commandparser.h"
#include "tcphandler.h"


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
        qInfo("Please specify a cmd file!");
        parser.showHelp(-1);
    }

    QString ipAddress = parser.value(ipAddressOption);
    if(ipAddress.isEmpty())
    {
        qInfo("Please specify an IP-address!");
        parser.showHelp(-1);
    }

    quint16 portNumber = parser.value(portNumberOption).toUInt();
    if(portNumber <= 0 || portNumber > 65535)
    {
        qInfo("The port number needs to be within range 1..65535!");
        parser.showHelp(-1);
    }

    // Prepare for and perform the task itself
    TcpHandler tcpHandler;
    if (tcpHandler.connectTCP(ipAddress, portNumber))
    {
        qInfo("IP connection %s:%i was opened successfully!", qPrintable(ipAddress), portNumber);
    }
    else
    {
        qInfo("IP connection %s:%i could not be opened!", qPrintable(ipAddress), portNumber);
        exit(1);
    }

    CommandParser cmdParser(tcpHandler);
    cmdParser.parseCmdFile(cmdFile);

    exit(0);
}
