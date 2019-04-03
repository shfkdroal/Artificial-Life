#pragma once
#include "Header.h"

class Cell {
protected:
	int HP;
	bool isExist;

	float rotate_x;
	float rotate;
	float x;
	float y;
	float z;

	CollisionBox collisionBox;
	
	SimpleVertex *model;
	WORD *index;
	int V_count;
	int I_count;
	int id;

public:
	Cell() {
		HP = 10;
		x = 0; y = 0; z = 0;
		rotate = 0.0f;
		rotate_x = 0.0f;
		isExist = false;

		collisionBox.obox.Center = XMFLOAT3(0, 0, 0);
		collisionBox.obox.Extents = XMFLOAT3(0.9f, 0.9f, 0.9f);
		collisionBox.obox.Orientation = XMFLOAT4(0, 0, 0, 1);
		collisionBox.collision = DISJOINT;
	};

	~Cell() {
		delete []model;
		delete []index;
	}

	void initializeCell() {
		HP = 10;
		x = 0; y = 0; z = 0;
		rotate = 0.0f;
		rotate_x = 0.0f;
		isExist = false;

		collisionBox.obox.Center = XMFLOAT3(0, 0, 0);
		collisionBox.obox.Extents = XMFLOAT3(0.9f, 0.9f, 0.9f);
		collisionBox.obox.Orientation = XMFLOAT4(0, 0, 0, 1);
		collisionBox.collision = DISJOINT;
	}

	// HP �����Լ�
	//------------------------------------------------------------------------------------

	int full_HP() {
		return 10;
	}

	void set_HP(int hp) {
		HP = hp;
	}

	int get_HP() {
		return HP;
	}

	void increaseHP(int food) {
		HP += food;
		if (HP > 10 ) {
			HP = 10;
		}
	}

	void decreaseHP(int damage) {
		HP -= damage;
		if (HP <= 0) {
			isExist = false;
		}
	}

	// ���� �����Լ�
	//------------------------------------------------------------------------------------

	void set_Exist() {
		isExist = true;
	}

	void set_Delete() {
		isExist = false;
	}

	bool get_isExist() {
		return isExist;
	}

	void modify_isExist(bool synch) //6.20 ������ �߰�
	{
		isExist = synch;
	}


	void set_id(int ID)
	{
		id = ID;
	}
	int get_id()
	{
		return id;
	}


	// �浹ó�� �����Լ�
	//------------------------------------------------------------------------------------

	CollisionBox* get_collisionBox() {
		return &collisionBox;
	}

	void setCollisionBox() {
		collisionBox.obox.Center.x = x;
		collisionBox.obox.Center.y = y;
		collisionBox.obox.Center.z = z;
		XMStoreFloat4(&collisionBox.obox.Orientation, XMQuaternionRotationRollPitchYaw(0,rotate,0));
	}


	// ��ǥ �����Լ�
	//------------------------------------------------------------------------------------

	void setWorldCoord(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}

	void set_x(float X) {
		x = X;
	}

	void set_y(float Y) {
		y = Y;
	}

	void set_z(float Z) {
		z = Z;
	}

	float get_x() {
		return x;
	}

	float get_y() {
		return y;
	}

	float get_z() {
		return z;
	}


	// ȸ�� �����Լ�
	//------------------------------------------------------------------------------------

	void setRotate(float t) {
		rotate = t;
	}

	void setRotateX(float t) {
		rotate_x = t;
	}

	float get_rotate() {
		return rotate;
	}

	float get_rotateX() {
		return rotate_x;
	}


	// �𵨸� �����Լ�
	//------------------------------------------------------------------------------------

	SimpleVertex* RenderModel() {
		return model;
	}

	WORD* RenderIndex() {
		return index;
	}
	
	int get_vc() {
		return V_count;
	}
	int get_ic() {
		return I_count;
	}

	HRESULT readModel(char * filename) { 
		ifstream fin;
		char input;
		int i;
		int m_vertexCount, m_indexCount;
		string dist;
		ModelType * m_model;
		float r = 0, g = 0, b = 0;

		fin.open(filename);
		if (fin.fail()) {
			return false;
		}

		fin.get(input);
		while (input != ':') {
			fin.get(input);
		}
		
		fin >> m_vertexCount;
		m_indexCount = m_vertexCount;
		V_count = m_vertexCount;
		I_count = m_indexCount;
		m_model = new ModelType[m_vertexCount];

		if (!m_model) {
			return false;
		}

		fin.get(input);
		while (input != ':') {
			fin.get(input);
		}
		fin.get(input);
		fin.get(input);

		for (i = 0; i<m_vertexCount; i++) {
			fin >> dist;
			if (dist == "d") {
				fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
				fin >> m_model[i].tu >> m_model[i].tv;
				fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz; 
				m_model[i].cx = r; m_model[i].cy = g; m_model[i].cz = b;
			}
			else if (dist == "c") {
				fin >> r >> g >> b;
				i -= 1;
			}
		}
		fin.close();

		model = new SimpleVertex[m_vertexCount];
		index = new WORD[m_indexCount];
	
		for (int i = 0; i<m_vertexCount; i++) {
			model[i].Pos = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
			model[i].Normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
			model[i].Color = XMFLOAT4(m_model[i].cx, m_model[i].cy, m_model[i].cz, 1.0f);
			model[i].TextrureCoord = XMFLOAT2(m_model[i].tu, m_model[i].tv);
			index[i] = (WORD)i;
		}
		delete []m_model;
		return true;
	}
};

class Background : public Cell {};

class HP : public Cell {};

class MainTitle : public Cell {};

class Food : public Cell {
public:
	void modif_Exist(bool TF) {
		isExist = TF;
	}
};

class identify
{
public:
	identify(float X, float Z, bool E, int HP) { x = X; z = Z; Exist = E; hp = HP;}
	float x;
	float z;
	bool Exist;

	vector<int> MyWeapon;
	vector<int> MyNormalBody;
	vector<int> MyTail;

	int hp;
	//int biteDamage;
	void decreseHP(int dec)
	{
		hp = hp - 2;
	}

};

class bodyCertification
{
public:
	bodyCertification(int Core, int Own, int Parent) { core_id = Core; id_own = Own; parantBodyId = Parent; }
	int core_id; //�ڽ��� �Ҽ� �� core id�� ���
	int id_own; //�ڱ� �ڽ��� body id;
	int parantBodyId; //�ڽ��� ���ս�Ų �θ��� id. �θ� �ھ��� ��� �ھ� ���̵� ��ϵȴ�. 
	int Leaf[6];	//�ڽſ��� ���� �� 6������ body index�� ����ϴ� ��� 

	bool is_weapone;
	bool is_Tail;
};

class posAndIndex
{
public:
	posAndIndex(int POS, int INDEX) { pos = POS; index = INDEX; }
	int pos;
	int index;
};


