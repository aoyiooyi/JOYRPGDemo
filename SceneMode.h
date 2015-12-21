#ifndef SCENEMODE_H_
#define SCENEMODE_H_

#include <vector>

#include "GameMode.h"
#include "MediaEngineer.h"
#include "declare.h"

using std::vector;

using actor::Player;
using script::ScriptReader;
using media::SceneAnimation;

namespace mode
{

class Map;
class SceneRscManager;


class Portal
{
public:
	Portal(const hgeAnimation &anm, int x, int y, string destination);
	Portal::Portal(const Portal &thePortal);
	~Portal();

	void Update(float dt, int offsetX, int offsetY);
	void Render();
	
	// ��ҽ��봫�͵�Ļ����ͷ���true
	bool Active();

	// ���͵��Ŀ�ĵ�
	string GetDestination();
private:
	Player *m_thePly;
	string m_des;
	SceneAnimation *m_anm;
	int m_w, m_h;
};


class SceneMode : public GameMode
{
friend void thebind::BindModeToLua();

public:
	SceneMode();
	~SceneMode();

	//! Draws the next screen frame for the game mode.
	virtual bool Draw();
	
	//! Updates the state of the game mode.
	virtual bool Update();

	virtual void Reset();

	virtual void PlayBGM();
	virtual void StopBGM();
	
	void Initialize();
	void ChangeLocation(string lctName);
	void SetPlayerPoint(int x, int y);

	// TODO �Ƿ��������ʵ��
	void SetCursor(string name, int startX, int startY);
	
	
	
private:
	void LoadMap(string bgName, string bgMaskName, int gw, int gh, int w, int h);
	void FreeMap();
	
	void InitRscManager(int anmNumber);
	void ClearRscManager();
	
	void LoadPortalAnm(string texPath, int frams, float fps, int tx, int ty, int w, int h);
	void AddPortal(int x, int y, string destination);
	void ClearPortal();
	void RenderPortals();
	void UpdatePortals(float dt);
	
	int LoadAnimation(string texPath, int frams, float fps, int tx, int ty, int w, int h);
	void AddAnimation(int index, int x, int y); // û�мӷֺţ����汨�� error C3646: ��HGE��δ֪��д˵����
	void ClearAnimations();
	void RenderAnimation();
	void UpdateAnimations(float dt);
	
	HGE *m_hge;
	hgeGUI *m_sceneGUI;
	Map *m_theMap;
	SceneRscManager *m_rscManager;
	Player *m_player;

	// script
	ScriptReader *m_dataReader;
	
	//���
	HTEXTURE m_cursorTex;
	hgeSprite *m_cursorSpr;
	
	// ���͵�anm
	HTEXTURE m_portalTex;
	hgeAnimation *m_portalSpr;
	
	// ���͵�
	vector<Portal> m_portals;

	// animations
	vector<SceneAnimation> m_animations;
	
};




} // namespace mode


#endif // SCENEMODE_H_