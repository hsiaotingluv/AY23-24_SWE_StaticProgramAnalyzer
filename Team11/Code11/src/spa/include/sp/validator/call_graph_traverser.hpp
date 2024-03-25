#pragma once

#include "pkb/facades/write_facade.h"
#include "sp/validator/semantic_validator.hpp"

#include <memory>
#include <utility>

namespace sp {
class CallGraphTraverser {
    std::shared_ptr<pkb::WriteFacade> write_facade;

  public:
    CallGraphTraverser() = default;

    explicit CallGraphTraverser(std::shared_ptr<pkb::WriteFacade> write_facade)
        : write_facade(std::move(write_facade)) {
    }

    auto traverse(const SemanticValidator::CallGraph& call_graph) const -> void {
        if (write_facade == nullptr) {
            return;
        }

        for (const auto& [callee, callers] : call_graph) {
            for (const auto& caller : callers) {
                write_facade->add_calls(caller, callee);
            }
        }
    };
};

} // namespace sp
