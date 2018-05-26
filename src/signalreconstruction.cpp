

#include "signalreconstruction.h"

using namespace nucmath;

inline double sinc(double x)
{
    if(x > -1e-8 && x < 1e-8)
        return 1.0;

    return std::sin(PI*x) / (PI*x);
}

SignalReconstruction::SignalReconstruction()
{

}

SignalReconstruction::~SignalReconstruction()
{

}


bool SignalReconstruction::reconstruct(const std::vector<float> &y, int factor, std::vector<float> &rec_signal)
{
    if(y.size() < 10)
        return false;

    rec_signal.clear();
    rec_signal.resize(y.size()*factor,0);

  //   std::cout<<y.size()<<std::endl;
    int threadsN = 1;
    int step = floor(y.size()/(float)threadsN);
    for(int i = 0; i < threadsN; i++)
    {

        int from = step*i;
        int to = step*i+step;

        if(i==threadsN-1)
            to = y.size();

     //   std::cout<<from<<"-"<<to<<std::endl;

      //  reconstructThreadWorker(y, factor, from, to);

     //   threads.push_back(std::thread(&SignalReconstruction::reconstructThreadWorker, this, y, factor, from, to));
       // threads.push_back(std::async(&SignalReconstruction::reconstructThreadWorker, this, y, factor, from, to));

    }
/*
    auto a1 = std::thread(&SignalReconstruction::reconstructThreadWorker, this, y, factor, 0, step);
    auto a2 = std::thread(&SignalReconstruction::reconstructThreadWorker, this, y, factor, step, y.size());

    a1.join();
    a2.join();
*/
/*
    auto a1 = std::async(&SignalReconstruction::reconstructThreadWorker, this, y, factor, 0, step);
    auto a2 = std::async(&SignalReconstruction::reconstructThreadWorker, this, y, factor, step, y.size());

    a1.get();
    a2.get();
  */

    reconstructThreadWorker(y, factor, rec_signal, 0, y.size());

  //  auto f1 = std::async(&SignalReconstruction::reconstructThreadWorker, this, y, factor, 0, y.size()-1);
  //  f1.get();

   // auto t1 = std::thread(&SignalReconstruction::reconstructThreadWorker, this, y, factor, 0, y.size()-1);
   // t1.join();

    for(auto& thread : threads)
    {
        thread.join();
    }
    threads.clear();


/*
    for(size_t i = 0; i < y.size()*factor; ++i)
    {
        rec_signal.push_back(rec_signal_sum[i]);
      //   rec_signal[i] = rec_signal_sum[i];
    }
*/
    return true;
}

void SignalReconstruction::reconstructThreadWorker(std::vector<float> y, int factor, std::vector<float> &rec_signal, size_t from, size_t to)
{
    const double B = 1.0;
    const int a = 4;

    std::vector<float> rec_signal_part(y.size()*factor,0);

    for(size_t i = from; i < to; ++i)
    {
        for(int ai = -factor*a; ai < a*factor; ai++)
        {
            const int indx = i*factor + ai;
            if(indx >= 0 && indx < rec_signal_part.size())
            {
                const double sinc_arg = B*(static_cast<double>(ai)/static_cast<double>(factor));

                rec_signal_part[indx] += B*sinc(sinc_arg)*sinc(sinc_arg/3)*y[i];
            }
        }
    }


    std::lock_guard<std::mutex> lockguard(mutexSignalSum);
    for(size_t i = 0; i < y.size()*factor; ++i)
    {
        rec_signal[i] +=  rec_signal_part[i];
    }
}


