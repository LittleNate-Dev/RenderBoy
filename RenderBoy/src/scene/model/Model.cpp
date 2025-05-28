#include "Model.h"

Model::Model()
{
    m_FilePath = "";
    m_Name = "";
    m_Instance = 1;
    m_Current = 1;
    m_SetInstance = false;
    m_Position = glm::vec3(0.0f);
    m_Scale = glm::vec3(1.0f);
    m_EulerAngle = glm::vec3(0.0f);
    m_InstancePosition.push_back(glm::vec3(0.0f));
    m_InstanceScale.push_back(glm::vec3(1.0f));
    m_InstanceEulerAngle.push_back(glm::vec3(0.0f));
    m_ModelMats.push_back(glm::mat4(1.0f));
    m_UpdateShadow = true;
}

Model::~Model()
{
}

void Model::UpdateStatics()
{
    // Mesh statics
    m_Statics.MeshCount = (unsigned int)m_Meshes.size();
    m_Statics.VertexCount = 0;
    m_Statics.TriangleCount = 0;
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        // Vertex statics
        m_Statics.VertexCount += (unsigned int)m_Meshes[i].GetVertices().size();
        // Triangle statics
        m_Statics.TriangleCount += (unsigned int)m_Meshes[i].GetIndices().size() / 3;
    }
    // Analysing model's data and decide it's render mode
    bool hasTexture = false;
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        hasTexture |= m_Meshes[i].HasTexture();
    }
    if (hasTexture)
    {
        m_Statics.RenderMode = HASTEX_ALBEDO;
        bool hasSpecularTex = false;
        bool hasMetallicTex = false;
        bool hasRoughnessTex = false;
        for (unsigned int i = 0; i < m_Meshes.size(); i++)
        {
            hasSpecularTex |= m_Meshes[i].HasSpecularTex();
            hasMetallicTex |= m_Meshes[i].HasMetallicTex();
            hasRoughnessTex |= m_Meshes[i].HasRoughnessTex();
        }
        if (hasSpecularTex)
        {
            m_Statics.RenderMode = HASTEX_BLINN;
            if (HasNHTex())
            {
                m_Statics.RenderMode = HASTEX_BLINN_NH;
            }
        }
        else if (hasMetallicTex)
        {
            for (unsigned int i = 0; i < m_Meshes.size(); i++)
            {
                if (m_Meshes[i].HasMetallicTex())
                {
                    if (m_Meshes[i].GetMetallicTexFilePath() == m_Meshes[i].GetRoughnessTexFilePath())
                    {
                        m_Statics.RenderMode = HASTEX_PBR_3;
                        if (HasNHTex())
                        {
                            m_Statics.RenderMode = HASTEX_PBR_3_NH;
                        }
                    }
                    else
                    {
                        m_Statics.RenderMode = HASTEX_PBR_4; 
                        if (HasNHTex())
                        {
                            m_Statics.RenderMode = HASTEX_PBR_4_NH;
                        }
                    }
                    break;
                }
            }
        }
    }
    else
    {
        m_Statics.RenderMode = NOTEX;
    }
}

bool Model::LoadModel(std::string name, std::string filepath)
{
    m_Name = name;
    m_UpdateShadow = true;
    std::vector<Mesh>().swap(m_Meshes);
    if (core::CheckFileFormatAssimp(core::GetFileFormat(filepath)))
    {
        if (LoadModelAssimp(filepath))
        {
            return true;
        }
        return false;
    }
    return false;
}

bool Model::LoadModelAssimp(std::string filepath)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        core::ShowWarningMsg("Failed to load file! Check log for details.");
        spdlog::error("ERROR::ASSIMP " + std::string(import.GetErrorString()));
        return false;
    }
    m_FilePath = filepath;
    AssimpProcessNode(scene->mRootNode, scene);
    UpdateStatics();
    spdlog::info("Model " + m_Name + " loaded.");
    return true;
}

void Model::AssimpProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(AssimpProcessMesh(mesh, node, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        AssimpProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::AssimpProcessMesh(aiMesh* mesh, aiNode* node, const aiScene* scene)
{
    Mesh newMesh;
    // Process mesh's material;
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // Get material colors
    aiColor3D color;
    material->Get(AI_MATKEY_COLOR_AMBIENT, color);
    newMesh.SetAmbientValue(glm::vec3(color.r, color.g, color.b));
    material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    newMesh.SetDiffuseValue(glm::vec3(color.r, color.g, color.b));
    material->Get(AI_MATKEY_COLOR_SPECULAR, color);
    newMesh.SetSpecularValue(glm::vec3(color.r, color.g, color.b));
    material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
    newMesh.SetEmissiveValue(glm::vec3(color.r, color.g, color.b));
    material->Get(AI_MATKEY_COLOR_REFLECTIVE, color);
    newMesh.SetReflectiveValue(color.r);
    material->Get(AI_MATKEY_COLOR_TRANSPARENT, color);
    newMesh.SetTransparentValue(color.r);
    // TODO: Load Tf, Ns, Ni, Tr, d, illum from .mtl file
    // Load texture path
    if (AssimpLoadTexturePath(material, aiTextureType_DIFFUSE).size())
    {
        std::string filepath = AssimpLoadTexturePath(material, aiTextureType_DIFFUSE)[0];
        newMesh.GetAlbedoTexFilePath() = core::ReplaceBackwardSlash(filepath);
    }
    if (AssimpLoadTexturePath(material, aiTextureType_SPECULAR).size())
    {
        std::string filepath = AssimpLoadTexturePath(material, aiTextureType_SPECULAR)[0];
        newMesh.GetSpecularTexFilePath() = core::ReplaceBackwardSlash(filepath);
    }
    if (AssimpLoadTexturePath(material, aiTextureType_METALNESS).size())
    {
        std::string filepath = AssimpLoadTexturePath(material, aiTextureType_METALNESS)[0];
        newMesh.GetMetallicTexFilePath() = core::ReplaceBackwardSlash(filepath);
    }
    if (AssimpLoadTexturePath(material, aiTextureType_DIFFUSE_ROUGHNESS).size())
    {
        std::string filepath = AssimpLoadTexturePath(material, aiTextureType_DIFFUSE_ROUGHNESS)[0];
        newMesh.GetRoughnessTexFilePath() = core::ReplaceBackwardSlash(filepath);
    }
    if (AssimpLoadTexturePath(material, aiTextureType_AMBIENT_OCCLUSION).size())
    {
        std::string filepath = AssimpLoadTexturePath(material, aiTextureType_AMBIENT_OCCLUSION)[0];
        newMesh.GetAoTexFilePath() = core::ReplaceBackwardSlash(filepath);
    }
    if (core::GetFileFormat(m_FilePath) == "obj")
    {
        if (AssimpLoadTexturePath(material, aiTextureType_HEIGHT).size())
        {
            std::string filepath = AssimpLoadTexturePath(material, aiTextureType_HEIGHT)[0];
            newMesh.GetNormalTexFilePath() = core::ReplaceBackwardSlash(filepath);
        }
    }
    else
    {
        if (AssimpLoadTexturePath(material, aiTextureType_NORMALS).size())
        {
            std::string filepath = AssimpLoadTexturePath(material, aiTextureType_NORMALS)[0];
            newMesh.GetNormalTexFilePath() = core::ReplaceBackwardSlash(filepath);
        }
        if (AssimpLoadTexturePath(material, aiTextureType_HEIGHT).size())
        {
            std::string filepath = AssimpLoadTexturePath(material, aiTextureType_HEIGHT)[0];
            newMesh.GetHeightTexFilePath() = core::ReplaceBackwardSlash(filepath);
        }
    }
    //Process the indicies of each mesh
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            newMesh.GetIndices().push_back(face.mIndices[j]);
        }
    }
    // Process each vertex
    // For some models, each mesh has to be transform to the correct position first
    glm::vec4 transLine1 = glm::vec4(node->mTransformation[0][0], node->mTransformation[1][0], node->mTransformation[2][0], node->mTransformation[3][0]);
    glm::vec4 transLine2 = glm::vec4(node->mTransformation[0][1], node->mTransformation[1][1], node->mTransformation[2][1], node->mTransformation[3][1]);
    glm::vec4 transLine3 = glm::vec4(node->mTransformation[0][2], node->mTransformation[1][2], node->mTransformation[2][2], node->mTransformation[3][2]);
    glm::vec4 transLine4 = glm::vec4(node->mTransformation[0][3], node->mTransformation[1][3], node->mTransformation[2][3], node->mTransformation[3][3]);
    glm::mat4 transMat = glm::mat4(transLine1, transLine2, transLine3, transLine4);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 vector;
        Vertex vertex;
        //Get the position of the vertex
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = glm::vec3(transMat * glm::vec4(vector, 1.0f));
        //Get the normal of the vertex
        if (mesh->mNormals != nullptr)
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            //vertex.Normal = glm::vec3(transMat * glm::vec4(vector, 1.0f));
            vertex.Normal = vector;
        }
        else
        {
            vertex.Normal = glm::vec3(0.0f);
        }
        //Get the texture coord of the vertex
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoord = vec;
        }
        else
        {
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);
        }
        // Vertex tangent and bitangent
        if (mesh->mTangents != nullptr)
        {
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = glm::vec3(transMat * glm::vec4(vector, 1.0f));
            vertex.Tangent = vector;
        }
        else
        {
            vertex.Tangent = glm::vec3(0.0f);
        }
        if (mesh->mBitangents != nullptr)
        {
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = glm::vec3(transMat * glm::vec4(vector, 1.0f));
            vertex.Bitangent = vector;
        }
        else
        {
            vertex.Bitangent = glm::vec3(0.0f);
        }
        newMesh.GetVertices().push_back(vertex);
    }

    return newMesh;
}

std::vector<std::string> Model::AssimpLoadTexturePath(aiMaterial* mat, aiTextureType type)
{
    std::vector<std::string> texPath;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string filepath = core::GetFileDirectory(m_FilePath) + std::string(str.C_Str());
        texPath.push_back(filepath);
    }
    return texPath;
}

void Model::SetName(std::string name)
{
    m_Name = name;
}

void Model::SetInstance(unsigned int instance)
{
    instance = instance == 0 ? 1 : instance;
    // Resize vectors
    if (instance < m_Instance)
    {
        m_Current = m_Current > instance ? instance : m_Current;
        std::vector<glm::vec3> newPos, newScale, newEuler;
        std::vector<glm::mat4> newMat;
        for (unsigned int i = 0; i < instance; i++) 
        {
            newPos.push_back(m_InstancePosition[i]);
            newScale.push_back(m_InstanceScale[i]);
            newEuler.push_back(m_InstanceEulerAngle[i]);
            newMat.push_back(m_ModelMats[i]);
        }
        m_InstancePosition = newPos;
        m_InstanceScale = newScale;
        m_InstanceEulerAngle = newEuler;
        m_ModelMats = newMat;
    }
    else if (instance > m_Instance)
    {
        for (unsigned int i = 0; i < instance - m_Instance; i++)
        {
            m_InstancePosition.push_back(glm::vec3(0.0f));
            m_InstanceScale.push_back(glm::vec3(1.0f));
            m_InstanceEulerAngle.push_back(glm::vec3(0.0f));
            m_ModelMats.push_back(glm::mat4(1.0f));
        }
    }
    m_Instance = instance;
    m_UpdateShadow = true;
}

void Model::UpdateModelMat(unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (!current)
    {
        for (unsigned int i = 0; i < m_Instance; i++)
        {
            glm::mat4 modelMat = GetTranslateMat(i + 1) * GetRotateMat(i + 1) * GetScaleMat(i + 1);
            m_ModelMats[i] = modelMat;
        }
    }
    else
    {
        glm::mat4 modelMat = GetTranslateMat(current) * GetRotateMat(current) * GetScaleMat(current);
        m_ModelMats[current - 1] = modelMat;
    }
    m_UpdateShadow = true;
}

bool Model::HasNHTex()
{
    bool hasNormal = false;
    bool hasHeight = false;
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        hasNormal |= m_Meshes[i].HasNormalTex();
        hasHeight |= m_Meshes[i].HasHeightTex();
    }
    if (hasNormal || hasHeight)
    {
        return true;
    }
    return false;
}

void Model::SetCurrent(unsigned int current)
{
    if (current > m_Instance)
    {
        current = m_Instance;
    }
    else if (current == 0)
    {
        current = 1;
    }
    m_Current = current;
}

void Model::SetPosition(float posX, float posY, float posZ, unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (!current)
    {
        m_Position = glm::vec3(posX, posY, posZ);
    }
    else
    {
        m_InstancePosition[current - 1] = glm::vec3(posX, posY, posZ);
    }
    UpdateModelMat(current);
}

void Model::SetPosition(glm::vec3 position, unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (!current)
    {
        m_Position = position;
    }
    else
    {
        m_InstancePosition[current - 1] = position;
    }
    UpdateModelMat(current);
}

void Model::SetScale(float scaleX, float scaleY, float scaleZ, unsigned int current)
{
    SetScale(glm::vec3(scaleX, scaleY, scaleZ), current);
}

void Model::SetScale(glm::vec3 scale, unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    scale.x = scale.x < 0.0f ? 0.0f : scale.x;
    scale.y = scale.y < 0.0f ? 0.0f : scale.y;
    scale.z = scale.z < 0.0f ? 0.0f : scale.z;
    if (!current)
    {
        m_Scale = scale;
    }
    else
    {
        m_InstanceScale[current - 1] = scale;
    }
    UpdateModelMat(current);
}

void Model::SetPitch(float pitch, unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (abs(pitch) > 360.0f)
    {
        if (pitch > 0.0f)
        {
            pitch = pitch - (float)((int)pitch / 360) * 360.0f;
        }
        else
        {
            pitch = pitch - (float)((int)abs(pitch) / 360) * -360.0f;
        }
    }
    if (!current)
    {
        m_EulerAngle.x = pitch;
    }
    else
    {
        m_InstanceEulerAngle[current - 1].x = pitch;
    }
    UpdateModelMat(current);
}

void Model::SetYaw(float yaw, unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (abs(yaw) > 360.0f)
    {
        if (yaw > 0.0f)
        {
            yaw = yaw - (float)((int)yaw / 360) * 360.0f;
        }
        else
        {
            yaw = yaw - (float)((int)abs(yaw) / 360) * -360.0f;
        }
    }
    if (!current)
    {
        m_EulerAngle.y = yaw;
    }
    else
    {
        m_InstanceEulerAngle[current - 1].y = yaw;
    }
    UpdateModelMat(current);
}

void Model::SetRoll(float roll, unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (abs(roll) > 360.0f)
    {
        if (roll > 0.0f)
        {
            roll = roll - (float)((int)roll / 360) * 360.0f;
        }
        else
        {
            roll = roll - (float)((int)abs(roll) / 360) * -360.0f;
        }
    }
    if (!current)
    {
        m_EulerAngle.z = roll;
    }
    else
    {
        m_InstanceEulerAngle[current - 1].z = roll;
    }
    UpdateModelMat(current);
}

void Model::SetEulerAngle(float pitch, float yaw, float roll, unsigned int current)
{
    SetEulerAngle(glm::vec3(pitch, yaw, roll), current);
}

void Model::SetEulerAngle(glm::vec3 eulerAngle, unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (abs(eulerAngle.x) > 360.0f)
    {
        if (eulerAngle.x > 0.0f)
        {
            eulerAngle.x = eulerAngle.x - (float)((int)eulerAngle.x / 360) * 360.0f;
        }
        else
        {
            eulerAngle.x = eulerAngle.x - (float)((int)abs(eulerAngle.x) / 360) * -360.0f;
        }
    }
    if (abs(eulerAngle.y) > 360.0f)
    {
        if (eulerAngle.y > 0.0f)
        {
            eulerAngle.y = eulerAngle.y - (float)((int)eulerAngle.y / 360) * 360.0f;
        }
        else
        {
            eulerAngle.y = eulerAngle.y - (float)((int)abs(eulerAngle.y) / 360) * -360.0f;
        }
    }
    if (abs(eulerAngle.z) > 360.0f)
    {
        if (eulerAngle.z > 0.0f)
        {
            eulerAngle.z = eulerAngle.z - (float)((int)eulerAngle.z / 360) * 360.0f;
        }
        else
        {
            eulerAngle.z = eulerAngle.z - (float)((int)abs(eulerAngle.z) / 360) * -360.0f;
        }
    }
    if (!current)
    {
        m_EulerAngle = eulerAngle;
    }
    else
    {
        m_InstanceEulerAngle[current - 1] = eulerAngle;
    }
    UpdateModelMat(current);
}

glm::vec3 Model::GetPosition(unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (!current)
    {
        return m_Position;
    }
    else
    {
        return m_InstancePosition[current - 1];
    }
}

glm::vec3 Model::GetScale(unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (!current)
    {
        return m_Scale;
    }
    else
    {
        return m_InstanceScale[current - 1];
    }
}

glm::vec3 Model::GetEulerAngle(unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (!current)
    {
        return m_EulerAngle;
    }
    else
    {
        return m_InstanceEulerAngle[current - 1];
    }
}

glm::mat4 Model::GetModelMat(unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    if (!current)
    {
        glm::mat4 modelMat = GetTranslateMat() * GetRotateMat() * GetScaleMat();
        return modelMat;
    }
    else
    {
        return m_ModelMats[current - 1];
    }
}

glm::mat4 Model::GetTranslateMat(unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    glm::mat4 translateMat;
    if (!current)
    {
        translateMat = glm::translate(glm::mat4(1.0f), m_Position);
    }
    else
    {
        translateMat = glm::translate(glm::mat4(1.0f), m_Position + m_InstancePosition[current - 1]);
    }
    return translateMat;
}

glm::mat4 Model::GetScaleMat(unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    glm::mat4 scaleMat;
    if (!current)
    {
        scaleMat = glm::scale(glm::mat4(1.0f), m_Scale);
    }
    else
    {
        scaleMat = glm::scale(glm::mat4(1.0f), m_Scale * m_InstanceScale[current - 1]);
    }
    return scaleMat;
}

glm::mat4 Model::GetRotateMat(unsigned int current)
{
    current = current > m_Instance ? m_Instance : current;
    glm::mat4 rotateMat;
    if (!current)
    {
        rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y), glm::vec3(0, 1, 0)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x), glm::vec3(1, 0, 0));
        glm::vec4 axis = rotateMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        rotateMat = core::GetRodrigue(glm::normalize(axis), m_EulerAngle.z) * rotateMat;
    }
    else
    {
        rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.y + m_InstanceEulerAngle[current - 1].y), glm::vec3(0, 1, 0)) * 
                    glm::rotate(glm::mat4(1.0f), glm::radians(m_EulerAngle.x + m_InstanceEulerAngle[current - 1].x), glm::vec3(1, 0, 0));
        glm::vec4 axis = rotateMat * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        rotateMat = core::GetRodrigue(glm::normalize(axis), m_EulerAngle.z + m_InstanceEulerAngle[current - 1].z) * rotateMat;
    }
    return rotateMat;
}

void Model::Free()
{
    for (unsigned int i = 0; i < m_Meshes.size(); i++)
    {
        std::vector<Vertex>().swap(m_Meshes[i].GetVertices());
        std::vector<unsigned int>().swap(m_Meshes[i].GetIndices());
    }
}

void Model::DrawUI()
{
    /*ImGui::CenterAlignWidget("Wireframe");
    ImGui::LabelHighlighted("Wireframe");
    ImGui::Checkbox("##Wireframe", &m_Wireframe);*/
    ImGui::CenterAlignWidget("Set All Instance");
    ImGui::LabelHighlighted("Set All Instance");
    if (ImGui::Checkbox("##SetAllInstance", &m_SetInstance))
    {
        m_Current = !m_SetInstance;
    }
    // Set all instances
    if (m_SetInstance)
    {
        // Position
        if (ImGui::TreeNode("Position"))
        {
            ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
            ImGui::CenterAlignWidget("X", 80.0f * core::GetWidgetWidthCoefficient());
            ImGui::LabelHighlighted("X");
            if (ImGui::InputFloat("##X", &m_Position.x))
            {
                UpdateModelMat();
            }
            ImGui::CenterAlignWidget("Y", 80.0f * core::GetWidgetWidthCoefficient());
            ImGui::LabelHighlighted("Y");
            if (ImGui::InputFloat("##Y", &m_Position.y))
            {
                UpdateModelMat();
            }
            ImGui::CenterAlignWidget("Z", 80.0f * core::GetWidgetWidthCoefficient());
            ImGui::LabelHighlighted("Z");
            if (ImGui::InputFloat("##Z", &m_Position.z))
            {
                UpdateModelMat();
            }
            ImGui::PopItemWidth();
            ImGui::TreePop();
        }
        // Rotation
        static bool slideRotate = true;
        if (ImGui::TreeNode("Rotation"))
        {
            ImGui::Checkbox("Slider", &slideRotate);
            if (slideRotate)
            {
                ImGui::PushItemWidth(280.0f * core::GetWidgetWidthCoefficient());
                ImGui::CenterAlignWidget("Pitch", 280.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Pitch");
                if (ImGui::SliderFloat("##Pitch", &m_EulerAngle.x, -360.0f, 360.0f))
                {
                    UpdateModelMat();
                }
                ImGui::CenterAlignWidget("Yaw", 280.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Yaw");
                if (ImGui::SliderFloat("##Yaw", &m_EulerAngle.y, -360.0f, 360.0f))
                {
                    UpdateModelMat();
                }
                ImGui::CenterAlignWidget("Roll", 280.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Roll");
                if (ImGui::SliderFloat("##Roll", &m_EulerAngle.z, -360.0f, 360.0f))
                {
                    UpdateModelMat();
                }
                ImGui::PopItemWidth();
            }
            else
            {
                ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
                ImGui::CenterAlignWidget("Pitch", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Pitch");
                if (ImGui::InputFloat("##Pitch", &m_EulerAngle.x))
                {
                    SetPitch(m_EulerAngle.x);
                }
                ImGui::CenterAlignWidget("Yaw", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Yaw");
                if (ImGui::InputFloat("##Yaw", &m_EulerAngle.y))
                {
                    SetYaw(m_EulerAngle.y);
                }
                ImGui::CenterAlignWidget("Roll", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Roll");
                if (ImGui::InputFloat("##Roll", &m_EulerAngle.z))
                {
                    SetRoll(m_EulerAngle.z);
                }
                ImGui::PopItemWidth();
            }
            ImGui::TreePop();
        }
        // Scale
        static bool keepScale = false;
        if (ImGui::TreeNode("Scale"))
        {
            ImGui::CenterAlignWidget("Keep Scale");
            ImGui::LabelHighlighted("Keep Scale");
            ImGui::Checkbox("##KeepScale", &keepScale);
            ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
            if (keepScale)
            {
                ImGui::CenterAlignWidget("Scale", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Scale");
                if (ImGui::InputFloat("##Scale", &m_Scale.x))
                {
                    SetScale(glm::vec3(m_Scale.x));
                }
            }
            else
            {
                ImGui::CenterAlignWidget("X", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("X");
                if (ImGui::InputFloat("##X", &m_Scale.x))
                {
                    SetScale(m_Scale);
                }
                ImGui::CenterAlignWidget("Y", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Y");
                if (ImGui::InputFloat("##Y", &m_Scale.y))
                {
                    SetScale(m_Scale);
                }
                ImGui::CenterAlignWidget("Z", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Z");
                if (ImGui::InputFloat("##Z", &m_Scale.z))
                {
                    SetScale(m_Scale);
                }
            }
            ImGui::PopItemWidth();
            ImGui::TreePop();
        }
    }
    // Set specific instance
    else
    {
        int instance = m_Instance;
        ImGui::PushItemWidth(120.0f * core::GetWidgetWidthCoefficient());
        ImGui::CenterAlignWidget("Instance", 120.0f * core::GetWidgetWidthCoefficient());
        ImGui::LabelHighlighted("Instance");
        if (ImGui::InputInt("##Instance", &instance))
        {
            SetInstance(instance);
        }
        int current = m_Current;
        ImGui::CenterAlignWidget("Current", 120.0f * core::GetWidgetWidthCoefficient());
        ImGui::LabelHighlighted("Current");
        if (ImGui::InputInt("##Current", &current))
        {
            SetCurrent(current);
        }
        ImGui::PopItemWidth();
        // Position
        if (ImGui::TreeNode("Position"))
        {
            ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
            ImGui::CenterAlignWidget("X", 80.0f * core::GetWidgetWidthCoefficient());
            ImGui::LabelHighlighted("X");
            if (ImGui::InputFloat("##X", &m_InstancePosition[m_Current - 1].x))
            {
                UpdateModelMat(m_Current);
            }
            ImGui::CenterAlignWidget("Y", 80.0f * core::GetWidgetWidthCoefficient());
            ImGui::LabelHighlighted("Y");
            if (ImGui::InputFloat("##Y", &m_InstancePosition[m_Current - 1].y))
            {
                UpdateModelMat(m_Current);
            }
            ImGui::CenterAlignWidget("Z", 80.0f * core::GetWidgetWidthCoefficient());
            ImGui::LabelHighlighted("Z");
            if (ImGui::InputFloat("##Z", &m_InstancePosition[m_Current - 1].z))
            {
                UpdateModelMat(m_Current);
            }
            ImGui::PopItemWidth();
            ImGui::TreePop();
        }
        // Rotation
        static bool slideRotate = true;
        if (ImGui::TreeNode("Rotation"))
        {
            ImGui::Checkbox("Slider", &slideRotate);
            if (slideRotate)
            {
                ImGui::PushItemWidth(280.0f * core::GetWidgetWidthCoefficient());
                ImGui::CenterAlignWidget("Pitch", 280.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Pitch");
                if (ImGui::SliderFloat("##Pitch", &m_InstanceEulerAngle[m_Current - 1].x, -360.0f, 360.0f))
                {
                    UpdateModelMat(m_Current);
                }
                ImGui::CenterAlignWidget("Yaw", 280.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Yaw");
                if (ImGui::SliderFloat("##Yaw", &m_InstanceEulerAngle[m_Current - 1].y, -360.0f, 360.0f))
                {
                    UpdateModelMat(m_Current);
                }
                ImGui::CenterAlignWidget("Roll", 280.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Roll");
                if (ImGui::SliderFloat("##Roll", &m_InstanceEulerAngle[m_Current - 1].z, -360.0f, 360.0f))
                {
                    UpdateModelMat(m_Current);
                }
                ImGui::PopItemWidth();
            }
            else
            {
                ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
                ImGui::CenterAlignWidget("Pitch", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Pitch");
                if (ImGui::InputFloat("##Pitch", &m_InstanceEulerAngle[m_Current - 1].x))
                {
                    SetPitch(m_InstanceEulerAngle[m_Current - 1].x, m_Current);
                }
                ImGui::CenterAlignWidget("Yaw", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Yaw");
                if (ImGui::InputFloat("##Yaw", &m_InstanceEulerAngle[m_Current - 1].y))
                {
                    SetYaw(m_InstanceEulerAngle[m_Current - 1].y, m_Current);
                }
                ImGui::CenterAlignWidget("Roll", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Roll");
                if (ImGui::InputFloat("##Roll", &m_InstanceEulerAngle[m_Current - 1].z))
                {
                    SetRoll(m_InstanceEulerAngle[m_Current - 1].z, m_Current);
                }
                ImGui::PopItemWidth();
            }
            ImGui::TreePop();
        }
        // Scale
        static bool keepScale = false;
        if (ImGui::TreeNode("Scale"))
        {
            ImGui::CenterAlignWidget("Keep Scale");
            ImGui::LabelHighlighted("Keep Scale");
            ImGui::Checkbox("##KeepScale", &keepScale);
            ImGui::PushItemWidth(80.0f * core::GetWidgetWidthCoefficient());
            if (keepScale)
            {
                ImGui::CenterAlignWidget("Scale", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Scale");
                if (ImGui::InputFloat("##Scale", &m_InstanceScale[m_Current - 1].x))
                {
                    SetScale(glm::vec3(m_InstanceScale[m_Current - 1].x), m_Current);
                }
            }
            else
            {
                ImGui::CenterAlignWidget("X", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("X");
                if (ImGui::InputFloat("##X", &m_InstanceScale[m_Current - 1].x))
                {
                    SetScale(m_InstanceScale[m_Current - 1], m_Current);
                }
                ImGui::CenterAlignWidget("Y", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Y");
                if (ImGui::InputFloat("##Y", &m_InstanceScale[m_Current - 1].y))
                {
                    SetScale(m_InstanceScale[m_Current - 1], m_Current);
                }
                ImGui::CenterAlignWidget("Z", 80.0f * core::GetWidgetWidthCoefficient());
                ImGui::LabelHighlighted("Z");
                if (ImGui::InputFloat("##Z", &m_InstanceScale[m_Current - 1].z))
                {
                    SetScale(m_InstanceScale[m_Current - 1], m_Current);
                }
            }
            ImGui::PopItemWidth();
            ImGui::TreePop();
        }
    }
    if (ImGui::TreeNode("Info"))
    {
        ImGui::LabelHighlighted("FilePath:");
        ImGui::TextWrapped(m_FilePath.c_str());
        ImGui::LabelHighlighted("Meshes:");
        ImGui::Text(std::to_string(m_Statics.MeshCount).c_str());
        ImGui::LabelHighlighted("Triangles:");
        ImGui::Text(std::to_string(m_Statics.TriangleCount).c_str());
        ImGui::LabelHighlighted("Vertices:");
        ImGui::Text(std::to_string(m_Statics.VertexCount).c_str());
        ImGui::TreePop();
    }
    // Reset all attributes
    ImGui::CenterAlignWidget("Reset");
    if (ImGui::Button("Reset"))
    {
        if (m_SetInstance)
        {
            SetInstance(1);
            SetPosition(glm::vec3(0.0f), 1);
            SetScale(glm::vec3(1.0f), 1);
            SetEulerAngle(glm::vec3(0.0f), 1);
            SetPosition(glm::vec3(0.0f));
            SetScale(glm::vec3(1.0f));
            SetEulerAngle(glm::vec3(0.0f));
        }
        else
        {
            SetPosition(glm::vec3(0.0f), m_Current);
            SetScale(glm::vec3(1.0f), m_Current);
            SetEulerAngle(glm::vec3(0.0f), m_Current);
            
        }
    }
}