#include "Model.hpp"
#include <iostream>

Model::Model(const std::string& path) {
	load_model(path);
}

void Model::render(Shader& shader) const {
	for (auto& mesh : meshes) {
		mesh.render(shader);
	}
}

void Model::load_model(const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (scene == nullptr)
	{
		throw importer.GetErrorString();
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
