#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector>
#include "Mesh.h"

class Model
{
public:
	Model();
	Model(const Model& m);
	Model(Model&& m) noexcept;
	~Model();

	Model&				operator=(const Model& m);
	Model&				operator=(Model&& m) noexcept;

	bool				LoadModel(const char* filePath);
	void				Render() const;
	glm::vec3			GetSize();

private:
	std::vector<Mesh>	meshes;

	void				LoadMesh(aiMesh* mesh, const aiScene* scene);
	void				ReadNode_r(aiNode* node, const aiScene* scene);
};