#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

/**
 * Manages the Calls and Calls* (transitive Calls) relationships
 * between procedures in a SIMPLE program.
 */
class CallsStore {
  public:
    using ProcedureName = std::string;
    using ProcedureSet = std::unordered_set<ProcedureName>;
    using ProcedureToProcedureSetMap = std::unordered_map<ProcedureName, ProcedureSet>;

    CallsStore();

    /**
     * Adds a direct Calls relationship between two procedures.
     *
     * @param caller The name of the calling procedure.
     * @param callee The name of the called procedure.
     */
    void addCalls(const ProcedureName& caller, const ProcedureName& callee);

    /**
     * Checks if a direct Calls relationship exists between two procedures.
     *
     * @param caller The name of the calling procedure.
     * @param callee The name of the called procedure.
     * @return True if caller directly calls callee, false otherwise.
     */
    [[nodiscard]] bool hasCalls(const ProcedureName& caller, const ProcedureName& callee) const;

    /**
     * Gets all direct Calls relationship.
     *
     * @return A map of all direct caller-callee procedures.
     */
    [[nodiscard]] ProcedureToProcedureSetMap getAllCalls() const;

    /**
     * Gets all procedures that are called by at least one other procedure.
     *
     * @return A set of all callee procedures.
     */
    [[nodiscard]] ProcedureSet getAllCallsValues() const;

    /**
     * Gets all procedures that call at least one other procedure.
     *
     * @return A set of all caller procedures.
     */
    [[nodiscard]] ProcedureSet getAllCallsKeys() const;

    /**
     * Gets all procedures called by a specific procedure.
     *
     * @param caller The name of the calling procedure.
     * @return A set of procedures called by caller.
     */
    [[nodiscard]] ProcedureSet getCallees(const ProcedureName& caller) const;

    /**
     * Gets all procedures that call a specific procedure.
     *
     * @param callee The name of the called procedure.
     * @return A set of procedures that call callee.
     */
    [[nodiscard]] ProcedureSet getCallers(const ProcedureName& callee) const;

    /**
     * Adds a transitive Calls* relationship between two procedures.
     *
     * @param caller The name of the ancestor calling procedure.
     * @param callee The name of the descendant called procedure.
     */
    void addCallsStar(const ProcedureName& caller, const ProcedureName& callee);

    /**
     * Checks if a transitive Calls* relationship exists between two procedures.
     *
     * @param caller The name of the potentially ancestor calling procedure.
     * @param callee The name of the potentially descendant called procedure.
     * @return True if a transitive Calls* relationship exists from caller to callee, false otherwise.
     */
    [[nodiscard]] bool hasCallsStar(const ProcedureName& caller, const ProcedureName& callee) const;

    /**
     * Gets all Calls* relationship.
     *
     * @return A map of all transitive caller-callee procedures.
     */
    [[nodiscard]] ProcedureToProcedureSetMap getAllCallsStar() const;

    /**
     * Gets all procedures that are transitively called by at least one other procedure.
     *
     * @return A set of all callee procedures in Calls* relationships.
     */
    [[nodiscard]] ProcedureSet getAllCallsStarValues() const;

    /**
     * Gets all procedures that transitively call at least one other procedure.
     *
     * @return A set of all caller procedures in Calls* relationships.
     */
    [[nodiscard]] ProcedureSet getAllCallsStarKeys() const;

    /**
     * Gets all procedures transitively called by a specific procedure in Calls* relationships.
     *
     * @param caller The name of the ancestor calling procedure.
     * @return A set of procedures transitively called by caller.
     */
    [[nodiscard]] ProcedureSet getCallsStarCallees(const ProcedureName& caller) const;

    /**
     * Gets all procedures that transitively call a specific procedure in Calls* relationships.
     *
     * @param callee The name of the descendant called procedure.
     * @return A set of procedures that transitively call callee.
     */
    [[nodiscard]] ProcedureSet getCallsStarCallers(const ProcedureName& callee) const;

  private:
    ProcedureToProcedureSetMap callsStore;
    ProcedureToProcedureSetMap reversedCallsStore;
    ProcedureToProcedureSetMap callsStarStore;
    ProcedureToProcedureSetMap reversedCallsStarStore;
};
