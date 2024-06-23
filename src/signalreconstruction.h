
#pragma once

#include "constants.h"
#include "functions.h"

#include <array>
#include <random>

namespace nucmath
{

class SignalReconstruction
{
public:
    SignalReconstruction();
    ~SignalReconstruction();

    bool reconstruct(const std::vector<double>& y, int factor, std::vector<double>& rec_signal);

private:
    void reconstructWorker(const std::vector<double>& y, int factor, std::vector<double>& rec_signal, size_t from, size_t to);
};

}
