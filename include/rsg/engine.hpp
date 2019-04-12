/* Copyright (c) 2015. The SimGrid Team. All rights reserved.              */

/* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU Affero Licence (see in file LICENCE).        */

#pragma once

namespace simgrid {
    namespace rsg {
        
        double getClock();
	// ask rsg if this thread is an rsg client
	// if we can make calls to rsg 
	bool isClient();
    } // namespace simgrid::rsg
} // namespace simgrid
