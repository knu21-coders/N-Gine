#pragma once
#include "framework.h"
#include "NGineSLL.h"
#include "NGineMesh.h"
#include "NGineString.h"
using namespace std;



void ReadOBJ(const char* file_name, Vertex return_verticies[], unsigned int return_indicies[])
{
	ifstream fin(file_name);

	if (fin.fail()) {
		cout << "파일이 열리지 않습니다!" << endl;
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
	unsigned int vertex_count = 0;
	unsigned int vertex_normal_count = 0;
	unsigned int vertex_texture_count = 0;
	unsigned int face_count = 0;

	XMFLOAT3 vert;
	while (!fin.eof())    //파일 끝까지 읽었는지 확인
	{
		fin.getline(_line, MAX_STRING_CHAR_COUNT);    //한줄씩 읽어오기
		if (_line[0] == '#') cout << _line << endl;
		else {
			_tokens = SplitString(_line, ' ', &tmp_token_count);
			tmp_head = _tokens[0];

			if (strcmp(tmp_head, "v") == 0) vertex_count++;
			else if (strcmp(tmp_head, "vn") == 0) vertex_normal_count++;
			else if (strcmp(tmp_head, "vt") == 0) vertex_texture_count++;
			else if (strcmp(tmp_head, "f") == 0) {
				//unsigned int vIndicies[3], vtIndicies[3], vnIndicies[3];
			}
			else if (strcmp(tmp_head, "o") == 0) {
				//오브젝트 이름 등록
				char object_name[100];
				strcpy_s(object_name, 100, _tokens[1]);
				cout << "오브젝트 이름 : " << object_name << endl;
			}
			else if (strcmp(tmp_head, "g") == 0) {
				char group_name[MAX_STRING_CHAR_COUNT];
				strcpy_s(group_name, MAX_STRING_CHAR_COUNT, _tokens[1]);
				cout << "그룹명 : " << group_name << endl;
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
				cout << "머티리얼 파일 이름 : " << material_file_name << endl;
			}
			else if (strcmp(tmp_head, "usemtl") == 0) {
				char material_name[MAX_STRING_CHAR_COUNT];
				strcpy_s(material_name, MAX_STRING_CHAR_COUNT, _tokens[1]);
				cout << "머티리얼명 : " << material_name << endl;
			}
			else {
				is_contained_unsuported = true;
				cout << "비지원형식 : " << tmp_head << endl;
			}

			//토큰 동적 배열 할당해제
			for (int i = 0; i < tmp_token_count; i++) delete[] _tokens[i];
			delete[] _tokens;
		}

	}

	fin.close();
	fin.open(file_name);

	XMFLOAT3* verticies = new XMFLOAT3[vertex_count];
	XMFLOAT3* vertex_normals = new XMFLOAT3[vertex_normal_count];
	XMFLOAT2* vertex_textures = new XMFLOAT2[vertex_texture_count];
	IntPooingSLL indicies;
	VertexSLL dxmesh_verticies;
	unsigned int vertex_index = 0;
	unsigned int vertex_normal_index = 0;
	unsigned int vertex_texture_index = 0;
	unsigned int face_index = 0;
	while (!fin.eof())    //파일 끝까지 읽었는지 확인
	{
		fin.getline(_line, MAX_STRING_CHAR_COUNT);    //한줄씩 읽어오기
		if (_line[0] != '#') {
			_tokens = SplitString(_line, ' ', &tmp_token_count);
			tmp_head = _tokens[0];

			if (strcmp(tmp_head, "v") == 0) { //어떤 경우에도 룩업테이블로 유도해야댐
				float v[3];
				for (int i = 1; i <= 3; i++) v[i - 1] = atof(_tokens[i]);
				verticies[vertex_index++] = XMFLOAT3(v[0], v[1], v[2]);

			}
			else if (strcmp(tmp_head, "vn") == 0) {
				float vn[3];

				for (int i = 1; i <= 3; i++) vn[i - 1] = atof(_tokens[i]);
				vertex_normals[vertex_normal_index++] = XMFLOAT3(vn[0], vn[1], vn[2]);
				cout << vertex_normals[vertex_normal_index - 1].x << " " << vertex_normals[vertex_normal_index - 1].y << " " << vertex_normals[vertex_normal_index - 1].z << endl;
			}
			else if (strcmp(tmp_head, "vt") == 0) {
				float vt[2];
				for (int i = 1; i <= 2; i++) vt[i - 1] = atof(_tokens[i]);
				vertex_textures[vertex_texture_index++] = XMFLOAT2(vt[0], vt[1]);
			}
			else if (strcmp(tmp_head, "f") == 0) {
				unsigned int vIndicies[3], vtIndicies[3], vnIndicies[3];

				for (int i = 1; i <= 3; i++) {

					tmp_indicies = SplitString(_tokens[i], '/', &tmp_indicies_count);

					if (tmp_indicies_count == 3) {
						vIndicies[i - 1] = atoi(tmp_indicies[0]);
						vtIndicies[i - 1] = atoi(tmp_indicies[1]);
						vnIndicies[i - 1] = atoi(tmp_indicies[2]);
					}
					else {
						std::cout << "블렌더 Export세팅을 통해 삼각형 메시로 변환하세요(Triangulate)!" << endl;
					}
				}

				for (int i = 0; i < 3; i++)
				{
					int vertexindex = vIndicies[i] - 1;
					int normalindex = vnIndicies[i] - 1;
					int textureIndex = vtIndicies[i] - 1;
					Vertex resultData = { verticies[vertexindex], vertex_normals[normalindex], vertex_textures[textureIndex]  };


					int target_index = 0;
					if (dxmesh_verticies.IsContain(resultData, &target_index))
					{
						indicies.Add(target_index);
					}
					else
					{
						indicies.Add(dxmesh_verticies.GetCount());
						dxmesh_verticies.Add(resultData);

					}
				}

			}

			//토큰 동적 배열 할당해제
			for (int i = 0; i < tmp_token_count; i++) delete[] _tokens[i];
			delete[] _tokens;
		}


	}
	if (is_contained_unsuported) cout << "지원하지 않는 양식이 포함되어있습니다." << endl;
	cout << "인덱스 수: " << indicies.GetCount() << endl;
	for (int i = 0; i < indicies.GetCount(); i++) {
		return_indicies[i] = (*indicies[i]);
		cout << *indicies[i];
		if (i % 3 == 2) cout << endl;
		else cout << " ";
	}
	cout << endl;
	cout << "버텍스 수: " << vertex_index << endl;
	cout << "버텍스 노멀 수: " << vertex_normal_index << endl;
	cout << "실질 버텍스 수: " << dxmesh_verticies.GetCount() << endl;
	for (int i = 0; i < dxmesh_verticies.GetCount(); i++) {
		return_verticies[i] = (*dxmesh_verticies[i]);
		cout << "Position: " << dxmesh_verticies[i]->Position.x << " "
			<< dxmesh_verticies[i]->Position.y << " "
			<< dxmesh_verticies[i]->Position.z << " " << "Normal: "
			<< dxmesh_verticies[i]->Normal.x << " "
			<< dxmesh_verticies[i]->Normal.y << " "
			<< dxmesh_verticies[i]->Normal.z << " " << "UV: "
			<< dxmesh_verticies[i]->UV.x << " "
			<< dxmesh_verticies[i]->UV.y << endl;

	}
	delete[] verticies;
	delete[] vertex_normals;
	delete[] vertex_textures;
	fin.close();
}
void ReadMtl(const char* file_name) {
	ifstream fin(file_name);
	if (!fin.is_open()) {
		cout << "Material File doesn't Open!" << endl;
		return;
	}
	
}
