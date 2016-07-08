 // This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <thrift/protocol/TBinaryProtocol.h>

#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "rsg/services.hpp"
#include "client/RsgClient.hpp"
#include "rsg/actor.hpp"
#include "rsg/mailbox.hpp"
#include "rsg/comm.hpp"
#include "rsg/host.hpp"
#include "RsgMsg.hpp"

#include "xbt.h"
#include "simgrid/s4u.h"

#include <iostream>
#include <sys/types.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <inttypes.h>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

XBT_LOG_NEW_CATEGORY(RSG_THRIFT_CLIENT, "Remote SimGrid");
XBT_LOG_NEW_DEFAULT_SUBCATEGORY(RSG_THRIFT_REMOTE_CLIENT, RSG_THRIFT_CLIENT , "RSG server (Remote SimGrid)");

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using boost::shared_ptr;
using namespace ::RsgService;
using namespace ::simgrid;

#define UNUSED(x) (void)(x)

class PidComp {
  public:
  PidComp(std::string name) : pName(name) {}
  std::string pName;
  int operator()(void * ) {
    rsg::Mailbox *mbox = rsg::Mailbox::byName(this->pName.c_str());
    int *pid = (int*) rsg::this_actor::recv(*mbox);
    int id = rsg::this_actor::getPid();
    XBT_INFO("PidComp ->  %" PRIu64 "", id - *pid);
    rsg::this_actor::quit();
    return 1;
  }
};


int Spwaner(void * ) {
  rsg::Host host1 = rsg::Host::by_name("host1");

  for(int i = 0; i < 100; i++) {
    std::string name = "PidComp" + std::to_string(i);
    rsg::Mailbox *mbox = rsg::Mailbox::byName(name.c_str());

    rsg::Actor* actor = rsg::Actor::createActor("hello" , host1 , PidComp(name), NULL);
    int pid = actor->getPid();
    rsg::this_actor::send(*mbox, (char*) &(pid), sizeof(int));
    delete actor;
  }

  rsg::this_actor::quit();
  return 1;
}

int main(int argc, char **argv) {
  rsg::Host host1 = rsg::Host::by_name("host1");

  for(int i = 0; i < 4; i++) {
    rsg::Actor* actor =  rsg::Actor::createActor("spawner" , host1 , Spwaner, NULL);
    UNUSED(actor);
    delete actor;
  }
  
  rsg::this_actor::sleep(1000);
  rsg::this_actor::quit();
  
  return 0; 
}