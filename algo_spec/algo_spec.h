#pragma once

#include "algo_impl.h"
#include <type_traits>

/// Use one of the following:
/// AlgoRes ComputeIntegral(data);
/// AlgoRes ComputeUnsignedIntegral(data);
/// AlgoRes ComputeFloat(data);
/// AlgoRes ComputeGeneral(data);

template <class T>
AlgoRes Run(T data) {
    if constexpr (std::is_unsigned_v<T>) {
        return ComputeUnsignedIntegral(data);
    } else if constexpr (std::is_integral_v<T>) {
        return ComputeIntegral(data);
    } else if constexpr (std::is_floating_point_v<T>) {
        return ComputeFloat(data);
    } else {
        return ComputeGeneral(data);
    }
}
