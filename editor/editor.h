#pragma once
#include <vector>
#include <string>
#include <memory>

class ICommand {
public:
    virtual bool Do() = 0;
    virtual void Undo() = 0;
    virtual ~ICommand() = default;
};

class Editor {
    friend class TypeCommand;
    friend class BackspaceCommand;
    friend class LShiftCommand;
    friend class RShiftCommand;

public:
    const std::string& GetText() const {
        return buffer_;
    }

    void Type(char c);

    void ShiftLeft();

    void ShiftRight();

    void Backspace();

    void Undo();

    void Redo();

private:
    bool LShift() {
        if (pos_ == 0) {
            return false;
        }
        --pos_;
        return true;
    }

    bool RShift() {
        if (pos_ == buffer_.size()) {
            return false;
        }
        ++pos_;
        return true;
    }

    bool Insert(char c) {
        buffer_.insert(pos_, 1, c);
        ++pos_;
        return true;
    }

    char Back() {
        if (pos_ == 0) {
            return false;
        }
        --pos_;
        char temp = buffer_[pos_];
        buffer_.erase(pos_, 1);
        return temp;
    }

    void AddCommand(std::unique_ptr<ICommand>&& command) {
        if (command->Do()) {
            do_.push_back(std::move(command));
            undo_.clear();
        }
    }
    std::vector<std::unique_ptr<ICommand>> do_;
    std::vector<std::unique_ptr<ICommand>> undo_;
    std::string buffer_;
    size_t pos_ = 0;
};

class TypeCommand : public ICommand {
public:
    TypeCommand(Editor* ed, char ch) : ed_(ed), ch_(ch) {
    }
    bool Do() override {
        return ed_->Insert(ch_);
    }
    void Undo() override {
        ed_->Back();
    }
    Editor* ed_;
    char ch_;
};

class BackspaceCommand : public ICommand {
public:
    BackspaceCommand(Editor* ed) : ed_(ed) {
    }
    bool Do() override {
        ch_ = ed_->Back();
        return ch_;
    }
    void Undo() override {
        ed_->Insert(ch_);
    }
    Editor* ed_;
    char ch_;
};

class LShiftCommand : public ICommand {
public:
    LShiftCommand(Editor* ed) : ed_(ed) {
    }
    bool Do() override {
        return ed_->LShift();
    }
    void Undo() override {
        ed_->RShift();
    }
    Editor* ed_;
};

class RShiftCommand : public ICommand {
public:
    RShiftCommand(Editor* ed) : ed_(ed) {
    }
    bool Do() override {
        return ed_->RShift();
    }
    void Undo() override {
        ed_->LShift();
    }
    Editor* ed_;
};

void Editor::Type(char c) {
    AddCommand(std::make_unique<TypeCommand>(this, c));
}

void Editor::Backspace() {
    AddCommand(std::make_unique<BackspaceCommand>(this));
}

void Editor::ShiftLeft() {
    AddCommand(std::make_unique<LShiftCommand>(this));
}

void Editor::ShiftRight() {
    AddCommand(std::make_unique<RShiftCommand>(this));
}

void Editor::Undo() {
    if (!do_.empty()) {
        do_.back()->Undo();
        undo_.push_back(std::move(do_.back()));
        do_.pop_back();
    }
}

void Editor::Redo() {
    if (!undo_.empty()) {
        undo_.back()->Do();
        do_.push_back(std::move(undo_.back()));
        undo_.pop_back();
    }
}