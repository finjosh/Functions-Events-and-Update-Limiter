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
    // @brief Calling this function will call all Terminating Functions and erase ther from the list if they are finished running
    // @param deltaTime the deltaTime in seconds
    static void UpdateFunctions(double deltaTime);
    // @param replace if there is already the same function currently running does it get replaced
    // @warning The function will only be added if it is valid
    static void Add(funcHelper<StateType> function, bool replace = false);
    // @brief clears all terminating functions from the list 
    static void clear();
    // @brief erases the given function from the terminating functions list IF there is one in the list
    // @warning has to have the EXACT same function (input vars included)
    static void erase(funcHelper<StateType> function);
    // @brief returns the deltaTime from the last time it was updated
    // @warning does NOT update unless the UpdateFunctions function is called
    static double getDeltaTime();
private:
    static double deltaTime;
    static std::unordered_set<funcHelper<StateType>> terminatingFunctions;
};

typedef funcHelper<StateType> TFunc;

#endif // TERMINATINGFUNCTION_H
