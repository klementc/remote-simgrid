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

int main()
{
    rsg::MailboxPtr mbox = rsg::Mailbox::byName("toto");
    char *received = rsg::this_actor::recv(*mbox);

    rsg::this_actor::sleep(1);
    XBT_INFO("Setting receiver to pid=%d...", rsg::this_actor::getPid());
    mbox->setReceiver(rsg::Actor::byPid(rsg::this_actor::getPid()));
    XBT_INFO("Setting receiver to pid=%d... done", rsg::this_actor::getPid());
    rsg::this_actor::sleep(1);

    XBT_INFO("Setting receiver to nullptr...");
    mbox->setReceiver(nullptr);
    XBT_INFO("Setting receiver to nullptr... done");
    XBT_INFO("Received from client: \"%s\" with size of %lu", received, strlen(received) );
    rsg::this_actor::quit();
    return 0;
}
