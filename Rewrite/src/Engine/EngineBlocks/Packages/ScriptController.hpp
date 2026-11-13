#pragma once
#include <Package.hpp>
#include <Script.hpp>
class ScriptController : public Package{
    public:
        ScriptController() = default;
        Script* script;
};