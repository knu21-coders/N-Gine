#pragma once
#include "framework.h"

#define MAX_VALUE_PER_POOLS 2560
using namespace DirectX;
using namespace std;

struct SimpleVertex {
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
};
bool operator==(const SimpleVertex& a, const SimpleVertex& b) {
	bool IsNormalSame = (a.Normal.x == b.Normal.x) && (a.Normal.y == b.Normal.y) && (a.Normal.z == b.Normal.z);
	bool IsPositionSame = (a.Position.x == b.Position.x) && (a.Position.y == b.Position.y) && (a.Position.z == b.Position.z);
	return IsNormalSame && IsPositionSame;
}
struct VertexNode {
private:
	VertexNode* m_pNext;
	bool m_is_pool_full = false;
	unsigned int m_contained_points_count = 0;

public:
	SimpleVertex m_points[MAX_VALUE_PER_POOLS];
	void Add(const SimpleVertex& _target) {
		m_points[m_contained_points_count++] = _target;
		if (m_contained_points_count >= MAX_VALUE_PER_POOLS) m_is_pool_full = true;
	}
	bool IsContain(SimpleVertex& _target) {
		for (int index = 0; index < MAX_VALUE_PER_POOLS; index++) {
			if (m_points[index] == _target) return true;
		}
		return false;
	}
	bool IsContain(SimpleVertex& _target, int* return_index) {
		for (int index = 0; index < MAX_VALUE_PER_POOLS; index++) {
			if (m_points[index] == _target) {
				*return_index = index;
				return true;
			}
		}
		*return_index = -1;
		return false;
	}
	bool IsFull() {
		return m_is_pool_full;
	}
	VertexNode* GetNext() {
		return m_pNext;
	}
	void SetNext(VertexNode** _target) {
		m_pNext = *_target;
	}
};
class VertexSLL {
	VertexNode* m_pStart = nullptr, * m_pEnd = nullptr;
	unsigned int m_count = 0;
public:
	void Add(const SimpleVertex& _target) {
		if (m_pStart == nullptr) {

			m_pStart = new VertexNode();
			if (m_pStart != nullptr) {
				m_pStart->Add(_target);
				m_pEnd = m_pStart;
				m_count++;
			}
			else cout << "힙메모리 과다사용" << endl;
		}
		else {
			if (m_pEnd->IsFull()) {
				VertexNode* tmp = new VertexNode();
				if (tmp != nullptr) {
					m_pEnd->SetNext(&tmp);
					m_pEnd = tmp;
					m_pEnd->Add(_target);
					m_count++;
				}
				else cout << "힙메모리 과다사용" << endl;
			}
			else {
				m_pEnd->Add(_target);
				m_count++;
			}
		}
	}
	SimpleVertex* operator[](const int& index) {
		VertexNode* _iter = m_pStart;
		for (int _iter_count = 0; _iter != nullptr && _iter_count < index / MAX_VALUE_PER_POOLS; _iter = _iter->GetNext(), _iter_count++);
		if (_iter != nullptr) return &_iter->m_points[index % MAX_VALUE_PER_POOLS];
		else return nullptr;
	}
	bool IsContain(SimpleVertex& _target) {
		VertexNode* _iter = m_pStart;
		while (_iter != nullptr) {
			if (_iter->IsContain(_target)) return true;
			else _iter = _iter->GetNext();
		}
		return false;
	}
	bool IsContain(SimpleVertex& _target, int* return_index) {
		VertexNode* _iter = m_pStart;
		int _iter_count = 0;
		int tmp_return_index = 0;
		while (_iter != nullptr) {
			if (_iter->IsContain(_target, &tmp_return_index)) {
				*return_index = (_iter_count * MAX_VALUE_PER_POOLS) + tmp_return_index;
				return true;
			}
			else {
				_iter = _iter->GetNext();
				_iter_count++;
			}
		}
		*return_index = -1;
		return false;
	}
	unsigned int GetCount() {
		return m_count;
	}
	~VertexSLL() {
		VertexNode* _iter = m_pStart, * _iter_copy = nullptr;
		while (_iter != nullptr) {
			_iter_copy = _iter;
			_iter = _iter->GetNext();
			delete _iter_copy;
		}
	}
};

struct Node {
private:
	Node* m_pNext;
	bool m_is_pool_full = false;
	unsigned int m_contained_points_count = 0;

public:
	int m_points[MAX_VALUE_PER_POOLS];
	void Add(const int& _target) {
		m_points[m_contained_points_count++] = _target;
		if (m_contained_points_count >= MAX_VALUE_PER_POOLS) {
			m_is_pool_full = true;
			//cout << "풀이 곽차서 다음으로 이관합니다." << endl;
		}
	}
	bool IsContain(int& _target) {
		for (int index = 0; index < MAX_VALUE_PER_POOLS; index++) {
			if (m_points[index] == _target) return true;
		}
		return false;
	}
	bool IsContain(int& _target, int* return_index) {
		for (int index = 0; index < MAX_VALUE_PER_POOLS; index++) {
			if (m_points[index] == _target) {
				*return_index = index;
				return true;
			}
		}
		*return_index = -1;
		return false;
	}

	bool IsFull() {
		return m_is_pool_full;
	}
	Node* GetNext() {
		return m_pNext;
	}
	void SetNext(Node** _target) {
		m_pNext = *_target;
	}
};
class IntPooingSLL {
	Node* m_pStart = nullptr, * m_pEnd = nullptr;
	unsigned int m_count = 0;
public:
	void Add(const int& _target) {
		if (m_pStart == nullptr) {

			m_pStart = new Node();
			if (m_pStart != nullptr) {
				m_pStart->Add(_target);
				m_pEnd = m_pStart;
				m_count++;
			}
			else cout << "힙메모리 과다사용" << endl;
		}
		else {
			if (m_pEnd->IsFull()) {
				Node* tmp = new Node();
				if (tmp != nullptr) {
					m_pEnd->SetNext(&tmp);
					m_pEnd = tmp;
					m_pEnd->Add(_target);
					m_count++;
				}
				else cout << "힙메모리 과다사용" << endl;
			}
			else {
				m_pEnd->Add(_target);
				m_count++;
			}
		}

	}
	int* operator[](const int& index) {
		Node* _iter = m_pStart;
		for (int _iter_count = 0; _iter != nullptr && _iter_count < index / MAX_VALUE_PER_POOLS; _iter = _iter->GetNext(), _iter_count++);
		if (_iter != nullptr) return &_iter->m_points[index % MAX_VALUE_PER_POOLS];
		else return NULL;
	}
	bool IsContain(int& _target) {
		Node* _iter = m_pStart;
		while (_iter != nullptr) {
			if (_iter->IsContain(_target)) return true;
			else _iter = _iter->GetNext();
		}
		return false;
	}
	bool IsContain(int& _target, int* return_index) {
		Node* _iter = m_pStart;
		int _iter_count = 0;
		int tmp_return_index = 0;
		while (_iter != nullptr) {
			if (_iter->IsContain(_target, &tmp_return_index)) {
				*return_index = (_iter_count * MAX_VALUE_PER_POOLS) + tmp_return_index;
				return true;
			}
			else {
				_iter = _iter->GetNext();
				_iter_count++;
			}
		}
		*return_index = -1;
		return false;
	}
	unsigned int GetCount() {
		return m_count;
	}
	~IntPooingSLL() {
		Node* _iter = m_pStart, * _iter_copy = nullptr;
		while (_iter != nullptr) {
			_iter_copy = _iter;
			_iter = _iter->GetNext();
			delete _iter_copy;
			//cout << "풀 하나를 할당해제합니다." << endl;
		}
	}
};