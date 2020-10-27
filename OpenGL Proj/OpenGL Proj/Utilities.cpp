#include "Utilities.h"
#include "RenderMesh.h"
#include <vector>

BitMap* BitMapReader(std::string filename)
{
	int offset, headerSize;

	// Initialize bitmap files for RGB (input) and RGBA (output).
	BitMap* bmpRGB = new BitMap;
	BitMap* bmpRGBA = new BitMap;

	// Read input bmp file name.
	std::ifstream infile(filename.c_str(), std::ios::binary);

	// Get starting point of image data in bmp file.
	infile.seekg(10);
	infile.read((char*)& offset, 4);

	// Get header size of bmp file.
	infile.read((char*)& headerSize, 4);

	// Get image width and height values from bmp file header.
	infile.seekg(18);
	infile.read((char*)& bmpRGB->size_x, 4);
	infile.read((char*)& bmpRGB->size_y, 4);

	// Determine the length of zero-byte padding of the scanlines 
	// (each scanline of a bmp file is 4-byte aligned by padding with zeros).
	int padding = (3 * bmpRGB->size_x) % 4 ? 4 - (3 * bmpRGB->size_x) % 4 : 0;

	// Add the padding to determine size of each scanline.
	int sizeScanline = 3 * bmpRGB->size_x + padding;

	// Allocate storage for image in input bitmap file.
	int sizeStorage = sizeScanline * bmpRGB->size_y;
	bmpRGB->data = new unsigned char[sizeStorage];

	// Read bmp file image data into input bitmap file.
	infile.seekg(offset);
	infile.read((char*)bmpRGB->data, sizeStorage);

	// Reverse color values from BGR (bmp storage format) to RGB.
	int startScanline, endScanlineImageData, temp;
	for (int y = 0; y < bmpRGB->size_y; y++)
	{
		startScanline = y * sizeScanline; // Start position of y'th scanline.
		endScanlineImageData = startScanline + 3 * bmpRGB->size_x; // Image data excludes padding.
		for (int x = startScanline; x < endScanlineImageData; x += 3)
		{
			temp = bmpRGB->data[x];
			bmpRGB->data[x] = bmpRGB->data[x + 2];
			bmpRGB->data[x + 2] = temp;
		}
	}

	// Set image width and height values and allocate storage for image in output bitmap file.
	bmpRGBA->size_x = bmpRGB->size_x;
	bmpRGBA->size_y = bmpRGB->size_y;
	bmpRGBA->data = new unsigned char[4 * bmpRGB->size_x * bmpRGB->size_y];

	// Copy RGB data from input to output bitmap files, set output A to 1.
	for (int j = 0; j < 4 * bmpRGB->size_y * bmpRGB->size_x; j += 4)
	{
		bmpRGBA->data[j] = bmpRGB->data[(j / 4) * 3];
		bmpRGBA->data[j + 1] = bmpRGB->data[(j / 4) * 3 + 1];
		bmpRGBA->data[j + 2] = bmpRGB->data[(j / 4) * 3 + 2];
		bmpRGBA->data[j + 3] = 0xFF;
	}

	return bmpRGBA;
}

char* ParseFile(const char* text_file)
{
	FILE* filePointer;
	fopen_s(&filePointer,text_file, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);

	return content;
}

void shaderCompileTest(GLuint shader)
{
	GLint result = GL_FALSE;
	int logLength; 
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(shader, logLength, NULL, &vertShaderError[0]);
	std::cout << &vertShaderError[0] << std::endl;
}

RenderMesh* OBJReader(std::string path)
{
	std::ifstream file;
	file.open(path);
	std::string str;

	RenderMesh* mesh = new RenderMesh();

	std::vector<Vertex> vertex_data = (*new std::vector<Vertex>);

	std::vector<std::vector<float>> stored_vertices;
	std::vector<std::vector<float>> stored_normals;
	std::vector<std::vector<float>> stored_texture;
	std::vector<std::vector<int>> stored_faces;

	int vertex_count = 0;
	int texture_count = 0;
	int normal_count = 0;
	int face_count = 0;

	while (!file.eof()) //while we are still in the file
	{

		std::getline(file, str);

		while (str[0] == 'v' && str[1] == ' ')
		{
			std::vector<float> set = { 1.0f, 1.0f, 1.0f, 1.0f };
			stored_vertices.emplace_back(set);
			str[0] = ' ';
			sscanf_s(str.c_str(), "%f %f %f", &stored_vertices[vertex_count][0], &stored_vertices[vertex_count][1], &stored_vertices[vertex_count][2]);
			std::getline(file, str);
			stored_vertices[vertex_count][3] = 1.0f;
			vertex_count++;
		}

		while (str[0] == 'v' && str[1] == 't') 
		{
			std::vector<float> set = { 1.0f, 1.0f};
			stored_texture.emplace_back(set);
			str[0] = ' '; str[1] = ' ';
			sscanf_s(str.c_str(), "%f %f", &stored_texture[texture_count][0], &stored_texture[texture_count][1]);
			str = ' ';
			texture_count++;
			std::getline(file, str);
		}

		while (str[0] == 'v' && str[1] == 'n')
		{
			std::vector<float> set = { 1.0f, 1.0f, 1.0f };
			stored_normals.emplace_back(set);
			str[0] = ' '; str[1] = ' ';
			sscanf_s(str.c_str(), "%f %f %f", &stored_normals[normal_count][0], &stored_normals[normal_count][1], &stored_normals[normal_count][2]);
			normal_count++;
			std::getline(file, str);
		}

		while (str[0] == 'f' && str[1] == ' ')
		{
			std::vector<int> set = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
			stored_faces.emplace_back(set);
			str[0] = ' ';
			sscanf_s(str.c_str(), "%d/%d/%d %d/%d/%d %d/%d/%d", 
				&stored_faces[face_count][0], &stored_faces[face_count][1], &stored_faces[face_count][2],
				&stored_faces[face_count][3], &stored_faces[face_count][4], &stored_faces[face_count][5], 
				&stored_faces[face_count][6], &stored_faces[face_count][7], &stored_faces[face_count][8]);
			face_count++;
			std::getline(file, str);
		}
	}

	int j = 0;

	for (int i = 0; i < 3 * stored_faces.size(); i += 3)
	{

		vertex_data.emplace_back(Vertex(stored_vertices[stored_faces[j][0]-1], stored_normals[stored_faces[j][2]-1], stored_texture[stored_faces[j][1]-1]));
		vertex_data.emplace_back(Vertex(stored_vertices[stored_faces[j][3]-1], stored_normals[stored_faces[j][5]-1], stored_texture[stored_faces[j][4]-1]));
		vertex_data.emplace_back(Vertex(stored_vertices[stored_faces[j][6]-1], stored_normals[stored_faces[j][8]-1], stored_texture[stored_faces[j][7]-1]));
		j++;
		/*vertex_data[i + 1].coordinates = stored_vertices[stored_faces[j][3]];
		vertex_data[i + 2].coordinates = stored_vertices[stored_faces[j][6]];

		vertex_data[i].normals = stored_normals[stored_faces[j][1]];
		vertex_data[i + 1].normals = stored_normals[stored_faces[j][4]];
		vertex_data[i + 2].normals = stored_normals[stored_faces[j][7]];

		vertex_data[i].texture_coordinates = stored_texture[stored_faces[j][2]];
		vertex_data[i + 1].texture_coordinates = stored_texture[stored_faces[j][5]];
		vertex_data[i + 2].texture_coordinates = stored_texture[stored_faces[j][8]];*/
	}

	mesh->vertices = vertex_data;
	file.close();
	return mesh;
}
