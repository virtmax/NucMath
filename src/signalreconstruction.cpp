

#include "signalreconstruction.h"

nucmath::SignalReconstruction::SignalReconstruction()
{
}

nucmath::SignalReconstruction::~SignalReconstruction()
{
}


bool nucmath::SignalReconstruction::reconstruct(const std::vector<double>& y, int factor, std::vector<double>& rec_signal)
{
    if(y.size() < 10)
        return false;

    rec_signal.clear();
    rec_signal.resize(y.size() * factor, 0);

    reconstructWorker(y, factor, rec_signal, 0, y.size());

    return true;
}

void nucmath::SignalReconstruction::reconstructWorker(
    const std::vector<double>& y, int factor, std::vector<double>& rec_signal, size_t from, size_t to)
{
    const double B = 1.0;
    const int a = 4;

    std::vector<float> rec_signal_part(y.size() * factor, 0);

    for(size_t i = from; i < to; ++i)
    {
        for(int ai = -factor * a; ai < a * factor; ai++)
        {
            const int indx = static_cast<int>(i) * factor + ai;
            if(indx >= 0 && indx < static_cast<int>(rec_signal_part.size()))
            {
                const double sinc_arg = B * static_cast<double>(ai) / static_cast<double>(factor);

                rec_signal_part[indx] += static_cast<float>(B * sinc(sinc_arg) * sinc(sinc_arg / 3.0) * y.at(i));
            }
        }
    }

    for(size_t i = 0; i < y.size() * factor; ++i)
    {
        rec_signal[i] += rec_signal_part[i];
    }
}
