#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using PROC = short;

class TNode;

class VarTable; // no need to #include "VarTable.h" as all I need is pointer

class PKB {
  public:
    static VarTable *varTable;
    static auto setProcToAST(PROC p, TNode *r) -> int;
    static auto getRootAST(PROC p) -> TNode *;
};