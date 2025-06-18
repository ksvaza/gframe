#include "shader.hpp"

int Shader::Read(const char* filepath, GLenum type)
{
    int err_code = 0;
    long length = 0;
    // Open file
    FILE* file;
    fopen_s(&file, filepath, "r");
    if (!file)
    {
        printf("Failed to open shader file: %s\n", filepath);
        err_code = 1; goto close;
    }

    // Get file length
    fseek(file, 0, SEEK_END);
    length = ftell(file);
    if (length < 0) { err_code = 2; goto close; }
    fseek(file, 0, SEEK_SET);

    // Write to correct buffer and setup source
    if (type == GL_VERTEX_SHADER)
    {
        vertexShaderSrc = (char*)malloc(length + 1);
        if (!vertexShaderSrc)
        {
            printf("Failed to allocate vertex source!\n");
            err_code = 3; goto close;
        }
        memset(vertexShaderSrc, 0, length + 1);
        fread(vertexShaderSrc, 1, length, file);
    }
    else if (type == GL_FRAGMENT_SHADER)
    {
        fragmentShaderSrc = (char*)malloc(length + 1);
        if (!fragmentShaderSrc)
        {
            printf("Failed to allocate fragment source!\n");
            err_code = 3; goto close;
        }
        memset(fragmentShaderSrc, 0, length + 1);
        fread(fragmentShaderSrc, 1, length + 1, file);
    }
    else
    {
        printf("Invalid shader type: %d\n", type);
        err_code = 4; goto close;
    }

close:
    if (file) { fclose(file); }
    return err_code;
}

int Shader::Compile()
{
    glDeleteShader(vertexShaderID);
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (!vertexShaderSrc) { return 1; }
    glShaderSource(vertexShaderID, 1, &vertexShaderSrc, NULL);

    glCompileShader(vertexShaderID);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        printf("Vertex shader compilation failed: %s\n", infoLog);
        return 2;
    }

    glDeleteShader(fragmentShaderID);
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (!fragmentShaderSrc) { return 1; }
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSrc, NULL);

    glCompileShader(fragmentShaderID);
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
        printf("Fragment shader compilation failed: %s\n", infoLog);
        return 3;
    }

    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID, vertexShaderID);
    glAttachShader(shaderProgramID, fragmentShaderID);
    glLinkProgram(shaderProgramID);
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
        return 4;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return 0;
}

GLuint Shader::GetID()
{
	return shaderProgramID;
}

void Shader::Use()
{
    if (shaderProgramID)
    {
        glUseProgram(shaderProgramID);
    }
}

void Shader::Delete() // Shis ir pavisam nepareizi un uzreiz kaa straadaa failuceli shaderiem jaaatkomentee
{
    //if (vertexShaderSrc) { free(vertexShaderSrc); }
    //if (fragmentShaderSrc) { free(fragmentShaderSrc); }
    if (shaderProgramID) { glDeleteProgram(shaderProgramID); }
}

Shader::Shader()
{
    vertexShaderSrc = NULL;
    fragmentShaderSrc = NULL;
    vertexShaderID = 0;
    fragmentShaderID = 0;
    shaderProgramID = 0;
}

Shader::~Shader()
{
    Delete();
}
