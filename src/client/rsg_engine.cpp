#include "rsg/engine.hpp"
#include "RsgClient.hpp"


using namespace ::simgrid;

double rsg::getClock() {
    return client->engine->getClock();
}

bool rsg::isClient() {
    return client != NULL;
}
