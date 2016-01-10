#pragma once

#include <string>
#include <vector>
#include <utility>
#include "memory.h"

class Assignment : public std::pair<std::string, shared_ptr<class Expression>>
{
public:
    Assignment(std::string name) { first = name; second = shared_ptr<class Expression>(); }
    Assignment(std::string name, shared_ptr<class Expression> expr) { first = name; second = expr; }
};

typedef std::vector<Assignment> AssignmentList;
typedef std::vector<class ModuleInstantiation*> ModuleInstantiationList;
