#ifndef FRUSTRUM_H
#define FRUSTRUM_H

namespace gla
{

class Frustum
{
public:
    Frustum();
    ~Frustum();

    void SetFrustum(float angle, float ratio, float nearD, float farD);
    void SetCamera(const Vector3d &pos, const Vector3d &target, const Vector3d &up);

    int PointInFrustum(const Vector3d &point);
    int SphereInFrustum(const Vector3d &point, float radius);
    int CubeInFrustum(const Vector3d &center, float x, float y, float z);

public:
    enum {
        FRUSTUM_TOP = 0,
        FRUSTUM_BOTTOM,
        FRUSTUM_LEFT,
        FRUSTUM_RIGHT,
        FRUSTUM_NEAR,
        FRUSTUM_FAR,
    };

    static enum {
        FRUSTUM_OUTSIDE = 0,
        FRUSTUM_INTERSECT,
        FRUSTUM_INSIDE,
    };

    Plane3D planes[6];

    Vector3d nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
    Vector3d farTopLeft, farTopRight, farBottomLeft, farBottomRight;

    float nearDistance, farDistance;
    float nearWidth, nearHeight;
    float farWidth, farHeight;
    float ratio, angle, tang;
};

}

#endif // FRUSTRUM_H

