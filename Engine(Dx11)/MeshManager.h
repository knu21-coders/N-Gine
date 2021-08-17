#pragma once
#include "framework.h"

struct Vertex{
	
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Tex;

};
struct MeshData {
public:
	MeshData() {

	}
	~MeshData() {

	}
	std::vector<DirectX::XMFLOAT3> vertices;
	std::vector<UINT> indicies;
	UINT IndexOffset = 0;
	UINT VertexOffset = 0;
};

class MeshDataManager {
private:
	static MeshDataManager* m_pInstance;
	MeshDataManager() {}
	~MeshDataManager() {}
	UINT m_CurrentIndexOffset = 0;
	UINT m_CurrentVertexOffset = 0;

public:

	//싱글톤 패턴
	static MeshDataManager* GetInstance() {
		if (m_pInstance == nullptr) m_pInstance = new MeshDataManager();
		return m_pInstance;
	}
	static void Release() {
		if (m_pInstance != nullptr) delete m_pInstance;
	}

	void AddObject(const char* name, const int& vertCount, const int & indiciesCount, const std::vector<DirectX::XMFLOAT3> &vertices, const std::vector<UINT> &indicies) {
		
		MeshData tmp_mesh;
		tmp_mesh.indicies = indicies;
		tmp_mesh.vertices = vertices;
		tmp_mesh.IndexOffset = m_CurrentIndexOffset;
		tmp_mesh.VertexOffset = m_CurrentVertexOffset;
		objects[name] = tmp_mesh;
		
		//추가한 뒤
		m_CurrentIndexOffset += indiciesCount;
		m_CurrentVertexOffset += vertCount;

	}
	MeshData operator[](const char* name) {
		return objects[name];
	}
private:
	std::unordered_map<const char*, MeshData> objects;
};
MeshDataManager* MeshDataManager::m_pInstance = nullptr;

//맵필요 키-> 오브젝트이름 , 값-> 버텍스/인덱스 버퍼 시작 인덱스