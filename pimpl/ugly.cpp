#include <ugly.h>
#include <bad.h>

struct SplineImpl {
    SplineImpl(const std::vector<double>& x, const std::vector<double>& y, double a, double b)
        : x_(x), y_(y) {
        sp_.resize(x.size());
        mySplineSnd(x.data(), y.data(), sp_.size(), a, b, sp_.data());
    }
    double Interpolate(double x) {
        double ans;
        mySplintCube(x_.data(), y_.data(), sp_.data(), sp_.size(), x, &ans);
        return ans;
    }

private:
    std::vector<double> x_;
    std::vector<double> y_;
    std::vector<double> sp_;
};

Spline::Spline(const std::vector<double>& x, const std::vector<double>& y, double a, double b)
    : impl_(std::make_shared<SplineImpl>(x, y, a, b)) {
}

double Spline::Interpolate(double x) {
    return impl_->Interpolate(x);
}