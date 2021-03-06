//
// Created by bartosz on 2/11/21.
//

#include "AnimatedModel.h"
#include <spdlog/spdlog.h>

AnimatedModel::AnimatedModel( std::vector<std::shared_ptr<Model>> & frames )
		: frames( frames )
{}

void AnimatedModel::draw( const Shader & shader )
{
	frames[current]->toOrigin().transform( modelMatrix );
	frames[current]->draw( shader );

	if ( !current || current == frames.size() - 1 )
		increase *= (-1);

	current += increase;
}

static std::string makeFileName( const std::string & path, int number )
{
	std::string fileName = path + "_000000.obj";
	std::string numStr = std::to_string( number );
	int numOffset = fileName.length() - 4 - numStr.length();
	return fileName.replace( numOffset, numStr.length(), numStr );
}

std::shared_ptr<AnimatedModel> AnimationBuilder::createAnimation( const std::string & path, int frameCount,
																  ModelLoader & loader )
{
	std::vector<std::shared_ptr<Model>> frames;
	frames.reserve( frameCount );

	for ( int i = 0; i < frameCount; i++ )
	{
		std::string filePath = makeFileName( path, i + 1 );
		frames.push_back( loader.loadModel( filePath ) );
	}

	return std::make_shared<AnimatedModel>( frames );
}
