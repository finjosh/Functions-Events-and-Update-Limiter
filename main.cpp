#include <iostream>
#include <chrono>

#include "funcHelper.h"
// requires no setup

#include "UpdateLimiter.h"
// requires no setup

#include "TerminatingFunction.h"
// Requires funcHelper and C++20

// requires limited setup
//  - Call TerminatingFunction::UpdateFunctions(deltaTime); at the start of the thread update
//  - "deltaTime" is a float in seconds this will allow any function to get access to the delta time as the getter function is static

// Template to make a function that can be used with "TerminatingFunction"
// Requirements - Return a StateType
// Template:
StateType TemplateFunction(float* inputVar)
{
    if (*inputVar >= 10)
    {
        delete(inputVar);
        std::cout << "Finished Adding to Input" << std::endl;
        return StateType::Finished;
    }

    *(inputVar) += 0.5;
    std::cout << "Added to Terminating Function Template Input: " << std::to_string(*inputVar) << std::endl;
    return StateType::Running;
}
// ---------
// to use this you can use the TFunc when adding functions to the terminating function list
// if you dont use the TFunc you can use a normal funcHelper which returns type StateType (funcHelper<StateType>)
// example: TerminatingFunction::Add(TFunc(TemplateFunction(float(10))));
//          OR TerminatingFunction::Add(funcHelper<StateType>(TemplateFunction(float(10))));

#include "EventHelper.h"
// Requires funcHelper
// to make a function that can be use in a Event make sure that it does not return anything

// requires limited setup
// - Call EventHelper::Update() at the beginning of each update of ONE thread

void TestForUpdateLimit();
void TestForFuncHelper();
void Test(float* i);
void Test1(float i);
void Test2(float &i);
void Test3(float &&i);
void TestForTerminatingFunctions();
StateType TestTerminatingFunction(float* totalTime);

int main()
{
    // TestForFuncHelper();
    // TestForTerminatingFunctions();
    // TestForUpdateLimit();
}

void TestForUpdateLimit()
{
    // set update limit at creation (can be changed at any time)
    UpdateLimiter UpdateLimit(10);

    int secondsPassed = 0;
    int updatesPerSecond = 0;
    std::chrono::steady_clock secondClock;
    std::chrono::_V2::steady_clock::time_point nextSecond = secondClock.now() + std::chrono::seconds(1);

    while (secondsPassed <= 5)
    {
        if (secondClock.now() >= nextSecond)
        {
            std::cout << "Updates Last Second: " << updatesPerSecond << std::endl;
            updatesPerSecond = 0;
            secondsPassed++;
            nextSecond = secondClock.now() + std::chrono::seconds(1);
        }
        ++updatesPerSecond;

        // wait till the next update cycle is ready
        UpdateLimit.wait();
    }
}

void TestForFuncHelper()
{
    using std::cout, std::endl;
    
    // To use the funcHelper class with a function that returns a void you can use func which is equivalent to "funcHelper<void>"

    float* tempFloatP = new float(10);
    func firstFunction(Test, tempFloatP);
    // can also be written as "funcHelper<void> firstFunction(Test, tempFloatP);"
    cout << "First function name: " << firstFunction.getName() << endl;

    // The first and second function names will not be the same as they have different pointers in the input
    func secondFunction(Test, new float(10));
    cout << "Second function name: " << secondFunction.getName() << endl;

    // The first and third function names will be the same as they have the same pointer as the input
    func thirdFunction(Test, tempFloatP);
    cout << "Third function name: " << thirdFunction.getName() << endl;

    cout << "\nNext Test\n" << endl;

    // any function that is Test1 and given the same value will have the same name / be the "same"
    firstFunction.setFunction(Test1, 7);
    cout << "First function name: " << firstFunction.getName() << endl;

    secondFunction.setFunction(Test1, 7);
    cout << "Second function name: " << secondFunction.getName() << endl;

    // and any function that is Test1 with a different value will have a different name / not be the same
    thirdFunction.setFunction(Test1, 10);
    cout << "Third function name: " << thirdFunction.getName() << endl;

    cout << "\nNext Test\n" << endl;

    // Will not compile as references are not able to be used with the funcHelper
    /*
        int i = 10;

        firstFunction.setFunction(Test2, i);
    */
   
    // next test

    firstFunction.setFunction(Test3, 30);
    cout << "First function name: " << firstFunction.getName() << endl;   

    // the names will not be the same as the values are not the same
    secondFunction.setFunction(Test3, 20);
    cout << "Second function name: " << secondFunction.getName() << endl;

    // the second and third functions will have the same name as the input variables have the same value
    thirdFunction.setFunction(Test3, 20);
    cout << "Third function name: " << thirdFunction.getName() << endl;
}

void Test(float* i)
{
    std::cout << *i << std::endl;
}

void Test1(float i)
{
    std::cout << i << std::endl;
}

void Test2(float &i)
{
    std::cout << i << std::endl;
}

void Test3(float &&i)
{
    std::cout << i << std::endl;
}

void TestForTerminatingFunctions()
{
    // when testing terminating functions I am going to use the UpdateLimiter to show the functionality of terminating functions for 
    // tasks which are wanted to be done overtime. This is more useful when wanting to create an "animation" of sorts.
    
    UpdateLimiter updateLimit(2);
    
    // as the time between updates is not constant I can't just hard code the deltaTime
    clock_t start, end;

    // this is programmed to run till the inputted value is >= 10 while add 0.5 each update
    TerminatingFunction::Add(TFunc(TemplateFunction, new float(-5)));
    // this is programmed to run for 15 seconds while outputting the deltaTime
    TerminatingFunction::Add(TFunc(TestTerminatingFunction, new float(0)));

    while (TerminatingFunction::terminatingFunctions.size() > 0)
    {
        double deltaTime = double(end - start) / double(CLOCKS_PER_SEC);
        // calling this will update all the terminating functions 
        TerminatingFunction::UpdateFunctions(deltaTime);
        start = clock();
        updateLimit.wait();
        end = clock();
    }
}

StateType TestTerminatingFunction(float* totalTime)
{
    *totalTime += TerminatingFunction::getDeltaTime();
    if (*totalTime >= 15) 
    {
        delete(totalTime);
        return StateType::Finished;
    }

    std::cout << std::to_string(TerminatingFunction::getDeltaTime()) << std::endl;

    return StateType::Running;
}
