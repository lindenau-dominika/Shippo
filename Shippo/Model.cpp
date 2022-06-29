#include "Model.hpp"
#include <iostream>
#include <filesystem>

Model::Model(const std::string& path) {
	load_model(path);
}

void Model::render(Shader& shader) const {
	for (auto& mesh : meshes) {
		mesh.render(shader);
	}
}

void Model::bind_texture(Shader& shader, const std::string& uniform_name, Texture& texture)
{
	shader.use();
	shader.set_uniform(uniform_name, texture.unit_index());
	texture.bind();
}

std::unordered_map<std::string, Texture>& Model::get_textures()
{
	return textures;
}

void Model::add_texture(std::string name, Texture&& texture)
{
	textures.insert({ name, std::move(texture) });
}

void Model::load_model(const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (scene == nullptr)
	{
		throw importer.GetErrorString();
	}

	if (scene->HasMaterials()) {
		for (int i = 0; i < scene->mNumMaterials; i++) {
			const aiMaterial* material = scene->mMaterials[i];
			for (int j = 0; j < material->GetTextureCount(aiTextureType_DIFFUSE); j++) {
				aiString texture_path;
				auto ret = material->GetTexture(aiTextureType_DIFFUSE, j, &texture_path);

				if (auto texture = scene->GetEmbeddedTexture(texture_path.C_Str())) {
					size_t size = texture->mWidth * texture->mHeight;
					if (texture->mHeight == 0) {
						size = texture->mWidth;
					}
					
					Texture text = Texture((unsigned char*)texture->pcData, size, TextureType::Diffuse);
					if (text.is_valid()) {
						std::string texture_name = texture->mFilename.C_Str();
						auto last_slash_index = texture_name.find_last_of("/\\") + 1;
						texture_name = texture_name.substr(last_slash_index, texture_name.find_last_of(".") - last_slash_index);
						textures.insert({ texture_name, std::move(text) });
					} 
				}
				else {
					std::string real_texture_path = (std::filesystem::path(path).parent_path() / std::string(texture_path.C_Str())).string();
					Texture text = Texture(real_texture_path, TextureType::Diffuse);
					std::string texture_name = texture_path.C_Str();
					auto last_slash_index = texture_name.find_last_of("/\\") + 1;
					texture_name = texture_name.substr(last_slash_index, texture_name.find_last_of(".") - last_slash_index);
					if (text.is_valid()) {
						textures.insert({ texture_name, std::move(text) });
					}
				}
			}
			
		}
	}

	process_node(scene, scene->mRootNode);
}

void Model::process_node(const aiScene* scene, aiNode* node)
{
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(process_mesh(scene, mesh));
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		aiNode* child = node->mChildren[i];
		process_node(scene, child);
	}
}

Mesh Model::process_mesh(const aiScene* scene, aiMesh* mesh)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex{};
		aiVector3D v = mesh->mVertices[i];
		vertex.position.x = v.x;
		vertex.position.y = v.y;
		vertex.position.z = v.z;

		if (mesh->HasNormals()) {
			aiVector3D n = mesh->mNormals[i];
			vertex.normal.x = n.x;
			vertex.normal.y = n.y;
			vertex.normal.z = n.z;
		}

		if (mesh->HasTextureCoords(0)) {
			aiVector3D t = mesh->mTextureCoords[0][i];
			vertex.tex_coords.x = t.x;
			vertex.tex_coords.y = t.y;
		}
		
		vertices.emplace_back(vertex);
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}		
	}

	return Mesh(vertices, indices);
}
