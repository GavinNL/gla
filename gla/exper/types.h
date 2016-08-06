#ifndef EXP_TYPES
#define EXP_TYPES

#include<gla/glad.h>

enum class DataType
{
    UNKNOWN        = -1,
    BYTE           = GL_BYTE ,
    UNSIGNED_BYTE  = GL_UNSIGNED_BYTE ,
    SHORT          = GL_SHORT ,
    UNSIGNED_SHORT = GL_UNSIGNED_SHORT ,
    INT            = GL_INT ,
    UNSIGNED_INT   = GL_UNSIGNED_INT ,
    FLOAT          = GL_FLOAT ,
    DOUBLE         = GL_DOUBLE
};

#endif
