#include "SceneMode.h"

#include "Map.h"
#include "Player.h"
#include "ScriptReader.h"
#include "SceneRscManager.h"

namespace mode
{

//Player *Portal::m_thePly = 0;  �������ʹ��static����ô���������ʼ���أ������0��λGetPlayer����

SceneMode::SceneMode()
{
	// �õ�hgeָ��
	m_hge = hgeCreate(HGE_VERSION);
	// �õ�playerָ��
	m_player = Player::GetPlayer();
	
	// �ű��ļ�
	m_dataReader = new ScriptReader();

	// gui�Ĵ���
	m_sceneGUI = new hgeGUI();
	
	// ������ͼ������ӵ�gui
	m_theMap = new Map(1, this);		
	m_sceneGUI->AddCtrl(m_theMap);

	// ������Դ������
	m_rscManager = new SceneRscManager();
	
	// ������꾫��
	m_cursorSpr = new hgeSprite(0,0,0,32,32);

	// ���ó�ʼ�Ĵ��͵����Ŀ��С
	m_portals.reserve(10);

	// ���ó�ʼ��animation����Ŀ��С
	m_animations.reserve(20);

		
}

void SceneMode::Initialize()
{
	if(!m_dataReader->OpenFile("./dat/map/initial.lua")) 
	{
		return ;
	}
	
	call_function<void>(m_dataReader->GetThreadState(), "Initialize", this);
	
	// TODO: real closing
	m_dataReader->CloseFile();
}

void SceneMode::ChangeLocation(string lctName)
{	
	if(!m_dataReader->OpenFile(lctName)) 
	{
		return ;
	}
	
	// ���ýű�������ʵ����Ϸ���ݵĶ�ȡ
	call_function<void>(m_dataReader->GetThreadState(), "ChangeLocation", this);

	m_dataReader->CloseFile();
}


int SceneMode::LoadAnimation(string texPath, int frams, float fps, int tx, int ty, int w, int h)
{
	
	return m_rscManager->LoadAnimation(texPath, frams, fps, tx, ty, w, h);
}

void SceneMode::AddAnimation(int index, int x, int y)
{
	m_animations.push_back(SceneAnimation(m_rscManager->GetAnimation(index), x, y));	
}	

void SceneMode::ClearAnimations()
{
	m_animations.clear();
}

void SceneMode::RenderAnimation()
{
	static vector<SceneAnimation>::iterator it;

	for(it = m_animations.begin(); it != m_animations.end(); ++it)
	{
		(*it).Render();
	}
}

void SceneMode::UpdateAnimations(float dt)
{
	static vector<SceneAnimation>::iterator it;

	for(it = m_animations.begin(); it != m_animations.end(); ++it)
	{
		(*it).Update(dt, m_theMap->GetOffSetX(), m_theMap->GetOffSetY());
	}
}

SceneMode::~SceneMode()
{
	delete m_portalSpr;
	m_hge->Texture_Free(m_portalTex);

	delete m_cursorSpr;
	m_hge->Texture_Free(m_cursorTex);
	
	delete m_dataReader;
	delete m_rscManager;
	delete m_theMap;
	delete m_sceneGUI;
}

//! Updates the state of the game mode.
bool SceneMode::Update()
{
	float dt = m_hge->Timer_GetDelta();
	
	m_sceneGUI->Update(dt);
	m_player->Update(dt);
	UpdateAnimations(dt);
	UpdatePortals(dt);
	
	return false;
}

//! Draws the next screen frame for the game mode.
bool SceneMode::Draw()
{	
	// Render graphics
	m_hge->Gfx_BeginScene();
	m_hge->Gfx_Clear(0);

	
	m_sceneGUI->Render(); //��Ϊͼ��ʵ�����ƣ�m_player->Render();������������
	
	RenderAnimation();	
	RenderPortals();
	

	m_hge->Gfx_EndScene();
	
	return false;
}

void SceneMode::PlayBGM()
{

}

void SceneMode::StopBGM()
{

}



void SceneMode::SetCursor(string name, int startX, int startY)
{
	m_cursorTex = m_hge->Texture_Load(name.c_str());  
	m_cursorSpr->SetTexture(m_cursorTex);
	m_cursorSpr->SetTextureRect(startX,startY,32,32);
	m_sceneGUI->SetCursor(m_cursorSpr);
}

void SceneMode::LoadMap(string bgName, string bgMaskName, int gw, int gh, int w, int h)
{
	m_theMap->Load(bgName, bgMaskName, gw, gh, w, h, (*m_dataReader));
}

void SceneMode::InitRscManager(int anmNumber)
{
	m_rscManager->Initialize(anmNumber);
}

void SceneMode::ClearRscManager()
{
	m_rscManager->Clear();
}

void SceneMode::FreeMap()
{
	m_theMap->Free();
}

void SceneMode::LoadPortalAnm(string texPath, int frams, float fps, int tx, int ty, int w, int h)
{
	m_portalTex = m_hge->Texture_Load(texPath.c_str());
	m_portalSpr = new hgeAnimation(m_portalTex, frams, fps, tx, ty, w, h);
}

void SceneMode::AddPortal(int x, int y, string destination)
{
	m_portals.push_back(Portal(*m_portalSpr, x, y, destination));
}

void SceneMode::RenderPortals()
{
	//m_portals

	static vector<Portal>::iterator it;

	for(it = m_portals.begin(); it != m_portals.end(); ++it)
	{
		(*it).Render();
	}
}

void SceneMode::UpdatePortals(float dt)
{
	static vector<Portal>::iterator it;

	for(it = m_portals.begin(); it != m_portals.end(); ++it)
	{
		(*it).Update(dt, m_theMap->GetOffSetX(), m_theMap->GetOffSetY());

		if((*it).Active())
		{
			ChangeLocation((*it).GetDestination());
			m_theMap->ClearPaces();
			break;
		}
	}
}

void SceneMode::ClearPortal()
{
	m_portals.clear();
}


void SceneMode::SetPlayerPoint(int x, int y)
{
	m_theMap->SetPlayerLocation(x,y);
}



void SceneMode::Reset()
{

}


// -------- Portal --------


Portal::Portal(const hgeAnimation &anm, int mx, int my, string destination)
{
	m_thePly = Player::GetPlayer();
	m_des = destination;
	m_anm = new SceneAnimation(anm, mx, my);
	m_w = anm.GetWidth(); 
	m_h = anm.GetHeight();
}	

Portal::Portal(const Portal &thePortal)
{
	m_thePly = thePortal.m_thePly;
	m_des = thePortal.m_des;
	m_anm = new SceneAnimation(*thePortal.m_anm);
	m_w = thePortal.m_w; 
	m_h = thePortal.m_h;
}	


Portal::~Portal()
{
	delete m_anm;
}


void Portal::Update(float dt, int offsetX, int offsetY)
{
	m_anm->Update(dt, offsetX, offsetY);
}

void Portal::Render()
{
	m_anm->Render();
}

bool Portal::Active()
{
	static int gapX = 0;
	static int gapY = 0;
	
	// �����͵㲻����Ұ�ڣ��϶�������봫�͵�
	if(!m_anm->IsInView())
	{
		return false;
	}
	
	// �����͵�����Ұ�ڣ��ж��Ƿ���뵽���͵�
	gapX = m_thePly->GetFootX() - m_anm->GetWindowX();
	gapY = m_thePly->GetFootY() - m_anm->GetWindowY();	
	if((gapX >= 0) && (gapX <= m_w) && (gapY >= m_h/2)  && (gapY <= 128))
	{
		return true;
	}
	else
	{
		return false;
	}
}

string Portal::GetDestination()
{
	
	return m_des;
}



} // namespace scene