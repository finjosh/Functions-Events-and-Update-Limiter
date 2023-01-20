#ifndef TERMINATINGFUNCTION_H
#define TERMINATINGFUNCTION_H

#pragma once

#include <unordered_set>

#include "funcHelper.h"

enum StateType
{
    Finished = 0,
    Running = 1
};

class TerminatingFunction
{
public:
    // @param deltaTime the deltaTime in seconds
    static void UpdateFunctions(float deltaTime);
    // @param replace if there is already the same function currently running does it get replaced
    // @warning The function will only be added if it is valid
    static void Add(funcHelper<StateType> function, bool replace = false);
    static void clear();
    static void erase(funcHelper<StateType> function);
    static float getDeltaTime();
    static std::unordered_set<funcHelper<StateType>> terminatingFunctions;
private:
    static float deltaTime;
};

typedef funcHelper<StateType> TFunc;

#endif // TERMINATINGFUNCTION_H
