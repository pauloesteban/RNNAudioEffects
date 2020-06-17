#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED

#include "Layer.h"
#include <vector>

template<typename T>
class Model
{
public:
    Model (size_t in_size) :
        in_size (in_size)
    {}

    ~Model()
    {
        for (auto l : layers)
            delete l;
        layers.clear();

        for (auto o : outs)
            delete[] o;
        outs.clear();
    }

    size_t getNextInSize()
    {
        if (layers.empty())
            return in_size;
        
        return layers.back()->out_size;
    }

    void addLayer (Layer<T>* layer)
    {
        layers.push_back (layer);
        outs.push_back (new T[layer->out_size]);
    }

    void reset()
    {
        for (auto* l : layers)
        {
            if (auto* lCast = dynamic_cast<GRULayer<T>*> (l))
                lCast->reset();
        }
    }

    inline T forward (const T* input)
    {
        layers[0]->forward (input, outs[0]);

        for (size_t i = 1; i < layers.size(); ++i)
        {
            layers[i]->forward (outs[i-1], outs[i]);
        }

        return outs.back()[0];
    }

private:
    const size_t in_size;
    std::vector<Layer<T>*> layers;
    std::vector<T*> outs;
};

#endif // MODEL_H_INCLUDED
