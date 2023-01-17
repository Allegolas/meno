#include "model.h"

int
LoadModel(const char* filePath, Model* model) {
    const struct aiScene* Scene = aiImportFile(filePath, POSTPROCESS_FLAGS);
    if(!Scene) {
        fprintf(stderr, "Failed to load Assimp scene: %s\n", aiGetErrorString());
        return LOAD_FAIL;
    }

    // NOTE(Jovan): Allocate all mesh buffers
    model->NumMeshes = Scene->mNumMeshes;
    model->MeshBuffers = (MeshBuffer*)calloc(model->NumMeshes, sizeof(MeshBuffer));
    if(!model->MeshBuffers) {
        fprintf(stderr, "Failed to allocate mesh buffer memory.\n");
        return LOAD_FAIL;
    }

    unsigned ElementsPerVertex = 6;
    // NOTE(Jovan): Attempt to load vertex information into buffers
    for(unsigned MeshIdx = 0; MeshIdx < model->NumMeshes; ++MeshIdx) {
        MeshBuffer* CurrMeshBuffer = &model->MeshBuffers[MeshIdx];
        const struct aiMesh* CurrMesh = Scene->mMeshes[MeshIdx];
        unsigned NumVertices = CurrMesh->mNumVertices;
        float* Vertices = (float*)calloc(ElementsPerVertex * NumVertices, sizeof(float));

        if(!Vertices) {
            fprintf(stderr, "Failed to allocate vertices.\n");
            return LOAD_FAIL;
        }

        const struct aiMaterial* Material = Scene->mMaterials[Scene->mMeshes[MeshIdx]->mMaterialIndex];
        const struct aiColor4D Color = { 0.6f, 0.6f, 0.6f, 1.0f };
        aiGetMaterialColor(Material, AI_MATKEY_COLOR_DIFFUSE, &Color);
        for(unsigned VertIdx = 0; VertIdx < NumVertices; ++VertIdx) {
            Vertices[ElementsPerVertex * VertIdx] = CurrMesh->mVertices[VertIdx].x;
            Vertices[ElementsPerVertex * VertIdx + 1] = CurrMesh->mVertices[VertIdx].y;
            Vertices[ElementsPerVertex * VertIdx + 2] = CurrMesh->mVertices[VertIdx].z;
            Vertices[ElementsPerVertex * VertIdx + 3] = Color.r;
            Vertices[ElementsPerVertex * VertIdx + 4] = Color.g;
            Vertices[ElementsPerVertex * VertIdx + 5] = Color.b;
        }

        unsigned NumFaces = CurrMesh->mNumFaces;
        if(!NumFaces) continue;

        unsigned* Indices = (unsigned*)calloc(3 * NumFaces, sizeof(unsigned));
        if(!Indices) {
            fprintf(stderr, "Failed to allocate indices.\n");
            return LOAD_FAIL;
        }

        for(unsigned FaceIdx = 0; FaceIdx < NumFaces; ++FaceIdx) {
            const struct aiFace* CurrFace = &CurrMesh->mFaces[FaceIdx];
            Indices[3 * FaceIdx] = CurrFace->mIndices[0];
            Indices[3 * FaceIdx + 1] = CurrFace->mIndices[1];
            Indices[3 * FaceIdx + 2] = CurrFace->mIndices[2];
        }

        CurrMeshBuffer->VerticesCount = NumVertices;
        CurrMeshBuffer->IndicesCount = 3 * NumFaces;

        glGenVertexArrays(1, &CurrMeshBuffer->VAO);
        glBindVertexArray(CurrMeshBuffer->VAO);
        glGenBuffers(1, &CurrMeshBuffer->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, CurrMeshBuffer->VBO);
        glBufferData(GL_ARRAY_BUFFER, NumVertices * ElementsPerVertex * sizeof(float), Vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(LAYOUT_POSITION, 3, GL_FLOAT, GL_FALSE, ElementsPerVertex * sizeof(float), (void*)0);
        glEnableVertexAttribArray(LAYOUT_POSITION);
        glVertexAttribPointer(LAYOUT_COLOR, 3, GL_FLOAT, GL_FALSE, ElementsPerVertex * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(LAYOUT_COLOR);
        if(CurrMeshBuffer->IndicesCount) {
            glGenBuffers(1, &CurrMeshBuffer->EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CurrMeshBuffer->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, CurrMeshBuffer->IndicesCount * sizeof(unsigned), Indices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        if(NumVertices) {
            free(Vertices);
        }

        if(NumFaces){
            free(Indices);
        }
    }
    fprintf(stdout, "Loaded %d meshes.\n", model->NumMeshes);

    aiReleaseImport(Scene);
    return LOAD_SUCCESS;
}

void
FreeModelResources(Model* model) {
    fprintf(stdout, "Freeing model\n");
    for(unsigned MeshIdx = 0; MeshIdx < model->NumMeshes; ++MeshIdx) {
        MeshBuffer* CurrMesh = &model->MeshBuffers[MeshIdx];
        fprintf(stdout, "Freeing VBO\n");
        if(CurrMesh->VerticesCount) {
            glDeleteBuffers(1, &CurrMesh->VBO);
        }
        fprintf(stdout, "Freeing EBO\n");
        if(CurrMesh->IndicesCount) {
            glDeleteBuffers(1, &CurrMesh->EBO);
        }
        fprintf(stdout, "Freeing VAO\n");
        glDeleteVertexArrays(1, &CurrMesh->VAO);
    }
    fprintf(stdout, "Freeing meshes\n");
    free(model->MeshBuffers);
}

void
RenderModel(const Model* model) {
    for(unsigned MeshIdx = 0; MeshIdx < model->NumMeshes; ++MeshIdx) {
        const MeshBuffer* CurrBuffer = &model->MeshBuffers[MeshIdx];
        glBindVertexArray(CurrBuffer->VAO);
        if(CurrBuffer->IndicesCount) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CurrBuffer->EBO);
            glDrawElements(GL_TRIANGLES, CurrBuffer->IndicesCount, GL_UNSIGNED_INT, (void*)0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            continue;
        }
        glDrawArrays(GL_TRIANGLES, 0, CurrBuffer->VerticesCount);
        glBindVertexArray(0);
    }
}