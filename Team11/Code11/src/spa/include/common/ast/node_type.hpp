#pragma once

namespace sp {
enum class NodeType {
    EmptyString,

    Constant,
    Variable,

    // Binop
    Add,
    Sub,
    Mul, // Note: the grammar already defined the binop precedence
    Div,
    Mod,

    // Relational Binop
    Gt,
    Gte,
    Lt,
    Lte,
    Eq,
    Neq,

    // Cond Expr
    LNot,
    LAnd,
    LOr,

    // Statement
    Read,
    Print,
    Call,
    Assign,
    If,
    While,

    // Statement List
    StmtList,

    // Procedure
    Procedure,

    // Program
    Program,
};

} // namespace sp
