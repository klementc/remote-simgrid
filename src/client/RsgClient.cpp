#include <sys/syscall.h>

#include <thrift/protocol/TMultiplexedProtocol.h>

#include "RsgClient.hpp"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;


thread_local RsgClient* client;

RsgClient::RsgClient(std::string name)
    : ctx(1)
    , networkName_(name)
    , zmqclient_(new TZmqClient(ctx, networkName_))
{
    debug_client_print("RsgClient::RsgClient CONSTRUCTOR %p %s", this, networkName_.c_str());

    shared_ptr<TZmqClient> transport(zmqclient_);
    shared_ptr<TBinaryProtocol> protocol(new TBinaryProtocol(transport));

    engine = new RsgEngineClient(shared_ptr<TMultiplexedProtocol>(new TMultiplexedProtocol(protocol, "RsgEngine")));
    actor = new RsgActorClient(shared_ptr<TMultiplexedProtocol>(new TMultiplexedProtocol(protocol, "RsgActor")));
    mailbox = new RsgMailboxClient(shared_ptr<TMultiplexedProtocol>(new TMultiplexedProtocol(protocol, "RsgMailbox")));
    mutex = new RsgMutexClient(shared_ptr<TMultiplexedProtocol>(new TMultiplexedProtocol(protocol, "RsgMutex")));
    conditionvariable = new RsgConditionVariableClient(shared_ptr<TMultiplexedProtocol>(new TMultiplexedProtocol(protocol, "RsgConditionVariable")));
    host = new RsgHostClient(shared_ptr<TMultiplexedProtocol>(new TMultiplexedProtocol(protocol, "RsgHost")));
    comm = new RsgCommClient(shared_ptr<TMultiplexedProtocol>(new TMultiplexedProtocol(protocol, "RsgComm")));
    kvs = new RsgKVSClient(shared_ptr<TMultiplexedProtocol>(new TMultiplexedProtocol(protocol, "RsgKVS")));
    
    transport->open();
    debug_client_print("RSG client %s started", networkName_.c_str());
}



RsgClient::~RsgClient()
{
    /*delete engine;
    delete actor;
    delete mailbox;
    delete mutex;
    delete conditionvariable;
    delete host;
    delete comm;*/
    delete zmqclient_;
    debug_client_print("RSG client %s ended %p", networkName_.c_str(), this);
}


static void initialize_client(void) __attribute__((constructor));
void initialize_client(void) {
    char *rpcEnvPort = std::getenv("RsgRpcNetworkName");
    if(rpcEnvPort != NULL){
        client = new RsgClient(std::string(rpcEnvPort));
    } else {
        debug_client_print("No rpcEnvPort given, caution everythings related to clients are unavailable in %d", getpid());
    }
}

static void desinitialize_client(void) __attribute__((destructor));
void desinitialize_client(void) {
    char *program_cmd = std::getenv("_");
    char *rpcEnvPort = std::getenv("RsgRpcNetworkName");
    debug_client_print("A RSG client process %d exited (RRNN=%s), it was launched with %s", getpid(),rpcEnvPort,program_cmd);
    if(client != NULL)
      if(client->actor != NULL){
	debug_client_print("Actor should quit before the end of %d process",getpid());
      }
}


