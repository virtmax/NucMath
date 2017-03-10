
namespace nucmath
{

template <class T>
double leadingEdgeDiscriminator(const std::vector<T> &sample, T threshold, size_t start = 0, bool negative = true)
{
    if (sample.size() < 3)
        return -1;

    if (negative)
    {
        for (size_t i = start; i < sample.size() - 1; ++i)
        {
            if (sample[i] > threshold && threshold > sample[i + 1])
            {
                return (threshold - sample[i]) / static_cast<double>(sample[i + 1] - sample[i]) + static_cast<double>(i);
            }
        }
    }
    else
    {
        for (size_t i = start; i < sample.size() - 1; ++i)
        {
            if (sample[i] < threshold && threshold < sample[i + 1])
            {
                return (threshold - sample[i]) / static_cast<double>(sample[i + 1] - sample[i]) + static_cast<double>(i);
            }
        }
    }

    return -2;
}





template <class T>
float CFD(const std::vector<T> &sample, float fraction, size_t delay, std::vector<T> &new_sample )
{
    if(sample.size() < 5)
        return 0;


    // das zusammengesetze Signal erstellen
    std::vector<T> newsignal;
    double ymax = -1e100;
    size_t ymax_pos = 0;
    for (size_t i = 0; i < sample.size(); ++i)
    {
        if(i >= delay)
        {
            const double v = -sample[i]*fraction + sample[i-delay];
            newsignal.push_back(v);   // invertieren und skalieren + verz. signal
            if(v > ymax)
            {
                ymax = v;
                ymax_pos = i;
            }
        }
        else
        {
            newsignal.push_back(-sample[i]*fraction);
        }
    }




    // Null-Durchgang suchen
    double t0 = 0.0;
    bool zc_found = false;
    const size_t min_i = std::max(0,(int)ymax_pos-(int)delay*20);
    const size_t max_i = std::min(ymax_pos+delay*10, newsignal.size()-3);
    for (size_t i = min_i; i < max_i; ++i)
    {
        if(0 > newsignal[i-1] && 0 > newsignal[i-2]
                && newsignal[i] > 0 && newsignal[i+1] > 0 && newsignal[i+2] > 0)
        {
            const double m = static_cast<double>(newsignal[i]-newsignal[i-1]);
            const double xs = (0.0 - newsignal[i-1])/m;

            t0 = xs + (i-1);
            zc_found = true;
            break;
        }
    }

    if(zc_found==false)
    {
        std::cout << "CFD - Nulldurchgang nicht gefunden! Suchregion:"<<min_i <<"-"<<max_i << "max_pos:"<<ymax_pos << std::endl;
    }

    new_sample = newsignal;
    return t0;
}


}

