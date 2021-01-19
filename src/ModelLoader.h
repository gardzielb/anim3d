//
// Created by bartosz on 1/19/21.
//

#pragma once

#include "Model.h"
#include <optional>
#include <vector>
#include <assimp/scene.h>


class ModelLoader
{
private:
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;

public:
	std::optional<SimpleModel> loadModel( const std::string & path, const glm::mat4 & modelMatrix );

private:
	void processNode( aiNode * node, const aiScene * scene );

	void processMesh( aiMesh * mesh, const aiScene * scene );

	std::vector<Texture> loadMaterialTextures( aiMaterial * mat, aiTextureType type, std::string typeName );

	unsigned int loadTexture( const char * path );
};
