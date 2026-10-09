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
	srand(time(nullptr));
	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//m_meshSkyBox.CreateSkyBox(100, CPU_RED);//poourquoi ça fais laguer
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_meshPlayer.CreateSpaceship();
	m_meshRail.CreateCircle(4.5f, 20, CPU_BLACK);
	m_meshRailBis.CreateCircle(3.5f, 20, CPU_RED);
	m_meshFruit.CreateSphere();

	//pSkyBox = cpuEngine.CreateEntity();
	//pSkyBox->pMesh = &m_meshSkyBox;

	pRail = cpuEngine.CreateEntity();
	pRail->pMesh = &m_meshRail;
	pRail->transform.pos.z = 5.f;

	pRailBis = cpuEngine.CreateEntity();
	pRailBis->pMesh = &m_meshRailBis;
	pRailBis->transform.pos.y = 0.1f;
	pRailBis->transform.pos.z = 5.f;
	
	CenterRail = pRail->transform.pos;

	m_pPlayer = new Player;
	m_pPlayer->Create(&m_meshPlayer, &m_materialPlayer);
	m_pPlayer->GetFSM()->ToState(CPU_ID(StatePlayerIdle));
	m_pPlayer->GetEntity()->transform.SetScaling(0.3f);
	m_pPlayer->GetEntity()->transform.pos = CenterRail;
	CenterRail.y += 0.5f;
	m_pPlayer->GetEntity()->transform.pos.z += 4.f;
	m_pPlayer->GetEntity()->transform.pos.y += 0.5f;

	SpawnFruit();

	cpuEngine.GetCamera()->transform.pos.z = -6.0f;
	cpuEngine.GetCamera()->transform.pos.y= 3.5f;
	cpuEngine.GetCamera()->transform.SetYPR(0.f, 0.2f, 0.f);

}

void App::OnUpdate()
{
	// YOUR CODE HERE
	float dt = cpuTime.delta;
	float time = cpuTime.total;



	if (cpuInput.IsLeft())
	{
		m_angle += dt * 3.5f;
	}
	if (cpuInput.IsRight())
	{
		m_angle -= dt * 3.5f;
	}

	m_pPlayer->GetEntity()->transform.OrbitAroundAxis(CenterRail, CPU_VEC3_UP, 4.f, m_angle);

	for(int x = 0; x<fruits.size(); x++)
	{
		fruits[x]->transform.pos.y -= m_gravity;

		if (cpu::ObbObb(m_pPlayer->GetEntity()->obb, fruits[x]->obb))
			AddorSubstractPoint(m_pPlayer->GetEntity(), x);

		else if(cpu::ObbObb(pRail->obb, fruits[x]->obb))
			AddorSubstractPoint(pRail, x);
	}

	dtFruit += dt;
	dtGravity += dt;
	if (dtFruit >= 10)
	{	
		SpawnFruit();
		dtFruit = 0;
	}

	if (dtGravity >= 30)
	{
		m_gravity += 0.01;
		dtGravity = 0;
	}

	if(m_score<0)
		cpuEngine.Quit();

	if (cpuInput.IsBackPressed())
		cpuEngine.Quit();
}

void App::OnExit()
{
	// YOUR CODE HERE

	if (m_pPlayer)
		m_pPlayer->Destroy();
	CPU_DELPTR(m_pPlayer);
}

void App::OnRender(int pass)
{
	// YOUR CODE HERE
	std::string textScore;

	if(m_score == 1)
		textScore = CPU_STR(m_score) + " HEALTH POINT";
	else
		textScore = CPU_STR(m_score) + " HEALTH POINTS";

	cpuDevice.DrawText(&m_font, textScore.c_str(), 10, 10);
}

void App::Pause()
{
	while (true)
	{
		if (cpuInput.IsBackPressed())
			break;
	}
}

void App::SpawnFruit()
{
	if (fruits.size() > 4)
		return;

	cpu_entity* pFruit = cpuEngine.CreateEntity();
	pFruit->pMesh = &m_meshFruit;
	pFruit->transform.pos = CenterRail;
	float angle = PickNumber(0, XM_2PI);
	pFruit->transform.pos.x += cosf(angle) * 4.f;
	pFruit->transform.pos.z += sinf(angle) * 4.f;
	pFruit->transform.pos.y = 10.f;
	fruits.push_back(pFruit);
}

void App::AddorSubstractPoint(cpu_entity* EntityinCollision, int indexFruits)
{
	if (EntityinCollision == m_pPlayer->GetEntity())
		m_score++;
	else
		m_score--;
	
	fruits[indexFruits]->active = false;
	fruits.erase(fruits.begin() + indexFruits);
	SpawnFruit();
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
	//float dt = cpuTime.delta;

	//if (cpuInput.IsUp())
	//	m_pEntity->transform.Move(dt * 5.0f);
	//if (cpuInput.IsDown())
	//	m_pEntity->transform.AddYPR(0, dt * XM_PI);
	//if (cpuInput.IsRight())
	//	m_pEntity->transform.AddYPR(dt * XM_PI);
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

float App::PickNumber(int number, int total)
{
	number = rand() % (total - number + 1) + number;
	return number;
}