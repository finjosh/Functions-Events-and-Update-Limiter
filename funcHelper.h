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

    template<typename Func, typename... BoundArgs, typename std::enable_if_t<std::is_convertible<Func, std::function<void(const BoundArgs&...)>>::value>* = nullptr>
    funcHelper(Func& _function, BoundArgs&&... args)
    { this->setFunction(_function, args...); }

    template<typename Func, typename... BoundArgs, typename std::enable_if_t<std::is_convertible<Func, std::function<void(const BoundArgs&...)>>::value>* = nullptr>
    void setFunction(Func& _function, BoundArgs&&... args) 
    { this->__function = std::bind(std::forward<Func>(_function), std::forward<BoundArgs>(args)...); this->UpdateName(_function, args...); }

    // @returns true, if the function held is valid (not a nullptr)
    bool valid() const { return (bool)__function; }

    // @brief invokes the currently set function
    _ReturnType invoke() const
    { return this->__function(); }

    // @brief invokes the currently set function
    _ReturnType operator() () const
    { return this->__function(); }

    std::string getName() const
    { return this->name; }

    friend bool operator< (const funcHelper& _funcHelper, const funcHelper& _funcHelper2)
    { return _funcHelper.name < _funcHelper2.name; }

    friend bool operator== (const funcHelper& _funcHelper, const funcHelper& _funcHelper2)
    { return _funcHelper.name == _funcHelper2.name; }

    friend std::size_t std::hash<funcHelper<_ReturnType>>::operator()(const funcHelper<_ReturnType>& func) const;

private:
    //* storage of the _function
    std::function<_ReturnType()> __function;
    std::string name = "";

    template<typename Func, typename... BoundArgs>
    void UpdateName(Func function, BoundArgs... args)
    {
        std::ostringstream creatingString;
        creatingString << typeid(function).name();
        (creatingString << ... << args);
        name = creatingString.str();
    }
};

// allowing this class to be used in a hash table
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
