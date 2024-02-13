#include "qps/parser/parser.hpp"
#include "qps/evaluators/type_traits.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/expression_parser.hpp"
#include "qps/parser/parser_helper.hpp"

#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <iterator>
#include <memory>
#include <optional>
#include <tuple>
#include <type_traits>
#include <variant>

namespace qps {
auto QueryProcessingSystemParser::parse(std::string query) -> std::optional<Query> {
    const auto tokens = tokeniser_runner.apply_tokeniser(std::move(query));
    auto begin = tokens.begin();
    const auto end = tokens.end();

    // Parse declarations
    const auto maybe_declared_synonyms = parse_declarations(begin, end);
    if (!maybe_declared_synonyms.has_value()) {
        return std::nullopt;
    }
    auto declared_synonyms_success = maybe_declared_synonyms.value();
    auto& [declared_synonyms, rest] = declared_synonyms_success;
    begin = rest;

    // Parse reference
    const auto maybe_reference = parse_reference(declared_synonyms, begin, end);
    if (!maybe_reference.has_value()) {
        return std::nullopt;
    }
    auto reference_success = maybe_reference.value();
    auto& [reference, rest2] = reference_success;
    begin = rest2;

    std::vector<std::shared_ptr<Clause>> clauses{};
    auto can_still_parse = true;
    while (can_still_parse) {
        // Parse such that clauses
        const auto maybe_such_that_clause = parse_such_that_clause(declared_synonyms, begin, end);
        if (maybe_such_that_clause.has_value()) {
            const auto& [clause, rest] = maybe_such_that_clause.value();
            begin = rest;
            clauses.emplace_back(clause);
            continue;
        }

        // Parse pattern clauses
        const auto maybe_pattern_clause = parse_pattern_clause(declared_synonyms, begin, end);
        if (maybe_pattern_clause.has_value()) {
            const auto& [clause, rest] = maybe_pattern_clause.value();
            begin = rest;
            clauses.emplace_back(clause);
            continue;
        }
        can_still_parse = false;
    }

    if (begin != end) {
        // Still tokens left to parse
        return std::nullopt;
    }

    return Query{declared_synonyms, reference, clauses};
}

template <typename SynonymType>
auto build_synonyms(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::const_iterator>> {
    if (std::distance(it, end) < 2) {
        return std::nullopt;
    }

    // Match keyword
    constexpr auto expected = SynonymType::keyword;
    const auto keyword = *it;
    if (!is_keyword(keyword, expected)) {
        return std::nullopt;
    }
    it = std::next(it);

    // Try to consume first synonym
    Synonyms synonyms{};

    auto maybe_synonym = *it;
    if (!is_string(maybe_synonym)) {
        return std::nullopt;
    }
    it = std::next(it);
    synonyms.push_back(SynonymType{IDENT{maybe_synonym.content}});

    // Try to consume remaining synonyms with pattern: ',' <synonym>
    while (it != end) {
        if (!is_comma(*it)) {
            break;
        }
        it = std::next(it);

        if (it == end) {
            return std::nullopt;
        }

        const auto synonym = *it;
        if (!is_string(synonym)) {
            break;
        }
        it = std::next(it);
        synonyms.push_back(SynonymType{IDENT{synonym.content}});
    }

    // Consume ';' delimiter if it exists
    if (it != end && is_semicolon(*it)) {
        it = std::next(it);
    }
    return std::make_tuple(synonyms, it);
}

template <typename T>
auto try_declare_synonym(std::vector<Synonym>& synonyms, std::vector<Token>::const_iterator it,
                         const std::vector<Token>::const_iterator& end)
    -> std::optional<std::vector<Token>::const_iterator> {
    const auto maybe_decl = build_synonyms<T>(it, end);
    if (maybe_decl.has_value()) {
        const auto& [decl, rest] = maybe_decl.value();
        synonyms.insert(synonyms.end(), decl.begin(), decl.end());
        return rest;
    }
    return std::nullopt;
}

auto parse_declarations(std::vector<Synonym>& synonyms, std::vector<Token>::const_iterator it,
                        const std::vector<Token>::const_iterator& end, TypeList<> _)
    -> std::optional<std::vector<Token>::const_iterator> {
    return std::nullopt;
}

template <typename Head, typename... Tails>
auto parse_declarations(std::vector<Synonym>& synonyms, std::vector<Token>::const_iterator it,
                        const std::vector<Token>::const_iterator& end, TypeList<Head, Tails...> _)
    -> std::optional<std::vector<Token>::const_iterator> {
    auto maybe_it = try_declare_synonym<Head>(synonyms, it, end);
    return maybe_it.has_value() ? maybe_it.value() : parse_declarations(synonyms, it, end, TypeList<Tails...>{});
}

auto parse_declarations(std::vector<Token>::const_iterator it, const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Synonyms, std::vector<Token>::const_iterator>> {
    auto declared_synonyms = std::vector<Synonym>{};
    while (true) {
        const auto maybe_it = parse_declarations(declared_synonyms, it, end, SupportedSynonyms());
        if (!maybe_it.has_value()) {
            break;
        }
        it = maybe_it.value();
    }
    return !declared_synonyms.empty() ? std::make_optional(std::make_tuple(declared_synonyms, it)) : std::nullopt;
}

auto parse_reference(const Synonyms& declared, std::vector<Token>::const_iterator it,
                     const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Synonym, std::vector<Token>::const_iterator>> {
    if (std::distance(it, end) < 2) {
        return std::nullopt;
    }

    const auto keyword = *it;
    if (!is_keyword(keyword, "Select")) {
        return std::nullopt;
    }
    it = std::next(it);

    const auto maybe_synonym_str = *it;
    if (!is_string(maybe_synonym_str)) {
        return std::nullopt;
    }

    const auto maybe_syn = find_syn(declared, maybe_synonym_str.content);
    if (!maybe_syn.has_value()) {
        return std::nullopt;
    }

    it = std::next(it);
    return std::make_tuple(maybe_syn.value(), it);
}

// Forward declarations of helper functions
auto parse_ent_ref(const Synonyms& declared, std::vector<Token>::const_iterator it,
                   const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<EntRef, std::vector<Token>::const_iterator>>;
auto parse_rel_ref(const Synonyms& declared_synonyms, std::vector<Token>::const_iterator it,
                   const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Relationship, std::vector<Token>::const_iterator>>;

auto parse_such_that_clause(const Synonyms& declared, std::vector<Token>::const_iterator it,
                            const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<std::shared_ptr<SuchThatClause>, std::vector<Token>::const_iterator>> {
    if (std::distance(it, end) < 3) {
        return std::nullopt;
    }

    if (!is_keyword(*it, "such")) {
        return std::nullopt;
    }
    it = std::next(it);

    if (!is_keyword(*it, "that")) {
        return std::nullopt;
    }
    it = std::next(it);

    const auto maybe_rel_ref = parse_rel_ref(declared, it, end);
    if (!maybe_rel_ref.has_value()) {
        return std::nullopt;
    }
    const auto& [rel_ref, rest] = maybe_rel_ref.value();
    return std::make_tuple(std::make_shared<SuchThatClause>(rel_ref), rest);
}

auto parse_pattern_clause(const Synonyms& declared, std::vector<Token>::const_iterator it,
                          const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<std::shared_ptr<PatternClause>, std::vector<Token>::const_iterator>> {

    // pattern_cl := pattern <assign_synonym> ( <ent_ref> , <pattern_expr> )
    if (std::distance(it, end) < 6) {
        return std::nullopt;
    }

    const auto maybe_keyword = *it;
    const auto maybe_syn_assign = *std::next(it);
    const auto maybe_open_bracket = *std::next(it, 2);

    if (!is_keyword(maybe_keyword, "pattern") || !is_string(maybe_syn_assign) || !is_open_bracket(maybe_open_bracket)) {
        return std::nullopt;
    }

    // Check that synonym is a valid assign synonym
    const auto maybe_syn = find_syn(declared, maybe_syn_assign.content);
    if (!maybe_syn.has_value() || !is_assign_syn(maybe_syn.value())) {
        return std::nullopt;
    }
    const auto assign_syn = std::get<AssignSynonym>(maybe_syn.value());

    // Consume confirmed tokens
    it = std::next(it, 3);

    // Get entity reference
    const auto maybe_ent_ref = parse_ent_ref(declared, it, end);
    if (!maybe_ent_ref.has_value()) {
        return std::nullopt;
    }
    // Consume according to entity reference
    const auto& [ent_ref, rest] = maybe_ent_ref.value();
    it = rest;

    // Check for comma
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }
    const auto maybe_comma = *it;
    if (!is_comma(maybe_comma)) {
        return std::nullopt;
    }
    it = std::next(it, 1);

    // Check for expression spec
    const auto maybe_expr_spec = parse_expression_spec(it, end);
    if (!maybe_expr_spec.has_value()) {
        return std::nullopt;
    }
    const auto& [expr_spec, rest2] = maybe_expr_spec.value();
    it = rest2;

    // Check for closing bracket
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }

    const auto maybe_close_bracket = *it;
    if (!is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }
    it = std::next(it, 1);

    return std::make_tuple(std::make_shared<PatternClause>(assign_syn, ent_ref, expr_spec), it);
}

template <typename RelationshipType, bool is_direct>
auto parse_stmt_stmt(const Synonyms& declared, std::vector<Token>::const_iterator it,
                     const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<RelationshipType, std::vector<Token>::const_iterator>> {
    constexpr auto offset = is_direct ? 0 : 1;
    if (std::distance(it, end) < 6 + offset) {
        return std::nullopt;
    }

    if (!is_direct) {
        // Ancestor relationship must have a star
        const auto maybe_star = std::next(it);
        if (!is_star(*maybe_star)) {
            return std::nullopt;
        }
    }

    constexpr auto expected = RelationshipType::keyword;
    const auto& maybe_keyword = *it;
    const auto& maybe_open_bracket = *std::next(it, 1 + offset);
    const auto& maybe_first_arg = *std::next(it, 2 + offset);
    const auto& maybe_comma = *std::next(it, 3 + offset);
    const auto& maybe_second_arg = *std::next(it, 4 + offset);
    const auto& maybe_close_bracket = *std::next(it, 5 + offset);

    if (!is_keyword(maybe_keyword, expected) || !is_open_bracket(maybe_open_bracket) || !is_comma(maybe_comma) ||
        !is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }

    // Arguments must be a statement synonym (string), an underscore (wildcard), or a number (integer)
    if (!is_stmt_ref(maybe_first_arg) || !is_stmt_ref(maybe_second_arg)) {
        return std::nullopt;
    }

    const auto maybe_arg1 = get_stmt_ref(declared, maybe_first_arg);
    const auto maybe_arg2 = get_stmt_ref(declared, maybe_second_arg);

    if (!maybe_arg1.has_value() || !maybe_arg2.has_value()) {
        return std::nullopt;
    }

    const auto& arg1 = maybe_arg1.value();
    const auto& arg2 = maybe_arg2.value();

    const auto follow_rel = RelationshipType{arg1, arg2};
    return std::make_tuple(follow_rel, std::next(it, 6 + offset));
}

template <typename RelationshipType>
auto parse_stmt_ent(const Synonyms& declared, std::vector<Token>::const_iterator it,
                    const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<StmtRef, EntRef, std::vector<Token>::const_iterator>> {
    if (std::distance(it, end) < 6) {
        return std::nullopt;
    }

    constexpr auto expected = RelationshipType::keyword;
    const auto maybe_keyword = *it;
    const auto maybe_open_bracket = *std::next(it, 1);
    const auto maybe_first_arg = *std::next(it, 2);
    const auto maybe_comma = *std::next(it, 3);

    if (!is_keyword(maybe_keyword, expected) || !is_open_bracket(maybe_open_bracket) || !is_comma(maybe_comma)) {
        return std::nullopt;
    }

    // Consume confirmed tokens
    it = std::next(it, 4);

    // Get entity reference
    const auto maybe_ent_ref = parse_ent_ref(declared, it, end);
    if (!maybe_ent_ref.has_value()) {
        return std::nullopt;
    }

    // Consume according to entity reference
    const auto& [ent_ref, rest] = maybe_ent_ref.value();
    it = rest;

    // Check for closing bracket
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }
    const auto maybe_close_bracket = *it;
    if (!is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }
    it = std::next(it, 1);

    // Arguments must be a statement synonym (string), an underscore (wildcard), or a number (integer)
    if (!is_stmt_ref(maybe_first_arg)) {
        return std::nullopt;
    }
    const auto maybe_arg1 = get_stmt_ref(declared, maybe_first_arg);
    if (!maybe_arg1.has_value()) {
        return std::nullopt;
    }

    const auto& arg1 = maybe_arg1.value();

    // Require entity reference to be a VariableSynonym if it is a synonym
    if (is_synonym(ent_ref) && !is_var_syn(ent_ref)) {
        return std::nullopt;
    }

    return std::make_tuple(arg1, ent_ref, it);
}

template <typename RelationshipType>
auto parse_ent_ent(const Synonyms& declared, std::vector<Token>::const_iterator it,
                   const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<EntRef, EntRef, std::vector<Token>::const_iterator>> {
    if (std::distance(it, end) < 3) {
        return std::nullopt;
    }

    constexpr auto expected = RelationshipType::keyword;
    const auto maybe_keyword = *it;
    const auto maybe_open_bracket = *std::next(it, 1);

    if (!is_keyword(maybe_keyword, expected) || !is_open_bracket(maybe_open_bracket)) {
        return std::nullopt;
    }

    // Consume confirmed tokens
    it = std::next(it, 2);

    // Get entity reference
    const auto maybe_ent_ref1 = parse_ent_ref(declared, it, end);
    if (!maybe_ent_ref1.has_value()) {
        return std::nullopt;
    }

    const auto& [ent_ref1, rest] = maybe_ent_ref1.value();
    it = rest;

    if (is_synonym(ent_ref1) && !is_proc_syn(ent_ref1)) {
        return std::nullopt;
    }

    // Check for comma
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }

    const auto maybe_comma = *it;

    if (!is_comma(maybe_comma)) {
        return std::nullopt;
    }

    it = std::next(it, 1);

    // Check for second entity reference
    const auto maybe_ent_ref2 = parse_ent_ref(declared, it, end);
    if (!maybe_ent_ref2.has_value()) {
        return std::nullopt;
    }

    const auto& [ent_ref2, rest2] = maybe_ent_ref2.value();
    it = rest2;

    if (is_synonym(ent_ref2) && !is_var_syn(ent_ref2)) {
        return std::nullopt;
    }

    // Check for closing bracket
    if (std::distance(it, end) < 1) {
        return std::nullopt;
    }

    const auto maybe_close_bracket = *it;
    if (!is_close_bracket(maybe_close_bracket)) {
        return std::nullopt;
    }

    it = std::next(it, 1);
    return std::make_tuple(ent_ref1, ent_ref2, it);
}

// Helper functions

auto reject_wildcard(const StmtRef& stmt_ref) -> std::optional<StmtRefNoWildcard> {
    return std::visit(
        [](auto&& arg) -> std::optional<StmtRefNoWildcard> {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, WildCard>) {
                return std::nullopt;
            } else {
                return std::make_optional(StmtRefNoWildcard{arg});
            }
        },
        stmt_ref);
}

auto to_var_ref(const EntRef& ent_ref) -> std::optional<VarRef> {
    return std::visit(overloaded{[](const Synonym& x) -> std::optional<VarRef> {
                                     if (is_var_syn(x)) {
                                         return std::get<VarSynonym>(x);
                                     }
                                     return std::nullopt;
                                 },
                                 [](const auto& x) -> std::optional<VarRef> {
                                     return x;
                                 }},
                      ent_ref);
}

auto to_proc_ref(const EntRef& ent_ref) -> std::optional<ProcedureRefNoWildcard> {
    return std::visit(overloaded{[](const WildCard& x) -> std::optional<ProcedureRefNoWildcard> {
                                     return std::nullopt;
                                 },
                                 [](const Synonym& x) -> std::optional<ProcedureRefNoWildcard> {
                                     if (is_proc_syn(x)) {
                                         return std::get<ProcSynonym>(x);
                                     }
                                     return std::nullopt;
                                 },
                                 [](const auto& x) -> std::optional<ProcedureRefNoWildcard> {
                                     return x;
                                 }},
                      ent_ref);
}

auto parse_rel_ref(const Synonyms& declared_synonyms, std::vector<Token>::const_iterator it,
                   const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<Relationship, std::vector<Token>::const_iterator>> {
    const auto maybe_follows = parse_stmt_stmt<Follows, true>(declared_synonyms, it, end);
    if (maybe_follows) {
        const auto& [follow_rel, rest] = maybe_follows.value();
        return std::make_tuple(follow_rel, rest);
    }

    const auto maybe_follows_t = parse_stmt_stmt<FollowsT, false>(declared_synonyms, it, end);
    if (maybe_follows_t) {
        const auto& [follow_rel, rest] = maybe_follows_t.value();
        return std::make_tuple(follow_rel, rest);
    }

    const auto maybe_parent = parse_stmt_stmt<Parent, true>(declared_synonyms, it, end);
    if (maybe_parent) {
        const auto& [parent_rel, rest] = maybe_parent.value();
        return std::make_tuple(parent_rel, rest);
    }

    const auto maybe_parent_t = parse_stmt_stmt<ParentT, false>(declared_synonyms, it, end);
    if (maybe_parent_t) {
        const auto& [parent_rel, rest] = maybe_parent_t.value();
        return std::make_tuple(parent_rel, rest);
    }

    const auto maybe_uses_s = parse_stmt_ent<UsesS>(declared_synonyms, it, end);
    if (maybe_uses_s) {
        const auto& [stmt, ent, rest] = maybe_uses_s.value();
        const auto maybe_arg1 = reject_wildcard(stmt);
        if (!maybe_arg1.has_value()) {
            return std::nullopt;
        }

        const auto maybe_arg2 = to_var_ref(ent);
        if (!maybe_arg2.has_value()) {
            return std::nullopt;
        }
        return std::make_tuple(UsesS{maybe_arg1.value(), maybe_arg2.value()}, rest);
    }

    const auto maybe_uses_p = parse_ent_ent<UsesP>(declared_synonyms, it, end);
    if (maybe_uses_p) {
        const auto& [ent_ref1, ent_ref2, rest] = maybe_uses_p.value();

        const auto maybe_arg1 = to_proc_ref(ent_ref1);
        if (!maybe_arg1.has_value()) {
            return std::nullopt;
        }

        const auto maybe_arg2 = to_var_ref(ent_ref2);
        if (!maybe_arg2.has_value()) {
            return std::nullopt;
        }
        return std::make_tuple(UsesP{maybe_arg1.value(), maybe_arg2.value()}, rest);
    }

    const auto maybe_modifies_s = parse_stmt_ent<ModifiesS>(declared_synonyms, it, end);
    if (maybe_modifies_s) {
        const auto& [stmt, ent, rest] = maybe_modifies_s.value();
        const auto maybe_arg1 = reject_wildcard(stmt);
        if (!maybe_arg1.has_value()) {
            return std::nullopt;
        }

        const auto maybe_arg2 = to_var_ref(ent);
        if (!maybe_arg2.has_value()) {
            return std::nullopt;
        }

        return std::make_tuple(ModifiesS{maybe_arg1.value(), maybe_arg2.value()}, rest);
    }

    const auto maybe_modifies_p = parse_ent_ent<ModifiesP>(declared_synonyms, it, end);
    if (maybe_modifies_p) {
        const auto& [ent_ref1, ent_ref2, rest] = maybe_modifies_p.value();
        const auto maybe_arg1 = to_proc_ref(ent_ref1);
        if (!maybe_arg1.has_value()) {
            return std::nullopt;
        }

        const auto maybe_arg2 = to_var_ref(ent_ref2);
        if (!maybe_arg2.has_value()) {
            return std::nullopt;
        }
        return std::make_tuple(ModifiesP{maybe_arg1.value(), maybe_arg2.value()}, rest);
    }

    return std::nullopt;
}

auto parse_ent_ref(const Synonyms& declared, std::vector<Token>::const_iterator it,
                   const std::vector<Token>::const_iterator& end)
    -> std::optional<std::tuple<EntRef, std::vector<Token>::const_iterator>> {
    if (it == end) {
        return std::nullopt;
    }

    const auto& first_token = *it;
    if (is_string(first_token)) {
        const auto maybe_syn = find_syn(declared, first_token.content);
        return maybe_syn.has_value() ? std::make_optional(std::make_tuple(maybe_syn.value(), std::next(it)))
                                     : std::nullopt;
    } else if (is_wildcard(first_token)) {
        return std::make_tuple(WildCard{}, std::next(it));
    } else {
        // Quoted Ident
        if (std::distance(it, end) < 3) {
            return std::nullopt;
        }
        const auto& maybe_open_quote = *it;
        const auto& maybe_ident = *std::next(it);
        const auto& maybe_close_quote = *std::next(it, 2);

        if (!is_open_quote(maybe_open_quote) || !is_string(maybe_ident) || !is_close_quote(maybe_close_quote)) {
            return std::nullopt;
        }

        return std::make_tuple(QuotedIdent{maybe_ident.content}, std::next(it, 3));
    }
}

} // namespace qps
