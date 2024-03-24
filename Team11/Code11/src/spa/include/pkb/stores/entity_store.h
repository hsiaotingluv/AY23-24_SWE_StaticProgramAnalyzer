#pragma once

#include "pkb/common_types/constant.h"
#include "pkb/common_types/procedure.h"
#include "pkb/common_types/variable.h"
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
    void add_variable(const Variable& variable);

    /**
     * Adds a procedure entity to the store.
     *
     * @param procedure The procedure to be added.
     */
    void add_procedure(const Procedure& procedure);

    /**
     * Adds a constant entity to the store.
     *
     * @param constant The constant to be added.
     */
    void add_constant(const Constant& constant);

    bool has_procedure(const std::string& procedure) const;

    bool has_variable(const std::string& variable) const;

    bool has_constant(const std::string& constant) const;

    /**
     * Retrieves all procedure entities from the store.
     *
     * @return A set of all procedures in the store.
     */
    [[nodiscard]] ProcedureSet get_procedures() const;

    /**
     * Retrieves all variable entities from the store.
     *
     * @return A set of all variables in the store.
     */
    [[nodiscard]] VariableSet get_variables() const;

    /**
     * Retrieves all constant entities from the store.
     *
     * @return A set of all constants in the store.
     */
    [[nodiscard]] ConstantSet get_constants() const;

  private:
    ProcedureSet procedureSet;
    VariableSet variableSet;
    ConstantSet constantSet;
};
