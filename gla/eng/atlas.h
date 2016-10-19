/*
 * MIT License
 *
 * Copyright (c) [year] [fullname]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef GLA_ATLAS_H
#define GLA_ATLAS_H

#include <gla/core/sampler2d.h>

namespace gla
{
namespace eng
{

/**
 * @brief The Atlas2D class
 *
 * A 2D virtual texture atlas  used for storing many textures in a single sampler.
 */
class Atlas2D
{
    public:

    /**
     * @brief The Ref class
     * A reference to a location on the atlas where this virtual texture is stored
     */
    class Ref
    {
        Sampler2D m_Sampler;
        glm::vec2 m_uv_xy;
        glm::vec2 m_uv_wh;
    };


    Sampler2D m_Sampler;
};


}
}
#endif
