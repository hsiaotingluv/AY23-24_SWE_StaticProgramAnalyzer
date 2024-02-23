#pragma once

enum class StatementType {
    Assign,
    Call,
    If,
    While,
    Read,
    Print,

    // PKB and QPS Usage
    Raw,
};