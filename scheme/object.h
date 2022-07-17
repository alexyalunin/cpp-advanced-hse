#pragma once
#include "error.h"
#include "tokenizer.h"
#include <memory>
#include <unordered_map>
#include <vector>
#include <unordered_set>
class Scope;

class Interpreter;

class Object {
public:
    virtual ~Object() = default;
    virtual void Reach() = 0;
    virtual void Unreach() = 0;
    virtual bool Reached() = 0;
    virtual Object* Eval(Scope* scope) = 0;
    virtual std::string Print() = 0;
};

class Saver : public Object {
public:
    Saver(Object* obj, Scope* scp);
    void Reach() override;
    void Unreach() override;
    bool Reached() override;
    Object* Eval(Scope* scope) override;
    std::string Print() override;
    Object* obj_;
    Scope* scp_;
    bool reachable_ = false;
};

std::string Print(Object* ptr);

class Scope : public Object {
public:
    Scope(Interpreter* interpreter);
    Scope(Scope* father);
    Scope* Create();
    Scope* Ptr();
    void Reach() override;
    void Unreach() override;
    bool Reached() override;
    Object* Eval(Scope* scope) override;
    std::string Print() override;
    Scope* father_;
    Interpreter* interpreter_;
    std::unordered_map<std::string, Object*> scope_;
    bool reachable_ = false;
};

class Interpreter {
public:
    Interpreter();

    template <typename T, typename... Args>
    T* Allocate(Args&&... args) {
        if (free_.empty()) {
            heap_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
            return dynamic_cast<T*>(heap_.back().get());
        } else {
            size_t ind = *free_.begin();
            free_.erase(free_.begin());
            heap_[ind] = std::make_unique<T>(std::forward<Args>(args)...);
            return dynamic_cast<T*>(heap_[ind].get());
        }
    }
    std::string Run(const std::string& str);
    void GarbageCollector();
    std::unordered_set<size_t> free_;
    std::vector<std::unique_ptr<Object>> heap_;
    Scope main_;
    Tokenizer tokenizer_;
};

class SetReturn : public Object {
    void Reach() override;
    void Unreach() override;
    bool Reached() override;
    Object* Eval(Scope*) override;
    std::string Print() override;
};

SetReturn* GetNull();

class Number : public Object {
public:
    Number(int64_t value);
    int64_t GetValue() const;
    void Reach() override;
    void Unreach() override;
    bool Reached() override;
    Object* Eval(Scope*) override;
    std::string Print() override;
    int64_t value_;
    bool reachable_ = false;
};

class Symbol : public Object {
public:
    Symbol(std::string name);
    const std::string& GetName() const;
    void Reach() override;
    void Unreach() override;
    bool Reached() override;
    Object* Eval(Scope* scope) override;
    std::string Print() override;
    std::string name_;
    bool reachable_ = false;
};

class Cell : public Object {
public:
    Cell();
    template <typename F, typename S>
    Cell(F* first, S* second) : first_(first), second_(second) {
    }
    template <typename F>
    Cell(F* first) : first_(first), second_(nullptr) {
    }
    Object* GetFirst() const;
    Object* GetSecond() const;
    operator bool() const;
    void Reach() override;
    void Unreach() override;
    bool Reached() override;

    Object* Eval(Scope* scope) override;
    std::string Print() override;
    Object* first_;
    Object* second_;
    bool reachable_ = false;
};

class Boolean : public Object {
public:
    Boolean(bool value);
    void Reach() override;
    void Unreach() override;
    bool Reached() override;
    Object* Eval(Scope*) override;
    std::string Print() override;
    bool value_;
    bool reachable_ = false;
};

template <class T>
T* As(Object* obj) {
    return dynamic_cast<T*>(obj);
}

template <class T>
bool Is(Object* obj) {
    return bool(dynamic_cast<T*>(obj));
}

class FunctionBase : public Object {
public:
    virtual Object* Apply(const std::vector<Object*>& data, Scope* scp) = 0;
};

class QuoteHelper : public FunctionBase {};

template <typename F>
class Quote : public QuoteHelper {
public:
    Quote(F&& func, Scope* scope) : func_(std::forward<F>(func)), scope_(scope) {
    }
    Object* Apply(const std::vector<Object*>& data, Scope* scp) override {
        return func_(data, scp);
    }
    void Reach() override {
        reachable_ = true;
        if (!scope_->reachable_) {
            scope_->Reach();
        }
    }
    void Unreach() override {
        reachable_ = false;
        if (scope_->reachable_) {
            scope_->Unreach();
        }
    }
    bool Reached() override {
        return reachable_;
    }
    Object* Eval(Scope*) override {
        return this;
    }
    std::string Print() override {
        return "";
    }
    F func_;
    Scope* scope_;
    bool reachable_ = false;
};

template <typename F>
class FunctionSimple : public FunctionBase {
public:
    FunctionSimple(F&& func, Scope* scope) : func_(std::forward<F>(func)), scope_(scope) {
    }
    Object* Apply(const std::vector<Object*>& data, Scope* scp) override {
        std::vector<Object*> args;
        args.reserve(data.size());
        for (auto i : data) {
            if (!i) {
                throw RuntimeError("");
            }
            args.push_back(i->Eval(scp));
        }
        return func_(args, scp);
    }
    void Reach() override {
        reachable_ = true;
        if (!scope_->reachable_) {
            scope_->Reach();
        }
    }
    void Unreach() override {
        reachable_ = false;
        if (scope_->reachable_) {
            scope_->Unreach();
        }
    }
    bool Reached() override {
        return reachable_;
    }
    Object* Eval(Scope*) override {
        return this;
    }
    std::string Print() override {
        return "";
    }
    F func_;
    Scope* scope_;
    bool reachable_ = false;
};

template <typename F>
class FunctionSpecial : public FunctionBase {
public:
    FunctionSpecial(F&& func, Scope* scope) : func_(std::forward<F>(func)), scope_(scope) {
    }
    Object* Apply(const std::vector<Object*>& data, Scope* scp) override {
        return func_(data, scp);
    }
    void Reach() override {
        reachable_ = true;
        if (!scope_->reachable_) {
            scope_->Reach();
        }
    }
    void Unreach() override {
        reachable_ = false;
        if (scope_->reachable_) {
            scope_->Unreach();
        }
    }
    bool Reached() override {
        return reachable_;
    }
    Object* Eval(Scope*) override {
        return this;
    }
    std::string Print() override {
        return "";
    }
    F func_;
    Scope* scope_;
    bool reachable_ = false;
};

class Lambda : public FunctionBase {
public:
    Lambda(Scope* scope, std::vector<std::string>&& args, std::vector<Object*>&& body);
    Object* Apply(const std::vector<Object*>& data, Scope* scp) override;
    Object* Eval(Scope*) override;
    void Reach() override;
    void Unreach() override;
    bool Reached() override;
    std::string Print() override;
    Scope* scope_;
    bool reachable_ = false;
    std::vector<std::string> args_;
    std::vector<Object*> body_;
};

template <typename F>
void DefineSimpleFunc(const std::string& name, F&& func, Scope* father) {
    father->scope_[name] =
        father->interpreter_->template Allocate<FunctionSimple<F>>(std::forward<F>(func), father);
}

template <typename F>
void DefineSpecialFunc(const std::string& name, F&& func, Scope* father) {
    if (!father->scope_.contains(name)) {
        father->scope_[name] = father->interpreter_->template Allocate<FunctionSpecial<F>>(
            std::forward<F>(func), father);
    }
}

template <typename F>
void DefineQuoteFunc(const std::string& name, F&& func, Scope* father) {
    if (!father->scope_.contains(name)) {
        father->scope_[name] =
            father->interpreter_->template Allocate<Quote<F>>(std::forward<F>(func), father);
    }
}