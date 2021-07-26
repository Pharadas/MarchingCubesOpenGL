#ifndef LINE_TRIANGLE_INTERSECTION_H
#define LINE_TRIANGLE_INTERSECTION_H

#include <vector>
#include <glm/glm.hpp>

std::pair<bool, glm::vec3> rayIntersectsTriangle(glm::vec3 rayOrigin, 
												glm::vec3 rayVector, 
												glm::vec3 inTriangle[3]) {
    const float EPSILON = 0.0000001;
    glm::vec3 vertex0 = inTriangle[0];
    glm::vec3 vertex1 = inTriangle[1];  
    glm::vec3 vertex2 = inTriangle[2];

    glm::vec3 edge1, edge2, h, s, q;
    float a,f,u,v;
    edge1 = vertex1 - vertex0;
    edge2 = vertex2 - vertex0;
    h = glm::cross(rayVector, edge2);
    a = glm::dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return std::make_pair(false, glm::vec3(0));    // This ray is parallel to this triangle.

    f = 1.0/a;
    s = rayOrigin - vertex0;
    u = f * glm::dot(s, h);

    if (u < 0.0 || u > 1.0)
        return std::make_pair(false, glm::vec3(0));

    q = glm::cross(s, edge1);
    v = f * glm::dot(rayVector, q);

    if (v < 0.0 || u + v > 1.0)
        return std::make_pair(false, glm::vec3(0));

    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * glm::dot(edge2, q);

    if (t > EPSILON) // ray intersection
    {
        return std::make_pair(true, rayOrigin + rayVector * t);
    }

    else
		return std::make_pair(false, glm::vec3(0));
}

std::pair<bool, glm::vec3> meshRayCasting(glm::vec3 rayOrigin, glm::vec3 rayVector, std::vector<float> triangles, int floatsPerTriangle, glm::vec3 objectPosition) {
	glm::vec3 closestPosition(0xffffffff, 0xffffffff, 0xffffffff); // max possible value of a 32 bit int
	std::pair<bool, glm::vec3> currentHitPair;
	bool hasHitSomething = false;

	for (int i = 0; i < (int) triangles.size(); i += floatsPerTriangle * 3) {
		// gamin time debugging
		// std::cout << "new triangle" << std::endl;
		// std::cout << inTriangle[0].x << ' ' << inTriangle[0].y << ' ' << inTriangle[0].z << std::endl;
		// std::cout << inTriangle[1].x << ' ' << inTriangle[1].y << ' ' << inTriangle[1].z << std::endl;
		// std::cout << inTriangle[2].x << ' ' << inTriangle[2].y << ' ' << inTriangle[2].z << std::endl;

		glm::vec3 inTriangle[3];
		inTriangle[0] = glm::vec3(triangles[i], triangles[i + 1], triangles[i + 2]);
		inTriangle[1] = glm::vec3(triangles[i + floatsPerTriangle], triangles[i + floatsPerTriangle + 1], triangles[i + floatsPerTriangle + 2]);
		inTriangle[2] = glm::vec3(triangles[i + (floatsPerTriangle * 2)], triangles[i + (floatsPerTriangle  * 2) + 1], triangles[i + (floatsPerTriangle * 2) + 2]);

		inTriangle[0] += objectPosition;
		inTriangle[1] += objectPosition;
		inTriangle[2] += objectPosition;

		currentHitPair = rayIntersectsTriangle(rayOrigin, rayVector, inTriangle);

		// for some reason, glm::lessthan doesnt return the bool of the operation, i have to extract it from
		// a vector of 'glm::vec<3, bool, glm::packed_highp>' (the second value is the bool, so i take the y)
		if ((currentHitPair.first) && glm::lessThan(currentHitPair.second, closestPosition).y)
			closestPosition = currentHitPair.second;
			hasHitSomething = true;
	}

	if (hasHitSomething)
		return std::make_pair(true, closestPosition);
	return std::make_pair(false, closestPosition);
}


#endif