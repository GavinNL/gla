#ifndef GNL_FRUSTRUM_HPP
#define GNL_FRUSTRUM_HPP

#include "Plane3d.hpp"

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
