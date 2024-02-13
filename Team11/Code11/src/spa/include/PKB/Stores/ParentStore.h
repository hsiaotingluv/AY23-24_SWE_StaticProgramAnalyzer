#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

/**
 * Stores and manages the Parent and Parent* (transitive) relationships
 * between statements in a SIMPLE program.
 */

class ParentStore {
  public:
    using StatementNumber = std::string;
    using StatementSet = std::unordered_set<StatementNumber>;
    using StatementToSetMap = std::unordered_map<StatementNumber, StatementSet>;
    using StatementToStatementMap = std::unordered_map<StatementNumber, StatementNumber>;

    ParentStore();

    /**
     * Adds a direct Parent relationship between a parent and a child statement.
     *
     * @param parent The statement number of the parent.
     * @param child The statement number of the child.
     */
    void addParent(const StatementNumber& parent, const StatementNumber& child);

    /**
     * Checks if a direct Parent relationship exists between a parent and a child statement.
     *
     * @param parent The statement number of the parent.
     * @param child The statement number of the child.
     * @return True if the parent-child relationship exists, false otherwise.
     */
    [[nodiscard]] bool hasParent(const StatementNumber& parent, const StatementNumber& child) const;

    /**
     * Retrieves all Parent relationships.
     *
     * @return A mapping of parent statements to their direct children.
     */
    [[nodiscard]] StatementToSetMap getAllParent() const;

    /**
     * Retrieves all parents from the Parent relationship.
     *
     * @return A set of parent statement numbers.
     */
    [[nodiscard]] StatementSet getAllParentKeys() const;

    /**
     * Retrieves all children from the Parent relationship.
     *
     * @return A set of child statement numbers.
     */
    [[nodiscard]] StatementSet getAllParentValues() const;

    /**
     * Retrieves the direct children of a given parent statement.
     *
     * @param parent The statement number of the parent.
     * @return A set of statement numbers of the direct children.
     */
    [[nodiscard]] StatementSet getParentChildren(const StatementNumber& parent) const;

    /**
     * Retrieves the direct parent of a given child statement.
     *
     * @param child The statement number of the child.
     * @return The statement number of the direct parent.
     */
    [[nodiscard]] StatementNumber getParent(const StatementNumber& child) const;

    /**
     * Adds a transitive Parent* relationship between an ancestor and a descendant statement.
     *
     * @param parent The statement number of the ancestor.
     * @param child The statement number of the descendant.
     */
    void addParentStar(const StatementNumber& parent, const StatementNumber& child);

    /**
     * Checks if a transitive Parent* relationship exists between a ancestor and a descendant statement.
     *
     * @param parent The statement number of the ancestor.
     * @param child The statement number of the descendant.
     * @return True if the parent-child relationship exists, false otherwise.
     */
    [[nodiscard]] bool hasParentStar(const StatementNumber& parent, const StatementNumber& child) const;

    /**
     * Retrieves all Parent* relationships.
     *
     * @return A mapping of ancestor statements to all their descendants in the Parent* relationship.
     */
    [[nodiscard]] StatementToSetMap getAllParentStar() const;

    /**
     * Retrieves all ancestors from the Parent* relationship.
     *
     * @return A set of parent statement numbers.
     */
    [[nodiscard]] StatementSet getAllParentStarKeys() const;

    /**
     * Retrieves all descendants from the Parent* relationship.
     *
     * @return A set of descendant statement numbers.
     */
    [[nodiscard]] StatementSet getAllParentStarValues() const;

    /**
     * Retrieves all descendants of a given ancestor statement in the Parent* relationship.
     *
     * @param parent The statement number of the ancestor.
     * @return A set of statement number for all descendants.
     */
    [[nodiscard]] StatementSet getParentStarChildren(const StatementNumber& parent) const;

    /**
     * Retrieves all ancestors of a given descendant statement in the Parent* relationship.
     *
     * @param child The statement number of the descendant.
     * @return A set of statement number for all ancestors.
     */
    [[nodiscard]] StatementSet getStarParent(const StatementNumber& child) const;

  private:
    StatementToSetMap parentStore;
    StatementToStatementMap reversedParentStore;
    StatementToSetMap parentStarStore;
    StatementToSetMap reversedParentStarStore;
};
