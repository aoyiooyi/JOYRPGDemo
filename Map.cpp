#include "Map.h"

#include <cmath>

#include "SceneMode.h"
#include "Player.h"
#include "ScriptReader.h"

namespace mode
{

// --------------------------- Map Mode --------------------------------

Map::Map(int theID, SceneMode* scene) //
{
	// 初始化八个方向的位移量
	m_gaps[BACK_DRC_UP].x =     0;  m_gaps[BACK_DRC_UP].y =     -1;
	m_gaps[BACK_DRC_LEFT].x =  -1;  m_gaps[BACK_DRC_LEFT].y =    0;
	m_gaps[BACK_DRC_WN].x =    -1;  m_gaps[BACK_DRC_WN].y =     -1;
	m_gaps[BACK_DRC_EN].x =     1;  m_gaps[BACK_DRC_EN].y =     -1;
	m_gaps[BACK_DRC_WS].x =    -1;  m_gaps[BACK_DRC_WS].y =		 1;
	m_gaps[BACK_DRC_ES].x =     1;  m_gaps[BACK_DRC_ES].y =      1;
	m_gaps[BACK_DRC_RIGHT].x =  1;  m_gaps[BACK_DRC_RIGHT].y =   0;
	m_gaps[BACK_DRC_DOWN].x =   0;  m_gaps[BACK_DRC_DOWN].y =    1;



	// 控件id
	id = theID;
		
	// 创建背景精灵，未读取图片，以黑色为texture
	m_bgImg = 0;
	m_rightX = 0;
	m_rightY = 0;
	m_bgSpr = new hgeSprite(0,m_rightX,m_rightY,800,600);
	m_bgSpr->SetColor(0xFFFFFFFF);
	m_bgSpr->SetZ(1.0f);
	
	// 创建遮挡精灵，未读取图片，以黑色为texture
	m_bgMaskImg = 0;
	m_bgMaskSpr = new hgeSprite(0,m_rightX,m_rightY,800,600);
	m_bgMaskSpr->SetColor(0x80FFFFFF);
	m_bgMaskSpr->SetZ(0.3f);

	// 创建寻路信息数据结构
	m_grids = new Point*[GRID_MAX];
	for(int i = 0; i < GRID_MAX; ++i)
	{
		m_grids[i] = new Point[GRID_MAX];
	}

	// 创建地图逻辑层数据结构
	m_gridTypes = new int*[GRID_MAX];
	for(int i = 0; i < GRID_MAX; ++i)
	{
		m_gridTypes[i] = new int[GRID_MAX]; 
	}
	
	// other
	m_thePly = Player::GetPlayer();
	m_theScene = scene;
	m_paceAmount = -1; //因为被作为数组索引，所以不能初始化为0
	
	bStatic=false;
	bVisible=true;
	bEnabled=true;

	rect.Set(0, 0, 800, 600);
}

Map::~Map()
{	
	// 释放地图逻辑层数据内存
	for(int i = 0; i < GRID_MAX; ++i)
	{
		delete[] m_gridTypes[i];
	}
	delete m_gridTypes;

	// 释放寻路信息数据内存
	for(int i = 0; i < GRID_MAX; ++i)
	{
		delete[] m_grids[i]; 
	}
	delete m_grids;
	
	// 释放精灵内存
	delete m_bgMaskSpr;
	delete m_bgSpr;
	hge->Texture_Free(m_bgMaskImg);
	hge->Texture_Free(m_bgImg);
}



void Map::FindPath(int npcX, int npcY, int mouseX, int mouseY)
{
	ResetGrids();
	ResetOpenQueue();
	
	// add the offset，get the coordinate 
	int startX = (npcX + m_rightX)/GRID_SIZE;
	int startY = (npcY + m_rightY)/GRID_SIZE;
	int endX = (mouseX + m_rightX)/GRID_SIZE;
	int endY = (mouseY + m_rightY)/GRID_SIZE;

	// 如果目标点是障碍点，则进行处理，
	// 把目标点重设为适当的可达的点
	if(m_grids[endY][endX].type == TYPE_BARRIER)
	{
		CalNewTarget(startX, startY, &endX, &endY);
	}

	bool find = false;

	if((startX == endX) && (startY == endY))
	{
		find = true;
	}
	
	// set the start point 
	m_grids[startY][startX].fstLen = 0;
	
	m_openPoints.Add(&m_grids[startY][startX]);
	
	
	while(!m_openPoints.Empty() && !find) //open表不为空
	{	
		Point *cenPoint = m_openPoints.Top();
		m_openPoints.Pop();
		cenPoint->open = false;
		cenPoint->close = true;
		
		// 对每一个点进行处理
		for(int i = 0; i < BACK_DRC_NUM; i++)
		{		
			// get the current point
			int curCdnX = cenPoint->x + m_gaps[i].x;
			int curCdnY = cenPoint->y + m_gaps[i].y;
			Point &curPoint = m_grids[curCdnY][curCdnX];
			
			// 如果不在open和close中，且不是障碍，
			// 则加入open，并计算相关值	
			if(!curPoint.open)
			{
				if(!(curPoint.type == TYPE_BARRIER) && !curPoint.close)
				{
					if(!(curCdnX == endX) || !(curCdnY == endY))
					{			
						//计算到起始点的距离
						curPoint.fstLen = cenPoint->fstLen + 1;

						//计算到目标点的距离
						curPoint.secLen = abs(curCdnX - endX) + abs(curCdnY - endY);
						
						// 相加
						curPoint.totalLen = curPoint.fstLen + curPoint.secLen;

						// 设置回溯的方向
						curPoint.direciton = GetOppositeDirection(i);

						// 加入open表
						m_openPoints.Add(&curPoint);
						curPoint.open = true;
					}
					else
					{
						// 设置回溯的方向
						curPoint.direciton = GetOppositeDirection(i);

						// 标志设为找到了
						find =true;	
					}
				}	
			}
			// 如果在open中，则查看是否需要更新fstLen
			else
			{
				if((cenPoint->fstLen + 1) < curPoint.fstLen)
				{
					// 计算新的fstLen和totalLen
					m_openPoints.ModifyFstLen(curPoint.x, curPoint.y, 
						cenPoint->fstLen+1, GetOppositeDirection(i));
				}
			}
		}
	}

	if(find)
	{
		BackPath(startX, startY, endX, endY);
	}
	else
	{
		// do nothing
	}
}

void Map::BackPath(int startX, int startY, int endX, int endY) 
{
	int curX = endX;
	int curY = endY;
	// 图书写形式的方便，而采用引用导致的错误
	// Point &curPoint = m_grids[curY][curX]; 
	Point *curPoint = &m_grids[curY][curX]; 
	
	int paceCount = 0;
	while(!(curPoint->x == startX) || !(curPoint->y == startY))
	{
		m_pace[paceCount] = GetOppositeDirection(curPoint->direciton);
		++paceCount;

		curX = curPoint->x + m_gaps[curPoint->direciton].x;
		curY = curPoint->y + m_gaps[curPoint->direciton].y;
		curPoint = &m_grids[curY][curX];
	}

	m_paceAmount = paceCount - 1;
}

// 重置grids数据
void Map::ResetGrids()
{
	for(int h = 0; h < m_gridsHeight; ++h)
	{
		for(int w = 0; w < m_gridsWidth; ++w)
		{
			m_grids[h][w].open = false;
			m_grids[h][w].close = false;
			m_grids[h][w].direciton = 0;
			m_grids[h][w].fstLen = 0;
			m_grids[h][w].secLen = 0;
			m_grids[h][w].totalLen = 0;
		}
	}
}

// 重置openqueue数据
void Map::ResetOpenQueue()
{
	m_openPoints.clear();
}

void Map::CalNewTarget(int sx, int sy, int *ex, int *ey)
{
	int rx = sx - (*ex);
	int ry = sy - (*ey);

	// 取绝对值大的数的绝对值为除数
	double divisor  = (abs(rx) >= abs(ry)) ? abs(rx) : abs(ry);
	
	//排除除数为0的状况
	//本不需要考虑rx，ry都为0的状况，因为玩家的坐标不应该在障碍点上
	//但是为了某些测试情况的意外，所以加上这个处理
	if(divisor == 0)
	{
		if(rx < 0)
		{
			divisor = -rx;
		}
		else if(ry < 0)
		{
			divisor = -ry;
		}
		else if(rx == 0)
		{
			divisor = 1;
		}
	}

	double ix = rx/divisor;
	double iy = ry/divisor;
	
	double curx = (*ex) + ix;
	double cury = (*ey) + iy;
	
	// 这里假设总会找到一个可达的点
	while(m_grids[(int)cury][(int)curx].type == TYPE_BARRIER) 
	{
		curx += ix;
		cury += iy;
	}

	(*ex) = (int)curx;
	(*ey) = (int)cury;
}

void Map::Render()
{
	m_bgSpr->Render(0, 0);
	m_thePly->Render();
	m_bgMaskSpr->Render(0, 0);
}

void Map::Update(float dt)
{	
	static int smallPaceCount = 0;
	static int xMovement = 0;
	static int yMovement = 0;

	// 如果还有步数，就行走（更新坐标）
	if(m_paceAmount >= 0) // 
	{
		if(smallPaceCount == 0)
		{
			m_thePly->ChangeDrc(m_pace[m_paceAmount]);
		}
		
		// 使状态保持（切换）为跑动状态
		if(m_thePly->IsStand())
		{
			m_thePly->Run();
		}
			
		// 当前这一帧的位移量	
		xMovement =  PACE_SIZE * m_gaps[m_pace[m_paceAmount]].x;
		yMovement =  PACE_SIZE * m_gaps[m_pace[m_paceAmount]].y;
		
		// 处理X的移动
		if(m_xMoveState == MAP_MOVE_CENTRE) // 中心移动（地图移动）
		{
			// 更新地图Texture的X坐标
			m_rightX += xMovement;
			
			// 判断更新后的X坐标是否超出边界（地图移动到了边界）
			if(m_rightX >= m_rightBound) 
			{
				m_rightX = m_rightBound;
				m_xMoveState = MAP_MOVE_FREE; // 变更为自由移动状态
			}
			
			if(m_rightX <= m_leftBound)
			{
				m_rightX = m_leftBound;
				m_xMoveState = MAP_MOVE_FREE; // 变更为自由移动状态
			}

		}
		else if(m_xMoveState == MAP_MOVE_FREE) // 自由移动（人物移动）
		{
			// 更新人物的渲染X坐标
			m_thePly->Move(xMovement, 0);

			// 若人物走到中心，X坐标转换到中心移动状态
			if((m_rightX == m_leftBound && m_thePly->GetFootX() >= 400) ||
				(m_rightX == m_rightBound && m_thePly->GetFootX() <= 400))
			{
				m_xMoveState = MAP_MOVE_CENTRE;
			}
		}

		// 处理Y的移动
		if(m_yMoveState == MAP_MOVE_CENTRE) // 中心移动（地图移动）
		{
			// 更新地图Texture的Y坐标
			m_rightY += yMovement;
			
			// 判断更新后的X坐标是否超出边界（地图移动到了边界）
			if(m_rightY >= m_downBound || m_rightY <= m_upBound) 
			{
				m_rightY = (m_rightY >= m_downBound) ? m_downBound : m_upBound;
				m_yMoveState = MAP_MOVE_FREE;
			}
			
		}
		else if(m_yMoveState == MAP_MOVE_FREE)// 自由移动（人物移动）
		{
			// 更新人物的渲染Y坐标
			m_thePly->Move(0, yMovement);

			// 若人物走到中心，Y坐标转换到中心移动状态
			if((m_rightY == m_upBound && m_thePly->GetFootY() >= 300) ||
				(m_rightY == m_downBound && m_thePly->GetFootY() <= 300))
			{
				m_yMoveState = MAP_MOVE_CENTRE;
			}
		}
		
		// 因为x，y是分开处理的，为了效率，最后一次更新地图Texture的区域
		m_bgSpr->SetTextureRect(m_rightX, m_rightY, 800, 600);
		m_bgMaskSpr->SetTextureRect(m_rightX, m_rightY, 800, 600);
				
		
		// 更新步数信息，每一个pace(grid)分8次(帧)走完
		smallPaceCount = (smallPaceCount == 7) ? 0 : smallPaceCount+1;
		if(smallPaceCount == 0)
		{
			--m_paceAmount;
		}
	}
	else
	{
		// 如果没有步数了，保持在站立状态
		if(m_thePly->IsRun())
		{
			m_thePly->Stand();
		}
	}	
}

void Map::ClearPaces()
{
	m_paceAmount = -1;
}

// This method is called to notify the control
// that the left mouse button state has changed.
// If it returns true - the caller will receive
// the control's ID
bool Map::MouseRButton(bool bDown)
{
	static float mouseX = 0;
	static float mouseY = 0;


	// 取得鼠标右击的坐标
	// 传入寻路函数
	if(bDown)
	{
		hge->Input_GetMousePos(&mouseX, &mouseY);

		FindPath(m_thePly->GetFootX(), m_thePly->GetFootY(), mouseX, mouseY);
	}

	return false;	
}

int Map::GetOffSetX()
{
	
	return m_rightX;
}

int Map::GetOffSetY()
{
	
	return m_rightY;
}

void Map::Load(string bgName, string bgMaskName, int gw, int gh, int w, int h, ScriptReader &reader)
{		
	// 读取背景精灵
	m_bgImg = hge->Texture_Load(bgName.c_str());
	m_bgSpr->SetTexture(m_bgImg);
	
	// 读取背景遮罩精灵
	m_bgMaskImg = hge->Texture_Load(bgMaskName.c_str());
	m_bgMaskSpr->SetTexture(m_bgMaskImg);
	m_bgMaskSpr->SetZ(0.1);

	// 读取地图大小
	m_gridsWidth = gw;
	m_gridsHeight = gh;	
	m_width = w;
	m_height = h; 

	// 设置移动边界
	m_leftBound = 0;
	m_rightBound = m_width - 800;
	m_upBound = 0;
	m_downBound = m_height - 600;

	// 读取逻辑层信息
	for(int h = 0; h < m_gridsHeight; ++h)
	{
		for(int w = 0; w < m_gridsWidth; ++w)
		{
			m_grids[h][w].x = w;
			m_grids[h][w].y = h;
		}
	}	

	// 读取逻辑层信息	
	reader.Read2DVector("mapGrids", m_gridTypes);
	for(int h = 0; h < m_gridsHeight; ++h)
	{
		for(int w = 0; w < m_gridsWidth; ++w)
		{
			m_grids[h][w].type = m_gridTypes[h][w];
		}
	}
}

void Map::Free()
{
	// 释放旧的背景精灵的texture
	if(m_bgImg != 0)
	{
		hge->Texture_Free(m_bgImg);
		m_bgImg = 0;
	}

	// 释放旧的背景遮罩精灵的texture
	if(m_bgMaskImg != 0)
	{
		hge->Texture_Free(m_bgMaskImg);
		m_bgMaskImg = 0;
	}
}

void Map::SetPlayerLocation(int x, int y)
{
	int fx = 0;
	int fy = 0;
	
	int offSetX = m_thePly->GetOffSetX();
	int offSetY = m_thePly->GetOffSetY();

	// 如果人物不完全在地图里，重设为边缘值
	if(x>=0 && x<400)
	{
		m_xMoveState = MAP_MOVE_FREE;
		m_rightX = 0;
		fx = x - m_rightX;
	}
	else if((x > (m_width - 400)) && (x <= m_width))
	{
		m_xMoveState = MAP_MOVE_FREE;
		m_rightX = m_width - 800;
		fx = x - m_rightX;
	}
	else
	{
		m_xMoveState = MAP_MOVE_CENTRE;
		m_rightX = x - 400;
		fx = 400;
	}

	
	// 如果人物不完全在地图里，重设为边缘值
	if(y>=0 && y<300)
	{
		m_yMoveState = MAP_MOVE_FREE;
		m_rightY = 0;
		fy = y - m_rightY;
	}
	else if((y > (m_height - 300)) && (y <= m_height))
	{
		m_yMoveState = MAP_MOVE_FREE;
		m_rightY = m_height - 600;
		fy = y - m_rightY;
	}
	else
	{
		m_yMoveState = MAP_MOVE_CENTRE;
		m_rightY = y-300;
		fy = 300;
	}

	m_thePly->SetFootX(fx);
	m_thePly->SetFootY(fy);
	m_bgSpr->SetTextureRect(m_rightX, m_rightY, 800, 600);
	m_bgMaskSpr->SetTextureRect(m_rightX, m_rightY, 800, 600);
}

//---------------------------------------





// ----------------- Open Queue -----------------------	
Map::OpenQueue::OpenQueue()
{
	// TODO:合适的大小
	m_points.reserve(35000);
}

void Map::OpenQueue::Add(Point *thePoint)
{
	m_points.push_back(thePoint);
	sort(m_points.size());
}

void Map::OpenQueue::sort(int order)
{
	int curIndex = order;
	
	// 排序
	bool finish = false;
	while(!finish && curIndex >= 2) // 堆里面不小于2个元素,或者没有排序结束，都继续循环
	{
		if(m_points[curIndex/2-1]->totalLen >= m_points[curIndex-1]->totalLen) // 当>时，加入的节点比父节点的优先权高，需要交换
																	   // 当==时，加入的节点肯定比它的父节点加入顺序大，所以交换
		{
			Point* temp = m_points[curIndex/2-1];
			m_points[curIndex/2-1] = m_points[curIndex-1];
			m_points[curIndex-1] = temp; 

			//往上继续比较排序
			curIndex = curIndex/2;
		}
		
		else // 不比父节点大，则满足了小根堆的性质，排序（循环）停止
		{
			// 但是在这种情况下，如果存在兄弟，还需要和兄弟节点对比
			if((curIndex+1)/2 != curIndex/2)
			{
				if(m_points[curIndex-2] == m_points[curIndex-1])
				{
					Point *temp = m_points[curIndex-2]; 
					m_points[curIndex-2] = m_points[curIndex-1];
					m_points[curIndex-1] = temp; 
				}
			}

			finish = true;
		}		
	}	
}

Map::Point *Map::OpenQueue::Top()
{
	
	return m_points.front();
}

void Map::OpenQueue::Pop()
{
	// 交换根节点和尾节点
	m_points[0] = m_points[m_points.size()-1];
	// 删除尾节点
	m_points.pop_back();

	// 对堆排序
	int curOrder = 1;
	int leftOrder = curOrder * 2;
	int rightOrder = curOrder * 2 + 1;
	int nextOrder = 0;
	bool finish = false;
	while(!finish && leftOrder <= m_points.size()) // 如果有至少一个子节点，才继续循环
	{	
		Point **largerPoint;
		
		// 一个子节点
		if(rightOrder > m_points.size())
		{
			largerPoint = &m_points[leftOrder-1];
			nextOrder = leftOrder;
		}
		// 两个子节点
		else 
		{
			// 选出优先权较高的子节点
			if(m_points[leftOrder-1]->totalLen <= m_points[rightOrder-1]->totalLen)
			{
				largerPoint = &m_points[leftOrder-1];
				nextOrder = leftOrder;
			}
			else
			{
				largerPoint = &m_points[rightOrder-1];
				nextOrder = rightOrder;
			}
		}
		
		// 如果父节点比子节点优先权低，就交换
		if(m_points[curOrder-1]->totalLen > (*largerPoint)->totalLen)
		{
			Point *temp = m_points[curOrder-1];
			m_points[curOrder-1] = *largerPoint;
			*largerPoint = temp;
		}
		// 如果父节点优先权比子节点大或相等，循环停止
		else
		{	
			finish = true;
		}

		curOrder = nextOrder;
		leftOrder = curOrder * 2;
		rightOrder = curOrder * 2 + 1;
	}	
}

void Map::OpenQueue::ModifyFstLen(int x, int y, int len, int direction)
{
	
	Point *curPoint = 0;;
	for(int i = 0; i < m_points.size(); ++i)
	{
		// 如果找到，修改相关值，并跳出循环
		curPoint = m_points[i];
		if(curPoint->x == x && curPoint->y == y)
		{
			curPoint->fstLen = len;
			curPoint->totalLen = curPoint->fstLen + curPoint->secLen;
			
			// 设置新的回溯方向
			curPoint->direciton = direction;

			sort(i+1);

			break;
		}
	}
}

bool Map::OpenQueue::Empty(void)
{
	
	return m_points.empty();
}

void Map::OpenQueue::clear()
{
	m_points.clear();
}

int Map::GetHeight()
{
	
	return m_height;
}

}

