#include "find_subsets.h"
#pragma GCC optimize("O3")
#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <string>
#include <thread>
#include <mutex>
#include <atomic>

std::mutex a;

void Compute(std::unordered_map<int64_t, std::pair<std::pair<size_t, size_t>, std::string>>& lsums,
             std::string& mask, std::vector<int64_t>& left, int64_t csum, size_t ind, size_t l,
             size_t r) {
    if (ind == left.size() - 1) {
        lsums[csum] = {{l, r}, mask};
        mask[ind] = '1';
        lsums[csum + left[ind]] = {{l + 1, r}, mask};
        mask[ind] = '2';
        lsums[csum - left[ind]] = {{l, r + 1}, mask};
        mask[ind] = '0';
        return;
    }
    Compute(lsums, mask, left, csum, ind + 1, l, r);
    mask[ind] = '1';
    Compute(lsums, mask, left, csum + left[ind], ind + 1, l + 1, r);
    mask[ind] = '2';
    Compute(lsums, mask, left, csum - left[ind], ind + 1, l, r + 1);
    mask[ind] = '0';
}

void Find(
    std::string& mask, std::vector<int64_t>& right, int64_t csum, size_t ind,
    std::vector<std::unordered_map<int64_t, std::pair<std::pair<size_t, size_t>, std::string>>>&
        vev,
    std::atomic_bool& found, std::string& ans, size_t l, size_t r) {
    if (found) {
        return;
    }
    if (ind == right.size() - 1) {
        for (size_t i = 0; i < vev.size(); ++i) {
            if (vev[i].contains(csum) && vev[i].at(csum).first.first + l &&
                vev[i].at(csum).first.second + r) {
                found = true;
                a.lock();
                ans = vev[i].at(csum).second + mask;
                a.unlock();
                return;
            }
        }
        mask[ind] = '2';
        for (size_t i = 0; i < vev.size(); ++i) {
            if (vev[i].contains(csum + right[ind]) &&
                vev[i].at(csum + right[ind]).first.first + l) {
                found = true;
                a.lock();
                ans = vev[i].at(csum + right[ind]).second + mask;
                a.unlock();
                return;
            }
        }
        mask[ind] = '1';
        for (size_t i = 0; i < vev.size(); ++i) {
            if (vev[i].contains(csum - right[ind]) &&
                vev[i].at(csum - right[ind]).first.second + r) {
                found = true;
                a.lock();
                ans = vev[i].at(csum - right[ind]).second + mask;
                a.unlock();
                return;
            }
        }
        mask[ind] = '0';
        return;
    }
    Find(mask, right, csum, ind + 1, vev, found, ans, l, r);
    mask[ind] = '2';
    Find(mask, right, csum + right[ind], ind + 1, vev, found, ans, l, r + 1);
    mask[ind] = '1';
    Find(mask, right, csum - right[ind], ind + 1, vev, found, ans, l + 1, r);
    mask[ind] = '0';
}

Subsets FindEqualSumSubsets(const std::vector<int64_t>& data) {
    if (data.size() < 2) {
        return {{}, {}, false};
    }
    std::vector<int64_t> left((data.size() / 2) + data.size() % 2);
    std::vector<int64_t> right(data.size() / 2);
    for (size_t i = 0; i < left.size(); ++i) {
        left[i] = data[i];
    }
    for (size_t i = left.size(); i < data.size(); ++i) {
        right[i - left.size()] = data[i];
    }
    if (data.size() < 12) {
        std::vector<std::unordered_map<int64_t, std::pair<std::pair<size_t, size_t>, std::string>>>
            vec(1);
        std::string mask(left.size(), '0');
        Compute(vec[0], mask, left, 0, 0, 0, 0);
        if (vec[0][0].first.first && vec[0][0].first.second) {
            std::string ms = vec[0][0].second;
            std::vector<size_t> l;
            std::vector<size_t> r;
            for (size_t i = 0; i < left.size(); ++i) {
                if (ms[i] == '1') {
                    l.push_back(i);
                } else if (ms[i] == '2') {
                    r.push_back(i);
                }
            }
            return {l, r, true};
        }
        if (data.size() % 2) {
            mask.pop_back();
        }
        std::atomic_bool found(false);
        std::string ans;
        Find(mask, right, 0, 0, vec, found, ans, 0, 0);
        if (found) {
            std::vector<size_t> l;
            std::vector<size_t> r;
            for (size_t i = 0; i < ans.size(); ++i) {
                if (ans[i] == '1') {
                    l.push_back(i);
                } else if (ans[i] == '2') {
                    r.push_back(i);
                }
            }
            return {l, r, true};
        }
        return {{}, {}, false};
    } else {
        std::string nn(left.size(), '0');
        std::string fn(left.size(), '0');
        fn[0] = '1';
        std::string sn(left.size(), '0');
        sn[0] = '2';
        std::string nf(left.size(), '0');
        nf[1] = '1';
        std::string ff(left.size(), '0');
        ff[0] = '1';
        ff[1] = '1';
        std::string sf(left.size(), '0');
        sf[0] = '2';
        sf[1] = '1';
        std::string ns(left.size(), '0');
        ns[1] = '2';
        std::string fs(left.size(), '0');
        fs[0] = '1';
        fs[1] = '2';
        std::string ss(left.size(), '0');
        ss[0] = '2';
        ss[1] = '2';
        std::vector<std::unordered_map<int64_t, std::pair<std::pair<size_t, size_t>, std::string>>>
            vec(9);
        std::thread tnn(Compute, std::ref(vec[0]), std::ref(nn), std::ref(left), 0, 2, 0, 0);
        std::thread tfn(Compute, std::ref(vec[1]), std::ref(fn), std::ref(left), left[0], 2, 1, 0);
        std::thread tsn(Compute, std::ref(vec[2]), std::ref(sn), std::ref(left), -left[0], 2, 0, 1);
        std::thread tnf(Compute, std::ref(vec[3]), std::ref(nf), std::ref(left), left[1], 2, 1, 0);
        std::thread tff(Compute, std::ref(vec[4]), std::ref(ff), std::ref(left), left[0] + left[1],
                        2, 2, 0);
        std::thread tsf(Compute, std::ref(vec[5]), std::ref(sf), std::ref(left), -left[0] + left[1],
                        2, 1, 1);
        std::thread tns(Compute, std::ref(vec[6]), std::ref(ns), std::ref(left), -left[1], 2, 0, 1);
        std::thread tfs(Compute, std::ref(vec[7]), std::ref(fs), std::ref(left), left[0] - left[1],
                        2, 1, 1);
        std::thread tss(Compute, std::ref(vec[8]), std::ref(ss), std::ref(left), -left[0] - left[1],
                        2, 0, 2);
        tnn.join();
        tfn.join();
        tsn.join();
        tnf.join();
        tff.join();
        tsf.join();
        tns.join();
        tfs.join();
        tss.join();
        if (data.size() % 2) {
            nn.pop_back();
            fn.pop_back();
            sn.pop_back();
            nf.pop_back();
            ff.pop_back();
            sf.pop_back();
            ns.pop_back();
            fs.pop_back();
            ss.pop_back();
        }
        std::atomic_bool found(false);
        std::string ans;
        std::thread snn(Find, std::ref(nn), std::ref(right), 0, 2, std::ref(vec), std::ref(found),
                        std::ref(ans), 0, 0);
        std::thread sfn(Find, std::ref(fn), std::ref(right), -right[0], 2, std::ref(vec),
                        std::ref(found), std::ref(ans), 1, 0);
        std::thread ssn(Find, std::ref(sn), std::ref(right), right[0], 2, std::ref(vec),
                        std::ref(found), std::ref(ans), 0, 1);
        std::thread snf(Find, std::ref(nf), std::ref(right), -right[1], 2, std::ref(vec),
                        std::ref(found), std::ref(ans), 1, 0);
        std::thread sff(Find, std::ref(ff), std::ref(right), -right[0] - right[1], 2, std::ref(vec),
                        std::ref(found), std::ref(ans), 2, 0);
        std::thread ssf(Find, std::ref(sf), std::ref(right), right[0] - right[1], 2, std::ref(vec),
                        std::ref(found), std::ref(ans), 1, 1);
        std::thread sns(Find, std::ref(ns), std::ref(right), right[1], 2, std::ref(vec),
                        std::ref(found), std::ref(ans), 0, 1);
        std::thread sfs(Find, std::ref(fs), std::ref(right), -right[0] + right[1], 2, std::ref(vec),
                        std::ref(found), std::ref(ans), 1, 1);
        std::thread sss(Find, std::ref(ss), std::ref(right), right[0] + right[1], 2, std::ref(vec),
                        std::ref(found), std::ref(ans), 0, 2);
        snn.join();
        sfn.join();
        ssn.join();
        snf.join();
        sff.join();
        ssf.join();
        sns.join();
        sfs.join();
        sss.join();
        if (found) {
            std::vector<size_t> l;
            std::vector<size_t> r;
            for (size_t i = 0; i < ans.size(); ++i) {
                if (ans[i] == '1') {
                    l.push_back(i);
                } else if (ans[i] == '2') {
                    r.push_back(i);
                }
            }
            return {l, r, true};
        }
        return {{}, {}, false};
    }
}