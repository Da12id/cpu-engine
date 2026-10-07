#include "App.h"
#include "pch.h"

App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);

	m_pPlayer = nullptr;
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	// YOUR CODE HERE

	m_pPlayer = new Player;
	m_pPlayer->Create(&m_meshPlayer, &m_materialPlayer);
	m_pPlayer->GetFSM()->ToState(CPU_ID(StatePlayerIdle));
}

void App::OnUpdate()
{
	// YOUR CODE HERE
}

void App::OnExit()
{
	// YOUR CODE HERE
}

void App::OnRender(int pass)
{
	// YOUR CODE HERE
}

void App::MyPixelShader(cpu_ps_io& io)
{
	io.color = io.p.color;
}



Player::Player()
{
	m_pEntity = nullptr;
	m_pFSM = nullptr;
}

Player::~Player()
{
}

void Player::Create(cpu_mesh* pMesh, cpu_material* pMaterial)
{
	m_pEntity = cpuEngine.CreateEntity();
	m_pEntity->pMesh = pMesh;
	m_pEntity->pMaterial = pMaterial;
	m_pEntity->transform.pos.z = 5.0f;
	m_pEntity->transform.pos.y = -3.0f;

	m_pFSM = cpuEngine.CreateFSM(this);
	m_pFSM->SetPostGlobal<StatePlayerGlobal>();
}

void Player::Destroy()
{
	m_pFSM = cpuEngine.Release(m_pFSM);
	m_pEntity = cpuEngine.Release(m_pEntity);
}

void Player::Update()
{

}

void StatePlayerGlobal::OnEnter(Player& cur, int from, void* pParam)
{
}

void StatePlayerGlobal::OnExecute(Player& cur)
{
	cur.Update();
}

void StatePlayerGlobal::OnExit(Player& cur, int to)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StatePlayerIdle::OnEnter(Player& cur, int from, void* pParam)
{
}

void StatePlayerIdle::OnExecute(Player& cur)
{
	cur.Update();
}

void StatePlayerIdle::OnExit(Player& cur, int to)
{

}