#include "scheme.h"
void MyDefine(const std::string& name, Object* obj, Scope* scope) {
    scope->scope_[name] = obj->Eval(scope);
}

void DefineBool(const std::string& name, bool value, Scope* father) {
    if (!father->scope_.contains(name)) {
        father->scope_[name] = father->interpreter_->Allocate<Boolean>(value);
    }
}

Interpreter::Interpreter() : main_(this) {
    DefineBool("#t", true, main_.Ptr());
    DefineBool("#f", false, main_.Ptr());
    DefineSimpleFunc(
        "+",
        [](const std::vector<Object*>& args, Scope* scope) {
            int64_t ans = 0;
            if (args.empty()) {
                return scope->interpreter_->Allocate<Number>(ans);
            }
            for (auto arg : args) {
                if (!Is<Number>(arg)) {
                    throw RuntimeError("");
                }
                ans += As<Number>(arg)->value_;
            }
            return scope->interpreter_->Allocate<Number>(ans);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "-",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty() || !Is<Number>(args.front())) {
                throw RuntimeError("");
            }
            int64_t ans = As<Number>(args.front())->value_;
            for (size_t i = 1; i < args.size(); ++i) {
                if (!Is<Number>(args[i])) {
                    throw RuntimeError("");
                }
                ans -= As<Number>(args[i])->value_;
            }
            return scope->interpreter_->Allocate<Number>(ans);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "*",
        [](const std::vector<Object*>& args, Scope* scope) {
            int64_t ans = 1;
            if (args.empty()) {
                return scope->interpreter_->Allocate<Number>(ans);
            }
            for (auto arg : args) {
                if (!Is<Number>(arg)) {
                    throw RuntimeError("");
                }
                ans *= As<Number>(arg)->value_;
            }
            return scope->interpreter_->Allocate<Number>(ans);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "/",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty() || !Is<Number>(args.front())) {
                throw RuntimeError("");
            }
            int64_t ans = As<Number>(args.front())->value_;
            for (size_t i = 1; i < args.size(); ++i) {
                if (!Is<Number>(args[i])) {
                    throw RuntimeError("");
                }
                ans /= As<Number>(args[i])->value_;
            }
            return scope->interpreter_->Allocate<Number>(ans);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "abs",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 1 || !Is<Number>(args.front())) {
                throw RuntimeError("");
            }
            return scope->interpreter_->Allocate<Number>(
                std::abs(As<Number>(args.front())->value_));
        },
        main_.Ptr());

    DefineSimpleFunc(
        "max",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                throw RuntimeError("");
            }
            int64_t ans = INT64_MIN;
            for (auto arg : args) {
                if (!Is<Number>(arg)) {
                    throw RuntimeError("");
                }
                ans = std::max(ans, As<Number>(arg)->value_);
            }
            return scope->interpreter_->Allocate<Number>(ans);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "min",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                throw RuntimeError("");
            }
            int64_t ans = INT64_MAX;
            for (auto arg : args) {
                if (!Is<Number>(arg)) {
                    throw RuntimeError("");
                }
                ans = std::min(ans, As<Number>(arg)->value_);
            }
            return scope->interpreter_->Allocate<Number>(ans);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "number?",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 1) {
                throw RuntimeError("");
            }
            return scope->interpreter_->Allocate<Boolean>(Is<Number>(args.front()));
        },
        main_.Ptr());
    DefineSimpleFunc(
        "symbol?",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 1) {
                throw RuntimeError("");
            }
            return scope->interpreter_->Allocate<Boolean>(Is<Symbol>(args.front()));
        },
        main_.Ptr());
    DefineSimpleFunc(
        "=",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                return scope->interpreter_->Allocate<Boolean>(true);
            }
            if (!Is<Number>(args.front())) {
                throw RuntimeError("");
            }
            for (size_t i = 1; i < args.size(); ++i) {
                if (!Is<Number>(args[i])) {
                    throw RuntimeError("");
                }
                if (As<Number>(args[i])->value_ != As<Number>(args[i - 1])->value_) {
                    return scope->interpreter_->Allocate<Boolean>(false);
                }
            }
            return scope->interpreter_->Allocate<Boolean>(true);
        },
        main_.Ptr());
    DefineSimpleFunc(
        ">",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                return scope->interpreter_->Allocate<Boolean>(true);
            }
            if (!Is<Number>(args.front())) {
                throw RuntimeError("");
            }
            for (size_t i = 1; i < args.size(); ++i) {
                if (!Is<Number>(args[i])) {
                    throw RuntimeError("");
                }
                if (As<Number>(args[i])->value_ >= As<Number>(args[i - 1])->value_) {
                    return scope->interpreter_->Allocate<Boolean>(false);
                }
            }
            return scope->interpreter_->Allocate<Boolean>(true);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "<",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                return scope->interpreter_->Allocate<Boolean>(true);
            }
            if (!Is<Number>(args.front())) {
                throw RuntimeError("");
            }
            for (size_t i = 1; i < args.size(); ++i) {
                if (!Is<Number>(args[i])) {
                    throw RuntimeError("");
                }
                if (As<Number>(args[i])->value_ <= As<Number>(args[i - 1])->value_) {
                    return scope->interpreter_->Allocate<Boolean>(false);
                }
            }
            return scope->interpreter_->Allocate<Boolean>(true);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "<=",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                return scope->interpreter_->Allocate<Boolean>(true);
            }
            if (!Is<Number>(args.front())) {
                throw RuntimeError("");
            }
            for (size_t i = 1; i < args.size(); ++i) {
                if (!Is<Number>(args[i])) {
                    throw RuntimeError("");
                }
                if (As<Number>(args[i])->value_ < As<Number>(args[i - 1])->value_) {
                    return scope->interpreter_->Allocate<Boolean>(false);
                }
            }
            return scope->interpreter_->Allocate<Boolean>(true);
        },
        main_.Ptr());
    DefineSpecialFunc(
        "if",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 2 && args.size() != 3) {
                throw SyntaxError("");
            }
            Object* cond = args.front()->Eval(scope);
            if (!Is<Boolean>(cond)) {
                throw RuntimeError("");
            }
            if (As<Boolean>(cond)->value_) {
                return args[1]->Eval(scope);
            } else if (args.size() == 3) {
                return args[2]->Eval(scope);
            } else {
                return static_cast<Object*>(nullptr);
            }
        },
        main_.Ptr());
    DefineSimpleFunc(
        ">=",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                return scope->interpreter_->Allocate<Boolean>(true);
            }
            if (!Is<Number>(args.front())) {
                throw RuntimeError("");
            }
            for (size_t i = 1; i < args.size(); ++i) {
                if (!Is<Number>(args[i])) {
                    throw RuntimeError("");
                }
                if (As<Number>(args[i])->value_ > As<Number>(args[i - 1])->value_) {
                    return scope->interpreter_->Allocate<Boolean>(false);
                }
            }
            return scope->interpreter_->Allocate<Boolean>(true);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "boolean?",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 1) {
                throw RuntimeError("");
            }
            return scope->interpreter_->Allocate<Boolean>(Is<Boolean>(args.front()));
        },
        main_.Ptr());
    DefineSimpleFunc(
        "not",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 1) {
                throw RuntimeError("");
            }
            if (Is<Boolean>(args.front())) {
                return scope->interpreter_->Allocate<Boolean>(!As<Boolean>(args.front())->value_);
            }
            return scope->interpreter_->Allocate<Boolean>(false);
        },
        main_.Ptr());
    DefineSpecialFunc(
        "and",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                return As<Object>(scope->interpreter_->Allocate<Boolean>(true));
            }
            Object* ans;
            for (size_t i = 0; i < args.size() - 1; ++i) {
                if (!args[i]) {
                    throw RuntimeError{""};
                }
                ans = args[i]->Eval(scope);
                if (Is<Boolean>(ans) && !As<Boolean>(ans)->value_) {
                    return ans;
                }
            }
            if (args.back()) {
                ans = args.back()->Eval(scope);
            }
            return ans;
        },
        main_.Ptr());
    DefineSpecialFunc(
        "or",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                return As<Object>(scope->interpreter_->Allocate<Boolean>(false));
            }
            Object* ans;
            for (size_t i = 0; i < args.size() - 1; ++i) {
                if (!args[i]) {
                    throw RuntimeError{""};
                }
                ans = args[i]->Eval(scope);
                if (!Is<Boolean>(ans) || As<Boolean>(ans)->value_) {
                    return ans;
                }
            }
            if (args.back()) {
                ans = args.back()->Eval(scope);
            }
            return ans;
        },
        main_.Ptr());
    DefineQuoteFunc(
        "quote",
        [](const std::vector<Object*>& args, Scope*) {
            if (args.size() != 1) {
                throw RuntimeError("");
            }
            return args.front();
        },
        main_.Ptr());
    DefineSimpleFunc(
        "pair?",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 1) {
                throw RuntimeError("");
            }
            if (Is<Cell>(args.front())) {
                Cell* ptr = As<Cell>(args.front());
                if (ptr->first_ && ptr->second_ &&
                    (!Is<Cell>(ptr->second_) ||
                     (As<Cell>(ptr->second_)->first_ &&
                      (!As<Cell>(ptr->second_)->second_ ||
                       (Is<Cell>(As<Cell>(ptr->second_)->second_) &&
                        !As<Cell>(As<Cell>(ptr->second_)->second_)->operator bool()))))) {
                    return scope->interpreter_->Allocate<Boolean>(true);
                }
            }
            return scope->interpreter_->Allocate<Boolean>(false);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "null?",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 1) {
                throw RuntimeError("");
            }
            if ((Is<Cell>(args.front()) && !As<Cell>(args.front())->operator bool()) ||
                !args.front()) {
                return scope->interpreter_->Allocate<Boolean>(true);
            }
            return scope->interpreter_->Allocate<Boolean>(false);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "list?",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 1) {
                throw RuntimeError("");
            }
            if (!args.front()) {
                return scope->interpreter_->Allocate<Boolean>(true);
            }
            if (Is<Cell>(args.front())) {
                auto ptr = As<Cell>(args.front());
                while (Is<Cell>(ptr->second_)) {
                    ptr = As<Cell>(ptr->second_);
                }
                if (!ptr->second_) {
                    return scope->interpreter_->Allocate<Boolean>(true);
                }
            }
            return scope->interpreter_->Allocate<Boolean>(false);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "car",
        [](const std::vector<Object*>& args, Scope*) {
            if (args.size() != 1 || !Is<Cell>(args.front()) ||
                !As<Cell>(args.front())->operator bool()) {
                throw RuntimeError("");
            }
            return As<Cell>(args.front())->first_;
        },
        main_.Ptr());
    DefineSimpleFunc(
        "cdr",
        [](const std::vector<Object*>& args, Scope*) {
            if (args.size() != 1 || !Is<Cell>(args.front()) ||
                !As<Cell>(args.front())->operator bool()) {
                throw RuntimeError("");
            }
            return As<Cell>(args.front())->second_;
        },
        main_.Ptr());
    DefineSimpleFunc(
        "cons",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() != 2) {
                throw RuntimeError("");
            }
            return scope->interpreter_->Allocate<Cell>(args[0], args[1]);
        },
        main_.Ptr());
    DefineSimpleFunc(
        "list",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.empty()) {
                return static_cast<Cell*>(nullptr);
            }
            auto ans = scope->interpreter_->Allocate<Cell>(args.front());
            auto ptr = ans;
            for (size_t i = 1; i < args.size(); ++i) {
                ptr->second_ = scope->interpreter_->Allocate<Cell>(args[i]);
                ptr = As<Cell>(ptr->second_);
            }
            return ans;
        },
        main_.Ptr());
    DefineSimpleFunc(
        "list-tail",
        [](const std::vector<Object*>& args, Scope*) {
            if (args.size() != 2 || !Is<Cell>(args.front()) || !Is<Number>(args.back())) {
                throw RuntimeError("");
            }
            auto ptr = As<Cell>(args.front());
            int64_t counter = As<Number>(args.back())->value_;
            while (counter && ptr) {
                --counter;
                if (Is<Cell>(ptr->second_)) {
                    ptr = As<Cell>(ptr->second_);
                } else if (!ptr->second_) {
                    ptr = nullptr;
                } else {
                    throw RuntimeError("");
                }
            }
            if (counter) {
                throw RuntimeError("");
            }
            return ptr;
        },
        main_.Ptr());
    DefineSimpleFunc(
        "list-ref",
        [](const std::vector<Object*>& args, Scope*) {
            if (args.size() != 2 || !Is<Cell>(args.front()) || !Is<Number>(args.back())) {
                throw RuntimeError("");
            }
            auto ptr = As<Cell>(args.front());
            int64_t counter = As<Number>(args.back())->value_;
            while (counter && ptr) {
                --counter;
                if (Is<Cell>(ptr->second_)) {
                    ptr = As<Cell>(ptr->second_);
                } else if (!ptr->second_) {
                    ptr = nullptr;
                } else {
                    throw RuntimeError("");
                }
            }
            if (counter || !ptr || !ptr->first_) {
                throw RuntimeError("");
            }
            return ptr->first_;
        },
        main_.Ptr());
    DefineSpecialFunc(
        "lambda",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() < 2) {
                throw SyntaxError("");
            }
            std::vector<std::string> params;
            if (args.front()) {
                if (Is<Cell>(args.front())) {
                    Cell* cur = As<Cell>(args.front());
                    while (Is<Cell>(cur->second_)) {
                        if (Is<Symbol>(cur->first_)) {
                            params.push_back(As<Symbol>(cur->first_)->name_);
                            cur = As<Cell>(cur->second_);
                        } else {
                            throw RuntimeError("");
                        }
                    }
                    if (Is<Symbol>(cur->first_)) {
                        params.push_back(As<Symbol>(cur->first_)->name_);
                    }
                    if (Is<Symbol>(cur->second_)) {
                        params.push_back(As<Symbol>(cur->second_)->name_);
                    }
                } else {
                    throw RuntimeError("");
                }
            }
            std::vector<Object*> body;
            body.reserve(args.size() - 1);
            for (size_t i = 1; i < args.size(); ++i) {
                body.push_back(args[i]);
            }
            return scope->interpreter_->Allocate<Lambda>(
                scope, std::forward<std::vector<std::string>>(params),
                std::forward<std::vector<Object*>>(body));
        },
        main_.Ptr());
    DefineSpecialFunc(
        "define",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() == 2 && Is<Symbol>(args.front())) {
                MyDefine(As<Symbol>(args.front())->name_, args.back(), scope);
                return nullptr;
            } else if (args.size() > 1 && Is<Cell>(args.front()) &&
                       Is<Symbol>(As<Cell>(args.front())->first_)) {
                std::vector<std::string> params;
                Object* ptr = As<Cell>(args.front())->second_;
                while (ptr && Is<Cell>(ptr) && As<Cell>(ptr)->first_) {
                    if (Is<Symbol>(As<Cell>(ptr)->first_)) {
                        params.push_back(As<Symbol>(As<Cell>(ptr)->first_)->name_);
                    } else if (!Is<Symbol>(As<Cell>(ptr)->first_)) {
                        throw SyntaxError("");
                    }
                    ptr = As<Cell>(ptr)->second_;
                }
                if (Is<Symbol>(ptr)) {
                    params.push_back(As<Symbol>(ptr)->name_);
                }
                std::vector<Object*> body;
                for (size_t i = 1; i < args.size(); ++i) {
                    body.push_back(args[i]);
                }
                MyDefine(As<Symbol>(As<Cell>(args.front())->first_)->name_,
                         scope->interpreter_->Allocate<Lambda>(
                             scope, std::forward<std::vector<std::string>>(params),
                             std::forward<std::vector<Object*>>(body)),
                         scope);
                return nullptr;
            } else {
                throw SyntaxError("");
            }
        },
        main_.Ptr());
    DefineSpecialFunc(
        "set!",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() == 2 && Is<Symbol>(args.front())) {
                Scope* cur = scope;
                while (!cur->scope_.contains(As<Symbol>(args.front())->name_) && cur->father_) {
                    cur = cur->father_;
                }
                if (!cur->scope_.contains(As<Symbol>(args.front())->name_)) {
                    throw NameError("");
                }
                MyDefine(As<Symbol>(args.front())->name_, args.back(), cur);
                return GetNull();
            } else {
                throw SyntaxError("");
            }
        },
        main_.Ptr());
    DefineSpecialFunc(
        "set-car!",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() == 2) {
                Cell* ptr = As<Cell>(args.front()->Eval(scope));
                ptr->first_ = args.back()->Eval(scope);
                return GetNull();
            } else {
                throw SyntaxError("");
            }
        },
        main_.Ptr());
    DefineSpecialFunc(
        "set-cdr!",
        [](const std::vector<Object*>& args, Scope* scope) {
            if (args.size() == 2) {
                Cell* ptr = As<Cell>(args.front()->Eval(scope));
                ptr->second_ = args.back()->Eval(scope);
                return GetNull();
            } else {
                throw SyntaxError("");
            }
        },
        main_.Ptr());
}

std::string Interpreter::Run(const std::string& str) {
    std::stringstream ss{str};
    tokenizer_.Change(&ss);
    auto obj = Read(this);
    if (!tokenizer_.IsEnd()) {
        throw SyntaxError{"Syntax Error"};
    }
    if (!obj) {
        throw RuntimeError{"Runtime"};
    }
    std::string ans = Print(obj->Eval(main_.Ptr()));
    GarbageCollector();
    return ans;
}
void Interpreter::GarbageCollector() {
    main_.Reach();
    for (size_t i = 0; i < heap_.size(); ++i) {
        if (!free_.contains(i)) {
            if (!heap_[i]->Reached()) {
                heap_[i].reset();
                free_.insert(i);
            }
        }
    }
    main_.Unreach();
}

std::string Print(Object* ptr) {
    if (!ptr) {
        return "()";
    }
    return ptr->Print();
}

Scope::Scope(Interpreter* interpreter) : father_(nullptr), interpreter_(interpreter) {
}

Scope::Scope(Scope* father) : father_(father), interpreter_(father->interpreter_) {
}

Scope* Scope::Create() {
    return interpreter_->Allocate<Scope>(this);
}

Scope* Scope::Ptr() {
    return this;
}

bool Scope::Reached() {
    return reachable_;
}

void Scope::Reach() {
    reachable_ = true;
    for (auto& [key, value] : scope_) {
        if (!value->Reached()) {
            value->Reach();
        }
    }
}

void Scope::Unreach() {
    reachable_ = false;
    for (auto& [key, value] : scope_) {
        if (value->Reached()) {
            value->Unreach();
        }
    }
}

Object* Scope::Eval(Scope*) {
    return this;
}

std::string Scope::Print() {
    return "";
}
