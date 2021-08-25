#pragma once
#include "framework.h"
#define MAX_STRING_CHAR_COUNT 100
#define MAX_TOKEN_CHAR_COUNT 10
/*

	다음을 참고함
	http://www.opengl-tutorial.org/kr/beginners-tutorials/tutorial-7-model-loading/
	공부&학습 용도 사용/ 개인 목적, 업으로서 사용되지 않음, 비영리 (for study)

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


char** returnSplit(const char* line, char seperator, unsigned int* return_count) {
	int line_index = 0;

	int token_index = 0;
	int _count = 0;
	while (true) {
		char chr = line[line_index];
		if (chr == seperator || chr == '\0') _count++;
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
		//chr 적용
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
	

	unsigned int tmp_token_count = 0;
	char* tmp_head = nullptr;
	char** _tokens = nullptr;
	char** tmp_indicies = nullptr;
	unsigned int tmp_indicies_count = 0;
	while (!fin.eof())    //파일 끝까지 읽었는지 확인
	{
		fin.getline(_line, MAX_STRING_CHAR_COUNT);    //한줄씩 읽어오기

		tmp_token_count = 0;
		_tokens = returnSplit(_line, ' ', &tmp_token_count);
		tmp_head = _tokens[0];
		if (strcmp(tmp_head, "v") == 0) { //어떤 경우에도 룩업테이블로 유도해야댐
			float v[3];
			for (int i = 1; i <= 3; i++) v[i - 1] = atof(_tokens[i]);
		}
		else if (strcmp(tmp_head, "vn") == 0) {
			float vn[3];
			for (int i = 1; i <= 3; i++) vn[i - 1] = atof(_tokens[i]);
		}
		else if (strcmp(tmp_head, "vt") == 0) {
			float vt[3];
			for (int i = 1; i <= 3; i++) vt[i - 1] = atof(_tokens[i]);
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
					cout << "블렌더 Export세팅을 통해 삼각형 메시로 변환하세요(Triangulate)!" << endl;
				}
			}
		}
		else if (strcmp(tmp_head, "o") == 0) {
			//오브젝트 이름 등록
			char object_name[100];
			strcpy_s(object_name,100,_tokens[1]);
		}
		else if (strcmp(tmp_head, "g") == 0) {
			char group_name[MAX_STRING_CHAR_COUNT];
			strcpy_s(group_name, MAX_STRING_CHAR_COUNT, _tokens[1]);
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
		}
		else if (strcmp(tmp_head, "usemtl") == 0) {
			char material_name[MAX_STRING_CHAR_COUNT];
			strcpy_s(material_name, MAX_STRING_CHAR_COUNT, _tokens[1]);
		}
		else {
			cout << "Warning: 렌더링 불가능한 옵션이 포함되었습니다!" << endl;
		}

		//토큰 동적 배열 할당해제
		for (int i = 0; i < tmp_token_count; i++) delete[] _tokens[i];
		delete[] _tokens;

	}


	fin.close();
	UINT vertex_count = 0, face_count = 0;
}

