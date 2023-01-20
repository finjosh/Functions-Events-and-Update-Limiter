#include "TerminatingFunction.h"

std::unordered_set<funcHelper<StateType>> TerminatingFunction::terminatingFunctions;
float TerminatingFunction::deltaTime = 0;

void TerminatingFunction::UpdateFunctions(float deltaTime)
{
    TerminatingFunction::deltaTime = deltaTime;
    for (auto function = TerminatingFunction::terminatingFunctions.begin(); function != TerminatingFunction::terminatingFunctions.end(); function++)
    {
        if (!function->valid()) return;
        if (function->invoke() == StateType::Finished)
        {
            TerminatingFunction::terminatingFunctions.erase(function);
            if (TerminatingFunction::terminatingFunctions.size() == 0) return;
        }
    }
}

void TerminatingFunction::Add(funcHelper<StateType> function, bool replace)
{ 
    if (function.valid())
    {
        if (TerminatingFunction::terminatingFunctions.contains(function) && replace)
        {
            TerminatingFunction::terminatingFunctions.erase(function);
            TerminatingFunction::terminatingFunctions.insert({function}); 
        }
        else
            TerminatingFunction::terminatingFunctions.insert({function}); 
    }
}

void TerminatingFunction::clear()
{ TerminatingFunction::terminatingFunctions.clear(); }

void TerminatingFunction::erase(funcHelper<StateType> function)
{ TerminatingFunction::terminatingFunctions.erase(TerminatingFunction::terminatingFunctions.find(function)); }

float TerminatingFunction::getDeltaTime()
{ return TerminatingFunction::deltaTime; }
