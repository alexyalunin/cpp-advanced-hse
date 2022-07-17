#include "tokenizer.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>

SymbolToken::SymbolToken(std::string name) : name_(std::forward<std::string>(name)) {
}
bool SymbolToken::operator==(const SymbolToken &other) const {
    return name_ == other.name_;
}
ConstantToken::ConstantToken(int64_t value) : value_(value) {
}

bool ConstantToken::operator==(const ConstantToken &other) const {
    return value_ == other.value_;
}

bool QuoteToken::operator==(const QuoteToken &) const {
    return true;
}

bool DotToken::operator==(const DotToken &) const {
    return true;
}

bool NoToken::operator==(const NoToken &) const {
    return true;
}
Tokenizer::Tokenizer() : in_(nullptr), token_(Token{NoToken{}}) {
}

Tokenizer::Tokenizer(std::istream *in) : in_(in), token_(Token{NoToken{}}) {
    Next();
}

void Tokenizer::Change(std::istream *in) {
    in_ = in;
    token_ = Token{NoToken{}};
    Next();
}

bool ValidStart(int ch) {
    return std::isalpha(ch) || (ch >= 60 && ch <= 62) || ch == 42 || ch == 47 || ch == 35;
}

bool ValidContinue(int ch) {
    return ValidStart(ch) || std::isdigit(ch) || ch == 63 || ch == 33 || ch == 45;
}

void Tokenizer::NextImpl() {
    int cur = in_->get();
    if (cur == 46) {
        token_ = DotToken{};
    } else if (cur == 39) {
        token_ = QuoteToken{};
    } else if (cur == 40) {
        token_ = Token{BracketToken::OPEN};
    } else if (cur == 41) {
        token_ = Token{BracketToken::CLOSE};
    } else if (ValidStart(cur) || std::isdigit(cur) || cur == 43 || cur == 45) {
        std::string s;
        s.push_back(char(cur));
        if ((cur == 43 || cur == 45) && !std::isdigit(in_->peek())) {
            token_ = Token{SymbolToken{s}};
        } else if (std::isdigit(cur) || cur == 43 || cur == 45) {
            while (std::isdigit(in_->peek())) {
                s.push_back(char(in_->get()));
            }
            token_ = Token{ConstantToken{std::stoll(s)}};
        } else {
            while (ValidContinue(in_->peek())) {
                s.push_back(char(in_->get()));
            }
            token_ = Token{SymbolToken{s}};
        }
    } else {
        throw SyntaxError{"Syntax error!"};
    }
}

void Tokenizer::Next() {
    while (std::isspace(in_->peek())) {
        in_->get();
    }
    if (in_->peek() == EOF) {
        token_ = Token{NoToken{}};
    } else {
        NextImpl();
    }
}
Token Tokenizer::GetToken() {
    return token_;
}
bool Tokenizer::IsEnd() {
    return token_ == Token{NoToken()};
}
