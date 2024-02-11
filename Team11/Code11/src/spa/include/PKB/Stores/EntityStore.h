#pragma once

#include "PKB/CommonTypes/Constant.h"
#include "PKB/CommonTypes/Procedure.h"
#include "PKB/CommonTypes/Variable.h"
#include <unordered_set>

/**
 * Class to store and manage entities (variables, procedures, constants) in a SIMPLE program.
 */
class EntityStore {
  public:
    using ProcedureSet = std::unordered_set<Procedure>;
    using VariableSet = std::unordered_set<Variable>;
    using ConstantSet = std::unordered_set<Constant>;

    EntityStore();

    /**
     * Adds a variable entity to the store.
     *
     * @param variable The variable to be added.
     */
    void addVariable(const Variable& variable);

    /**
     * Adds a procedure entity to the store.
     *
     * @param procedure The procedure to be added.
     */
    void addProcedure(const Procedure& procedure);

    /**
     * Adds a constant entity to the store.
     *
     * @param constant The constant to be added.
     */
    void addConstant(const Constant& constant);

    /**
     * Retrieves all procedure entities from the store.
     *
     * @return A set of all procedures in the store.
     */
    [[nodiscard]] ProcedureSet getProcedures() const;

    /**
     * Retrieves all variable entities from the store.
     *
     * @return A set of all variables in the store.
     */
    [[nodiscard]] VariableSet getVariables() const;

    /**
     * Retrieves all constant entities from the store.
     *
     * @return A set of all constants in the store.
     */
    [[nodiscard]] ConstantSet getConstants() const;

  private:
    ProcedureSet procedureSet;
    VariableSet variableSet;
    ConstantSet constantSet;
};
