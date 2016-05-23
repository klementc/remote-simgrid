#include "rsg/services.hpp"
#include "rsg/Server.hpp"

#include "xbt.h"
#include "simgrid/s4u.h"

#include <iostream>
#include <boost/shared_ptr.hpp>

#include <thrift/processor/TMultiplexedProcessor.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;


using namespace ::apache::thrift::server;
using namespace  ::RsgService;

using namespace  ::simgrid;

rsg::RsgActorHandler::RsgActorHandler()  : pServer(NULL) {
}

void rsg::RsgActorHandler::setServer(TServerFramework *server) {
  pServer = server;
}

void rsg::RsgActorHandler::close() {
  pServer->stop();
}

void  rsg::RsgActorHandler::sleep(const double duration) {
  s4u::this_actor::sleep(duration);
}

void rsg::RsgActorHandler::execute(const double flops) {
  s4u::this_actor::execute(flops);
}

void rsg::RsgActorHandler::send(const int64_t mbAddr, const std::string& content, const int64_t simulatedSize) {
  s4u::Mailbox *mbox = (s4u::Mailbox*)mbAddr;
  std::string *internalPtr = new std::string(content.data(), content.length());
  s4u::this_actor::send(*mbox, (void*) internalPtr, simulatedSize);
}

void rsg::RsgActorHandler::recv(std::string& _return, const int64_t mbAddr) {
  s4u::Mailbox *mbox = (s4u::Mailbox*) mbAddr;
  std::string *content = (std::string*) s4u::this_actor::recv(*mbox);
  _return.assign(content->data(), content->length());
  delete content;
}

void rsg::RsgActorHandler::getName(std::string& _return, const int64_t addr) {
    s4u::Actor *actor = (s4u::Actor*) addr;
    const char * c_name = actor->getName();
  _return.assign(c_name);
}


void rsg::RsgActorHandler::getHost(rsgHostCurrentResType& _return, const int64_t addr) {
  s4u::Actor *actor = (s4u::Actor*) addr;

  s4u::Host *host = actor->getHost();
  _return.name = host->name();
  _return.addr = (unsigned long int) host;
}


int32_t rsg::RsgActorHandler::getPid(const int64_t addr) {
  s4u::Actor *actor = (s4u::Actor*) addr;
  return actor->getPid();
}


void rsg::RsgActorHandler::setAutoRestart(const int64_t addr, const bool autorestart) {
  s4u::Actor *actor = (s4u::Actor*) addr;
  actor->setAutoRestart(autorestart);
}


void rsg::RsgActorHandler::setKillTime(const int64_t addr, const double time) {
  s4u::Actor *actor = (s4u::Actor*) addr;
  actor->setKillTime(time);
}


double rsg::RsgActorHandler::getKillTime(const int64_t addr) {
  s4u::Actor *actor = (s4u::Actor*) addr;
  return actor->getKillTime();
}

void rsg::RsgActorHandler::killAll() {
  s4u::Actor::killAll();
}

int rsgActor() {
      boost::shared_ptr<rsg::RsgActorHandler> handler(new rsg::RsgActorHandler());
      boost::shared_ptr<rsg::RsgMailboxHandler> mbHandler(new rsg::RsgMailboxHandler());
      boost::shared_ptr<rsg::RsgHostHandler> hostHandler(new rsg::RsgHostHandler());
      boost::shared_ptr<rsg::RsgCommHandler> commHandler(new rsg::RsgCommHandler());

      TMultiplexedProcessor* processor = new TMultiplexedProcessor();

      processor->registerProcessor(
          "RsgActor",
          boost::shared_ptr<RsgActorProcessor>(new RsgActorProcessor(handler)));

      processor->registerProcessor(
          "RsgMailbox",
          boost::shared_ptr<RsgMailboxProcessor>(new RsgMailboxProcessor(mbHandler)));

      processor->registerProcessor(
          "RsgHost",
          boost::shared_ptr<RsgHostProcessor>(new RsgHostProcessor(hostHandler)));

      processor->registerProcessor(
          "RsgComm",
          boost::shared_ptr<RsgCommProcessor>(new RsgCommProcessor(commHandler)));

      TServerFramework *server = SocketServer::getSocketServer().acceptClient(processor);

      handler->setServer(server);
      server->serve();
      delete server;
      return 1;
}

int64_t rsg::RsgActorHandler::createActor(const std::string& name, const int64_t hostAddr, const int32_t killTime) {
  s4u::Host *host = (s4u::Host*)hostAddr;
  new simgrid::s4u::Actor(name.c_str(), host, rsgActor);
 
  return 0;
}

