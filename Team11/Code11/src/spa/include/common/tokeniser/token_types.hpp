#pragma once

namespace tokenizer {
/**
 * @brief Enum class representing different token types used in the tokenizer.
 *
 * The TokenType enum class defines various token types that can be encountered during tokenization.
 * These token types include binary operators, primitive types, delimiters, junk, and specific tokens
 * for the QPS and SP languages. Each token type is represented by a unique enumerator value.
 *
 */
enum class TokenType {
    // Binary Operator
    Add,
    Sub,
    Mul,
    Div,
    Mod,

    // Primitive Type
    String,
    Integer,

    // Delimiter
    LParen,
    RParen,
    Semicolon,

    // Junk
    Junk,

    // ============= QPS specifics Token ==============
    // QPS Delimiter
    DQuote,
    Comma,

    // QPS Wildcard Token
    Wildcard,

    // QPS Miscellaneous Token
    Dot,
    SingleEqual,

    // QPS AttrName Keyword Token
    AttrName,

    // ============= SP specifics Token ===============
    // Keyword is captured as string since keyword names are not reserved
    // SP Delimiter
    LCurly,
    RCurly,

    // SP Relational Token
    LessThan,
    LessThanEqual,
    GreaterThan,
    GreaterThanEqual,
    DoubleEqual,
    NotEqual,

    // SP Assignment, this has similar token to relational
    Assignment,

    // SP Logical Conditional Token
    LNot,
    LAnd,
    LOr,

    // Done
    Done,
};
} // namespace tokenizer