#include <QCoreApplication>
#include "serverchatter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //converting argv[] to integer
    int port = std::atoi(argv[1]);
    //creating  objs
    serverChatter server;
    server.startServer(port);
    //rest of the code
    return a.exec();
}
