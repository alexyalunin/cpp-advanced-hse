#include "parser.h"
std::string SetReturn::Print() {
    return "";
}

void SetReturn::Reach() {
}
void SetReturn::Unreach() {
}

bool SetReturn::Reached() {
    return true;
}

Object* SetReturn::Eval(Scope*) {
    return this;
}
SetReturn* GetNull() {
    static SetReturn obj;
    return &obj;
}

Lambda::Lambda(Scope* scope, std::vector<std::string>&& args, std::vector<Object*>&& body)
    : scope_(scope),
      args_(std::forward<std::vector<std::string>>(args)),
      body_(std::forward<std::vector<Object*>>(body)) {
}

Object* Lambda::Apply(const std::vector<Object*>& data, Scope* scope) {
    if (args_.size() != data.size()) {
        throw SyntaxError("Wrong amount of args");
    }
    Scope* cur_scope = scope_->Create();
    for (size_t i = 0; i < data.size(); ++i) {
        if (!data[i]) {
            throw RuntimeError("");
        }
        cur_scope->scope_[args_[i]] = data[i]->Eval(scope);
    }
    for (size_t i = 0; i < body_.size() - 1; ++i) {
        Object* ptr = body_[i]->Eval(cur_scope);
        if (ptr && ptr != GetNull()) {
            throw RuntimeError("bad lambda!");
        }
    }
    return body_.back()->Eval(cur_scope);
}

Object* Lambda::Eval(Scope*) {
    return this;
}
void Lambda::Reach() {
    reachable_ = true;
    if (!scope_->reachable_) {
        scope_->Reach();
    }
    for (auto& i : body_) {
        if (i && !i->Reached()) {
            i->Reach();
        }
    }
}
void Lambda::Unreach() {
    reachable_ = false;
    if (scope_->reachable_) {
        scope_->Unreach();
    }
    for (auto& i : body_) {
        if (i && !i->Reached()) {
            i->Unreach();
        }
    }
}
bool Lambda::Reached() {
    return reachable_;
}
std::string Lambda::Print() {
    return "";
}

Saver::Saver(Object* obj, Scope* scp) : obj_(obj), scp_(scp) {
}
void Saver::Reach() {
    reachable_ = true;
    if (!obj_->Reached()) {
        obj_->Reach();
    }
}
void Saver::Unreach() {
    reachable_ = true;
    if (!obj_->Reached()) {
        obj_->Unreach();
    }
}
bool Saver::Reached() {
    return reachable_;
}
Object* Saver::Eval(Scope*) {
    return obj_->Eval(scp_);
}

std::string Saver::Print() {
    return obj_->Print();
}

Number::Number(int64_t value) : value_(value) {
}

int64_t Number::GetValue() const {
    return value_;
}

void Number::Reach() {
    reachable_ = true;
}

void Number::Unreach() {
    reachable_ = false;
}

bool Number::Reached() {
    return reachable_;
}

Boolean::Boolean(bool value) : value_(value) {
}

void Boolean::Reach() {
    reachable_ = true;
}

void Boolean::Unreach() {
    reachable_ = false;
}

bool Boolean::Reached() {
    return reachable_;
}

Object* Boolean::Eval(Scope* scope) {
    return scope->interpreter_->Allocate<Boolean>(value_);
}

std::string Boolean::Print() {
    if (value_) {
        return "#t";
    } else {
        return "#f";
    }
}

void Symbol::Reach() {
    reachable_ = true;
}

void Symbol::Unreach() {
    reachable_ = false;
}

bool Symbol::Reached() {
    return reachable_;
}
Symbol::Symbol(std::string name) : name_(std::forward<std::string>(name)) {
}

const std::string& Symbol::GetName() const {
    return name_;
}

Cell::Cell() : first_(nullptr), second_(nullptr) {
}
Object* Cell::GetFirst() const {
    return first_;
}

Object* Cell::GetSecond() const {
    return second_;
}

void Cell::Reach() {
    reachable_ = true;
    if (first_ && !first_->Reached()) {
        first_->Reach();
    }
    if (second_ && !second_->Reached()) {
        second_->Reach();
    }
}

void Cell::Unreach() {
    reachable_ = false;
    if (first_ && first_->Reached()) {
        first_->Unreach();
    }
    if (second_ && second_->Reached()) {
        second_->Unreach();
    }
}

bool Cell::Reached() {
    return reachable_;
}

Object* Number::Eval(Scope* scope) {
    return scope->interpreter_->Allocate<Number>(value_);
}

Object* Symbol::Eval(Scope* scope) {
    if (!scope->scope_.contains(name_)) {
        if (scope->father_) {
            return Eval(scope->father_);
        }
        throw NameError{"Name error"};
    }
    return scope->scope_[name_];
}

Object* Cell::Eval(Scope* scope) {
    if (!first_) {
        throw RuntimeError{"Runtime error"};
    }
    auto ptr = first_->Eval(scope);
    if (!Is<FunctionBase>(ptr)) {
        throw RuntimeError{"Runtime error"};
    }
    std::vector<Object*> data;
    if (Is<QuoteHelper>(ptr)) {
        data.push_back(As<Cell>(second_)->first_);
        return As<FunctionBase>(ptr)->Apply(data, scope);
    } else if (Is<Cell>(second_)) {
        data = Ltv(As<Cell>(second_));
    }
    if (Is<Lambda>(ptr)) {
        for (auto i : data) {
            i = i->Eval(scope);
        }
    }
    Object* ans = As<FunctionBase>(ptr)->Apply(data, scope);
    if (Is<Symbol>(ans)) {
        return ans->Eval(scope);
    }
    return ans;
}

std::vector<Object*> Ltv(Cell* ptr) {
    std::vector<Object*> ans;
    Cell* cur = ptr;
    while (Is<Cell>(cur->second_)) {
        ans.push_back(cur->first_);
        cur = As<Cell>(cur->second_);
    }
    if (cur->first_ && cur->second_) {
        ans.push_back(cur->first_);
        ans.push_back(cur->second_);
    } else if (cur->first_) {
        ans.push_back(cur->first_);
    } else {
        ans.push_back(cur);
    }
    return ans;
}

std::string Number::Print() {
    return std::to_string(value_);
}

std::string Symbol::Print() {
    return name_;
}

Cell::operator bool() const {
    return first_ != nullptr;
}

std::string Cell::Print() {
    Cell* ptr = this;
    std::string ans = "(";
    ans += ::Print(ptr->first_);
    while (Is<Cell>(ptr->second_)) {
        ptr = As<Cell>(ptr->second_);
        ans += ' ';
        ans += ptr->first_->Print();
    }
    if (!Is<Cell>(ptr->second_) && ptr->second_) {
        ans += " . " + ::Print(ptr->second_);
    }
    ans.push_back(')');
    return ans;
}

Object* Read(Interpreter* interpreter, Tokenizer* tokenizer) {
    if (!tokenizer) {
        tokenizer = &interpreter->tokenizer_;
    }
    if (tokenizer->GetToken() == Token{BracketToken::OPEN}) {
        tokenizer->Next();
        return ReadList(interpreter, tokenizer);
    } else if (std::holds_alternative<ConstantToken>(tokenizer->GetToken())) {
        auto ans =
            interpreter->Allocate<Number>(std::get<ConstantToken>(tokenizer->GetToken()).value_);
        tokenizer->Next();
        return ans;
    } else if (std::holds_alternative<SymbolToken>(tokenizer->GetToken())) {
        auto ans =
            interpreter->Allocate<Symbol>(std::get<SymbolToken>(tokenizer->GetToken()).name_);
        tokenizer->Next();
        return ans;
    } else if (tokenizer->GetToken() == Token{QuoteToken{}}) {
        tokenizer->Next();
        auto quote = interpreter->Allocate<Symbol>("quote");
        auto ptr = Read(interpreter, tokenizer);
        ptr = interpreter->Allocate<Cell>(ptr);
        return interpreter->Allocate<Cell>(quote, ptr);
    } else {
        throw SyntaxError{"Syntax error"};
    }
}

std::string Print() {
    return "";
}
Object* obj_;
Scope* scp_;
bool reachable_ = false;

Object* ReadList(Interpreter* interpreter, Tokenizer* tokenizer) {
    if (!tokenizer) {
        tokenizer = &interpreter->tokenizer_;
    }
    if (tokenizer->GetToken() == Token{BracketToken::CLOSE}) {
        tokenizer->Next();
        return nullptr;
    }
    Cell* ans = interpreter->Allocate<Cell>(Read(interpreter, tokenizer));
    Cell* change = ans;
    while (!(tokenizer->GetToken() == Token{BracketToken::CLOSE})) {
        if (tokenizer->GetToken() == Token{DotToken{}}) {
            tokenizer->Next();
            change->second_ = Read(interpreter, tokenizer);
            if (!(tokenizer->GetToken() == Token{BracketToken::CLOSE})) {
                throw SyntaxError{"Syntax error"};
            }
            tokenizer->Next();
            return ans;
        }
        /*auto ptr = Read(interpreter, tokenizer);
        if (ptr) {
            change->second_ =
                    interpreter->Allocate<Cell>(ptr);
            change = dynamic_cast<Cell*>(change->second_);
        } else {
            continue;
        }*/
        auto ptr = Read(interpreter, tokenizer);
        change->second_ = interpreter->Allocate<Cell>(ptr);
        change = dynamic_cast<Cell*>(change->second_);
    }
    tokenizer->Next();
    return ans;
}

Interpreter* Singleton() {
    static Interpreter interpreter;
    return &interpreter;
}

Object* Read(Tokenizer* tokenizer) {
    return Read(Singleton(), tokenizer);
}

Object* ReadList(Tokenizer* tokenizer) {
    return ReadList(Singleton(), tokenizer);
}
