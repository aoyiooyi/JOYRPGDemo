#ifndef MAPMODE_H_
#define MAPMODE_H_

#include <string>
#include <vector>

#include "MediaEngineer.h"
#include "declare.h"

using std::vector;
using std::string;

using script::ScriptReader;
using actor::Player;
using mode::SceneMode;


namespace mode
{


class Map : public hgeGUIObject
{
public:
	Map(int theID, SceneMode* scene); 
	~Map();

	virtual void	Update(float dt);
	virtual void	Render();
	
	//virtual void	Enter();
	//virtual void	Leave();
	//virtual bool	IsDone();
	//virtual void	Focus(bool bFocused);
	//virtual void	MouseOver(bool bOver);

	//virtual bool	MouseLButton(bool bDown);
	virtual bool	MouseRButton(bool bDown);
	//virtual bool	KeyClick(int key, int chr);

	int GetOffSetX();
	int GetOffSetY();
	int GetHeight();

	// 暂时的,以后写到更换场景的相关函数里面
	void ClearPaces();

	void Load(string bgName, string bgMaskName, int gw, int gh, int w, int h, ScriptReader &reader);
	void Free();
	void SetPlayerLocation(int x, int y);
	

private:

	static const int BACK_DRC_NUM = 8;

	static const int BACK_DRC_UP = 0;
	static const int BACK_DRC_LEFT = 1;
	static const int BACK_DRC_WN = 2;
	static const int BACK_DRC_EN = 3;
	static const int BACK_DRC_WS = 4;
	static const int BACK_DRC_ES = 5;
	static const int BACK_DRC_RIGHT = 6;
	static const int BACK_DRC_DOWN = 7;

	static const int TYPE_BARRIER = 1;
	static const int TYPE_PORTAl = 2;

	static const int PACE_AMOUNT_MAX = 1875; // 800*600 / 16*16
	
	static const int GRID_SIZE = 16;
	static const int PACE_SIZE = 2;
	static const int GRID_MAX = 282;  // 4500 / 16

	typedef struct coordinate
	{
		int x,y;
	} Coordinate;

	typedef struct point
	{
		int x,y;
		int type;
		int totalLen;
		int fstLen;
		int secLen;
		int direciton;
		bool open;
		bool close;
		
		point()
		{
			open = false;
			close = false;
			x = 0;
			y = 0;
			type = 0;
			fstLen = 0;
			secLen = 0;
			totalLen = fstLen + secLen;
			direciton = 0;
		}
	} Point;

	class OpenQueue
	{
	public:
		OpenQueue();
		void ModifyFstLen(int x, int y, int len, int direction);
		void Add(Point* thePoint);
		Point *Top();
		void Pop();
		bool Empty(void);
		void clear();
	private:
		void sort(int order);
		vector<Point*> m_points;
	};

	int GetOppositeDirection(int index);
	void FindPath(int npcX, int npcY, int mouseX, int mouseY);
	void BackPath(int startX, int startY, int endX, int endY);
	void ResetGrids();
	void ResetOpenQueue();
	void CalNewTarget(int sx, int sy, int *ex, int *ey);

	Point **m_grids;
	int **m_gridTypes;
	OpenQueue m_openPoints;
	
	int m_rightX;
	int m_rightY;

	int m_gridsHeight;
	int m_gridsWidth;
	int m_height; 
	int	m_width;

	int m_pace[PACE_AMOUNT_MAX];
	int m_paceAmount;

	int m_leftBound;
	int m_rightBound;
	int m_upBound;
	int m_downBound;

	HTEXTURE m_bgImg;
	hgeSprite *m_bgSpr;
	
	HTEXTURE m_bgMaskImg;
	hgeSprite *m_bgMaskSpr;

	Player *m_thePly;
	SceneMode* m_theScene;

	int m_xMoveState;
	int m_yMoveState;
	static const int MAP_MOVE_CENTRE = 0;
	static const int MAP_MOVE_FREE = 1;

	Coordinate m_gaps[BACK_DRC_NUM];
};


inline int Map::GetOppositeDirection(int index)
{
	
	return BACK_DRC_NUM - 1 - index;
}


} // namespace mode

#endif // MAPMODE_H_