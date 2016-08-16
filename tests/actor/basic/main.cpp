// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.


#include "rsg/actor.hpp"
#include "rsg/mailbox.hpp"
#include "rsg/comm.hpp"
#include "rsg/host.hpp"
#include "rsg/engine.hpp"

#include "xbt.h"
#include "simgrid/s4u.hpp"

#include <iostream>
#define UNUSED(x) (void)(x)


XBT_LOG_NEW_CATEGORY(RSG_THRIFT_CLIENT, "Remote SimGrid");
XBT_LOG_NEW_DEFAULT_SUBCATEGORY(RSG_THRIFT_REMOTE_CLIENT, RSG_THRIFT_CLIENT , "RSG server (Remote SimGrid)");

using boost::shared_ptr;
using namespace ::simgrid;

int actor(void *) {
  rsg::Mailbox *mbox = rsg::Mailbox::byName("toto");
  char *received = rsg::this_actor::recv(*mbox);
  XBT_INFO("Received from client : %s with size of %d ", received, strlen(received) );
  XBT_INFO("My id is  : %d",rsg::this_actor::getPid());
  rsg::this_actor::sleep(99.999999);
  rsg::this_actor::quit();
  return 1;
}

int main(int argc, char **argv) {
  const char *msg = "Do you copy ? ";
  rsg::HostPtr host1 = rsg::Host::by_name("host1");

  rsg::Actor *act = rsg::Actor::createActor("receiver" , host1 , actor, NULL);
  rsg::Mailbox *mbox = rsg::Mailbox::byName("toto");
  XBT_INFO("I'll send %s with size : %d", msg, strlen(msg));

  rsg::this_actor::send(*mbox,msg, strlen(msg) + 1);
  XBT_INFO("send %s with size : %d", msg, strlen(msg));
  
  XBT_INFO("My id is  : %d",rsg::this_actor::getPid());
  
  rsg::Actor *getByPid = rsg::Actor::forPid(act->getPid());
  getByPid->join();
  XBT_INFO("getName  : %s", getByPid->getName());
  XBT_INFO("Main end at : %f", rsg::getClock());

  rsg::this_actor::quit();
  return 0;
}
