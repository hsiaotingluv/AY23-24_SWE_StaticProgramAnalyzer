#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

/**
 * Class to manage Next and Next* (transitive) relationships between program statements.
 *
 * The Next relationship indicates that a statement can be executed immediately after another
 * in some execution path. Next* is the transitive closure of Next, indicating that a statement
 * can be executed at some point after another statement in any execution path.
 */
class NextStore {
  public:
    using StatementNumber = std::string;
    using StatementSet = std::unordered_set<StatementNumber>;
    using StatementToStatementSetMap = std::unordered_map<StatementNumber, StatementSet>;

    NextStore();

    /**
     * Adds a direct Next relationship between two statements.
     *
     * @param before The statement number that comes before in the Next relationship.
     * @param after The statement number that comes after in the Next relationship.
     */
    void add_next(const StatementNumber& before, const StatementNumber& after);

    /**
     * Checks if there is a direct Next relationship between two statements.
     *
     * @param before The statement number that should come before in the Next relationship.
     * @param after The statement number that should come after in the Next relationship.
     * @return True if the Next relationship exists, false otherwise.
     */
    [[nodiscard]] bool has_next(const StatementNumber& before, const StatementNumber& after) const;

    /**
     * Retrieves all direct Next relationships.
     *
     * @return A mapping of all direct Next relationships,
     * where each key maps to a set of statement numbers that can come immediately after.
     */
    [[nodiscard]] StatementToStatementSetMap get_all_next() const;

    /**
     * Retrieves all statement numbers that have at least one Next relationship.
     *
     * @return A set of statement numbers that serve as keys in the direct Next relationships.
     */
    [[nodiscard]] StatementSet get_all_next_keys() const;

    /**
     * Retrieves all statement numbers that are values in at least one Next relationship.
     *
     * @return A set of statement numbers that are values in the direct Next relationships.
     */
    [[nodiscard]] StatementSet get_all_next_values() const;

    /**
     * Retrieves all statements that can be executed immediately after a given statement.
     *
     * @param before The statement number to find successors for.
     * @return A set of statement numbers that can come immediately after the given statement.
     */
    [[nodiscard]] StatementSet get_next(const StatementNumber& before) const;

    /**
     * Retrieves all statements that can lead directly to a given statement.
     *
     * @param after The statement number to find predecessors for.
     * @return A set of statement numbers that can lead directly to the given statement.
     */
    [[nodiscard]] StatementSet get_next_previous(const StatementNumber& after) const;

    /**
     * Adds a transitive Next* relationship between two statements.
     *
     * @param before The statement number that comes before in the Next* relationship.
     * @param after The statement number that comes after in the Next* relationship.
     */
    void add_next_star(const StatementNumber& before, const StatementNumber& after);

    /**
     * Checks if there is a transitive Next* relationship between two statements.
     *
     * @param before The statement number that should come before in the Next* relationship.
     * @param after The statement number that should come after in the Next* relationship.
     * @return True if the Next* relationship exists, false otherwise.
     */
    [[nodiscard]] bool has_next_star(const StatementNumber& before, const StatementNumber& after) const;

    /**
     * Retrieves all Next* relationships.
     *
     * @return A mapping of all Next* relationships,
     * where each key maps to a set of statement numbers that can come at some point after.
     */
    [[nodiscard]] StatementToStatementSetMap get_all_next_star() const;

    /**
     * Retrieves all statement numbers that have at least one Next* relationship.
     *
     * @return A set of statement numbers that serve as keys in the transitive Next* relationships.
     */
    [[nodiscard]] StatementSet get_all_next_star_keys() const;

    /**
     * Retrieves all statement numbers that are values in at least one Next* relationship.
     *
     * @return A set of statement numbers that are values in the transitive Next* relationships.
     */
    [[nodiscard]] StatementSet get_all_next_star_values() const;

    /**
     * Retrieves all statements that can be executed at some point after a given statement in the Next* relationship.
     *
     * @param before The statement number to find transitive successors for.
     * @return A set of statement numbers that can come at some point after the given statement.
     */
    [[nodiscard]] StatementSet get_next_star(const StatementNumber& before) const;

    /**
     * Retrieves all statements that can lead to a given statement at some point in the Next* relationship.
     *
     * @param after The statement number to find transitive predecessors for.
     * @return A set of statement numbers that can lead to the given statement at some point.
     */
    [[nodiscard]] StatementSet get_next_star_previous(const StatementNumber& after) const;

  private:
    StatementToStatementSetMap next_store;
    StatementToStatementSetMap reversed_next_store;
    StatementToStatementSetMap next_star_store;
    StatementToStatementSetMap reversed_next_star_store;
};
