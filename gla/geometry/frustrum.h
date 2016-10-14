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

#ifndef GNL_FRUSTRUM_HPP
#define GNL_FRUSTRUM_HPP

#include "plane3d.h"

/*
	Frustum class that represents the frustum in 3 dimensional space.
	it can be used to check clipping with bounding boxes.

*/
struct Frustum
{

	// Constructs the frustum using the view matrix and the projection matrix
	Frustum(const glm::mat4 & ViewMatrix, const glm::mat4 & ProjMatrix)
	{
		
	}
	
	// Transform the fustrum using a matrix
	// The matrix M needs to be unitary
	void Tranform(glm::mat4 & ViewMatrix);
	
	
	// Sets the position of the fustrum
	void SetPosition(const glm::vec3 & pos);
	
	
	glm::vec3 p; // position of the camera in 3d space. The position of the point of the pyrimad
		
	// the 6 planes that define the fustrum
	// these planes are in real coordinate space. not normalized.
	Plane3d top;    // this plane passes through the point, p,
	Plane3d right;  // this plane passes through the point, p,
	Plane3d bottom; // this plane passes through the point, p,
	Plane3d left;   // this plane passes through the point, p,
	Plane3d near;
	Plane3d far;
	
}

#endif
