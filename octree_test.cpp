#include <gla/containers/octree.h>
#include <gla/core/camera.h>
#include <memory>

struct MyObject;

using ObjectType = MyObject*;
using MyOctree   = gla::Octree< ObjectType , float, 8, 5>;

struct MyObject
{
    glm::vec3                  position;
    MyOctree                 * node;

    MyObject( glm::vec3 p ) : position(p)
    {

    }

    ~MyObject()
    {

    }
};



namespace gla
{
    template<>
    MyOctree *  MyOctree::GetNode( ObjectType  &obj )
    {
        return obj->node;
    }

    template<>
    void        MyOctree::SetNode( ObjectType & obj, MyOctree *m)
    {
        obj->node = m;
    }

    template<>
    gla::BoundingBox<float>  MyOctree::GetBoundingBox( ObjectType & obj )
    {
        return gla::BoundingBox<float>( obj->position-glm::vec3(4), obj->position+glm::vec3(4) );
    }

}



int main()
{

    MyOctree Tree;

    for(int i=0 ; i < 100 ; i++)
    {
        auto o = new MyObject( glm::vec3( rand()%100-50, rand()%100-50, rand()%100-50) );
        Tree.Insert( o );
    }

    gla::Camera C;

    C.Perspective(160, 4/3,1.0,50);
    C.SetPosition( glm::vec3(0,0,-10));


    gla::Frustum F( C.GetProjectionMatrix() );
    F.Transform( C.GetMatrix() );

    std::function<void(MyObject*&)>  lamb = []( MyObject* &obj)
        {
            static int i=0;
            std::cout << ++i << "  Intersected with: " << obj->position.x << ", " << obj->position.y << ", " << obj->position.z << std::endl;
        }    ;

    Tree.ForAllInFrustum( F, lamb);
    //Tree.print();


    return 0;
}
