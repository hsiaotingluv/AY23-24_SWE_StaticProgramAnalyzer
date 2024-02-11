#pragma once

#include "sp/annotator/annotator.hpp"
#include "sp/annotator/stmt_num_annotator.hpp"
#include <array>
#include <memory>

namespace sp {
class AstAnnotator : public Annotator {
  private:
    static inline const auto annotators =
        std::array<std::shared_ptr<Annotator>, 1>{std::make_shared<StmtNumAnnotator>()};

  public:
    auto annotate(std::shared_ptr<AstNode> ast) -> std::shared_ptr<AstNode> override;
};

} // namespace sp