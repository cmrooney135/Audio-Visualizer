#include "circle.h"
#include "rect.h"


Circle::~Circle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Circle::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2); // +2 for center and last vertex
    glBindVertexArray(0);
}

void Circle::initVectors() {
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        vertices.push_back(radius * cosf(theta)); // x = r*cos(theta)
        vertices.push_back(radius * sinf(theta)); // y = r*sin(theta)
    }
}

float Circle::getLeft() const   { return pos.x - radius; }
float Circle::getRight() const  { return pos.x + radius; }
float Circle::getTop() const    { return pos.y + radius; }
float Circle::getBottom() const { return pos.y - radius; }