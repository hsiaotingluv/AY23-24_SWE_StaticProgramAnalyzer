#pragma once

#include "common/cfg/cfg.hpp"
#include "pkb/facades/write_facade.h"
#include "sp/cfg/program_cfgs.hpp"

namespace sp {

class AffectsTraverser {
    std::shared_ptr<pkb::WriteFacade> write_facade;

  private:
    auto get_statement_numbers(const std::shared_ptr<ProcedureCfg>& cfg) -> StatementNumbers;
    auto send_to_pkb(const std::string proc_name, const StatementNumbers& stmt_nos) -> StatementNumbers;

  public:
    explicit AffectsTraverser(std::shared_ptr<pkb::WriteFacade> write_facade) : write_facade(std::move(write_facade)) {
    }

    auto traverse(const ProcMap& cfgs) -> std::unordered_map<std::string, StatementNumbers>;
};
} // namespace sp