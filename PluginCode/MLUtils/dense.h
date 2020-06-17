#ifndef DENSE_H_INCLUDED
#define DENSE_H_INCLUDED

#include <algorithm>
#include <numeric>
#include "Layer.h"

template<typename T>
class Dense1
{
public:
    Dense1 (size_t in_size) :
        in_size (in_size)
    {
        weights = new T[in_size];
    }

    ~Dense1()
    {
        delete[] weights;
    }

    inline T forward(const T* input)
    {
        return std::inner_product(weights, weights + in_size, input, (T) 0) + bias;
    }

    void setWeights(const T* newWeights)
    {
        for(int i = 0; i < in_size; ++i)
            weights[i] = newWeights[i];
    }

    void setBias(T b) { bias = b; }

private:
    const size_t in_size;
    T* weights;
    T bias;
};

template<typename T>
class Dense : public Layer<T>
{
public:
    Dense (size_t in_size, size_t out_size) :
        Layer<T> (in_size, out_size)
    {
        subLayers = new Dense1<T>*[out_size];
        for (int i = 0; i < out_size; ++i)
            subLayers[i] = new Dense1<T> (in_size);
    }

    ~Dense()
    {
        for (int i = 0; i < out_size; ++i)
            delete subLayers[i];

        delete[] subLayers;
    }

    inline void forward (const T* input, T* out) override
    {
        for (int i = 0; i < out_size; ++i)
        {
            out[i] = subLayers[i]->forward (input);
        }
    }

    void setWeights(T** newWeights)
    {
        for(int i = 0; i < out_size; ++i)
            subLayers[i]->setWeights (newWeights[i]);
    }

    void setBias(T* b)
    {
        for(int i = 0; i < out_size; ++i)
            subLayers[i]->setBias (b[i]);
    }

private:
    Dense1<T>** subLayers;
};

#endif // DENSE_H_INCLUDED
