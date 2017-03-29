#include "Model.h"
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <glm.hpp>
#include <iostream>

Model::Model()
{}

Model::Model(const Model& m) : meshes(m.meshes)
{}

Model::Model(Model&& m) noexcept : meshes(std::move(m.meshes))
{}

Model::~Model()
{}

Model& Model::operator=(const Model& m)
{
	meshes = (this != &m) ? m.meshes : meshes;
	return *this;
}

Model& Model::operator=(Model&& m) noexcept
{
	meshes = std::move(m.meshes);
	return *this;
}

/*
=============================================================================
bool LoadModel

Load a file through ASSIMP and process scene tree nodes in order to load mesh
=============================================================================
*/
bool Model::LoadModel(const char* filePath)
{
	Assimp::Importer importer;
	const auto scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
		std::cerr << "[Model::LoadModel] ASSIMP cannot load: " << filePath << std::endl;
		return false;
	}

	meshes.reserve(scene->mNumMeshes);
	ReadNode_r(scene->mRootNode, scene);

	return true;
}

/*
====================================
void Render const

Call Render method on all sub meshes
====================================
*/
void Model::Render() const
{
	for (auto& mesh : meshes) {
		mesh.Render();
	}
}

glm::vec3 Model::GetSize()
{
	glm::vec3 sizeSum = glm::vec3(0);
	for (auto& m : meshes) {
		sizeSum += m.GetSize();
	}
	return sizeSum;
}

std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type)
{
	std::vector<Texture> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture(GL_TEXTURE_2D);
		texture.LoadImage(str.C_Str(), true);
		texture.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		texture.SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		textures.emplace_back(std::move(texture));
	}
	return textures;
}

/*
====================================================================================================
void LoadMesh

Read mesh information (position, normal, texture coordinates, indices) and pass it to a Mesh object.
This Mesh is added to our meshes vector.
====================================================================================================
*/
void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	for (size_t vertex_i = 0; vertex_i < mesh->mNumVertices; vertex_i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[vertex_i].x;
		vector.y = mesh->mVertices[vertex_i].y;
		vector.z = mesh->mVertices[vertex_i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[vertex_i].x;
		vector.y = mesh->mNormals[vertex_i].y;
		vector.z = mesh->mNormals[vertex_i].z;
		vertex.normal = vector;
		
		vertex.texCoords = (mesh->HasTextureCoords(0)) ?
			glm::vec2(mesh->mTextureCoords[0][vertex_i].x, mesh->mTextureCoords[0][vertex_i].y) :
			glm::vec2(0);

		vertices.push_back(vertex);
	}

	for (size_t face_i = 0; face_i < mesh->mNumFaces; face_i++) {
		aiFace face = mesh->mFaces[face_i];
		for (size_t index = 0; index < face.mNumIndices; index++)
			indices.push_back(face.mIndices[index]);
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	meshes.emplace_back();
	meshes.back().InitMesh(vertices, indices, textures);
}

/*
==================================================================================================
void ReadNode_r

Recursive method which loop through all meshes indexes stored in node and called LoadMesh on them.
Then loop through all children node and recursively call ReadNode_r on them.
==================================================================================================
*/
void Model::ReadNode_r(aiNode* node, const aiScene* scene)
{
	for (size_t mesh_i = 0; mesh_i < node->mNumMeshes; mesh_i++) {
		LoadMesh(scene->mMeshes[node->mMeshes[mesh_i]], scene);
	}
	for (size_t child_i = 0; child_i < node->mNumChildren; child_i++) {
		ReadNode_r(node->mChildren[child_i], scene);
	}
}