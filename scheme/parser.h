#pragma once

#include <memory>

#include "object.h"
#include "tokenizer.h"

void MyDefine(const std::string& name, Object* obj, Scope* scope);

void DefineBool(const std::string& name, bool value, Scope* father);

std::vector<Object*> Ltv(Cell* ptr);

Interpreter* Singleton();

Object* ReadList(Tokenizer* tokenizer);

Object* Read(Tokenizer* tokenizer);

Object* Read(Interpreter* interpreter, Tokenizer* tokenizer = nullptr);

Object* ReadList(Interpreter* interpreter, Tokenizer* tokenizer = nullptr);