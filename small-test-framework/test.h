#pragma once
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include <cstring>
class AbstractTest {
public:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual void Run() = 0;
    virtual ~AbstractTest() = default;
};

class AbstractTestProducer {
public:
    virtual std::unique_ptr<AbstractTest> Produce() = 0;
    virtual ~AbstractTestProducer() = default;
};

template <typename Test>
class TestProducer : public AbstractTestProducer {
public:
    std::unique_ptr<AbstractTest> Produce() override {
        return std::make_unique<Test>();
    }
};

class Pred {
public:
    virtual bool operator()(const std::string& test) const = 0;
    virtual ~Pred() = default;
};

class FullMatch : Pred {
public:
    FullMatch(const std::string& pred) : pred_(pred) {
    }
    bool operator()(const std::string& test) const override {
        return pred_ == test;
    }
    std::string pred_;
};

class Substr : public Pred {
public:
    Substr(const std::string& pred) : pred_(pred) {
    }
    bool operator()(const std::string& test) const override {
        return strstr(test.data(), pred_.data());
    }
    std::string pred_;
};
class TestRegistry {
public:
    template <class TestClass>
    void RegisterClass(const std::string& class_name) {
        tests[class_name] = std::make_unique<TestProducer<TestClass>>();
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string& class_name) {
        if (tests.find(class_name) == tests.end()) {
            throw std::out_of_range("");
        }
        return tests[class_name]->Produce();
    }

    void RunTest(const std::string& test_name) {
        auto cur_test = CreateTest(test_name);
        cur_test->SetUp();
        try {
            cur_test->Run();
        } catch (...) {
            cur_test->TearDown();
            throw;
        }
        cur_test->TearDown();
    }

    template <class Predicate>
    std::vector<std::string> ShowTests(Predicate callback) const {
        std::vector<std::string> ans;
        ans.reserve(tests.size());
        for (auto& [key, value] : tests) {
            if (callback(key)) {
                ans.push_back(key);
            }
        }
        return ans;
    }

    std::vector<std::string> ShowAllTests() const {
        std::vector<std::string> ans;
        ans.reserve(tests.size());
        for (auto& [key, value] : tests) {
            ans.push_back(key);
        }
        return ans;
    }

    template <class Predicate>
    void RunTests(Predicate callback) {
        for (auto& [key, value] : tests) {
            if (callback(key)) {
                RunTest(key);
            }
        }
    }

    void Clear() {
        tests.clear();
    }
    static TestRegistry& Instance() {
        static TestRegistry instance;
        return instance;
    }

    std::map<std::string, std::unique_ptr<AbstractTestProducer>> tests;
};