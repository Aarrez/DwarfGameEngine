#ifndef PUBLICENUMS_H
#define PUBLICENUMS_H

namespace Engine {
    enum class OBJLoadingState {
        OpenFile = 0,
        Vertex = 1,
        VertexNormal = 2,
        VertexUV = 3,
        Faces = 4,
        PrepData = 5,
    };

    enum class OBJFaceType {
        OnlyFaces = 0,
        AllIndices = 1,
    };
}

#endif //PUBLICENUMS_H
