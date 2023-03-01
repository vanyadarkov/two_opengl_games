#include "object2D.h"

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3};

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill, int type)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    switch (type) {
        case RIGHT_ANGLE_TRIANGLE:
        {
            vertices.push_back(VertexFormat(corner, color));
            vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), color));
            vertices.push_back(VertexFormat(corner + glm::vec3(0, length, 0), color));
            indices = { 0, 1, 2 };
            break;
        }
        case EQUILATERAL_TRIANGLE:
        {
            vertices.push_back(VertexFormat(corner, color));
            vertices.push_back(VertexFormat(corner + glm::vec3(length / 2, length * sqrt(3) / 2, 0), color));
            vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), color));
            indices = { 0, 1, 2};
            break;
        }
        default: {
            vertices.push_back(VertexFormat(corner, color));
            vertices.push_back(VertexFormat(corner + glm::vec3(length, 0, 0), color));
            vertices.push_back(VertexFormat(corner + glm::vec3(0, length, 0), color));
            indices = { 0, 1, 2 };
            break;
        }
    }

    Mesh* triangle = new Mesh(name);

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}

Mesh* object2D::CreateCircle(const std::string& name, glm::vec3 leftBottomCorner, float radius, int numberOfTriangles, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    float angle = M_PI * 2.0f / numberOfTriangles;

    float xPos = 0, yPos = 0;
    float prevX = xPos;
    float prevY = yPos - radius;

    for (int i = 0; i <= numberOfTriangles; i++) {
        float newX = radius * sin(angle * i);
        float newY = -radius * cos(angle * i);

        if (fill) {
            vertices.push_back(VertexFormat(corner, color));
            vertices.push_back(VertexFormat(glm::vec3(prevX, prevY, 0), color));
        }
        vertices.push_back(VertexFormat(glm::vec3(newX, newY, 0), color));

        prevX = newX;
        prevY = newY;
    }

    if (fill) {
        for (int i = 0; i <= numberOfTriangles; i++) {
            indices.push_back(3 * i);
            indices.push_back(3 * i + 1);
            indices.push_back(3 * i + 2);
        }
    }
    else {
        for (int i = 0; i <= numberOfTriangles; i++) {
            indices.push_back(i);
        }
    }
    

    Mesh* circle = new Mesh(name);
    

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateHeart(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(length / 2, 0, 0), color),
        VertexFormat(corner + glm::vec3(0, length / 2, 0), color),
        VertexFormat(corner + glm::vec3(0, length * 3 / 4, 0), color),
        VertexFormat(corner + glm::vec3(length / 4, length, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, length * 3 / 4, 0), color),
        VertexFormat(corner + glm::vec3(length * 3 / 4, length, 0), color),
        VertexFormat(corner + glm::vec3(length, length * 3 / 4, 0), color),
        VertexFormat(corner + glm::vec3(length, length / 2, 0), color)
    };

    Mesh* heart = new Mesh(name);
    std::vector<unsigned int> indices;

    if (!fill) {
        indices = { 0, 1, 2, 3, 4, 5, 6, 7 };
        heart->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices = { 0, 1, 7,
                    1, 4, 7,
                    1, 2, 4,
                    2, 3, 4,
                    4, 5, 6,
                    4, 6, 7 };
    }

    heart->InitFromData(vertices, indices);
    return heart;
}

Mesh* object2D::CreateBullet(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(length / 4, 0, 0), color),
        VertexFormat(corner + glm::vec3(length / 4, length * 3 / 4, 0), color),
        VertexFormat(corner + glm::vec3(length / 2, length, 0), color),
        VertexFormat(corner + glm::vec3(length * 3 / 4, length * 3 / 4, 0), color),
        VertexFormat(corner + glm::vec3(length * 3 / 4, 0, 0), color)
    };

    Mesh* bullet = new Mesh(name);
    std::vector<unsigned int> indices;

    if (!fill) {
        indices = { 0, 1, 2, 3, 4};
        bullet->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices = { 0, 1, 4,
                    1, 2, 3,
                    1, 3, 4 };
    }

    bullet->InitFromData(vertices, indices);
    return bullet;
}

void object2D::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices,std::unordered_map<std::string, Mesh*>& meshes)
{
    unsigned int VAO = 0;
    // TODO(student): Create the VAO and bind it

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // TODO(student): Create the VBO and bind it

    GLuint VBO_ID;			    // ID-ul (nume sau referinta) buffer-ului ce va fi cerut de la GPU
    glGenBuffers(1, &VBO_ID);	// se genereaza ID-ul (numele) bufferului

    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);  // bind

    // TODO(student): Send vertices data into the VBO buffer

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it

    GLuint IBO_ID;
    glGenBuffers(1, &IBO_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_ID);


    // TODO(student): Send indices data into the IBO buffer

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // TODO(student): Unbind the VAO

    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        std::cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        std::cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}