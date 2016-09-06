#include <stdlib.h>
#include <iostream>
#include <vector>

#include <thrift/processor/TMultiplexedProcessor.h>

#include "xbt.h"
#include "simgrid/s4u.hpp"

#include "rsg/services.hpp"
#include "rsg/Server.hpp"
#include "common.hpp"


using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
using namespace ::simgrid;

std::vector<std::thread*> threads;

XBT_LOG_NEW_CATEGORY(RSG_THRIFT, "Remote SimGrid");
XBT_LOG_NEW_DEFAULT_SUBCATEGORY(RSG_THRIFT_SERVER, RSG_THRIFT , "RSG server (Remote SimGrid)");

/* Fork a remote process, and serve it until it CMD_QUITs */
static int rsg_representative(int argc, char **argv) {
    debug_server_print("START");
    if (! fork()) {
        int newargc = argc-1+2+1;
        char **newargv = (char**)calloc(newargc, sizeof(char*));
        newargv[0] = (char*)"/usr/bin/env";
        newargv[1] = (char*)"--";
        for(int i=1; i < argc; i++) {
            newargv[1+i] = argv[i];
        }
        newargv[newargc-1] = NULL;
        debug_server_print("fork+exec: %s", newargv[2]);
        execv(newargv[0], newargv);
    }
    debug_server_print("RUN server");
    SocketServer &socketServer = SocketServer::getSocketServer();
    RsgThriftServerFramework *server = socketServer.acceptClient();
    server->serve();
    delete server;
    debug_server_print("END");
    return 0;
}

int main(int argc, char **argv) {
    
    SocketServer &socketServer = SocketServer::createSocketServer(std::string("127.0.0.1"), 9090);
    socketServer.connect();
    
    s4u::Engine *e = new s4u::Engine(&argc,argv);
    
    if (argc < 3) {
        fprintf(stderr,"Usage: rsg platform.xml deploy.xml port\n");
        exit(1);
    }
    
    /* Initialize the SimGrid world */
    e->loadPlatform(argv[1]);
    e->registerDefault(rsg_representative);
    e->loadDeployment(argv[2]);
    e->run();
    
    socketServer.closeServer();
    return 0;
}
