//
// Created by bartosz on 1/19/21.
//

//#define STB_IMAGE_IMPLEMENTATION

#include "../dependencies/stb_image.h"
#include "ModelLoader.h"
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>


std::shared_ptr<SimpleModel> ModelLoader::loadModel( const std::string & path )
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(
			path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs |
				  aiProcess_CalcTangentSpace
	);
	// check for errors
	if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
		return nullptr;
	}
	// retrieve the directory path of the filepath
	directory = path.substr( 0, path.find_last_of( '/' ) );
	texturesLoaded.clear();

	MeshesPtr meshes = std::make_shared<std::vector<Mesh>>();
	meshes->reserve( scene->mNumMeshes );

	// process ASSIMP's root node recursively
	processNode( scene->mRootNode, scene, meshes );

	return std::make_shared<SimpleModel>( meshes );
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void ModelLoader::processNode( aiNode * node, const aiScene * scene, MeshesPtr & meshes )
{
	// process each mesh located at the current node
	for ( unsigned int i = 0; i < node->mNumMeshes; i++ )
	{
		// the node object only contains indices to index the actual objects in the scene.
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh( mesh, scene, meshes );
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for ( unsigned int i = 0; i < node->mNumChildren; i++ )
	{
		processNode( node->mChildren[i], scene, meshes );
	}
}

void ModelLoader::processMesh( aiMesh * mesh, const aiScene * scene, MeshesPtr & meshes )
{
	// data to fill
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// walk through each of the mesh's vertices
	for ( unsigned int i = 0; i < mesh->mNumVertices; i++ )
	{
		Vertex vertex;
		vertex.position = glm::vec3( mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z );

		if ( mesh->HasNormals() )
		{
			vertex.normal = glm::vec3( mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z );
		}

		if ( mesh->mTextureCoords[0] ) // does the mesh contain texture coordinates?
		{
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vertex.texCoords = glm::vec2( mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y );
			vertex.tangent = glm::vec3( mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z );
			vertex.bitangent = glm::vec3( mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z );
		}
		else
			vertex.texCoords = glm::vec2( 0.0f, 0.0f );

		vertices.push_back( vertex );
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for ( unsigned int i = 0; i < mesh->mNumFaces; i++ )
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for ( unsigned int j = 0; j < face.mNumIndices; j++ )
			indices.push_back( face.mIndices[j] );
	}
	// process materials
	aiMaterial * material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures( material, aiTextureType_DIFFUSE, "diffuse" );
	textures.insert( textures.end(), diffuseMaps.begin(), diffuseMaps.end() );
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures( material, aiTextureType_SPECULAR, "specular" );
	textures.insert( textures.end(), specularMaps.begin(), specularMaps.end() );
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures( material, aiTextureType_HEIGHT, "normal" );
	textures.insert( textures.end(), normalMaps.begin(), normalMaps.end() );
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures( material, aiTextureType_AMBIENT, "height" );
	textures.insert( textures.end(), heightMaps.begin(), heightMaps.end() );

	// return a mesh object created from the extracted mesh data
	meshes->emplace_back( vertices, indices, textures );
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> ModelLoader::loadMaterialTextures( aiMaterial * mat, aiTextureType type, std::string typeName )
{
	std::vector<Texture> textures;
	for ( unsigned int i = 0; i < mat->GetTextureCount( type ); i++ )
	{
		aiString str;
		mat->GetTexture( type, i, &str );
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for ( unsigned int j = 0; j < texturesLoaded.size(); j++ )
		{
			if ( std::strcmp( texturesLoaded[j].path.data(), str.C_Str() ) == 0 )
			{
				textures.push_back( texturesLoaded[j] );
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if ( !skip )
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = loadTexture( str.C_Str() );
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back( texture );
			// store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			texturesLoaded.push_back( texture );
		}
	}
	return textures;
}


unsigned int ModelLoader::loadTexture( const char * path )
{
	std::string filename = std::string( path );
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures( 1, &textureID );

	int width, height, nrComponents;
	unsigned char * data = stbi_load( filename.c_str(), &width, &height, &nrComponents, 0 );
	if ( data )
	{
		GLenum format;
		if ( nrComponents == 1 )
			format = GL_RED;
		else if ( nrComponents == 3 )
			format = GL_RGB;
		else if ( nrComponents == 4 )
			format = GL_RGBA;

		glBindTexture( GL_TEXTURE_2D, textureID );
		glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data );
		glGenerateMipmap( GL_TEXTURE_2D );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		stbi_image_free( data );
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free( data );
	}

	return textureID;
}