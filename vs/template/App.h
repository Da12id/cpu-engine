#pragma once

class Rail
{
public:
	Rail();
	~Rail();

	void Create(cpu_mesh* pMesh, cpu_material* pMaterial);
};

class Player
{
public:
	Player();
	~Player();

	void Create(cpu_mesh* pMesh, cpu_material* pMaterial);
	void Destroy();

	void Update();

	cpu_entity* GetEntity() { return m_pEntity; }
	cpu_fsm<Player>* GetFSM() { return m_pFSM; }

protected:
	cpu_entity* m_pEntity;
	cpu_fsm<Player>* m_pFSM;
};

struct StatePlayerGlobal
{
	void OnEnter(Player& cur, int from, void* pParam);
	void OnExecute(Player& cur);
	void OnExit(Player& cur, int to);
};

struct StatePlayerIdle
{
	void OnEnter(Player& cur, int from, void* pParam);
	void OnExecute(Player& cur);
	void OnExit(Player& cur, int to);
};

class App
{
public:
	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	void SpawnFruit();

	static void MyPixelShader(cpu_ps_io& io);

	float PickNumber(int number, int total);

private:
	float m_angle = 0.f;
	float m_score = 0;

	inline static App* s_pApp = nullptr;
	DirectX::XMFLOAT3 CenterRail;

	cpu_mesh m_meshSkyBox;
	cpu_mesh m_meshPlayer;
	cpu_mesh m_meshRail;
	cpu_mesh m_meshRailBis;
	cpu_mesh m_meshFruit;

	cpu_material m_materialPlayer;
	cpu_material m_materialRail;
	cpu_material m_materialFruit;

	cpu_entity* pSkyBox;
	Player* m_pPlayer;
	cpu_entity* pRail;
	cpu_entity* pRailBis;
	std::vector<cpu_entity*> fruits;
};
