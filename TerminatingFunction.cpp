#include "TerminatingFunction.h"

std::unordered_multiset<funcHelper<StateType>> TerminatingFunction::terminatingFunctions;
double TerminatingFunction::deltaTime = 0;

void TerminatingFunction::UpdateFunctions(double deltaTime)
{
    TerminatingFunction::deltaTime = deltaTime;
    auto function = TerminatingFunction::terminatingFunctions.begin();
    while (function != TerminatingFunction::terminatingFunctions.end())
    {
        if (!function->valid() || function->invoke() == StateType::Finished)
        {
            auto temp = function;
            function++;
            TerminatingFunction::terminatingFunctions.erase(temp);
            if (TerminatingFunction::terminatingFunctions.size() == 0) return;
            continue;
        }
        function++;
    }
}

void TerminatingFunction::Add(funcHelper<StateType> function, bool replace)
{ 
    if (function.valid())
    {
        if (replace && TerminatingFunction::terminatingFunctions.contains(function))
        {
            TerminatingFunction::terminatingFunctions.erase(function);
            TerminatingFunction::terminatingFunctions.emplace(function); 
        }
        else
            TerminatingFunction::terminatingFunctions.emplace(function); 
    }
}

void TerminatingFunction::clear()
{ TerminatingFunction::terminatingFunctions.clear(); }

void TerminatingFunction::erase(funcHelper<StateType> function, bool all)
{
    if (all)
    {
        TerminatingFunction::terminatingFunctions.erase(function); 
        return;
    }
    TerminatingFunction::terminatingFunctions.erase(TerminatingFunction::terminatingFunctions.find(function));
}

double TerminatingFunction::getDeltaTime()
{ return TerminatingFunction::deltaTime; }
