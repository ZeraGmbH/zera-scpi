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

    QCommandLineOption checkErrorQueueOption("c", "Check target's error queue after sending (executing) the message.\n"
                                                  "0 = Disabled\n"
                                                  "1 = Enabled.",
                                             "CHECK_ERROR_QUEUE");
    checkErrorQueueOption.setDefaultValue("0");
    parser.addOption(checkErrorQueueOption);

    QCommandLineOption enableFormattedOutputOption("enable-formatted-output", "Enable formatted logging output.\n"
                                                  "0 = Disabled\n"
                                                  "1 = Enabled.",
                                             "ENABLE_FORMATTED_OUPUT");
    enableFormattedOutputOption.setDefaultValue("0");
    parser.addOption(enableFormattedOutputOption);

    QCommandLineOption receiveTimeoutOption("t", "Receive timeout [ms] (default = 3000). 0 = blocking.", "RECV_TIMEOUT");
    receiveTimeoutOption.setDefaultValue("3000");
    parser.addOption(receiveTimeoutOption);

    QCommandLineOption loopFileCntOption("l", "Number of repetitions executing of file.", "NUMBER_OF_LOOPS");
    loopFileCntOption.setDefaultValue("1");
    parser.addOption(loopFileCntOption);

    QCommandLineOption ignoreExistingVariablesOption("x", "Ignore existing variables.\n"
                                                          "0 = Disabled\n"
                                                          "1 = Enabled.",
                                                     "IGNORE_EXISTING_VARS");
    ignoreExistingVariablesOption.setDefaultValue("0");
    parser.addOption(ignoreExistingVariablesOption);

    // Parse command line arguments
    parser.process(a);

    // Read and check command line arguments
    bool ok = false;

    bool enableFormattedOutput = (parser.value(enableFormattedOutputOption).toUInt(&ok) != 0);
    if (ok) {
        Logging::enableFormattedOutput(enableFormattedOutput);
    } else {
        Logging::logMsg(QString("Enable formatted logging output (option --enable-formatted-output) needs to be an integer with 0 = disabled, >0 enabled!"), LoggingColor::YELLOW);
        parser.showHelp(-1);
    }

    QString cmdFile = parser.value(cmdFileOption);
    if(cmdFile.isEmpty()) {
        Logging::logMsg(QString("Please specify a command file!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    QString ipAddress = parser.value(ipAddressOption);
    if(ipAddress.isEmpty()) {
        Logging::logMsg(QString("Please specify an IP-address (option -i)!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    quint16 portNumber = parser.value(portNumberOption).toUInt(&ok);
    if(!ok || portNumber <= 0 || portNumber > 65535)
    {
        Logging::logMsg(QString("The port number (option -p) needs to be within range 1..65535!"), LoggingColor::RED);
        parser.showHelp(-1);
    }

    unsigned int handleErroneousMessages = parser.value(handleErroneousMessagesOption).toUInt(&ok);
    if (!ok || handleErroneousMessages > 3) {
        Logging::logMsg(QString("The handling of erroneous messages (option -e) need to be in range 0..3!"), LoggingColor::YELLOW);
        parser.showHelp(-1);
    }

    unsigned int receiveTimeout = parser.value(receiveTimeoutOption).toUInt(&ok);
    if (!ok) {
        Logging::logMsg(QString("Receive timeout (option -t) needs to be a positive integer!"), LoggingColor::YELLOW);
        parser.showHelp(-1);
    }

    unsigned int checkErrorQueue = (parser.value(checkErrorQueueOption).toUInt(&ok) != 0);
    if (!ok) {
        Logging::logMsg(QString("Check target error queue (option -c) needs to be an integer with 0 = disabled, >0 enabled!"), LoggingColor::YELLOW);
        parser.showHelp(-1);
    }

    unsigned int loopFileCnt = parser.value(loopFileCntOption).toUInt(&ok);
    if (!ok) {
        Logging::logMsg(QString("Number of repetitions (option -l) needs to be an integer >= 0!"), LoggingColor::YELLOW);
        parser.showHelp(-1);
    }

    unsigned int ignoreExistingVariables = (parser.value(ignoreExistingVariablesOption).toUInt(&ok) != 0);
    if (!ok) {
        Logging::logMsg(QString("Ignore existing variables (option -x) needs to be an integer with 0 = disabled, >0 enabled!"), LoggingColor::YELLOW);
        parser.showHelp(-1);
    }

    // Prepare for and perform the task itself
    TcpHandler tcpHandler(receiveTimeout);
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
    cmdParser.setLoopNumber(loopFileCnt);
    cmdParser.setIgnoreExistingVariables(ignoreExistingVariables);
    cmdParser.parseCmdFile(cmdFile);

    tcpHandler.disconnectFromHost();

    // Wait some time to print all logging outputs
    QTimer::singleShot(1000, &a, [&]() {
        a.exit();
    });

    return a.exec();
}
