// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.


#include "rsg/actor.hpp"
#include "rsg/mailbox.hpp"
#include "rsg/comm.hpp"
#include "rsg/host.hpp"
#include "../../../src/common.hpp"

#include "xbt.h"
#include "simgrid/s4u.hpp"

#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <thread>
#include <inttypes.h>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

XBT_LOG_NEW_CATEGORY(RSG_THRIFT_CLIENT, "Remote SimGrid");
XBT_LOG_NEW_DEFAULT_SUBCATEGORY(RSG_THRIFT_REMOTE_CLIENT, RSG_THRIFT_CLIENT , "RSG server (Remote SimGrid)");

using boost::shared_ptr;
using namespace ::simgrid;

#define UNUSED(x) (void)(x)

class hello {
 public:
 hello(std::string name) : pName(name) {}
 std::string pName;
 int operator()(void *) {
   XBT_INFO("hello");

   rsg::MailboxPtr mbox = rsg::Mailbox::byName(this->pName.c_str());
   uint64_t *pid = (uint64_t*) rsg::this_actor::recv(*mbox);
   UNUSED(pid);
   rsg::this_actor::quit();
   return 1;
 }
};


int Spwaner(void *) {
 rsg::HostPtr host1 = rsg::Host::by_name("host1");
 /* generate secret number between 1 and 10: */
 for(int i = 0; i < 2; i++) {
   rsg::Actor* actor = rsg::Actor::createActor("hello" , host1 , hello("hello"), NULL);
   if(rand() % 2 == 0) {
     rsg::this_actor::sleep(1);
     rsg::Actor::kill(actor->getPid());
   } else {
     actor->kill();
   }
   delete actor;
 }

 XBT_INFO("Spawner quit");
 rsg::this_actor::quit();
 return 1;
}

int main(int argc, char **argv) {
 rsg::HostPtr host1 = rsg::Host::by_name("host1");

 for(int i = 0; i < 4; i++) {
   rsg::Actor* actor =  rsg::Actor::createActor("spawner" , host1 , Spwaner, NULL);
   actor->join();
   XBT_INFO("joined spawner n°%d ", i);
   delete actor;
 }
 
 rsg::this_actor::quit();
 
 return 0; 
}
