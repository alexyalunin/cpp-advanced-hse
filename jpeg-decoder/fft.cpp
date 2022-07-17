#include "fft.h"

#include <stdexcept>
#include <cmath>

DctCalculator::DctCalculator(size_t width, std::vector<double> *input, std::vector<double> *output)
    : width_(width), input_(input) {
    if (!width || input->size() != output->size() || (input->size() / width == 0) ||
        (input->size() % width)) {
        throw std::invalid_argument("Bad dimensions");
    }
    plan_ = fftw_plan_r2r_2d(width, width, input->data(), output->data(), FFTW_REDFT01,
                             FFTW_REDFT01, FFTW_ESTIMATE);
}

void DctCalculator::Inverse() {
    for (size_t i = 0; i < width_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
            input_->data()[i + j * width_] /= 16;
        }
    }
    for (size_t i = 0; i < width_; ++i) {
        input_->data()[i * width_] /= M_SQRT1_2;
        input_->data()[i] /= M_SQRT1_2;
    }
    fftw_execute(plan_);
}

DctCalculator::~DctCalculator() {
    fftw_destroy_plan(plan_);
    fftw_cleanup();
}
