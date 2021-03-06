#include "rsg/actor.hpp"
#include "rsg/mailbox.hpp"
#include "rsg/comm.hpp"
#include "rsg/host.hpp"

#include <xbt.h>
#include <simgrid/s4u.hpp>

#include <iostream>

XBT_LOG_NEW_CATEGORY(RSG_THRIFT_CLIENT, "Remote SimGrid");
XBT_LOG_NEW_DEFAULT_SUBCATEGORY(RSG_THRIFT_REMOTE_CLIENT, RSG_THRIFT_CLIENT , "RSG server (Remote SimGrid)");

using namespace ::simgrid;

int main()
{
    const char *msg = "Do you copy?";
    rsg::MailboxPtr mbox = rsg::Mailbox::byName("toto");
    
    rsg::MailboxPtr fakeMbox = rsg::Mailbox::byName("fake");
    if(fakeMbox->receiver() == nullptr)
    {
        XBT_INFO("No receiver on mailbox \"fake\"");
    }
    
    rsg::this_actor::send(*mbox, msg, strlen(msg) + 1);
    rsg::this_actor::sleep(2);
    rsg::Actor *receiver = mbox->receiver();
    XBT_INFO("Send \"%s\" to pid=%d with size=%lu",
             msg, receiver->getPid(), strlen(msg));
    delete receiver;

    rsg::this_actor::sleep(10);
    //At this point the receiver should have reset his status
    receiver = mbox->receiver();
    if(receiver == nullptr)
    {
        XBT_INFO("Mailbox receiver has been reseted to nullptr");
    }

    rsg::this_actor::quit();
    return 0;
}
