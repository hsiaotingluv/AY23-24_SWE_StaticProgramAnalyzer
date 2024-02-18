#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {
auto Synonym::get_name() const -> IDENT {
    return name;
}

auto Synonym::operator==(const Synonym& rhs) const noexcept -> bool {
    return typeid(*this) == typeid(rhs) && name == rhs.name;
}

auto Synonym::operator<(const Synonym& rhs) const noexcept -> bool {
    return name.get_value() < rhs.name.get_value();
}

auto operator<<(std::ostream& os, const Synonym& synonym) -> std::ostream& {
    return os << synonym.representation() << "(" << synonym.name << ")";
}

auto AnyStmtSynonym::representation() const -> std::string {
    return "AnyStmtSynonym";
}

auto ReadSynonym::representation() const -> std::string {
    return "ReadSynonym";
}

auto PrintSynonym::representation() const -> std::string {
    return "PrintSynonym";
}

auto CallSynonym::representation() const -> std::string {
    return "CallSynonym";
}

auto WhileSynonym::representation() const -> std::string {
    return "WhileSynonym";
}

auto IfSynonym::representation() const -> std::string {
    return "IfSynonym";
}

auto AssignSynonym::representation() const -> std::string {
    return "AssignSynonym";
}

auto ProcSynonym::representation() const -> std::string {
    return "ProcSynonym";
}

auto VarSynonym::representation() const -> std::string {
    return "VarSynonym";
}

auto ConstSynonym::representation() const -> std::string {
    return "ConstSynonym";
}

auto AnyStmtSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto ReadSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto PrintSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto CallSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto WhileSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto IfSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto AssignSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto ProcSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto VarSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto ConstSynonym::get_keyword() const -> std::string {
    return keyword;
}

auto is_stmt_synonym(const std::shared_ptr<Synonym>& synonym) -> bool {
    return std::dynamic_pointer_cast<StmtSynonym>(synonym) != nullptr;
}

} // namespace qps
