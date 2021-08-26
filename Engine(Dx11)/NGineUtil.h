#pragma once
#include "framework.h"
#define MAX_STRING_CHAR_COUNT 100
#define MAX_TOKEN_CHAR_COUNT 10
using namespace DirectX;
/*

	������ ������
	http://www.opengl-tutorial.org/kr/beginners-tutorials/tutorial-7-model-loading/
	����&�н� �뵵 ���/ ���� ����, �����μ� ������ ����, �񿵸� (for study)

*/


using namespace std;

struct Vector3 {

struct OBJMesh {
	XMFLOAT3 *m_vertexNormal, *m_vertexPos;
	XMFLOAT2 m_vertexTexture;
	//const char* object_name, group_name, 
	
};


char** returnSplit(const char* line, char seperator, unsigned int* return_count) {
	int line_index = 0;

	int token_index = 0;
	int _count = 0;
	while (true) {
		char chr = line[line_index];
		if (chr == seperator) _count++;
		else if (chr == '\0') {
			_count++;
			break;
		}
		line_index++;
	}
	line_index = 0;
	char** chrResult = new char* [_count];

	int tmp_index = 0;
	char tmp_token[MAX_TOKEN_CHAR_COUNT] = { 0 };

	while (token_index != _count) {

		char chr = line[line_index];

		if (chr == seperator || chr == '\0') {
			//Break;
			tmp_token[tmp_index] = '\0';

			tmp_index = 0;

			char* ind = new char[MAX_TOKEN_CHAR_COUNT];
			strcpy_s(ind, MAX_TOKEN_CHAR_COUNT, tmp_token);
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
	*return_count = _count;
	return chrResult;
}


void ReadOBJ(const char* file_name) {
	ifstream fin(file_name);

	if (fin.fail()) {
		std::cout << "������ ������ �ʽ��ϴ�!" << std::endl;
		return;
		//Error
	}

	char _line[MAX_STRING_CHAR_COUNT];

	unsigned int tmp_token_count = 0;
	char* tmp_head = nullptr;
	char** _tokens = nullptr;
	char** tmp_indicies = nullptr;
	unsigned int tmp_indicies_count = 0;

	bool is_contained_unsuported = false;
	while (!fin.eof())    //���� ������ �о����� Ȯ��
	{
		fin.getline(_line, MAX_STRING_CHAR_COUNT);    //���پ� �о����
		if (_line[0] == '#') cout << _line << endl;
		else {
			_tokens = returnSplit(_line, ' ', &tmp_token_count);
			tmp_head = _tokens[0];

			if (strcmp(tmp_head, "v") == 0) { //� ��쿡�� ������̺�� �����ؾߴ�
				float v[3];
				for (int i = 1; i <= 3; i++) v[i - 1] = atof(_tokens[i]);
				cout << "Vertex: " << v[0] << " " << v[1] << " " << v[2] << " " << endl;
			}
			else if (strcmp(tmp_head, "vn") == 0) {
				float vn[3];
				for (int i = 1; i <= 3; i++) vn[i - 1] = atof(_tokens[i]);
				cout << "VertexNormal: " << vn[0] << " " << vn[1] << " " << vn[2] << " " << endl;
			}
			else if (strcmp(tmp_head, "vt") == 0) {
				float vt[2];
				for (int i = 1; i <= 2; i++) vt[i - 1] = atof(_tokens[i]);
				cout << "VertexTexture: " << vt[0] << " " << vt[1] << endl;
			}
			else if (strcmp(tmp_head, "f") == 0) {
				unsigned int vIndicies[3], vtIndicies[3], vnIndicies[3];

				for (int i = 1; i <= 3; i++) {
					tmp_indicies = returnSplit(_tokens[i], '/', &tmp_indicies_count);
					if (tmp_indicies_count == 3) {
						vIndicies[i - 1] = atoi(tmp_indicies[0]);
						vtIndicies[i - 1] = atoi(tmp_indicies[1]);
						vnIndicies[i - 1] = atoi(tmp_indicies[2]);
					}
					else {
						cout << "���� Export������ ���� �ﰢ�� �޽÷� ��ȯ�ϼ���(Triangulate)!" << endl;
					}
				}
				cout << "VertexIndicies: " << vIndicies[0] << " " << vIndicies[1] << " " << vIndicies[2] << " " << endl;
				cout << "VertexTextureIndicies: " << vtIndicies[0] << " " << vtIndicies[1] << " " << vtIndicies[2] << " " << endl;
				cout << "VertexNormalIndicies: " << vnIndicies[0] << " " << vnIndicies[1] << " " << vnIndicies[2] << " " << endl;

			}
			else if (strcmp(tmp_head, "o") == 0) {
				//������Ʈ �̸� ���
				char object_name[100];
				strcpy_s(object_name, 100, _tokens[1]);
				cout << "������Ʈ �̸� : " << object_name << endl;
			}
			else if (strcmp(tmp_head, "g") == 0) {
				char group_name[MAX_STRING_CHAR_COUNT];
				strcpy_s(group_name, MAX_STRING_CHAR_COUNT, _tokens[1]);
				cout << "�׷�� : " << group_name << endl;
			}
			else if (strcmp(tmp_head, "s") == 0) {

				bool is_smooth_shaded = false;
				if (strcmp(_tokens[1], "on") == 0) {
					is_smooth_shaded = true;
				}
			}
			else if (strcmp(tmp_head, "mtllib") == 0) {
				char material_file_name[MAX_STRING_CHAR_COUNT];
				strcpy_s(material_file_name, MAX_STRING_CHAR_COUNT, _tokens[1]);
				cout << "��Ƽ���� ���� �̸� : " << material_file_name << endl;
			}
			else if (strcmp(tmp_head, "usemtl") == 0) {
				char material_name[MAX_STRING_CHAR_COUNT];
				strcpy_s(material_name, MAX_STRING_CHAR_COUNT, _tokens[1]);
				cout << "��Ƽ����� : " << material_name << endl;
			}
			else {
				is_contained_unsuported = true;
				cout << "���������� : " << tmp_head << endl;
			}

			//��ū ���� �迭 �Ҵ�����
			for (int i = 0; i < tmp_token_count; i++) delete[] _tokens[i];
			delete[] _tokens;
		}
		if (is_contained_unsuported) cout << "�������� �ʴ� ����� ���ԵǾ��ֽ��ϴ�." << endl;

	}

	fin.close();
}
