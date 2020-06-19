#ifndef DENSEEIGEN_H_INCLUDED
#define DENSEEIGEN_H_INCLUDED

#include <Eigen/Eigen>
#include "Layer.h"

template<typename T>
class Dense : public Layer<T>
{
public:
    Dense (size_t in_size, size_t out_size) :
        Layer<T> (in_size, out_size)
    {
        weights.resize (out_size, in_size);
        bias.resize (out_size, 1);

        inVec.resize (in_size, 1);
        outVec.resize (out_size, 1);
    }

    virtual ~Dense() {}

    inline void forward (const T* input, T* out) override
    {
        inVec = Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>> (input, in_size, 1);
        outVec = weights * inVec + bias;

        std::copy (outVec.data(), outVec.data() + out_size, out);
    }

    void setWeights(T** newWeights)
    {
        for (int i = 0; i < out_size; ++i)
            for (int k = 0; k < in_size; ++k)
                weights (i, k) = newWeights[i][k];
    }

    void setBias(T* b)
    {
        for(int i = 0; i < out_size; ++i)
            bias (i, 0) = b[i];
    }

private:
    Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> weights;
    Eigen::Matrix<T, Eigen::Dynamic, 1> bias;

    Eigen::Matrix<T, Eigen::Dynamic, 1> inVec;
    Eigen::Matrix<T, Eigen::Dynamic, 1> outVec;
};

#endif // DENSEEIGEN_H_INCLUDED
