//
// Created by bartosz on 2/10/21.
//

#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>


class MoveBehavior
{
public:
	virtual glm::vec3 forward( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
							   const glm::vec3 & right ) = 0;

	virtual glm::vec3 back( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
							const glm::vec3 & right ) = 0;

	virtual glm::vec3 left( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
							const glm::vec3 & right ) = 0;

	virtual glm::vec3 right( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
							 const glm::vec3 & right ) = 0;

	virtual ~MoveBehavior() = default;
};


class NoMoveBehavior : public MoveBehavior
{
public:
	glm::vec3 forward( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
					   const glm::vec3 & right ) override;

	glm::vec3 back( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
					const glm::vec3 & right ) override;

	glm::vec3 left( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
					const glm::vec3 & right ) override;

	glm::vec3 right( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
					 const glm::vec3 & right ) override;
};


class SimpleXYMoveBehavior : public MoveBehavior
{
private:
	float speed;

public:
	SimpleXYMoveBehavior( float speed );

	glm::vec3 forward( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
					   const glm::vec3 & right ) override;

	glm::vec3 back( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
					const glm::vec3 & right ) override;

	glm::vec3 left( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
					const glm::vec3 & right ) override;

	glm::vec3 right( const glm::vec3 & position, const glm::vec3 & front, const glm::vec3 & up,
					 const glm::vec3 & right ) override;
};
