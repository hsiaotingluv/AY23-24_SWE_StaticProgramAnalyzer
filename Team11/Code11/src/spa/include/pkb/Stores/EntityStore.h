#include "pkb/CommonTypes.h"
#include <unordered_set>

class EntityStore {
  public:
    typedef CommonTypes::PROCEDURE Procedure;
    typedef CommonTypes::VARIABLE Variable;
    typedef CommonTypes::CONSTANT Constant;
    typedef std::unordered_set<Procedure> ProcedureSet;
    typedef std::unordered_set<Variable> VariableSet;
    typedef std::unordered_set<Constant> ConstantSet;

    EntityStore();

    ~EntityStore();

    void addVariable(Variable& variable);

    void addProcedure(Procedure& procedure);

    void addConstant(Constant& constant);

    ProcedureSet getProcedures();

    VariableSet getVariables();

    ConstantSet getConstants();

  private:
    ProcedureSet procedureSet;
    VariableSet variableSet;
    ConstantSet constantSet;
};