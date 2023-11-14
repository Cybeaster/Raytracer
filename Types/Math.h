#pragma once

#include <boost/qvm.hpp>


template<typename T, int D>
using TVec = boost::qvm::vec<T, D>;

template<typename T, int R, int C>
using TMat = boost::qvm::mat<T, R, C>;


using SVec3 = TVec<float, 3>;
using SVec4 = TVec<float, 4>;


using SOMat3 = TMat<float, 3, 3>;
using SMat4 = TMat<float, 4, 4>;
