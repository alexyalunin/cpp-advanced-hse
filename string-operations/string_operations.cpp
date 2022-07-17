#include "string_operations.h"

std::string StrCat() {
    return "";
}

bool StartsWith(std::string_view str, std::string_view text) {
    if (str.size() < text.size()) {
        return false;
    }
    for (size_t i = 0; i < text.size(); ++i) {
        if (str[i] != text[i]) {
            return false;
        }
    }
    return true;
}

bool EndsWith(std::string_view str, std::string_view text) {
    if (str.size() < text.size()) {
        return false;
    }
    for (size_t i = 0; i < text.size(); ++i) {
        if (str[str.size() - text.size() + i] != text[i]) {
            return false;
        }
    }
    return true;
}

std::string_view StripPrefix(std::string_view str, std::string_view prefix) {
    if (StartsWith(str, prefix)) {
        if (str.size() != prefix.size()) {
            return {&str[prefix.size()]};
        } else {
            return {};
        }
    }
    return str;
}

std::string_view StripSuffix(std::string_view str, std::string_view suffix) {
    if (EndsWith(str, suffix)) {
        if (str.size() != suffix.size()) {
            return {&str[0], str.size() - suffix.size()};
        } else {
            return {};
        }
    }
    return str;
}

std::string_view ClippedSubstr(std::string_view s, size_t pos, size_t n) {
    if (n > s.size()) {
        return s;
    }
    return {&s[pos], n};
}

std::string_view StripAsciiWhitespace(std::string_view str) {
    size_t beg = 0, end = str.size() - 1;
    while (beg < str.size() && std::isspace(str[beg])) {
        ++beg;
    }
    if (beg == str.size()) {
        return {};
    }
    while (std::isspace(str[end])) {
        --end;
    }
    return {&str[beg], end - beg + 1};
}

std::vector<std::string_view> StrSplit(std::string_view text, std::string_view delim) {
    if (text.size() < delim.size()) {
        std::vector<std::string_view> ans(1);
        ans[0] = std::string_view(&text[0]);
        return ans;
    }
    size_t count = 0, beg = 0;
    bool flag = true;
    for (size_t i = 0; i <= text.size() - delim.size();) {
        for (size_t j = 0; j < delim.size(); ++j) {
            if (text[i + j] != delim[j]) {
                ++i;
                flag = false;
                break;
            }
        }
        if (flag) {
            ++count;
            i += delim.size();
        }
        flag = true;
    }
    std::vector<std::string_view> ans(count + 1);
    count = 0;
    for (size_t i = 0; i <= text.size() - delim.size();) {
        for (size_t j = 0; j < delim.size(); ++j) {
            if (text[i + j] != delim[j]) {
                ++i;
                flag = false;
                break;
            }
        }
        if (flag) {
            ans[count] = std::string_view(&text[beg], i - beg);
            i += delim.size();
            beg = i;
            ++count;
        }
        flag = true;
    }
    ans[count] = std::string_view(&text[beg], text.size() - beg);
    return ans;
}

std::string ReadN(const std::string &filename, size_t n) {
    int file = open(filename.data(), O_RDONLY);
    if (file == -1) {
        return "";
    }
    std::string ans(n, 'a');
    int res = read(file, &ans[0], n);
    if (res == -1) {
        return "";
    }
    close(file);
    return ans;
}

std::string AddSlash(std::string_view path) {
    std::string ans;
    if (path.back() == '/') {
        ans.reserve(path.size());
        ans += path;
    } else {
        ans.reserve(path.size() + 1);
        ans += path;
        ans += '/';
    }
    return ans;
}

std::string_view RemoveSlash(std::string_view path) {
    if (path.back() != '/' || path.size() == 1) {
        return {&path[0]};
    }
    return {&path[0], path.size() - 1};
}

std::string_view Dirname(std::string_view path) {
    size_t end = path.size() - 1;
    while (path[end] != '/') {
        --end;
    }
    if (end == 0) {
        return {&path[0], 1};
    }
    return {&path[0], end};
}

std::string_view Basename(std::string_view path) {
    size_t end = path.size() - 1;
    while (path[end] != '/') {
        --end;
    }
    return {&path[end + 1]};
}

std::string CollapseSlashes(std::string_view path) {
    std::string ans;
    if (path.empty() || path[0] != '/') {
        ans.reserve(path.size());
        ans += path;
        return ans;
    }
    size_t i = 0;
    while (i != path.size() && path[i] == '/') {
        ++i;
    }
    if (i == path.size()) {
        return "/";
    }
    size_t count = 1;
    while (i != path.size()) {
        if (path[i] != '/') {
            ++count;
            ++i;
            continue;
        }
        ++count;
        while (i != path.size() && path[i] == '/') {
            ++i;
        }
    }
    if (count == path.size()) {
        ans.reserve(path.size());
        ans += path;
        return ans;
    }
    ans.reserve(count);
    i = 0;
    while (i != path.size()) {
        if (path[i] != '/') {
            ans += path[i];
            ++i;
            continue;
        }
        ans += path[i];
        while (i != path.size() && path[i] == '/') {
            ++i;
        }
    }
    return ans;
}

std::string StrJoin(const std::vector<std::string_view> &strings, std::string_view delimiter) {
    if (strings.empty()) {
        return "";
    }
    size_t n = (strings.size() - 1) * delimiter.size();
    for (const auto &str : strings) {
        n += str.size();
    }
    std::string ans(n, 'a');
    size_t j = 0;
    for (size_t i = 0; i < strings.size(); ++i) {
        for (auto ch : strings[i]) {
            ans[j] = ch;
            ++j;
        }
        if (i != strings.size() - 1) {
            for (auto ch : delimiter) {
                ans[j] = ch;
                ++j;
            }
        }
    }
    return ans;
}