#include <xbt.h>
#include <simgrid/s4u.hpp>
#include <iostream>

#include "../services.hpp"
#include "../../common.hpp"

using namespace ::apache::thrift::server;
using namespace  ::RsgService;

using namespace  ::simgrid;

XBT_LOG_NEW_CATEGORY(SERVER,"Remote SimGrid");
XBT_LOG_NEW_DEFAULT_SUBCATEGORY(RSG_ACTOR, SERVER, "RSG::Actor");

rsg::RsgHostHandler::RsgHostHandler() : pSelf(*s4u::Host::current()) {
}

int64_t rsg::RsgHostHandler::by_name(const std::string& name) {
    const char *c_name = name.c_str();
    s4u::Host *host = s4u::Host::by_name(c_name);
    
    if(host == nullptr) {
        xbt_die("No such Host (%s)", name.c_str());
        return 0;
    }
    return (int64_t)host;
}

void rsg::RsgHostHandler::current(rsgHostCurrentResType& _return) {
    debug_server_print("LALALALALLALALALA");
    s4u::Host *host = s4u::Host::current();
    debug_server_print("LALALALALLALALALA %p", host);
    _return.name = host->name();
    _return.addr = (unsigned long int) host;
    debug_server_print("LALALALALLALALALA ret:%s", _return.name.c_str());
}

double rsg::RsgHostHandler::speed(const int64_t addr) {
    s4u::Host *host = (s4u::Host*) addr;
    return (double) host->speed();
}

void rsg::RsgHostHandler::turnOn(const int64_t addr) {
    s4u::Host *host = (s4u::Host*) addr;
    host->turnOn();
}

void rsg::RsgHostHandler::turnOff(const int64_t addr) {
    s4u::Host *host = (s4u::Host*) addr;
    host->turnOff();
}

bool rsg::RsgHostHandler::isOn(const int64_t addr) {
    s4u::Host *host = (s4u::Host*) addr;
    return host->isOn();
}

double rsg::RsgHostHandler::currentPowerPeak(const int64_t addr) {
    s4u::Host *host = (s4u::Host*) addr;
    return host->getPstateSpeedCurrent(); //FIXME Change the name to fit s4u
}

double rsg::RsgHostHandler::powerPeakAt(const int64_t addr, const int32_t pstate_index) {
    s4u::Host *host = (s4u::Host*) addr;
    return host->getPstateSpeed((int) pstate_index); //FIXME Change the name to fit s4u
}

int32_t rsg::RsgHostHandler::pstatesCount(const int64_t addr) {
    s4u::Host *host = (s4u::Host*) addr;
    return host->pstatesCount();
}

void rsg::RsgHostHandler::setPstate(const int64_t addr, const int32_t pstate_index) {
    s4u::Host *host = (s4u::Host*) addr;
    host->setPstate(pstate_index);
}

int32_t rsg::RsgHostHandler::pstate(const int64_t addr) {
    s4u::Host *host = (s4u::Host*) addr;
    return host->pstate();
}


int32_t rsg::RsgHostHandler::coreCount(const int64_t addr) {
    s4u::Host *host = (s4u::Host*) addr;
    return host->coreCount();
}

void rsg::RsgHostHandler::getProperty(std::string& _return, const int64_t remoteAddr, const std::string& key) {
    s4u::Host *host = (s4u::Host*) remoteAddr;
    const char *prop = host->property(key.c_str());
    if(prop != NULL) {
        _return = std::string(prop); 
    } else {
        _return = ""; //FIXME Is that the right thing to do if the prop does not exists ? 
    }
    
}

void rsg::RsgHostHandler::setProperty(const int64_t remoteAddr, const std::string& key, const std::string& data) {
    s4u::Host *host = (s4u::Host*) remoteAddr;
    host->setProperty(key.c_str(), data.c_str());
}
