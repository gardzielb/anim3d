//
// Created by bartosz on 2/10/21.
//

#include "MoveBehavior.h"


SimpleXYMoveBehavior::SimpleXYMoveBehavior( float speed )
		: speed( speed )
{}

glm::vec3 SimpleXYMoveBehavior::forward( const glm::vec3 & position, const glm::vec3 & front,
										 const glm::vec3 & up, const glm::vec3 & right )
{
	return position + speed * up;
}

glm::vec3 SimpleXYMoveBehavior::back( const glm::vec3 & position, const glm::vec3 & front,
									  const glm::vec3 & up, const glm::vec3 & right )
{
	return position - speed * up;
}

glm::vec3 SimpleXYMoveBehavior::left( const glm::vec3 & position, const glm::vec3 & front,
									  const glm::vec3 & up, const glm::vec3 & right )
{
	return position - speed * right;
}

glm::vec3 SimpleXYMoveBehavior::right( const glm::vec3 & position, const glm::vec3 & front,
									   const glm::vec3 & up, const glm::vec3 & right )
{
	return position + speed * right;
}

glm::vec3 NoMoveBehavior::forward( const glm::vec3 & position, const glm::vec3 & front,
								   const glm::vec3 & up, const glm::vec3 & right )
{
	return position;
}

glm::vec3 NoMoveBehavior::back( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
								const glm::vec3 & right )
{
	return position;
}

glm::vec3 NoMoveBehavior::left( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
								const glm::vec3 & right )
{
	return position;
}

glm::vec3 NoMoveBehavior::right( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
								 const glm::vec3 & right )
{
	return position;
}
