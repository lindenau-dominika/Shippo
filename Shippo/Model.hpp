#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.hpp"
#include "Renderable.hpp"

class Model : Renderable {
public:
	Model(const std::string& path);

	 virtual void render(Shader& shader) const override;
	
private:
	std::vector<Mesh> meshes;
	
	void load_model(const std::string& path);
	void process_node(const aiScene* scene, aiNode* node);
	Mesh process_mesh(const aiScene* scene, aiMesh* mesh);
};