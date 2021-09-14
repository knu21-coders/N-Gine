#pragma once
#include "framework.h"
using namespace DirectX;

struct Vertex {
		XMFLOAT3 Position;
		XMFLOAT3 Normal;
		XMFLOAT2 UV;
};
bool operator==(const Vertex& a, const Vertex& b) {
	bool IsNormalSame = (a.Normal.x == b.Normal.x) && (a.Normal.y == b.Normal.y) && (a.Normal.z == b.Normal.z);
	bool IsPositionSame = (a.Position.x == b.Position.x) && (a.Position.y == b.Position.y) && (a.Position.z == b.Position.z);
	bool IsTextureSame = ((a.UV.x == b.UV.x) && (a.UV.y == b.UV.y));
	return IsNormalSame && IsPositionSame && IsTextureSame;
}
struct MeshData {
public:
	MeshData() {

	}
	~MeshData() {

	}
	Vertex* vertices;
	WORD* indicies;
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

	void AddObjectToBundle(const char* name, const int& vertCount, const int & indiciesCount, const std::vector<DirectX::XMFLOAT3> &vertices, const std::vector<UINT> &indicies) {
		
		BundleMeshData tmp_mesh= objects[name];
		tmp_mesh.indicies = indicies;
		tmp_mesh.vertices = vertices;
		tmp_mesh.IndexOffset = m_CurrentIndexOffset;
		tmp_mesh.VertexOffset = m_CurrentVertexOffset;
		 = tmp_mesh;
		
		//추가한 뒤
		m_CurrentIndexOffset += indiciesCount;
		m_CurrentVertexOffset += vertCount;

	}
	void AddObjectIndependently(const char* name, const int& vertCount, const int& indiciesCount, const std::vector<DirectX::XMFLOAT3>& vertices, const std::vector<UINT>& indicies) {

		MeshData tmp_mesh;
		tmp_mesh.indicies = indicies;
		tmp_mesh.vertices = vertices;
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