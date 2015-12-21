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
	
	// 玩家进入传送点的话，就返回true
	bool Active();

	// 传送点的目的地
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

	// TODO 是否放在这里实现
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
	void AddAnimation(int index, int x, int y); // 没有加分号，后面报错 error C3646: “HGE”未知重写说明符
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
	
	//鼠标
	HTEXTURE m_cursorTex;
	hgeSprite *m_cursorSpr;
	
	// 传送点anm
	HTEXTURE m_portalTex;
	hgeAnimation *m_portalSpr;
	
	// 传送点
	vector<Portal> m_portals;

	// animations
	vector<SceneAnimation> m_animations;
	
};




} // namespace mode


#endif // SCENEMODE_H_