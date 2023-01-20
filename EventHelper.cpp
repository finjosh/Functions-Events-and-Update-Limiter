#include "EventHelper.h"

std::deque<EventHelper*> EventHelper::eventQueue;

EventHelper::EventHelper(void* key)
{ 
    this->key = key; 
}

void EventHelper::add(funcHelper<void> funcHelper)
{ this->functions.insert(funcHelper); }

void EventHelper::clear()
{ this->functions.clear(); }

void EventHelper::remove(func funcHelper)
{ this->functions.erase(funcHelper); }

size_t EventHelper::size()
{ return this->functions.size(); }

void EventHelper::clearKey(void* currentKey)
{ if (this->key == currentKey) this->key = nullptr; }

void EventHelper::setKey(void* key, void* currentKey)
{ if (this->key == nullptr || this->key == currentKey) this->key = key; }

bool EventHelper::isThisKey(void* questionedKey)
{ return (this->key == questionedKey); }

bool EventHelper::hasKey()
{ return (this->key != nullptr); }

void EventHelper::operator() (std::string name, void* key)
{ 
    // std::cout << name << std::endl;
    if (this->key == key)
        EventHelper::eventQueue.push_back(this);
}

void EventHelper::ForceCallEvents(void* key)
{
    for (auto* event: EventHelper::eventQueue)
    {
        for (funcHelper<void> function: event->functions) 
        {
            function(); 
        }
    }
    EventHelper::eventQueue.clear();
}

void EventHelper::Update()
{
    for (auto* event: EventHelper::eventQueue)
    {
        for (funcHelper<void> function: event->functions) 
        {
            function(); 
        }
    }
    EventHelper::eventQueue.clear();
}
