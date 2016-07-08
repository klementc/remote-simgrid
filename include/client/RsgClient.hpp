/* Copyright (c) 2015. The SimGrid Team. All rights reserved.              */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Affero Licence (see in file LICENCE).        */

#ifndef SRC_ENGINE_HPP_
#define SRC_ENGINE_HPP_

#include <xbt.h>
#include <vector>
#include <string>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TMultiplexedProtocol.h>

#include <boost/unordered_map.hpp>
#include "rsg/services.hpp"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class IDel {
	public :
	IDel() {};
	virtual ~IDel(){};
	virtual void operator()(void *o) = 0;
};
 
template<class Obj> class ObjDeleter : public  IDel {
	public :
	ObjDeleter() {};
	void operator()(void *o) {
			Obj *obj = (Obj*) o;
			delete obj;
	}
};

/**
 * The engine will create the connection to the rpcServer. First of all engine try to connect to the localhost.
 * Once the first connection is made, the Engine will wait for the entry point to be able to connect to the rpc server.
 */
class Client {

public:

	boost::shared_ptr<TMultiplexedProtocol>  getMultiplexedProtocol(std::string serviceName) const;
	boost::shared_ptr<TBinaryProtocol> getProtocol() const ;
	boost::shared_ptr<TBufferedTransport> getTransport() const;

	void reset();
	
	/**
	* Because thrift doesn't create a generic interface for all clients, we have to store it into a void*.
	* It is an issue to delete the object when it is done.
	*/
	template<class ServiceType> ServiceType& serviceClientFactory(std::string name) {			
			ServiceType * res;
			try {
				res = static_cast<ServiceType*>(pServices->at(name));
			} catch (std::out_of_range& e) {
				res = new ServiceType(getMultiplexedProtocol(name));
				ObjDeleter<ServiceType> *del = new  ObjDeleter<ServiceType>();
				pDestructors->insert({name, del});
				pServices->insert({name, res});
			}
			return *res;
	};
	
	
	void close();	
	void init();	
	void connectToRpc(int rpcPort);	
	void connect();

	Client(std::string hostname, int port);
	Client(std::string hostname);

private: 

	int pSock;
  std::string pHostname;
	int pPort;
	boost::shared_ptr<TBinaryProtocol> pProtocol;
	boost::shared_ptr<TBufferedTransport> pTransport;
	
	boost::unordered_map<std::string, void *> *pServices; 
	boost::unordered_map<std::string, IDel* > *pDestructors;
	
};

#endif /* SRC_ENGINE_HPP_ */
