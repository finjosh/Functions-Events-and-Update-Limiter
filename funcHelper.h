#ifndef FUNCHELPER_H
#define FUNCHELPER_H

#pragma once

#include <functional>
#include <utility>
#include <optional>
#include <string>
#include <sstream>

template<typename _ReturnType = void>
class funcHelper
{
public:
    funcHelper() = default;
    ~funcHelper() = default;

    template<typename _Callable, typename... _Arg>
    funcHelper(_Callable __function, _Arg&&... __args)
    { this->setFunction(std::forward<_Callable>(__function), std::forward<_Arg>(__args)...); this->UpdateName(__function, __args...); }

    template<typename _Callable, typename... _Arg>
    void setFunction(_Callable __function, _Arg&&... __args) 
    { this->_function = std::bind(std::forward<_Callable>(__function), std::forward<_Arg>(__args)...); this->UpdateName(__function, __args...); }

    // @returns true, if the function held is valid (not a nullptr)
    bool valid() const { return (bool)_function; }

    // @brief invokes the currently set function
    _ReturnType invoke() const
    { return this->_function(); }

    // @brief invokes the currently set function
    _ReturnType operator() () const
    { return this->_function(); }

    std::string getName() const
    { return this->name; }

    friend bool operator< (const funcHelper& _funcHelper, const funcHelper& _funcHelper2)
    { return _funcHelper.name < _funcHelper2.name; }

    friend bool operator== (const funcHelper& _funcHelper, const funcHelper& _funcHelper2)
    { return _funcHelper.name == _funcHelper2.name; }

    friend std::size_t std::hash<funcHelper<_ReturnType>>::operator()(const funcHelper<_ReturnType>& func) const;

private:
    //* storage of the _function
    std::function<_ReturnType()> _function;
    std::string name = "";

    template<typename _Callable, typename... _Arg>
    void UpdateName(_Callable function, _Arg... args)
    {
        std::ostringstream creatingString;
        creatingString << typeid(function).name() << "(";
        (creatingString << ... << &args) << ")";
        name = creatingString.str();
    }
};

// allowing this class to be used in a hash table (unsigned_map, unsigned_set, ect)
namespace std {
    template<typename _ReturnType>
    struct hash<funcHelper<_ReturnType>> {
        std::size_t operator()(const funcHelper<_ReturnType>& func) const
        {
            // You can use the std::hash function to hash the type name of the function
            return std::hash<std::string>()(func.name);
        }
    };
}

// allowing funcHelper to be referenced as "func"
typedef funcHelper<void> func;

#endif // FUNCHELPER_H
