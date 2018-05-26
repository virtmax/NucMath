
#include <random>
#include <mutex>
#include <array>
#include <atomic>
#include <thread>
#include <future>

#include "constants.h"

#pragma once

namespace nucmath {

class SignalReconstruction
{
public:

    SignalReconstruction();
    ~SignalReconstruction();

   bool reconstruct(const std::vector<float> &y, int factor, std::vector<float> &rec_signal);

private:

   void reconstructThreadWorker(std::vector<float> y, int factor, std::vector<float> &rec_signal,size_t from, size_t to);

   std::mutex mutexSignalSum;
   std::vector<std::thread> threads;
};

}


