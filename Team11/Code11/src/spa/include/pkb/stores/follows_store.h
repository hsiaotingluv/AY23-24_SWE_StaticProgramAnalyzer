#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

/**
 * stores and manages the Follows and Follows* (transitive Follows) relationships
 * between statements in a SIMPLE program.
 */
class FollowsStore {
  public:
    using StatementNumber = std::string;
    using StatementSet = std::unordered_set<StatementNumber>;
    using StatementToStatementMap = std::unordered_map<StatementNumber, StatementNumber>;
    using StatementToSetMap = std::unordered_map<StatementNumber, StatementSet>;

    FollowsStore();

    /**
     * Adds a direct Follows relationship between two statements.
     *
     * @param s1 The statement number of the preceding statement.
     * @param s2 The statement number of the following statement.
     */
    void add_follows(const StatementNumber& s1, const StatementNumber& s2);

    /**
     * Populates the Follows* and reverse Follows* relationship.
     */
    void populate_follows_and_reverse_follows_star();

    /**
     * Checks if a direct Follows relationship exists between two statements.
     *
     * @param s1 The statement number of the potentially preceding statement.
     * @param s2 The statement number of the potentially following statement.
     * @return True if s1 directly follows s2, false otherwise.
     */
    [[nodiscard]] bool has_follows(const StatementNumber& s1, const StatementNumber& s2) const;

    /**
     * Retrieves all direct Follows relationships.
     *
     * @return A map of all direct Follows relationships, mapping preceding statements to following statements.
     */
    [[nodiscard]] StatementToStatementMap get_all_follows() const;

    /**
     * Retrieves all statement numbers that are followed by another statement.
     *
     * @return A set of all statement numbers that are directly followed by another statement.
     */
    [[nodiscard]] StatementSet get_all_follows_keys() const;

    /**
     * Retrieves all statement numbers that follow another statement.
     *
     * @return A set of all statement numbers that directly follow another statement.
     */
    [[nodiscard]] StatementSet get_all_follows_values() const;

    /**
     * Retrieves the statement number that directly follows the given statement.
     *
     * @param s The statement number to find the following statement for.
     * @return The statement number that directly follows s, or an empty string if none exists.
     */
    [[nodiscard]] StatementNumber get_follows_following(const StatementNumber& s) const;

    /**
     * Retrieves the statement number that is directly followed by the given statement.
     *
     * @param s The statement number to find the preceding statement for.
     * @return The statement number that is directly followed by s, or an empty string if none exists.
     */
    [[nodiscard]] StatementNumber get_follows_by(const StatementNumber& s) const;

    /**
     * Checks if a transitive Follows relationship (Follows*) exists between two statements.
     *
     * @param s1 The statement number of the potentially ancestor statement.
     * @param s2 The statement number of the potentially descendant statement.
     * @return True if there is a transitive Follows relationship from s1 to s2, false otherwise.
     */
    [[nodiscard]] bool has_follows_stars(const StatementNumber& s1, const StatementNumber& s2) const;

    /**
     * Retrieves all transitive Follows relationships (Follows*).
     *
     * @return A map of all transitive Follows relationships, mapping ancestor statements to sets of descendant
     * statements.
     */
    [[nodiscard]] StatementToSetMap get_all_follows_star() const;

    /**
     * Retrieves all statement numbers that are transitively followed by another statement.
     *
     * @return A set of all statement numbers that are transitively followed by another statement.
     */
    [[nodiscard]] StatementSet get_all_follows_star_keys() const;

    /**
     * Retrieves all statement numbers that transitively follow another statement.
     *
     * @return A set of all statement numbers that transitively follow another statement.
     */
    [[nodiscard]] StatementSet get_all_follows_star_values() const;

    /**
     * Retrieves all statement numbers that are transitively followed by the given statement.
     *
     * @param s The statement number to find descendants for in the transitive Follows relationships.
     * @return A set of all statement numbers that are transitively followed by s.
     */
    [[nodiscard]] StatementSet get_follows_stars_following(const StatementNumber& s) const;

    /**
     * Retrieves all statement numbers that transitively follow the given statement.
     *
     * @param s The statement number to find ancestors for in the transitive Follows relationships.
     * @return A set of all statement numbers that transitively follow s.
     */
    [[nodiscard]] StatementSet get_follows_stars_by(const StatementNumber& s) const;

  private:
    // Direct Follows relationships: statement -> following statement
    StatementToStatementMap follows_store;

    // Reverse mapping for Direct Follows: following statement -> statement
    StatementToStatementMap reversed_follows_store;

    // Transitive Follows relationships: statement -> set of transitively following statements
    StatementToSetMap follows_star_store;

    // Reverse mapping for Transitive Follows: transitively following statement -> set of statements
    StatementToSetMap reversed_follows_star_store;
};
