#include "qps/parser/entities/synonym.hpp"
#include <memory>

namespace qps {
auto Synonym::is_keyword_match(const std::string& str) const -> bool {
    return str == get_keyword();
}

auto Synonym::get_name() const -> IDENT {
    return name;
}

auto Synonym::operator==(const Synonym& rhs) const noexcept -> bool {
    return typeid(*this) == typeid(rhs) && name == rhs.name;
}

auto operator<<(std::ostream& os, const Synonym& synonym) -> std::ostream& {
    return os << synonym.get_keyword() << "(" << synonym.name << ")";
}

auto AnyStmtSynonym::representation() const -> std::string {
    return "AnyStmtSynonym(" + get_name().get_value() + ")";
}

auto ReadSynonym::representation() const -> std::string {
    return "ReadSynonym(" + get_name().get_value() + ")";
}

auto PrintSynonym::representation() const -> std::string {
    return "PrintSynonym(" + get_name().get_value() + ")";
}

auto CallSynonym::representation() const -> std::string {
    return "CallSynonym(" + get_name().get_value() + ")";
}

auto WhileSynonym::representation() const -> std::string {
    return "WhileSynonym(" + get_name().get_value() + ")";
}

auto IfSynonym::representation() const -> std::string {
    return "IfSynonym(" + get_name().get_value() + ")";
}

auto AssignSynonym::representation() const -> std::string {
    return "AssignSynonym(" + get_name().get_value() + ")";
}

auto ProcSynonym::representation() const -> std::string {
    return "ProcSynonym(" + get_name().get_value() + ")";
}

auto VarSynonym::representation() const -> std::string {
    return "VarSynonym(" + get_name().get_value() + ")";
}

auto ConstSynonym::representation() const -> std::string {
    return "ConstSynonym(" + get_name().get_value() + ")";
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

auto is_stmt_synonym(const std::shared_ptr<StmtSynonym>& synonym) -> bool {
    return true;
}

auto is_stmt_synonym(const std::shared_ptr<Synonym>& synonym) -> bool {
    return std::dynamic_pointer_cast<StmtSynonym>(synonym) != nullptr;
}

} // namespace qps
