#ifndef LINE_H
#define LINE_H


#include <glre/global.h>
#include <vector>


namespace glre
{

class Line
{
    public:
        Line();
        void sendToGPU();
        void clearGPU();

        std::vector<uV2> Indices;
        std::vector<V3>  Pos;
        std::vector<V4>  Colour;
};

};

#endif // LINE_H
