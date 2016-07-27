
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


}

