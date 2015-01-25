#include <glre/mesh.h>
#include <iostream>

namespace glre
{
    Mesh::Mesh() : m_VAO(0)
    {
    }

    /*
    bool Mesh::LoadMesh(const string& Filename)
    {
        // Release the previously loaded mesh (if it exists)
        Clear();

        // Create the VAO
        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        // Create the buffers for the vertices atttributes
        glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

        bool Ret = false;
        Assimp::Importer Importer;

        const aiScene* pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate |
                                    aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

        if (pScene) {
            Ret = InitFromScene(pScene, Filename);
        }
        else {
            printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
        }

        // Make sure the VAO is not changed from outside code
        glBindVertexArray(0);

        return Ret;
    }
    */


    void Mesh::insertVertex( const V3 & v, const V3 & n, const V2 & uv)
    {
        mPosition.insert(v);
        mNormal.insert(n);
        mUV.insert(uv);
        //Pos.push_back(v);
        //Normal.push_back(n);
        //UV.push_back(uv);
    }

    void Mesh::insertFace( const uV3 & triangle)
    {
        if( triangle[0] >= mPosition.size() ||
            triangle[1] >= mPosition.size() ||
            triangle[2] >= mPosition.size())
        {
            std::cout << " Throw Exception!! \n";
        }
        mIndex.insert(triangle);
        //Indices.push_back(triangle);
    }

    void Mesh::clearGPU()
    {
        if(m_VAO)
        {
            glDeleteVertexArrays(1, &m_VAO);
            //glDeleteBuffers(4, m_Buffers);

            mPosition.clearGPU();
            mNormal.clearGPU();
            mUV.clearGPU();
            mColour.clearGPU();
            mIndex.clearGPU();

            m_VAO = 0;
        }
    }

    bool Mesh::sendToGPU()
    {
        // Generate and bind the VAO.  The VAO will combine all 3 buffers into one handle
        // Positions, Normals, UVs

        if( m_VAO ) clearGPU();

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(    m_VAO);

        std::cout << "VOA: " << m_VAO << std::endl;
        // Create the buffers for the vertices atttributes

        //glGenBuffers(NUMBER_OF_BUFFERS, m_Buffers);
        std::cout << "Buffer 0: " << mPosition.mGLID << std::endl;
        std::cout << "Buffer 1: " << mNormal.mGLID << std::endl;
        std::cout << "Buffer 2: " << mUV.mGLID << std::endl;
        std::cout << "Buffer 3: " << mIndex.mGLID << std::endl;
        std::cout << "Buffer 4: " << m_VAO << std::endl;

        // Generate and populate the buffers with vertex attributes and the indices


        //===================================================================================================
               // Activate the Position Buffer

        mPosition.sendToGPU();
               // Enablee the 0th vertex attribute
        glEnableVertexAttribArray(0);
               // tell the GPU that the first attribute is 3 floats
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //===================================================================================================
                // Activate the UV buffer

        mUV.sendToGPU();
                // enable the second attribute
        glEnableVertexAttribArray(1);
                // tell the GPU that second attribute is 2 floats
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
        //===================================================================================================
                // Activate the Normal buffer

        mNormal.sendToGPU();
                // enable the third attribute
        glEnableVertexAttribArray(2);
                // tell the GPU that third attribute is 3 floats
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
        //===================================================================================================




        //===================================================================================================
        mIndex.sendToGPU();
                // activate the index buffer
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
        //        // Send the index data to the GPU
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
        //===================================================================================================

        return true;
    }

    //void Mesh::InitMesh(const aiMesh* paiMesh,
    //                     vector& Positions,
    //                     vector& Normals,
    //                     vector& TexCoords,
    //                     vector& Indices)
    //{
    //    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    //    // Populate the vertex attribute vectors
    //    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
    //        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
    //        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
    //        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ?
    //                                      &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

    //        Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
    //        Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
    //        TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
    //    }

    //    // Populate the index buffer
    //    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
    //        const aiFace& Face = paiMesh->mFaces[i];
    //        assert(Face.mNumIndices == 3);
    //        Indices.push_back(Face.mIndices[0]);
    //        Indices.push_back(Face.mIndices[1]);
    //        Indices.push_back(Face.mIndices[2]);
    //    }
    //}



    void Mesh::Render()
    {
        glBindVertexArray(m_VAO);
       // std::cout << "array bound\n";

    //    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
    //        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

    //        assert(MaterialIndex < m_Textures.size());

    //        if (m_Textures[MaterialIndex]) {
    //            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
    //        }

            glDrawElementsBaseVertex(GL_TRIANGLES,
                                      mIndex.size() * 3,
                                      GL_UNSIGNED_INT,
                                      0,
                                      0);
         //   std::cout << "array drawn\n";
        //}

        // Make sure the VAO is not changed from the outside
        glBindVertexArray(0);
    }

}
