#pragma once

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

	static void MyPixelShader(cpu_ps_io& io);

private:
	inline static App* s_pApp = nullptr;

	//cpu_font m_font;
	cpu_mesh m_meshPlayer;
	//cpu_rt* m_rts[1];

	cpu_material m_materialPlayer;

	Player* m_pPlayer = nullptr;
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