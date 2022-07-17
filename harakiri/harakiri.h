#pragma once

#include <string>

// Should not allow reuse and yell under sanitizers.
// Fix the interface and implementation.
// AwesomeCallback should add "awesomeness".

class OneTimeCallback {
public:
    virtual ~OneTimeCallback() = default;
    virtual std::string operator()() const& = delete;
    virtual std::string operator()() const&& = 0;
};

// Implement ctor, operator(), maybe something else...
class AwesomeCallback : public OneTimeCallback {
public:
    std::string sta;
    AwesomeCallback(const std::string& s) : sta(s) {
    }

    std::string operator()() const&& override {
        std::string output = sta + "awesomeness";
        delete this;
        return output;
    }
};
