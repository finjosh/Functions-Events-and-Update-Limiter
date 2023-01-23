#include "EventHelper.h"

std::deque<EventHelper*> EventHelper::eventQueue;

EventHelper::EventHelper(void* key)
{ this->key = key; }

void EventHelper::addListener(funcHelper<void> funcHelper)
{ this->Listeners.insert(funcHelper); }

void EventHelper::clearListeners()
{ this->Listeners.clear(); }

void EventHelper::removeListener(func funcHelper)
{ this->Listeners.erase(funcHelper); }

size_t EventHelper::size()
{ return this->Listeners.size(); }

void EventHelper::clearKey(void* currentKey)
{ if (this->key == currentKey) this->key = nullptr; }

void EventHelper::setKey(void* key, void* currentKey)
{ if (this->key == nullptr || this->key == currentKey) this->key = key; }

bool EventHelper::isThisKey(void* questionedKey)
{ return (this->key == questionedKey); }

bool EventHelper::hasKey()
{ return (this->key != nullptr); }

void EventHelper::operator() (void* key)
{ if (this->key == key) EventHelper::eventQueue.push_back(this); }

void EventHelper::ForceCallEvent(void* key)
{
    for (auto* event: EventHelper::eventQueue)
    {
        for (funcHelper<void> function: event->Listeners) 
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
        for (funcHelper<void> function: event->Listeners) 
        {
            function(); 
        }
    }
    EventHelper::eventQueue.clear();
}
