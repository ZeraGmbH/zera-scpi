#include <QObject>
#include <QTimer>
#include <QCommandLineParser>
#include <QThread>
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

    QCommandLineOption handleErroneousMessagesOption("e", "Handling of erroneous messages.\n"
                                                          "0 = No execution at all (default).\n"
                                                          "1 = Force execution (for debugging only!).\n"
                                                          "2 = Remove erroneous messages and execute the rest.\n"
                                                          "3 = Remove erroneous messages (silently) and execute the rest.",
                                                     "HANDLE_ERR_MSGS");
    handleErroneousMessagesOption.setDefaultValue("0");
    parser.addOption(handleErroneousMessagesOption);

    QCommandLineOption checkErrorQueueOption("c", "Check targets error queue after sending (executing) the message.\n"
                                                  "0 = False\n"
                                                  "1 = True.",
                                             "CHECK_ERROR_QUEUE");
    checkErrorQueueOption.setDefaultValue("0");
    parser.addOption(checkErrorQueueOption);

    QCommandLineOption receiveTimeoutOption("t", "Receive timeout [ms] (default = 3000). 0 = blocking.", "RECV_TIMEOUT");
    receiveTimeoutOption.setDefaultValue("3000");
    parser.addOption(receiveTimeoutOption);

    parser.process(a);

    // Read and check command line arguments
    QString cmdFile = parser.value(cmdFileOption);
    if(cmdFile.isEmpty())
    {
        Logging::logMsg(QString("Please specify a command file!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    QString ipAddress = parser.value(ipAddressOption);
    if(ipAddress.isEmpty())
    {
        Logging::logMsg(QString("Please specify an IP-address!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    quint16 portNumber = parser.value(portNumberOption).toUInt();
    if(portNumber <= 0 || portNumber > 65535)
    {
        Logging::logMsg(QString("The port number needs to be within range 1..65535!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    unsigned int handleErroneousMessages = parser.value(handleErroneousMessagesOption).toUInt();
    if (handleErroneousMessages < 0 || handleErroneousMessages > 3)
        handleErroneousMessages = 0;

    unsigned int receiveTimeout = parser.value(receiveTimeoutOption).toUInt();

    unsigned int checkErrorQueue = (parser.value(checkErrorQueueOption).toUInt() != 0);

    // Prepare for and perform the task itself
    TcpHandler tcpHandler;
    tcpHandler.setReceiveTimeout(receiveTimeout);
    if (tcpHandler.connectTCP(ipAddress, portNumber))
    {
        Logging::logMsg(QString("TCP connection to %1:%2 was opened successfully.").arg(ipAddress, QString::number(portNumber)));
    }
    else
    {
        Logging::logMsg(QString("TCP connection to %1:%2 could not be opened!").arg(ipAddress, QString::number(portNumber)), LoggingColor::RED);
        exit(1);
    }

    CommandParser cmdParser(tcpHandler);
    cmdParser.setHandleErroneousMessages(handleErroneousMessages);
    cmdParser.setCheckErrorQueue(checkErrorQueue);
    cmdParser.parseCmdFile(cmdFile);

    tcpHandler.disconnectFromHost();

    // Wait some time to print all logging outputs
    QTimer::singleShot(1000, &a, [&]() {
        a.exit();
    });

    return a.exec();
}
