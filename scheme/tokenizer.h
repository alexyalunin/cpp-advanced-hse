#pragma once
#include "error.h"
#include <utility>
#include <vector>
#include <variant>
#include <optional>
#include <iostream>
#include <string>
#include <sstream>

struct SymbolToken {
    std::string name_;
    SymbolToken(std::string name);
    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int64_t value_;
    ConstantToken(int64_t value);
    bool operator==(const ConstantToken& other) const;
};

struct NoToken {
    bool operator==(const NoToken&) const;
};
using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken, NoToken>;

class Tokenizer {
public:
    Tokenizer();
    Tokenizer(std::istream* in);

    bool IsEnd();

    void Change(std::istream* in);

    void Next();

    void NextImpl();

    Token GetToken();

    std::istream* in_;
    Token token_;
};

bool ValidStart(int ch);
bool ValidContinue(int ch);
