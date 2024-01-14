#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <glm/fwd.hpp>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <GL/glew.h>   // The GL Header File
#include <GL/gl.h>   // The GL Header File
#include <GLFW/glfw3.h> // The GLFW header
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

using namespace std;

unsigned long score = 0;

// function declaration
std::vector<int> randomInt();

glm::mat4 projectionMatrix;
glm::mat4 viewingMatrix;
glm::mat4 modelingMatrix;

GLint modelingMatrixLoc[5];
GLint viewingMatrixLoc[5];
GLint projectionMatrixLoc[5];
GLint eyePosLoc[5];

glm::vec3 eyePos(0, 1, 9);
glm::vec3 lookAtPoint(0, 1, -200);


int activeProgramIndex = 0;

float speed = 0.3f;
float incrementSpeed = 0.0001f;

float roadGone = 0.0f;
float handleRoadSwitch = 0.0f;
int firstRoad = 0;
int secondRoad = 0;

float jumpGone = 0.0f;

int checkPointHold = 0;

bool isRPressed = false;
bool isAPressed = false;
bool isDPressed = false;

float bunnyX = 0.0f;
float bunnyY = 0.0f;
bool isBunnyRising = true;
int bunnyDead = 0;
int bunnyDance = 0.f;
std::vector<int> randVec = randomInt();

GLuint gProgram[5];
GLint gIntensityLoc;
float gIntensity = 1000;
int gWidth = 1280, gHeight = 720;

vector<unsigned long> allSizes;
vector<int> faceSizesHelper;

int bunnyDeadBy = -1;

struct Vertex
{
    Vertex(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
    GLfloat x, y, z;
};

struct Texture
{
    Texture(GLfloat inU, GLfloat inV) : u(inU), v(inV) { }
    GLfloat u, v;
};

struct Normal
{
    Normal(GLfloat inX, GLfloat inY, GLfloat inZ) : x(inX), y(inY), z(inZ) { }
    GLfloat x, y, z;
};

struct Face
{
	Face(int v[], int t[], int n[]) {
		vIndex[0] = v[0];
		vIndex[1] = v[1];
		vIndex[2] = v[2];
		tIndex[0] = t[0];
		tIndex[1] = t[1];
		tIndex[2] = t[2];
		nIndex[0] = n[0];
		nIndex[1] = n[1];
		nIndex[2] = n[2];
	}
    GLuint vIndex[3], tIndex[3], nIndex[3];
};

vector<Vertex> gVertices;
vector<Texture> gTextures;
vector<Normal> gNormals;
vector<Face> gFaces;

GLuint gVertexAttribBuffer, gTextVBO, gIndexBuffer;
GLint gInVertexLoc, gInNormalLoc;
int gVertexDataSizeInBytes, gNormalDataSizeInBytes;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
    GLuint Advance;    // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;


bool ParseObj(const string& fileName)
{
    fstream myfile;

    // Open the input 
    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            stringstream str(curLine);
            GLfloat c1, c2, c3;
            GLuint index[9];
            string tmp;

            if (curLine.length() >= 2)
            {
                if (curLine[0] == '#') // comment
                {
                    continue;
                }
                else if (curLine[0] == 'v')
                {
                    if (curLine[1] == 't') // texture
                    {
                        str >> tmp; // consume "vt"
                        str >> c1 >> c2;
                        gTextures.push_back(Texture(c1, c2));
                    }
                    else if (curLine[1] == 'n') // normal
                    {
                        str >> tmp; // consume "vn"
                        str >> c1 >> c2 >> c3;
                        gNormals.push_back(Normal(c1, c2, c3));
                    }
                    else // vertex
                    {
                        str >> tmp; // consume "v"
                        str >> c1 >> c2 >> c3;
                        gVertices.push_back(Vertex(c1, c2, c3));
                    }
                }
                else if (curLine[0] == 'f') // face
                {
                    str >> tmp; // consume "f"
					char c;
					int vIndex[3],  nIndex[3], tIndex[3];
					str >> vIndex[0]; str >> c >> c; // consume "//"
					str >> nIndex[0]; 
					str >> vIndex[1]; str >> c >> c; // consume "//"
					str >> nIndex[1]; 
					str >> vIndex[2]; str >> c >> c; // consume "//"
					str >> nIndex[2]; 

					assert(vIndex[0] == nIndex[0] &&
						   vIndex[1] == nIndex[1] &&
						   vIndex[2] == nIndex[2]); // a limitation for now

					// make indices start from 0
					for (int c = 0; c < 3; ++c)
					{
						vIndex[c] -= 1;
						nIndex[c] -= 1;
						tIndex[c] -= 1;
					}

                    gFaces.push_back(Face(vIndex, tIndex, nIndex));
                }
                else
                {
                    cout << "Ignoring unidentified line in obj file: " << curLine << endl;
                }
            }

            //data += curLine;
            if (!myfile.eof())
            {
                //data += "\n";
            }
        }


        myfile.close();
    }
    else
    {
        return false;
    }

	/*
	for (int i = 0; i < gVertices.size(); ++i)
	{
		Vector3 n;

		for (int j = 0; j < gFaces.size(); ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				if (gFaces[j].vIndex[k] == i)
				{
					// face j contains vertex i
					Vector3 a(gVertices[gFaces[j].vIndex[0]].x, 
							  gVertices[gFaces[j].vIndex[0]].y,
							  gVertices[gFaces[j].vIndex[0]].z);

					Vector3 b(gVertices[gFaces[j].vIndex[1]].x, 
							  gVertices[gFaces[j].vIndex[1]].y,
							  gVertices[gFaces[j].vIndex[1]].z);

					Vector3 c(gVertices[gFaces[j].vIndex[2]].x, 
							  gVertices[gFaces[j].vIndex[2]].y,
							  gVertices[gFaces[j].vIndex[2]].z);

					Vector3 ab = b - a;
					Vector3 ac = c - a;
					Vector3 normalFromThisFace = (ab.cross(ac)).getNormalized();
					n += normalFromThisFace;
				}

			}
		}

		n.normalize();

		gNormals.push_back(Normal(n.x, n.y, n.z));
	}
	*/

        // std::cout << gVertices.size() << std::endl;
        // std::cout << gNormals.size() << std::endl;
        // std::cout << gFaces.size() << std::endl;

    unsigned long x = gVertices.size() * 3 * sizeof(GLfloat);

    allSizes.push_back(x);
    int z = 0;
    faceSizesHelper.push_back(gFaces.size());
    

	assert(gVertices.size() == gNormals.size());

    return true;
}

bool ReadDataFromFile(
    const string& fileName, ///< [in]  Name of the shader file
    string&       data)     ///< [out] The contents of the file
{
    fstream myfile;

    // Open the input 
    myfile.open(fileName.c_str(), std::ios::in);

    if (myfile.is_open())
    {
        string curLine;

        while (getline(myfile, curLine))
        {
            data += curLine;
            if (!myfile.eof())
            {
                data += "\n";
            }
        }

        myfile.close();
    }
    else
    {
        return false;
    }

    return true;
}

void createVS(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shader, &length);
    glCompileShader(vs);

    char output[1024] = {0};
    glGetShaderInfoLog(vs, 1024, &length, output);
    printf("VS compile log: %s\n", output);

    glAttachShader(program, vs);
}

void createFS(GLuint& program, const string& filename)
{
    string shaderSource;

    if (!ReadDataFromFile(filename, shaderSource))
    {
        cout << "Cannot find file name: " + filename << endl;
        exit(-1);
    }

    GLint length = shaderSource.length();
    const GLchar* shader = (const GLchar*) shaderSource.c_str();

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shader, &length);
    glCompileShader(fs);

    char output[1024] = {0};
    glGetShaderInfoLog(fs, 1024, &length, output);
    printf("FS compile log: %s\n", output);

    glAttachShader(program, fs);
}

bool isShaderLinked(int programID)
{
    GLint linkStatus;
    glGetProgramiv(gProgram[programID], GL_LINK_STATUS, &linkStatus);
    return linkStatus == GL_TRUE;
}


void initShaders()
{
    gProgram[0] = glCreateProgram();
    gProgram[1] = glCreateProgram();
    gProgram[2] = glCreateProgram();
    gProgram[3] = glCreateProgram();
    gProgram[4] = glCreateProgram();

    createVS(gProgram[1], "vert0.glsl");
    createFS(gProgram[1], "frag0.glsl");

    createVS(gProgram[0], "vert1.glsl");
    createFS(gProgram[0], "frag1.glsl");

    createVS(gProgram[2], "vert_text.glsl");
    createFS(gProgram[2], "frag_text.glsl");

    createVS(gProgram[3], "vert2.glsl");
    createFS(gProgram[3], "frag2.glsl");

    createVS(gProgram[4], "vert3.glsl");
    createFS(gProgram[4], "frag3.glsl");

    glBindAttribLocation(gProgram[0], 0, "inVertex");
    glBindAttribLocation(gProgram[0], 1, "inNormal");
    glBindAttribLocation(gProgram[1], 0, "inVertex");
    glBindAttribLocation(gProgram[1], 1, "inNormal");

    glBindAttribLocation(gProgram[2], 4, "vertex");

    glBindAttribLocation(gProgram[3], 0, "inVertex");
    glBindAttribLocation(gProgram[3], 1, "inNormal");
    glBindAttribLocation(gProgram[4], 0, "inVertex");
    glBindAttribLocation(gProgram[4], 1, "inNormal");

    for(int i = 0; i < 5; i++)
    {
        glLinkProgram(gProgram[i]);
        if (!isShaderLinked(i))
        {
            cout << "Shader cannot link: " << i <<endl;
            exit(-1);
        }
    }

    glUseProgram(gProgram[0]);

    gIntensityLoc = glGetUniformLocation(gProgram[0], "intensity");
    cout << "gIntensityLoc = " << gIntensityLoc << endl;
    glUniform1f(gIntensityLoc, gIntensity);
}

void initVBO()
{
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);


    assert(glGetError() == GL_NONE);

    glGenBuffers(1, &gVertexAttribBuffer);
    glGenBuffers(1, &gIndexBuffer);

    assert(gVertexAttribBuffer > 0 && gIndexBuffer > 0);

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    gVertexDataSizeInBytes = gVertices.size() * 3 * sizeof(GLfloat);
    gNormalDataSizeInBytes = gNormals.size() * 3 * sizeof(GLfloat);
    int indexDataSizeInBytes = gFaces.size() * 3 * sizeof(GLuint);
    GLfloat* vertexData = new GLfloat [gVertices.size() * 3];
    GLfloat* normalData = new GLfloat [gNormals.size() * 3];
    GLuint* indexData = new GLuint [gFaces.size() * 3];

    float minX = 1e6, maxX = -1e6;
    float minY = 1e6, maxY = -1e6;
    float minZ = 1e6, maxZ = -1e6;

    for (int i = 0; i < gVertices.size(); ++i)
    {
        vertexData[3*i] = gVertices[i].x;
        vertexData[3*i+1] = gVertices[i].y;
        vertexData[3*i+2] = gVertices[i].z;

        minX = std::min(minX, gVertices[i].x);
        maxX = std::max(maxX, gVertices[i].x);
        minY = std::min(minY, gVertices[i].y);
        maxY = std::max(maxY, gVertices[i].y);
        minZ = std::min(minZ, gVertices[i].z);
        maxZ = std::max(maxZ, gVertices[i].z);
    }

    std::cout << "minX = " << minX << std::endl;
    std::cout << "maxX = " << maxX << std::endl;
    std::cout << "minY = " << minY << std::endl;
    std::cout << "maxY = " << maxY << std::endl;
    std::cout << "minZ = " << minZ << std::endl;
    std::cout << "maxZ = " << maxZ << std::endl;

    for (int i = 0; i < gNormals.size(); ++i)
    {
        normalData[3*i] = gNormals[i].x;
        normalData[3*i+1] = gNormals[i].y;
        normalData[3*i+2] = gNormals[i].z;
    }

    for (int i = 0; i < gFaces.size(); ++i)
    {
        indexData[3*i] = gFaces[i].vIndex[0];
        indexData[3*i+1] = gFaces[i].vIndex[1];
        indexData[3*i+2] = gFaces[i].vIndex[2];
    }


    glBufferData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes + gNormalDataSizeInBytes, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertexDataSizeInBytes, vertexData);
    glBufferSubData(GL_ARRAY_BUFFER, gVertexDataSizeInBytes, gNormalDataSizeInBytes, normalData);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSizeInBytes, indexData, GL_STATIC_DRAW);

    // done copying; can free now
    delete[] vertexData;
    delete[] normalData;
    delete[] indexData;

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes + gNormalDataSizeInBytes));
}

void initFonts(int windowWidth, int windowHeight)
{
    // Set OpenGL options
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(windowWidth), 0.0f, static_cast<GLfloat>(windowHeight));
    glUseProgram(gProgram[2]);
    glUniformMatrix4fv(glGetUniformLocation(gProgram[2], "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    // Load font as face
    FT_Face face;
    if (FT_New_Face(ft, "/usr/share/fonts/truetype/liberation/LiberationSerif-Regular.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }

    // Set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    //
    // Configure VBO for texture quads
    //
    glGenBuffers(1, &gTextVBO);
    glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init() 
{
	ParseObj("bunny.obj");
    ParseObj("quad.obj");
    ParseObj("cube.obj");
    ParseObj("cube.obj");
    // for(int i = 0; i < 4; i++){
    //     cout << allSizes[i] << endl;
    //     cout << faceSizesHelper[i] << endl;
    // }

    glEnable(GL_DEPTH_TEST);
    initShaders();
    initFonts(gWidth, gHeight);
    initVBO();
}


void drawModel(int index)
{
    //glUseProgram(gProgram[index]);
    int offset = 0;
    if(index != 0)
    {
        offset = allSizes[index - 1];
    }

    glBindBuffer(GL_ARRAY_BUFFER, gVertexAttribBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIndexBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(offset));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes + offset));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(gVertexDataSizeInBytes + gNormalDataSizeInBytes + offset));

    int yapma = 0;
    if(index != 0)
    {
        yapma = faceSizesHelper[index - 1];
    }
    int lol = faceSizesHelper[index] - yapma;

    glDrawElements(GL_TRIANGLES, lol * 3, GL_UNSIGNED_INT, BUFFER_OFFSET(yapma * 3 * sizeof(GLuint)));
}

void renderText(const std::string& text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state	
    glUseProgram(gProgram[2]);
    glUniform3f(glGetUniformLocation(gProgram[2], "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };

        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, gTextVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        //glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)

        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

std::vector<int> randomInt()
{
    int x = rand() % 3;
    int y = rand() % 3;
    while (y == x)
    {
        y = rand() % 3;
    }
    int z = rand() % 3;
    while (z == x || z == y)
    {
        z = rand() % 3;
    }
    return std::vector<int> {x, y, z};
}
glm::vec3 coll = glm::vec3(1, 1, 0);
// utility functions

void restart ()
{
    roadGone = 0.0f;
    speed = 0.3f;
    incrementSpeed = 0.00005f;

    eyePos = glm::vec3(0, 1, 9);
    lookAtPoint = glm::vec3(0, 1, -200);
    //isRPressed = false;

    handleRoadSwitch = 0.0f;
    firstRoad = 0;
    secondRoad = 0;

    checkPointHold = 0;
    jumpGone = 0.0f;

    bunnyX = 0.0f;
    bunnyY = 0.0f;
    isBunnyRising = true;
    
    randVec = randomInt();
    bunnyDead = 0;
    bunnyDeadBy = -1;

    score = 0;
    coll = glm::vec3(1, 1, 0);
}



void gameOver()
{
    speed = 0.0f;
    incrementSpeed = 0.0f;
    bunnyY = 0.2f;
    bunnyDead = 90;
    coll = glm::vec3(1, 0, 0);
}

void bunnyYellow()
{
    bunnyDance+=speed * 40;
}



void display()
{

    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    	// road switch
	handleRoadSwitch += speed;

	if(handleRoadSwitch > 210.0f){
		if(firstRoad == secondRoad){
			firstRoad += 1;
		}
		else{
			secondRoad += 1;
		}
		handleRoadSwitch -= 200.0f;
	}

	// Bunny Dance
	if(bunnyDance >= 360 || bunnyDance == 0){
		bunnyDance = 0;
	}
	else{
		bunnyYellow();
	}

	// Keyboard handling
	if(isRPressed){
		restart();
	}
	if(isAPressed){
		bunnyX -= speed / 2.5f;
		bunnyX = bunnyX < -4.0f ? -4.0f : bunnyX;
	}
	if(isDPressed){
		bunnyX += speed / 2.5f;
		bunnyX = bunnyX > 4.0f ? 4.0f : bunnyX;
	}
	if(bunnyY >= 0.75f){
		bunnyY -= speed / 3.f;
		isBunnyRising = false;
	}
	else if(bunnyY <= 0.0f){
		bunnyY += speed / 3.f;
		isBunnyRising = true;
	}
	else{
		if(isBunnyRising){
			bunnyY += speed / 3.f;
		}
		else{
			bunnyY -= speed / 3.f;
		}
	}

  	float angleDeg = -90.0f;  // Adjust this value as needed
	float angleRad = glm::radians(angleDeg);

    // Infinite road use 2 ground objects and translate them
    // to create the illusion of infinite road
    activeProgramIndex = 1;
    glUseProgram(gProgram[activeProgramIndex]);
    // Ground 1

    // Create transformation matrices
	glm::mat4 matT = glm::translate(glm::mat4(1.0), glm::vec3(0.f, -1.f, 5.f));
	glm::mat4 matS = glm::scale(glm::mat4(1.0), glm::vec3(5.0, 100.0, 1.0));
	glm::mat4 matR = glm::rotate(glm::mat4(1.0), angleRad, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 chipiChipi = glm::translate(glm::mat4(1.0), glm::vec3(0.f , (firstRoad * 400.f + 98.f), 0.f));

	chipiChipi = matR * chipiChipi * glm::inverse(matR);
	
	glm::vec4 addThis =  glm::vec4(0, speed, 0, 1);

	// Combine the transformation matrices
	modelingMatrix = chipiChipi * matT * matR * matS;
	

	// Apply the ground's transformation to the camera's position
	addThis = matR * addThis;
	
	eyePos += glm::vec3(addThis);
	lookAtPoint += glm::vec3(addThis);

	viewingMatrix = glm::lookAt(eyePos, lookAtPoint, glm::vec3(0, 1, 0));

    glm::mat4 modelMat;
    glm::mat4 modelMatInv = glm::transpose(glm::inverse(modelingMatrix));

    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "viewingMat"), 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniform3fv(glGetUniformLocation(gProgram[activeProgramIndex], "eyePos"), 1, glm::value_ptr(eyePos));
    drawModel(activeProgramIndex);

    // Ground 2

	// Create transformation matrices
	matT = glm::translate(glm::mat4(1.0), glm::vec3(0.f, -1.f, 5.f));
	matS = glm::scale(glm::mat4(1.0), glm::vec3(5.0, 100.0, 1.0));
	matR = glm::rotate(glm::mat4(1.0), angleRad, glm::vec3(1.0, 0.0, 0.0));

	chipiChipi = glm::translate(glm::mat4(1.0), glm::vec3(0.f , (secondRoad * 400.f + 298.f), 0.f));

	chipiChipi = matR * chipiChipi * glm::inverse(matR);
	// Combine the transformation matrices
	modelingMatrix = chipiChipi * matT * matR * matS;

	glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "viewingMat"), 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniform3fv(glGetUniformLocation(gProgram[activeProgramIndex], "eyePos"), 1, glm::value_ptr(eyePos));
    drawModel(activeProgramIndex);

    // Bunny

    activeProgramIndex = 0;
    glUseProgram(gProgram[activeProgramIndex]);

	jumpGone += speed;
	
	angleRad = (float)(90 / 180.0) * M_PI;

	chipiChipi = glm::translate(glm::mat4(1.0), glm::vec3(0.f , (0), 0.f));
	chipiChipi = matR * chipiChipi * glm::inverse(matR);

	matT = glm::translate(glm::mat4(1.0), glm::vec3(0.f, -1.9f, -4.f));
	matS = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
	matR = glm::rotate<float>(glm::mat4(1.0), angleRad, glm::vec3(0.0, -1.0, 0.0));
	
	// Tavşanım sağa sola tavşanım zıpla tavşanım
	glm::mat4 matT2 = glm::translate(glm::mat4(1.0), glm::vec3(bunnyX, bunnyY, 0.f));

	// Tavşan bayıldı
	glm::mat4 matRr = glm::rotate<float>(glm::mat4(1.0), glm::radians(float(bunnyDead)), glm::vec3(-1.0, 0.0, 0.0));

	// Turn the bunny around
	glm::mat4 matRfull = glm::rotate<float>(glm::mat4(1.0), glm::radians(float(bunnyDance)), glm::vec3(0.0, 1.0, 0.0));

	modelingMatrix = matT2 * chipiChipi * matT * matS *  matR * matRr * matRfull;

    modelMatInv = glm::transpose(glm::inverse(modelingMatrix));

    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "viewingMat"), 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniform3fv(glGetUniformLocation(gProgram[activeProgramIndex], "eyePos"), 1, glm::value_ptr(eyePos));
    drawModel(activeProgramIndex);

    activeProgramIndex = 3;
    glUseProgram(gProgram[activeProgramIndex]);

    // CHECKPOINT HERE AFTER HERE WHEN CALLING DRAW MODEL USE (activeProgramIndex - 1)
    // ONEMLI
    int lol = 0;
    if (bunnyDeadBy == randVec[0])
    {
        lol = 100;
    }

	chipiChipi = glm::translate(glm::mat4(1.0), glm::vec3(-3.1f + (randVec[0] * 3.1) + lol , 0, -50 * (checkPointHold + 1)));


    matS = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 1, 0.5));
	chipiChipi =  chipiChipi * matS ;
	// Combine the transformation matrices
	modelingMatrix = chipiChipi;

    modelMatInv = glm::transpose(glm::inverse(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "viewingMat"), 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniform3fv(glGetUniformLocation(gProgram[activeProgramIndex], "eyePos"), 1, glm::value_ptr(eyePos));
    drawModel(activeProgramIndex - 1);

    matR = glm::rotate(glm::mat4(1.0), glm::radians(-80.f), glm::vec3(1.0, 0.0, 0.0));
    activeProgramIndex = 4;
    glUseProgram(gProgram[activeProgramIndex]);
    // Checkpoint 1

    
    lol = 0;
    if (bunnyDeadBy == randVec[1])
    {
        lol = 100;
    }
	chipiChipi = glm::translate(glm::mat4(1.0), glm::vec3(-3.1f + (randVec[1] * 3.1) + lol , 0, -50 * (checkPointHold + 1)));
    matS = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 1, 0.5));
	chipiChipi =  chipiChipi * matS ;
	// Combine the transformation matrices
	modelingMatrix = chipiChipi;

    modelMatInv = glm::transpose(glm::inverse(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "viewingMat"), 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniform3fv(glGetUniformLocation(gProgram[activeProgramIndex], "eyePos"), 1, glm::value_ptr(eyePos));
    drawModel(activeProgramIndex - 1);

    // CHECKPOINT HERE AFTER HERE WHEN CALLING DRAW MODEL USE (activeProgramIndex - 1)
    // ONEMLI

    lol = 0;
    if (bunnyDeadBy == randVec[2])
    {
        lol = 100;
    }

	chipiChipi = glm::translate(glm::mat4(1.0), glm::vec3(-3.1f + (randVec[2] * 3.1) + lol , 0, -50 * (checkPointHold + 1)));
    matS = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 1, 0.5));
	chipiChipi =  chipiChipi * matS ;
	// Combine the transformation matrices
	modelingMatrix = chipiChipi;

    modelMatInv = glm::transpose(glm::inverse(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMat"), 1, GL_FALSE, glm::value_ptr(modelingMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "modelingMatInvTr"), 1, GL_FALSE, glm::value_ptr(modelMatInv));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "perspectiveMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(glGetUniformLocation(gProgram[activeProgramIndex], "viewingMat"), 1, GL_FALSE, glm::value_ptr(viewingMatrix));
    glUniform3fv(glGetUniformLocation(gProgram[activeProgramIndex], "eyePos"), 1, glm::value_ptr(eyePos));
    drawModel(activeProgramIndex - 1);


    // Handle repetitive checkpoints
	// Also checkpoints do calculations for the score
	if(roadGone > checkPointHold * 50.f + 54.f){
        bool isHit = false;
		
		if((bunnyX > -2.f && bunnyX < -1.f) || (bunnyX < 2.f && bunnyX > 1.f)){
			
		}
		else{
			if(randVec[0] == 0){
				if(bunnyX <= -2.f){
                    isHit = true;
					bunnyYellow();
				}
				else{
                    if(bunnyX >= -1.f && bunnyX <= 1.f)
                    {
                        bunnyDeadBy = 1;
                    }
                    else{
                        bunnyDeadBy = 2;
                    }
                    gameOver();

				}
			}
			else if(randVec[0] == 1){
				if(bunnyX >= -1.f && bunnyX <= 1.f){
                    isHit = true;
					bunnyYellow();
				}
				else{
                    if(bunnyX <= -2.f)
                    {
                        bunnyDeadBy = 0;
                    }
                    else{
                        bunnyDeadBy = 2;
                    }
                    gameOver();
				}
			}
			else{
				if(bunnyX >= 2.f){
                    isHit = true;
					bunnyYellow();
				}
				else{
                    if(bunnyX >= -1.f && bunnyX <= 1.f)
                    {
                        bunnyDeadBy = 1;
                    }
                    else{
                        bunnyDeadBy = 0;
                    }
                    gameOver();
				}
			}
		}
		if(incrementSpeed == 0.0f){
			//cout << "Game Over" << endl;
		}
		else
		{checkPointHold += 1;
        if (isHit)
        {
            score += 1000;
        }
		randVec = randomInt();}
	}
    roadGone += speed;
	speed += incrementSpeed;
    score += (unsigned long)(speed * 10);
    string scoreStr = "Score: ";

    renderText(scoreStr + std::to_string(score - 3), 0, 670, 1, coll);

    assert(glGetError() == GL_NO_ERROR);

}

void reshape(GLFWwindow* window, int w, int h)
{
	w = w < 1 ? 1 : w;
	h = h < 1 ? 1 : h;

	gWidth = w;
	gHeight = h;

	glViewport(0, 0, w, h);

	// Use perspective projection
	float fovyRad = (float)(90.0 / 180.0) * M_PI;
	projectionMatrix = glm::perspective(fovyRad, w / (float)h, 0.1f, 200.0f);

	// Assume default camera position and orientation (camera is at
	// (0, 0, 0) with looking at -z direction and its up vector pointing
	// at +y direction)
	// 
	//viewingMatrix = glm::mat4(1);
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_R)
	{
        if(action == GLFW_PRESS)
		{
			isRPressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			isRPressed = false;
		}
	}
	if(key == GLFW_KEY_A)
	{
		if(action == GLFW_PRESS)
		{
			isAPressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			isAPressed = false;
		}
	}
	if(key == GLFW_KEY_D)
	{
		if(action == GLFW_PRESS)
		{
			isDPressed = true;
		}
		else if(action == GLFW_RELEASE)
		{
			isDPressed = false;
		}
	}
    // if(key == GLFW_KEY_K)
    // {
    //     if(action == GLFW_PRESS)
    //     {
    //         speed = 0;
    //         incrementSpeed = 0;
    //     }
    //     else{
    //         speed = 0.3f;
    //         incrementSpeed = 0.0001f;
    //     }
    // }

}

void mainLoop(GLFWwindow* window)
{
    while (!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char** argv)   // Create Main Function For Bringing It All Together
{
    GLFWwindow* window;
    if (!glfwInit())
    {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(gWidth, gHeight, "Simple Example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    char rendererInfo[512] = {0};
    strcpy(rendererInfo, (const char*) glGetString(GL_RENDERER));
    strcat(rendererInfo, " - ");
    strcat(rendererInfo, (const char*) glGetString(GL_VERSION));
    glfwSetWindowTitle(window, rendererInfo);

    init();

    glfwSetKeyCallback(window, keyboard);
    glfwSetWindowSizeCallback(window, reshape);

    reshape(window, gWidth, gHeight); // need to call this once ourselves
    mainLoop(window); // this does not return unless the window is closed

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

