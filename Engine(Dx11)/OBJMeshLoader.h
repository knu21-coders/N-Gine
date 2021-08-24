#pragma once
#include "framework.h"

/*

	������ ������
	http://www.opengl-tutorial.org/kr/beginners-tutorials/tutorial-7-model-loading/
	����&�н� �뵵 ���/ ���� ����, �����μ� ������ ����, �񿵸� (for study)

*/


using namespace std;

struct Vector3 {
	float x, y, z;
};
struct Vector2 {
	float u,v;
};
struct Mesh {
	//Vertex - Index
	//VertexTextur- Index
	//VertexNormal - Index
};


char** returnSplit(const char* line, char seperator, int count) {
	int line_index = 0;

	int token_index = 0;
	char** chrResult = new char* [count];

	int tmp_index = 0;
	char tmp_token[10] = { 0 };

	while (token_index != count) {

		char chr = line[line_index];

		if (chr == seperator || chr == '\0') {
			//Break;
			tmp_token[tmp_index] = '\0';

			tmp_index = 0;

			char* ind = new char[10];
			strcpy_s(ind, 10, tmp_token);
			chrResult[token_index] = ind;


			token_index++;

		}
		else {
			tmp_token[tmp_index] = chr;
			tmp_index++;
		}
		//chr ����
		line_index++;
	}
	return chrResult;
}


void ReadOBJ(const char* file_name, vector<Vector3> & out_vertices,
	vector<Vector2>& out_uvs,
	vector<Vector3>& out_normals) {
	ifstream fin(file_name);
	
	if(fin.fail()){
		std::cout << "Error" << std::endl;
		return;
		//Error
	}
	char _line[40];
	char _head = '0';
	while (!fin.eof())    //���� ������ �о����� Ȯ��
	{
		fin.getline(_line, 40);    //���پ� �о����
		_head = _line[0]; // ��ù���� _head������
		switch (_head) {
		case 'v':
			float v[3];
			char**_tokens = returnSplit(_line, ' ', 4);
			for (int i = 1; i < 4; i++) v[i - 1] = atof(_tokens[i]);
			//TODO
			for (int i = 0; i < 4; i++) delete[] _tokens[i];
			delete[] _tokens;
			break;
		case 'vn':
			float vn[3];
			char**_tokens = returnSplit(_line, ' ', 4);
			for (int i = 1; i < 4; i++) vn[i - 1] = atof(_tokens[i]);
			//TODO
			for (int i = 0; i < 4; i++) delete[] _tokens[i];
			delete[] _tokens;
			break;

		case 'vt':
			float vt[2];

			char**_tokens = returnSplit(_line, ' ', 3);
			for (int i = 1; i < 3; i++) v[i - 1] = atof(_tokens[i]);
			//TODO
			for (int i = 0; i < 3; i++) delete[] _tokens[i];
			delete[] _tokens;
			break;
		case 'f':
			UINT f[3][3];
			
			/*
				float _vi[4];
				char**_tokens = returnSplit(_line, ' ', 3);
			*/

			break;
		default:
			break;
		}
	}


	fin.close();
	UINT vertex_count = 0, face_count = 0, 
}

