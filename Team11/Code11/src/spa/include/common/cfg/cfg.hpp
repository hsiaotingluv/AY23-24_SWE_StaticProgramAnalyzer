#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace sp {

class CfgNode { // Per Statement
    using StatementNumbers = std::vector<std::string>;
    StatementNumbers statement_numbers;

  public:
    explicit CfgNode() {};
    virtual ~CfgNode() = default;
};

class Cfg { // Per Procedure
    using NextNodes = std::unordered_map<bool, std::shared_ptr<CfgNode>>; // True -> Next, False -> Else. Represents the two paths that if/while can take in a CFG.
    using CfgAdjList = std::unordered_map<std::shared_ptr<CfgNode>, NextNodes>; // Adjacency List of CfgNodes. CfgNode -> NextNodes (either 1 or 2 paths).
    using CfgInfo = std::pair<std::shared_ptr<CfgNode>, CfgAdjList>; // Complete Traversible Graph: Tuple of Start CfgNode + Adjacency List.
    NextNodes next_nodes{};
    CfgAdjList cfg_adj_list{};
    CfgInfo cfg_info{};

  public:
    explicit Cfg() {};
    virtual ~Cfg() = default;
};

class ProgramCfgStore { // Per Program
  using ProcMap = std::unordered_map<std::string, std::shared_ptr<Cfg>>; // Maps Procedure Statement Number to Cfg.
  ProcMap proc_map{};

  public:
    explicit ProgramCfgStore() {};
    virtual ~ProgramCfgStore() = default;
};

}