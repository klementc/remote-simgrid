// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "rsg/actor.hpp"
#include "rsg/mailbox.hpp"
#include "rsg/host.hpp"

#include <xbt.h>
#include <simgrid/s4u.hpp>

#include <stdio.h>
#include <iostream>

XBT_LOG_NEW_CATEGORY(RSG_THRIFT_CLIENT, "Remote SimGrid");
XBT_LOG_NEW_DEFAULT_SUBCATEGORY(RSG_THRIFT_REMOTE_SERVER, RSG_THRIFT_CLIENT , "RSG server (Remote SimGrid)");

using namespace ::simgrid;

int main() {

  XBT_INFO("hello from server");

  rsg::HostPtr host = rsg::Host::current();

  XBT_INFO("Hostname current Peak : %f",  host->currentPowerPeak());

  XBT_INFO("hostname : %s", rsg::Host::current()->getName().c_str());

  XBT_INFO("core count : %d", host->coreCount());
  XBT_INFO("state count %d", host->pstatesCount());
  XBT_INFO("state -> %d", host->pstate());

  rsg::this_actor::quit();
  return 0;
}
