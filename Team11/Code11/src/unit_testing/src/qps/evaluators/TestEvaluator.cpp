#include "common/statement_type.hpp"
#include "test_evaluator.hpp"

#include "pkb/facades/read_facade.h"
#include "pkb/facades/write_facade.h"
#include "pkb/pkb.h"

#include "qps/evaluators/query_evaluator.hpp"
#include "qps/parser/entities/clause.hpp"
#include "qps/parser/entities/synonym.hpp"

#include <memory>
#include <vector>

using namespace qps;

TEST_CASE("Test Evaluator Entities") {
    SECTION("Evaluate - All Variables") {
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
        const auto& [read_facade, write_facade] = pkb::PKB::create_facades();

        // Populate pkb::PKB
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
