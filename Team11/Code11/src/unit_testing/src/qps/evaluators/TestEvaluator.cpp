#include "catch.hpp"
#include "common/statement_type.hpp"
#include "qps/parser/entities/attribute.hpp"
#include "qps/parser/entities/attribute_name.hpp"
#include "qps/parser/entities/primitives.hpp"
#include "qps/parser/entities/relationship.hpp"
#include "qps/parser/entities/select.hpp"
#include "test_evaluator.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb_manager.h"

#include "qps/evaluators/query_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <memory>
#include <vector>

using namespace qps;

TEST_CASE("Test Evaluator Entities") {
    SECTION("Evaluate - All Variables") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate PkbManager
        const auto variable_strs = std::vector<std::string>{"v", "w", "x", "y", "z"};
        for (const auto& x : variable_strs) {
            write_facade->add_variable(x);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<VarSynonym>("v"),
            },
            std::make_shared<VarSynonym>("v"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), variable_strs);
    }

    SECTION("Evaluate - All Procedures") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate PkbManager
        const auto procedure_strs = std::vector<std::string>{"p", "q", "r", "s", "t"};
        for (const auto& x : procedure_strs) {
            write_facade->add_procedure(x);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ProcSynonym>("p"),
            },
            std::make_shared<ProcSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), procedure_strs);
    }

    SECTION("Evaluate - All Constants") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto constant_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : constant_strs) {
            write_facade->add_constant(x);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ConstSynonym>("c"),
            },
            std::make_shared<ConstSynonym>("c"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), constant_strs);
    }

    SECTION("Evaluate - All Assigns") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto assign_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : assign_strs) {
            write_facade->add_statement(x, StatementType::Assign);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<AssignSynonym>("a"),
            },
            std::make_shared<AssignSynonym>("a"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), assign_strs);
    }

    SECTION("Evaluate - All Ifs") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto if_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : if_strs) {
            write_facade->add_statement(x, StatementType::If);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<IfSynonym>("i"),
            },
            std::make_shared<IfSynonym>("i"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), if_strs);
    }

    SECTION("Evaluate - All Whiles") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto while_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : while_strs) {
            write_facade->add_statement(x, StatementType::While);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<WhileSynonym>("w"),
            },
            std::make_shared<WhileSynonym>("w"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), while_strs);
    }

    SECTION("Evaluate - All Calls") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto call_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : call_strs) {
            write_facade->add_statement(x, StatementType::Call);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<CallSynonym>("c"),
            },
            std::make_shared<CallSynonym>("c"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), call_strs);
    }

    SECTION("Evaluate - All Prints") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto print_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : print_strs) {
            write_facade->add_statement(x, StatementType::Print);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<PrintSynonym>("p"),
            },
            std::make_shared<PrintSynonym>("p"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), print_strs);
    }

    SECTION("Evaluate - All Reads") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto read_strs = std::vector<std::string>{"1", "2", "3", "4", "5"};
        for (const auto& x : read_strs) {
            write_facade->add_statement(x, StatementType::Read);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ReadSynonym>("r"),
            },
            std::make_shared<ReadSynonym>("r"),
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), read_strs);
    }
}

TEST_CASE("Test Evaluator Clauses - Select by Attribute Reference") {
    SECTION("Evaluate - All Reads - Request variable names") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto read_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto variable_name = "v";
        for (const auto& x : read_stmts) {
            write_facade->add_statement(x, StatementType::Read);
            write_facade->add_statement_modify_var(x, variable_name);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ReadSynonym>("r"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{variable_name});
    }

    SECTION("Evaluate - All Reads - Request stmt#") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto read_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto variable_name = "v";
        for (const auto& x : read_stmts) {
            write_facade->add_statement(x, StatementType::Read);
            write_facade->add_statement_modify_var(x, variable_name);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ReadSynonym>("r"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), StmtNum{}, AttrRef::Type::Integer}},
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), read_stmts);
    }

    SECTION("Evaluate - Get reads - Request variable names") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto read_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto variable_name = "v";
        for (const auto& x : read_stmts) {
            write_facade->add_statement(x, StatementType::Read);
            write_facade->add_statement_modify_var(x, variable_name);
        }

        for (int i = 0; i < read_stmts.size() - 1; i++) {
            write_facade->add_follows(read_stmts[i], read_stmts[i + 1]);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<ReadSynonym>("r"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<ReadSynonym>("r"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<ReadSynonym>("r"), WildCard{}}),
            },
        };

        const auto results = evaluator.evaluate(query);
        for (int i = 0; i < results.size() - 1; i++) {
            std::cout << results[i] << std::endl;
        }

        require_equal(evaluator.evaluate(query), std::vector<std::string>{variable_name});
    }

    SECTION("Evaluate - All prints - Request variable names") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto print_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto variable_name = "v";
        for (const auto& x : print_stmts) {
            write_facade->add_statement(x, StatementType::Print);
            write_facade->add_statement_use_var(x, variable_name);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<PrintSynonym>("pn"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<PrintSynonym>("pn"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{variable_name});
    }

    SECTION("Evaluate - All prints - Request stmt#") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto print_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto variable_name = "v";
        for (const auto& x : print_stmts) {
            write_facade->add_statement(x, StatementType::Print);
            write_facade->add_statement_use_var(x, variable_name);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<PrintSynonym>("pn"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<PrintSynonym>("pn"), StmtNum{}, AttrRef::Type::Integer}},
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), print_stmts);
    }

    SECTION("Evaluate - Get prints - Request variable names") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto print_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto variable_name = "v";
        for (const auto& x : print_stmts) {
            write_facade->add_statement(x, StatementType::Print);
            write_facade->add_statement_use_var(x, variable_name);
        }

        for (int i = 0; i < print_stmts.size() - 1; i++) {
            write_facade->add_follows(print_stmts[i], print_stmts[i + 1]);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<PrintSynonym>("pn"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<PrintSynonym>("pn"), VarName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<PrintSynonym>("pn"), WildCard{}}),
            },
        };

        const auto results = evaluator.evaluate(query);
        for (int i = 0; i < results.size() - 1; i++) {
            std::cout << results[i] << std::endl;
        }

        require_equal(evaluator.evaluate(query), std::vector<std::string>{variable_name});
    }

    SECTION("Evaluate - All calls - Request procedure names") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto call_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto procedure_name = "v";
        for (const auto& x : call_stmts) {
            write_facade->add_statement(x, StatementType::Call);
            write_facade->add_stmt_no_proc_called_mapping(x, procedure_name);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<CallSynonym>("c"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<CallSynonym>("c"), ProcName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{},
        };

        const auto results = evaluator.evaluate(query);
        require_equal(evaluator.evaluate(query), std::vector<std::string>{procedure_name});
    }

    SECTION("Evaluate - All calls - Request stmt#") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto call_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto procedure_name = "v";
        for (const auto& x : call_stmts) {
            write_facade->add_statement(x, StatementType::Call);
            write_facade->add_stmt_no_proc_called_mapping(x, procedure_name);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<CallSynonym>("c"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<CallSynonym>("c"), StmtNum{}, AttrRef::Type::Integer}},
            std::vector<std::shared_ptr<Clause>>{},
        };

        require_equal(evaluator.evaluate(query), call_stmts);
    }

    SECTION("Evaluate - Get calls - Request procedure names") {
        const auto& [read_facade, write_facade] = pkb::PkbManager::create_facades();

        // Populate pkb::PkbManager
        const auto call_stmts = std::vector<std::string>{"1", "2", "3", "4", "5"};
        const auto procedure_name = "v";
        for (const auto& x : call_stmts) {
            write_facade->add_statement(x, StatementType::Call);
            write_facade->add_stmt_no_proc_called_mapping(x, procedure_name);
        }

        for (int i = 0; i < call_stmts.size() - 1; i++) {
            write_facade->add_follows(call_stmts[i], call_stmts[i + 1]);
        }

        auto evaluator = QueryEvaluator{read_facade};
        const auto query = Query{
            Synonyms{
                std::make_shared<CallSynonym>("c"),
            },
            std::vector<Elem>{AttrRef{std::make_shared<CallSynonym>("c"), ProcName{}, AttrRef::Type::Name}},
            std::vector<std::shared_ptr<Clause>>{
                std::make_shared<SuchThatClause>(Follows{std::make_shared<CallSynonym>("c"), WildCard{}}),
            },
        };

        require_equal(evaluator.evaluate(query), std::vector<std::string>{procedure_name});
    }
}
