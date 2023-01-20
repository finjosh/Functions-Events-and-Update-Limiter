#ifndef EVENTHELPER_H
#define EVENTHELPER_H

#pragma once

#include <unordered_set>
#include <deque>
#include <thread>
#include <condition_variable>
#include <mutex>

#include "funcHelper.h"

// @brief StartEvents is called automatically at creation which makes a new thread if there is not already one
class EventHelper
{
    public:
        EventHelper() = default;

        // @brief When EventHelper has a key (void*) it means that only that class can call the EventHelper
        // @param key is a (void*) to the class that you want to be the key (void*) to this EventHelper
        EventHelper(void* key);
        
        // @attention Do NOT use a lambda unless you are going to clear all this events functions later as you can NOT search for a lambda function
        void add(funcHelper<void> funcHelper);

        void clear();
        
        void remove(func funcHelper);

        size_t size();

        // @brief Makes this EventHelper keyless (void*)
        // @warning If there is already a key (void*) for this EventHelper and the key (void*) given is not the key (void*) then nothing will happen
        // @param currentKey The current key (void*) of this EventHelper
        void clearKey(void* currentKey);

        // @brief When a key (void*) is set this EventHelper then it can only be used fully when that key (void*) is given
        // @warning If there is already a key (void*) for this EventHelper then nothing will happen UNLESS you give the current key along with the new key
        // @param key is a (void*) to the class that you want to be the key (void*) to this EventHelper
        // @param currentKey If you are reseting the key (void*) then fill in this variable
        void setKey(void* key, void* currentKey = nullptr);

        // @result true, if the key (void*) in question is the key (void*) of this EventHelper this function
        // @param questionedKey The key (void*) that is in question
        bool isThisKey(void* questionedKey);

        // @result true, if this EventHelper has an key (void*)
        bool hasKey();

        // @brief If there is no key (void*) for this EventHelper then no input is required
        // @attention using this operator without giving the key (void*) NOTHING will happen
        // @param key is a (void*) to the current key (void*) of this EventHelper
        void operator() (std::string name, void* key = nullptr);

        // @brief If there is no key (void*) for this EventHelper then no input is required
        // @attention using this operator without giving the key (void*) NOTHING will happen
        // @param key is a (void*) to the current key (void*) of this EventHelper
        // @warning this will call THIS events functions that are attached and clear them (they will not be called at the normal event update time)
        void ForceCallEvents(void* key = nullptr);

        // update has to be called to simplify the code as there will no longer be any need to deal with threads (atomic vars, mutex, ect)

        // @attention if you would like to call the event right away you can use the operator()
        // @warning this is for the main update of events that are being added from other threads 
        // @warning ONLY CALL ONCE
        static void Update();

    protected:
        std::unordered_set<funcHelper<void>> functions;
        void* key = nullptr;
        
        static std::deque<EventHelper*> eventQueue;
};

#endif // EVENTHELPER_H
