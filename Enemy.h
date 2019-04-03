#pragma once
#include "Body.h"


/*
0 dummy
1~99 body
100~149 weapon
150~199 tail
*/
Body body[200];

class Enemy : public Cell {
public:
	float l_speed;
	float r_speed;
	float speed;
	float b_speed;

	float weight;

	bool immortal;
	vector<int> weapon;

	action Action;
	int transit[25];  //한 경우에서 뻗어 나갈 수 있는 이동 수는 4. 한 이동이 +3 되면 나머지는 -1씩 떨어져야 함
	int character[9];
	float distPast;
	int fireControle;
	int biteControle;
	int rotateIntend;
	/*
	int transit[20] =  {
	r_r, r_l, r_g, r_atk, r_s, //0~4

	l_r, l_l, l_g, l_atk, l_s, //5~9

	g_r, g_l, g_g, g_atk, g_s, //10~14

	atk_r, atk_l, atk_g, atk_atk, atk_s, //15~19

	s_r, s_l, s_g, s_atk, s_s  //20~24

	};
	*/

	vector<int> randomSellect;
	bool keyListEnemy[10];

	/*
	1: up
	2: down
	3: left
	4: right
	5: back
	*/
	int Leaf[6];
public:
	Enemy() {
		memset(Leaf, 0, sizeof(Leaf));
		for (int i = 0; i < 25; i++)
			transit[i] = 2;
		int initRandAct = 0;

		immortal = false;
		fireControle = 0;
		biteControle = 0;
		rotateIntend = rand() % 1;

		character[0] = rand() % 10 + 10;  //max food dist
		character[1] = rand() % 10 + 10; //full hp
		character[2] = rand() % 50 + 150; //dist scaling
		character[3] = rand() % 2 + 1; //hp div scale
		character[4] = rand() % 4 + 1; //init reward;
		character[5] = rand() % 4 + 1;
		character[6] = rand() % 4 + 1;
		character[7] = rand() % 4 + 1;
		character[8] = 0; // rand() % 4 + 1;
	}

	void initializeEnemy() {
		initializeCell();

		immortal = false;
		memset(Leaf, 0, sizeof(Leaf));
		for (int i = 0; i < 25; i++)
			transit[i] = 2;
		int initRandAct = 0;

		fireControle = 0;
		biteControle = 0;

	}

	void travelLeaf(int bodyIndex) {
		if (body[bodyIndex].is_weapon() && body[bodyIndex].get_isExist()) {
			weapon.push_back(bodyIndex);
		}
		for (int i = 1; i < 6; i++) {
			if (body[bodyIndex].get_Index(i) == 0) continue;
			travelLeaf(body[bodyIndex].get_Index(i));
		}
	}

	//AI 관련 함수들  (AI_decideAction, AI_decideTransiiton, AI_decideTransitionProb)
	//_____________________________________________________________________________________________________________________
	void AI_decideAction() {

		//weight  = 0;
		//Action = r;
		//Action = l;
		switch (Action)
		{
		case r:
			keyListEnemy[1] = true;  //right
			AI_decideTransition(0);
			break;
		case l:
			keyListEnemy[0] = true;  //left
			AI_decideTransition(5);
			break;
		case g:
			keyListEnemy[2] = true;  //go
			AI_decideTransition(10);
			break;
		case atk:
			/*
			keyListEnemy[3] = true;  //attack //외부 함수에서 처리된다.
			*/
			AI_decideTransition(15);
			break;
		case s:	//all action wil stop and go back
			keyListEnemy[4] = true;
			AI_decideTransition(20);
			break;
		}

		if (keyListEnemy[0]) {
			this->turn_left();
		}
		if (keyListEnemy[1]) {
			this->turn_right();
		}
		if (keyListEnemy[2]) {
			this->go_forward();
		}
		/*
		if (keyListEnemy[3]) {
		this->attack();
		}*/ //공격은 외부 함수에서 처리 된다. 
		if (keyListEnemy[4]) {
			this->go_backward();
		}

		for (int i = 0; i < 5; i++)
			keyListEnemy[i] = false;
		//이후 keyListEnemy[3] 에 대해서 공격 행동 함수도 구현 하여야 한다

	}

	void AI_decideTransition(int action)
	{
		srand(time(NULL));
		//행동 후 행동결정 rewarding을 결정한다
		randomSellect.clear();
		AI_decideTransitionProb();
		for (int j = 0; j < 5; j++)
		{
			for (int i = 0; i < transit[action + j]; i++)
			{
				randomSellect.push_back(j);
			}
		}

		int sellectTransit = rand() % 4;
		if (randomSellect.size() == 1) { sellectTransit = randomSellect[0]; }
		else if (randomSellect.size() != 0)
			sellectTransit = randomSellect[(rand() % (randomSellect.size() - 1))]; //0~ maximum index

		switch (sellectTransit)
		{
		case 0:
			Action = r;
			break;
		case 1:
			Action = l;
			break;
		case 2:
			Action = g;
			break;
		case 3:
			Action = atk;
			break;
		case 4:
			Action = s;
			break;
		default:
			break;
		}

	}


	void AI_decideTransitionProb()
	{
		/*
		if (abs(rotate - 6.28318) < 0.001)
		rotate = 0;*/
		//fireControle++;
		biteControle++;
		if (biteControle == 1000)
			biteControle = 0;
	
		int threashHold_maxFoodDist = character[0];//15;
		int threashHold_fullHP = character[1]; //10;
		int threashHold_distScaling = character[2]; //= 180;
		int threashHold_hpScaling_div = 2 * character[3]; //= 2;
														  //int threashHold_attack = 30;
		int leftReward = character[4]; //= rand() % 3;
		int rightReward = character[5]; //= rand() % 3;
		int stopReward = character[6]; //= rand() % 3;
		int goReward = character[7]; //= rand() % 3;
		int attackReward = character[8]; //= rand() % 3;

		float xDir = sinf(rotate);
		float zDir = cosf(rotate);
		float myEuc = sqrt(pow(xDir, 2) + pow(zDir, 2));
		int index = 0;
		float xDif = 1000;
		float zDif = 1000;
		float distManhattan = 1000;
		float distEucHostil = 1000;
		float distEucFood = 1000;
		float minXdif = xDif;
		float minZdif = zDif;

		if (x > 75 || z > 75)	//벽 내부에 머물도록 바운더리 지정
		{
			rotate += 0.01;
		}
		else if (x < -75 || z < -75)
			rotate -= 0.01;


		for (int i = 0; i <enemyLocation.size(); i++) //enemyLocation.size()
		{
			if (enemyLocation[i].Exist)
			{
				if (i == this->id)
					continue;
				xDif = abs(enemyLocation[i].x - x);
				zDif = abs(enemyLocation[i].z - z);
				if (distManhattan > xDif + zDif)
				{
					index = i;
					distManhattan = xDif + zDif;
					minXdif = xDif;
					minZdif = zDif;
				}
			}
		}
		distEucHostil = sqrt((pow(minXdif, 2) + pow(minZdif, 2)));
		float Me_enemyVector_X = (enemyLocation[index].x - x);
		float Me_enemyVector_Z = (enemyLocation[index].z - z);
		float hostilCrossProduct = (xDir*Me_enemyVector_Z - zDir*Me_enemyVector_X);  //myEuc_vector X Me_enemyVector
		float angleHostil = asin((hostilCrossProduct) / ((distEucHostil)*(myEuc)));
		float Me_enemyVectorDotProduct = (xDir*Me_enemyVector_X + zDir*Me_enemyVector_Z);
		//calculate closest food and its ankle
		int index2 = 0;
		xDif = abs(foodLocation[1].x - x);
		zDif = abs(foodLocation[1].z - z);
		distManhattan = xDif + zDif;
		minXdif = xDif;
		minZdif = zDif;
		for (int i = 2; i < foodLocation.size(); i++)
		{
			if (foodLocation[i].Exist)
			{
				xDif = abs(foodLocation[i].x - x);
				zDif = abs(foodLocation[i].z - z);
				if (distManhattan > xDif + zDif)
				{
					index2 = i;
					minXdif = xDif;
					minZdif = zDif;
					distManhattan = xDif + zDif;
				}
			}
		}
		distEucFood = sqrt((pow(minXdif, 2) + pow(minZdif, 2)));
		float Me_foodVector_X = (foodLocation[index2].x - x);
		float Me_foodVector_Z = (foodLocation[index2].z - z);
		float foodCrossProduct = (xDir*Me_foodVector_Z - zDir*Me_foodVector_X);  //myEuc_vector X Me_enemyVector
		float angleFood = asin((foodCrossProduct) / ((distEucFood)*(myEuc)));
		float Me_foodVectorDotProduct = (xDir*Me_foodVector_X + zDir*Me_foodVector_Z);
		bool thereIsEnemyGettingInMyWay = false;
		bool thereIsFoodGettingInMyWay = false;

		float foodAngle2 = Me_foodVectorDotProduct / ((distEucFood)*(myEuc));
		float enemyAngle2 = Me_enemyVectorDotProduct / ((distEucHostil)*(myEuc));

		if (enemyAngle2 < 1.6)
		{
			thereIsEnemyGettingInMyWay = true;
		}
		if (foodAngle2 < 1.6)
		{
			thereIsFoodGettingInMyWay = true;
		}

		bool HostilinMyAngle = (angleHostil < 0.1 && -0.1 < angleHostil);
		bool FoodinMyAngle = (angleFood < 0.1 && -0.1 < angleFood);
		bool hostilLongerThanFood = (distEucHostil >= distEucFood);

		float distFactor = HP*(threashHold_distScaling / (distEucHostil + 1) + (HP / threashHold_hpScaling_div));
		distFactor = distFactor / threashHold_hpScaling_div;
		float hpFactor = (10 - HP)*((threashHold_distScaling / (HP + 1)) / (threashHold_hpScaling_div*distEucFood));
		//회전 각도 요소 고려
		//stopReward = 0;
		attackReward = 0;
		//goReward = 1;
		distPast = distEucHostil;

		if (distEucHostil < distPast && HP > 3)
		{
			leftReward = distFactor;
			//goReward = leftReward / 2;
		}
		//character

		if (HostilinMyAngle && thereIsEnemyGettingInMyWay)
		{
			goReward = (distFactor);
			leftReward = 0;
			rightReward = 0;
			stopReward = 0;
			attackReward = 0;
		}
		else if (HostilinMyAngle && !thereIsEnemyGettingInMyWay)
		{
			goReward = (hpFactor);
			stopReward = 0;
			attackReward = 0;
		}
		else if (angleHostil >= 0.1)
		{
			leftReward = (distFactor);
			goReward /= 2;

		}
		else if (angleHostil <= -0.1)
		{
			rightReward = (distFactor);
			goReward /= 2;
		}


		if (FoodinMyAngle && thereIsFoodGettingInMyWay) //바라보는 방향에 먹이만 있는 경우
		{
			if (hpFactor > distFactor)
				goReward = hpFactor;
			leftReward = 0;
			rightReward = 0;
			stopReward = 0;
			attackReward = 0;
		}
		else if (angleFood >= 0.1)
		{
			if (hpFactor > distFactor)
				leftReward = hpFactor;
			goReward /= 2;
		}
		else if (angleFood <= -0.1)
		{
			if (hpFactor > distFactor)
				rightReward = hpFactor;
			goReward /= 2;
		}

		if (FoodinMyAngle && HostilinMyAngle) //같은 선상에 적과 먹이가 있는 경우
		{
			if (!thereIsEnemyGettingInMyWay && !thereIsFoodGettingInMyWay && !hostilLongerThanFood)
			{
				stopReward = distFactor;
				goReward = 0;
			}
			else if (!thereIsEnemyGettingInMyWay && !thereIsFoodGettingInMyWay && hostilLongerThanFood)
			{
				stopReward = hpFactor;
				goReward = 0;
			}
			else if (!thereIsEnemyGettingInMyWay && thereIsFoodGettingInMyWay && !hostilLongerThanFood)
			{
				goReward = hpFactor;
				stopReward = distFactor / 2;
				if (stopReward > goReward)
					goReward = 0;
				else
					stopReward = 0;
			}
			else if (!thereIsEnemyGettingInMyWay && thereIsFoodGettingInMyWay && hostilLongerThanFood)
			{
				goReward = hpFactor;
				stopReward = distFactor / 2;
				if (stopReward > goReward)
					goReward = 0;
				else
					stopReward = 0;
			}
			else if (thereIsEnemyGettingInMyWay && !thereIsFoodGettingInMyWay && !hostilLongerThanFood)
			{
				goReward = distFactor;
				stopReward = hpFactor / 2;
				if (stopReward > goReward)
					goReward = 0;
				else
					stopReward = 0;

			}
			else if (thereIsEnemyGettingInMyWay && !thereIsFoodGettingInMyWay && hostilLongerThanFood)
			{
				goReward = distFactor;
				stopReward = hpFactor / 2;
				if (stopReward > goReward)
					goReward = 0;
				else
					stopReward = 0;
			}
			else if (thereIsEnemyGettingInMyWay && thereIsFoodGettingInMyWay && !hostilLongerThanFood)
			{
				goReward = hpFactor + distFactor;
				stopReward = 0;
			}
			if (thereIsEnemyGettingInMyWay && thereIsFoodGettingInMyWay && hostilLongerThanFood)
			{
				goReward = hpFactor + distFactor;
				stopReward = 0;
			}
		}


		if (leftReward < rightReward)
			leftReward = 0;
		else
			rightReward = 0;

		if (goReward < stopReward)
			goReward = 0;
		else
			stopReward = 0;

		if (distEucFood < 3) //먹이 먹는 과정
		{
			if (HP + 2 <= 10)
			{
				food[index2].modif_Exist(false);
				foodLocation[index2].Exist = false;
				//HP += 2;
				enemyLocation[id].hp = enemyLocation[id].hp + 2;
			}
		}


		if (distEucHostil < 5)
		{
			if (rotateIntend == 0)
			{
				leftReward = distFactor/10;
				rightReward = 0;
			}
			else
			{
				rightReward = distFactor/10;
				leftReward = 0;
			}
			if (thereIsEnemyGettingInMyWay && HostilinMyAngle)
			{
				goReward = 0;
			}
			else if (!(thereIsEnemyGettingInMyWay) && HostilinMyAngle)
			{
				stopReward = 0;
			}
			if (thereIsEnemyGettingInMyWay && distEucFood<threashHold_maxFoodDist && HP <4)  //6.20 윤승희 추가
			{
				leftReward = 0;
				rightReward = 0;
				stopReward = 0;
				goReward = 10;
				attackReward = 3;
				if (biteControle % 400 == 0)
				{
					goReward = distFactor;
					leftReward = 0;	//6.21 윤승희 추가.
					rightReward = 0;
					enemyLocation[index].decreseHP(3);
					//enemyLocation[index].biteDamage = 2;
					if (enemyLocation[index].hp <= 0)
					{
						enemyLocation[index].x = 100;
						enemyLocation[index].z = id;
						enemyLocation[index].Exist = false;
					}
					//enemyLocation[index].synchronization(index);
					//stopReward = distFactor/2;
				}
				if (distEucHostil < 3)
					goReward = 0;
			}
			if (thereIsEnemyGettingInMyWay && biteControle % 100 == 0 && angleHostil <0.4 && -0.4 <angleHostil)
			{
				goReward = distFactor;
				leftReward = 0;	//6.21 윤승희 추가.
				rightReward = 0;
				enemyLocation[index].decreseHP(1);//enemyLocation[index].hp - 2;
				//enemyLocation[index].biteDamage = 1;
				if (enemyLocation[index].hp <= 0)
				{
					enemyLocation[index].x = 100;
					enemyLocation[index].z = id;
					enemyLocation[index].Exist = false;
				}
				//enemyLocation[index].synchronization(index);
				//stopReward = distFactor;
				if (distEucHostil < 3)
					goReward = 0;
			}
		}


		if (distEucHostil < 11 ) //&& fireControle % 50 == 0
		{
			attackReward = ((int)((distFactor) / 10));
		}
		else
			attackReward = 0;
		if (distEucHostil < 3.5)
			attackReward = 0;

		int sumOfElement = stopReward + attackReward + leftReward + rightReward + goReward;
		while (sumOfElement > 40)
		{
			stopReward /= 2;
			attackReward /= 2;
			leftReward /= 2;
			rightReward /= 2;
			goReward /= 2;
			sumOfElement = stopReward + attackReward + leftReward + rightReward + goReward;
		}
		//int randFactor = rand() % 1 + 1;

		int state = (int)(Action);
		transit[5 * state] = (int)rightReward;
		transit[5 * state + 1] = (int)leftReward;
		transit[5 * state + 2] = (int)goReward;
		transit[5 * state + 3] = attackReward;// 3;// (int)attackReward;
		transit[5 * state + 4] = (int)stopReward;
		//if (state == atk)
			//transit[5 * state + 3] /= 4;
		//transit[5 * state + state] = randFactor * transit[5 * state + state];

		//Action = l;
	}

	////////////////////////////////

	// 이동 관련함수
	//------------------------------------------------------------------------------------

	void turn_left() {
		setCollisionBox();
		setLeaves();

		rotate -= l_speed / weight;
	}

	void turn_right() {
		setCollisionBox();
		setLeaves();

		rotate += r_speed / weight;
	}

	void go_forward() {
		setCollisionBox();
		setLeaves();
		
		x += speed * sin(rotate) / weight;
		z += speed * cos(rotate) / weight;
	}

	void go_backward() {
		setCollisionBox();
		setLeaves();
		
		x -= b_speed * sin(rotate) / weight;
		z -= b_speed * cos(rotate) / weight;
	}


	// 블록분리 관련함수
	//------------------------------------------------------------------------------------

	void detachBody(int pos) {
		if (Leaf[pos] == 0) return;
		setDetachLeaves(Leaf[pos], pos, x, y, z, rotate);
		Leaf[pos] = 0;
	}

	void detachBodyBody(int bodyIndex, int pos) {
		if (body[bodyIndex].get_Index(pos) == 0) return;
		setDetachLeaves(body[bodyIndex].get_Index(pos), pos, body[bodyIndex].get_x(), body[bodyIndex].get_y(), body[bodyIndex].get_z(), body[bodyIndex].get_rotate());
		body[bodyIndex].detachBody(pos);
	}

	void setDetachLeaves(int bodyIndex, int pos, float _x, float _y, float _z, float _rotate) {

		switch (pos) {
		case 1:
			body[bodyIndex].setWorldCoord(_x, _y + 2.5f, _z);
			break;
		case 2:
			body[bodyIndex].setWorldCoord(_x, _y - 2.5f, _z);
			break;
		case 3:
			body[bodyIndex].setWorldCoord(_x - 2.5f * cos(_rotate), _y, _z + 2.5f * sin(_rotate));
			break;
		case 4:
			body[bodyIndex].setWorldCoord(_x + 2.5f * cos(_rotate), _y, _z - 2.5f * sin(_rotate));
			break;
		case 5:
			body[bodyIndex].setWorldCoord(_x - 2.5f * sin(_rotate), _y, _z - 2.5f * cos(_rotate));
			break;
		}
		body[bodyIndex].setCollisionBox();
		body[bodyIndex].set_isConnect(false);

		for (int i = 1; i < 6; i++) {
			if (body[bodyIndex].get_Index(i) == 0) continue;
			setDetachLeaves(body[bodyIndex].get_Index(i), i, body[bodyIndex].get_x(), body[bodyIndex].get_y(), body[bodyIndex].get_z(), body[bodyIndex].get_rotate());
		}
		body[bodyIndex].detachAllBodies();
	}


	// 블록결합 관련함수
	//------------------------------------------------------------------------------------

	void setLeaves() {
		r_speed = 0.05f;
		l_speed = 0.05f;
		speed = 0.2f;
		b_speed = 0.2f;
		weight = 1.0f;
		immortal = false;

		for (int i = 1; i < 6; i++) {
			if (Leaf[i] == 0) continue;
			if (!body[Leaf[i]].get_isExist()) {
				detachBody(i);
				continue;
			}
			setAttachLeaves(Leaf[i], i, x, y, z, rotate);
		}
	}

	void insertBody(int pos, int bodyIndex) {
		if (Leaf[pos] != 0) return;
		if (body[Leaf[pos]].get_isConnect()) return;

		/*if (body[bodyIndex].color)
			enemyLocation[id].hp += 5;
		else if (!(body[bodyIndex].is_tail() || body[bodyIndex].is_weapon()))
			enemyLocation[id].hp += 2;*/

		Leaf[pos] = bodyIndex;
		body[bodyIndex].core_id = id;
		bodyInform[bodyIndex].core_id = id; 		//bodyInform에 관한 처리. (코어 id와 parent id를 기록)
		bodyInform[bodyIndex].parantBodyId = id;


		if (body[bodyIndex].is_weapon())
		{
			enemyLocation[id].MyWeapon.push_back(bodyIndex);
			bodyInform[bodyIndex].is_weapone = true;
		}
		else if (body[bodyIndex].is_tail())
		{
			enemyLocation[id].MyTail.push_back(bodyIndex);
			bodyInform[bodyIndex].is_Tail = true;
		}
		else
		{
			enemyLocation[id].MyNormalBody.push_back(bodyIndex);
		}
		setLeaves();
	}

	int get_Index(int pos) {
		return Leaf[pos];
	}

	void insertAnyWhere(int BodyIndexWeWannaInsert)	//6.20 윤승희 추가	//이 함수는 존재하는 body요소만을 입력으로 받아야 한다. 
	{
		if(body[BodyIndexWeWannaInsert].get_isConnect())
			return;
		for (int i = 5; i >= 1; i--) {
			if (Leaf[i] == 0)
			{
				insertBody(i, BodyIndexWeWannaInsert);	//코어 인근에 적당한 위치가 있으면 붙인다.
				return;
			}
		}
		int randompick = rand() % 5 + 1;
		//코어 주변이 꽉 찬 경우는 아래 절차를 따른다.
		posAndIndex PosAndInd = traversalBodyInform(Leaf[randompick]);
		if(PosAndInd.pos == -1)
			return;
		body[PosAndInd.index].insertBody(PosAndInd.pos, BodyIndexWeWannaInsert);
	}

	posAndIndex traversalBodyInform(int searchInd)
	{
		if(searchInd <=0 || searchInd >=200)
		{
			return posAndIndex(-1,-1);
		}

		int randompick = rand() % 5 + 1;
		if (randompick < 3)
		{
			for (int i = 1; i < 6; i++)
			{
				/*if(searchInd<=0 || searchInd>5)
					return posAndIndex(i, searchInd);
		
				try{*/

				if (bodyInform[searchInd].Leaf[i] == 0)
				{
					return posAndIndex(i, searchInd);
				}

				/*}catch(...)
				{
					return posAndIndex(-1, -1);
				}*/
			}
		}
		else
		{
			for (int i = 5; i >= 1; i--)
			{
				/*if(searchInd<=0|| searchInd>5)
					return posAndIndex(i, searchInd);

				try{*/
					if (bodyInform[searchInd].Leaf[i] == 0)
					{
						return posAndIndex(i, searchInd);
					}
				/*} 
				catch(...)
				{
					return posAndIndex(-1, -1);
				}*/
			}

		}
		//else
		return traversalBodyInform(bodyInform[searchInd].Leaf[randompick]);

	}

	void setAttachLeaves(int bodyIndex, int pos, float _x, float _y, float _z, float _rotate) {
		switch (pos) {
		case 1:
			body[bodyIndex].setWorldCoord(_x, _y + 2, _z);
			body[bodyIndex].setRotate(_rotate);
			body[bodyIndex].setRotateX(XM_PI / 2);
			break;
		case 2:
			body[bodyIndex].setWorldCoord(_x, _y - 2, _z);
			body[bodyIndex].setRotate(_rotate);
			body[bodyIndex].setRotateX(-XM_PI / 2);
			break;
		case 3:
			body[bodyIndex].setWorldCoord(_x - 2 * cos(_rotate), _y, _z + 2 * sin(_rotate));
			body[bodyIndex].setRotate(_rotate + XM_PI / 2);
			body[bodyIndex].setRotateX(0);
			break;
		case 4:
			body[bodyIndex].setWorldCoord(_x + 2 * cos(_rotate), _y, _z - 2 * sin(_rotate));
			body[bodyIndex].setRotate(_rotate - XM_PI / 2);
			body[bodyIndex].setRotateX(0);
			break;
		case 5:
			body[bodyIndex].setWorldCoord(_x - 2 * sin(_rotate), _y, _z - 2 * cos(_rotate));
			body[bodyIndex].setRotate(_rotate);
			body[bodyIndex].setRotateX(0);

			break;
		}
		body[bodyIndex].setCollisionBox();
		body[bodyIndex].set_isConnect(true);

		if (bodyIndex >= 80 && bodyIndex < 100) {
			immortal = true;
		}
		
		weight += 0.1f;
		if (body[bodyIndex].is_tail()) {
			float boost = rotate - body[bodyIndex].get_rotate();
			float cos_boost = cos(boost);
			float sin_boost = sin(boost);

			if (cos_boost > 0) {
				speed += cos_boost * 0.02f;
			}
			else {
				b_speed += -cos_boost * 0.02f;
			}

			if (sin_boost > 0) {
				l_speed += sin_boost * 0.005f;
			}
			else {
				r_speed += -sin_boost * 0.005f;
			}
		}

		for (int i = 1; i < 6; i++) {
			if (body[bodyIndex].get_Index(i) == 0) continue;
			if (!body[body[bodyIndex].get_Index(i)].get_isExist()) {
				detachBodyBody(bodyIndex, i);
				continue;
			}
			setAttachLeaves(body[bodyIndex].get_Index(i), i, body[bodyIndex].get_x(), body[bodyIndex].get_y(), body[bodyIndex].get_z(), body[bodyIndex].get_rotate());
		}
	}

};
