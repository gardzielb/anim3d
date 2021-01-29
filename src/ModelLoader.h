//
// Created by bartosz on 1/19/21.
//

#pragma once

#include "Model.h"
#include <vector>
#include <assimp/scene.h>


class ModelLoader
{
private:
	typedef std::shared_ptr<std::vector<Mesh>> MeshesPtr;
	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	std::vector<Texture> texturesLoaded;
	std::string directory;

public:
	std::shared_ptr<SimpleModel> loadModel( const std::string & path );

private:
	void processNode( aiNode * node, const aiScene * scene, MeshesPtr & meshes );

	void processMesh( aiMesh * mesh, const aiScene * scene, MeshesPtr & meshes );

	std::vector<Texture> loadMaterialTextures( aiMaterial * mat, aiTextureType type, std::string typeName );

	unsigned int loadTexture( const char * path );
};
