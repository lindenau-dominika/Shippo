#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.hpp"
#include "Renderable.hpp"
#include "Texture.hpp"
#include <unordered_map>

class Model : Renderable {
public:
	Model(const std::string& path);

	virtual void render(Shader& shader) const override;
	void bind_texture(Shader& shader, const std::string& uniform_name, Texture& texture);
	std::unordered_map<std::string, Texture>& get_textures();
	void add_texture(std::string name, Texture&& texture);

private:
	std::vector<Mesh> meshes;
	std::unordered_map<std::string, Texture> textures;
	
	void load_model(const std::string& path);
	void process_node(const aiScene* scene, aiNode* node);
	Mesh process_mesh(const aiScene* scene, aiMesh* mesh);
};