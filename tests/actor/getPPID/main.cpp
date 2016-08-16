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
  XBT_INFO("My id is  : %d",rsg::this_actor::getPid());
  XBT_INFO("My ppid is  : %d",rsg::this_actor::getPPid());

  rsg::this_actor::quit();
  return 1;
}

int main(int argc, char **argv) {
  rsg::HostPtr host1 = rsg::Host::by_name("host1");

  rsg::Actor *act = rsg::Actor::createActor("receiver" , host1 , actor, NULL);

  XBT_INFO("My id is  : %d",rsg::this_actor::getPid());
  XBT_INFO("My ppid is  : %d",rsg::this_actor::getPPid());
  
  rsg::Actor *getByPid = rsg::Actor::forPid(act->getPid());
  getByPid->join();
  XBT_INFO("getName  : %s", getByPid->getName());
  XBT_INFO("Main end at : %f", rsg::getClock());

  rsg::this_actor::quit();
  return 0;
}
