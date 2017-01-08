/* Copyright (c) 2006-2016. The SimGrid Team. All rights reserved.          */

/* This program is free software; you can redistribute it and/or modify it
* under the terms of the license (GNU LGPL) which comes with this package. */

#ifndef SIMGRID_RSG_MUTEX_HPP
#define SIMGRID_RSG_MUTEX_HPP

#include <rsg/conditionVariable.hpp>

namespace simgrid {namespace rsg {
        
        class ConditionVariable;
        class Mutex {
            friend ConditionVariable;
        public:
            Mutex();
            ~Mutex();
            
        public:
            void lock();
            void unlock();
            bool try_lock();
            void destroy();
            
        private:
            unsigned long int p_remoteAddr = 0;
        };
}} // namespace simgrid::rsg

#endif /* SIMGRID_RSG_MUTEX_HPP */
