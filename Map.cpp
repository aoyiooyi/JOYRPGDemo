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
	// ��ʼ���˸������λ����
	m_gaps[BACK_DRC_UP].x =     0;  m_gaps[BACK_DRC_UP].y =     -1;
	m_gaps[BACK_DRC_LEFT].x =  -1;  m_gaps[BACK_DRC_LEFT].y =    0;
	m_gaps[BACK_DRC_WN].x =    -1;  m_gaps[BACK_DRC_WN].y =     -1;
	m_gaps[BACK_DRC_EN].x =     1;  m_gaps[BACK_DRC_EN].y =     -1;
	m_gaps[BACK_DRC_WS].x =    -1;  m_gaps[BACK_DRC_WS].y =		 1;
	m_gaps[BACK_DRC_ES].x =     1;  m_gaps[BACK_DRC_ES].y =      1;
	m_gaps[BACK_DRC_RIGHT].x =  1;  m_gaps[BACK_DRC_RIGHT].y =   0;
	m_gaps[BACK_DRC_DOWN].x =   0;  m_gaps[BACK_DRC_DOWN].y =    1;



	// �ؼ�id
	id = theID;
		
	// �����������飬δ��ȡͼƬ���Ժ�ɫΪtexture
	m_bgImg = 0;
	m_rightX = 0;
	m_rightY = 0;
	m_bgSpr = new hgeSprite(0,m_rightX,m_rightY,800,600);
	m_bgSpr->SetColor(0xFFFFFFFF);
	m_bgSpr->SetZ(1.0f);
	
	// �����ڵ����飬δ��ȡͼƬ���Ժ�ɫΪtexture
	m_bgMaskImg = 0;
	m_bgMaskSpr = new hgeSprite(0,m_rightX,m_rightY,800,600);
	m_bgMaskSpr->SetColor(0x80FFFFFF);
	m_bgMaskSpr->SetZ(0.3f);

	// ����Ѱ·��Ϣ���ݽṹ
	m_grids = new Point*[GRID_MAX];
	for(int i = 0; i < GRID_MAX; ++i)
	{
		m_grids[i] = new Point[GRID_MAX];
	}

	// ������ͼ�߼������ݽṹ
	m_gridTypes = new int*[GRID_MAX];
	for(int i = 0; i < GRID_MAX; ++i)
	{
		m_gridTypes[i] = new int[GRID_MAX]; 
	}
	
	// other
	m_thePly = Player::GetPlayer();
	m_theScene = scene;
	m_paceAmount = -1; //��Ϊ����Ϊ�������������Բ��ܳ�ʼ��Ϊ0
	
	bStatic=false;
	bVisible=true;
	bEnabled=true;

	rect.Set(0, 0, 800, 600);
}

Map::~Map()
{	
	// �ͷŵ�ͼ�߼��������ڴ�
	for(int i = 0; i < GRID_MAX; ++i)
	{
		delete[] m_gridTypes[i];
	}
	delete m_gridTypes;

	// �ͷ�Ѱ·��Ϣ�����ڴ�
	for(int i = 0; i < GRID_MAX; ++i)
	{
		delete[] m_grids[i]; 
	}
	delete m_grids;
	
	// �ͷž����ڴ�
	delete m_bgMaskSpr;
	delete m_bgSpr;
	hge->Texture_Free(m_bgMaskImg);
	hge->Texture_Free(m_bgImg);
}



void Map::FindPath(int npcX, int npcY, int mouseX, int mouseY)
{
	ResetGrids();
	ResetOpenQueue();
	
	// add the offset��get the coordinate 
	int startX = (npcX + m_rightX)/GRID_SIZE;
	int startY = (npcY + m_rightY)/GRID_SIZE;
	int endX = (mouseX + m_rightX)/GRID_SIZE;
	int endY = (mouseY + m_rightY)/GRID_SIZE;

	// ���Ŀ������ϰ��㣬����д���
	// ��Ŀ�������Ϊ�ʵ��Ŀɴ�ĵ�
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
	
	
	while(!m_openPoints.Empty() && !find) //open��Ϊ��
	{	
		Point *cenPoint = m_openPoints.Top();
		m_openPoints.Pop();
		cenPoint->open = false;
		cenPoint->close = true;
		
		// ��ÿһ������д���
		for(int i = 0; i < BACK_DRC_NUM; i++)
		{		
			// get the current point
			int curCdnX = cenPoint->x + m_gaps[i].x;
			int curCdnY = cenPoint->y + m_gaps[i].y;
			Point &curPoint = m_grids[curCdnY][curCdnX];
			
			// �������open��close�У��Ҳ����ϰ���
			// �����open�����������ֵ	
			if(!curPoint.open)
			{
				if(!(curPoint.type == TYPE_BARRIER) && !curPoint.close)
				{
					if(!(curCdnX == endX) || !(curCdnY == endY))
					{			
						//���㵽��ʼ��ľ���
						curPoint.fstLen = cenPoint->fstLen + 1;

						//���㵽Ŀ���ľ���
						curPoint.secLen = abs(curCdnX - endX) + abs(curCdnY - endY);
						
						// ���
						curPoint.totalLen = curPoint.fstLen + curPoint.secLen;

						// ���û��ݵķ���
						curPoint.direciton = GetOppositeDirection(i);

						// ����open��
						m_openPoints.Add(&curPoint);
						curPoint.open = true;
					}
					else
					{
						// ���û��ݵķ���
						curPoint.direciton = GetOppositeDirection(i);

						// ��־��Ϊ�ҵ���
						find =true;	
					}
				}	
			}
			// �����open�У���鿴�Ƿ���Ҫ����fstLen
			else
			{
				if((cenPoint->fstLen + 1) < curPoint.fstLen)
				{
					// �����µ�fstLen��totalLen
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
	// ͼ��д��ʽ�ķ��㣬���������õ��µĴ���
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

// ����grids����
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

// ����openqueue����
void Map::ResetOpenQueue()
{
	m_openPoints.clear();
}

void Map::CalNewTarget(int sx, int sy, int *ex, int *ey)
{
	int rx = sx - (*ex);
	int ry = sy - (*ey);

	// ȡ����ֵ������ľ���ֵΪ����
	double divisor  = (abs(rx) >= abs(ry)) ? abs(rx) : abs(ry);
	
	//�ų�����Ϊ0��״��
	//������Ҫ����rx��ry��Ϊ0��״������Ϊ��ҵ����겻Ӧ�����ϰ�����
	//����Ϊ��ĳЩ������������⣬���Լ����������
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
	
	// ��������ܻ��ҵ�һ���ɴ�ĵ�
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

	// ������в����������ߣ��������꣩
	if(m_paceAmount >= 0) // 
	{
		if(smallPaceCount == 0)
		{
			m_thePly->ChangeDrc(m_pace[m_paceAmount]);
		}
		
		// ʹ״̬���֣��л���Ϊ�ܶ�״̬
		if(m_thePly->IsStand())
		{
			m_thePly->Run();
		}
			
		// ��ǰ��һ֡��λ����	
		xMovement =  PACE_SIZE * m_gaps[m_pace[m_paceAmount]].x;
		yMovement =  PACE_SIZE * m_gaps[m_pace[m_paceAmount]].y;
		
		// ����X���ƶ�
		if(m_xMoveState == MAP_MOVE_CENTRE) // �����ƶ�����ͼ�ƶ���
		{
			// ���µ�ͼTexture��X����
			m_rightX += xMovement;
			
			// �жϸ��º��X�����Ƿ񳬳��߽磨��ͼ�ƶ����˱߽磩
			if(m_rightX >= m_rightBound) 
			{
				m_rightX = m_rightBound;
				m_xMoveState = MAP_MOVE_FREE; // ���Ϊ�����ƶ�״̬
			}
			
			if(m_rightX <= m_leftBound)
			{
				m_rightX = m_leftBound;
				m_xMoveState = MAP_MOVE_FREE; // ���Ϊ�����ƶ�״̬
			}

		}
		else if(m_xMoveState == MAP_MOVE_FREE) // �����ƶ��������ƶ���
		{
			// �����������ȾX����
			m_thePly->Move(xMovement, 0);

			// �������ߵ����ģ�X����ת���������ƶ�״̬
			if((m_rightX == m_leftBound && m_thePly->GetFootX() >= 400) ||
				(m_rightX == m_rightBound && m_thePly->GetFootX() <= 400))
			{
				m_xMoveState = MAP_MOVE_CENTRE;
			}
		}

		// ����Y���ƶ�
		if(m_yMoveState == MAP_MOVE_CENTRE) // �����ƶ�����ͼ�ƶ���
		{
			// ���µ�ͼTexture��Y����
			m_rightY += yMovement;
			
			// �жϸ��º��X�����Ƿ񳬳��߽磨��ͼ�ƶ����˱߽磩
			if(m_rightY >= m_downBound || m_rightY <= m_upBound) 
			{
				m_rightY = (m_rightY >= m_downBound) ? m_downBound : m_upBound;
				m_yMoveState = MAP_MOVE_FREE;
			}
			
		}
		else if(m_yMoveState == MAP_MOVE_FREE)// �����ƶ��������ƶ���
		{
			// �����������ȾY����
			m_thePly->Move(0, yMovement);

			// �������ߵ����ģ�Y����ת���������ƶ�״̬
			if((m_rightY == m_upBound && m_thePly->GetFootY() >= 300) ||
				(m_rightY == m_downBound && m_thePly->GetFootY() <= 300))
			{
				m_yMoveState = MAP_MOVE_CENTRE;
			}
		}
		
		// ��Ϊx��y�Ƿֿ�����ģ�Ϊ��Ч�ʣ����һ�θ��µ�ͼTexture������
		m_bgSpr->SetTextureRect(m_rightX, m_rightY, 800, 600);
		m_bgMaskSpr->SetTextureRect(m_rightX, m_rightY, 800, 600);
				
		
		// ���²�����Ϣ��ÿһ��pace(grid)��8��(֡)����
		smallPaceCount = (smallPaceCount == 7) ? 0 : smallPaceCount+1;
		if(smallPaceCount == 0)
		{
			--m_paceAmount;
		}
	}
	else
	{
		// ���û�в����ˣ�������վ��״̬
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


	// ȡ������һ�������
	// ����Ѱ·����
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
	// ��ȡ��������
	m_bgImg = hge->Texture_Load(bgName.c_str());
	m_bgSpr->SetTexture(m_bgImg);
	
	// ��ȡ�������־���
	m_bgMaskImg = hge->Texture_Load(bgMaskName.c_str());
	m_bgMaskSpr->SetTexture(m_bgMaskImg);
	m_bgMaskSpr->SetZ(0.1);

	// ��ȡ��ͼ��С
	m_gridsWidth = gw;
	m_gridsHeight = gh;	
	m_width = w;
	m_height = h; 

	// �����ƶ��߽�
	m_leftBound = 0;
	m_rightBound = m_width - 800;
	m_upBound = 0;
	m_downBound = m_height - 600;

	// ��ȡ�߼�����Ϣ
	for(int h = 0; h < m_gridsHeight; ++h)
	{
		for(int w = 0; w < m_gridsWidth; ++w)
		{
			m_grids[h][w].x = w;
			m_grids[h][w].y = h;
		}
	}	

	// ��ȡ�߼�����Ϣ	
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
	// �ͷžɵı��������texture
	if(m_bgImg != 0)
	{
		hge->Texture_Free(m_bgImg);
		m_bgImg = 0;
	}

	// �ͷžɵı������־����texture
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

	// ������ﲻ��ȫ�ڵ�ͼ�����Ϊ��Եֵ
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

	
	// ������ﲻ��ȫ�ڵ�ͼ�����Ϊ��Եֵ
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
	// TODO:���ʵĴ�С
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
	
	// ����
	bool finish = false;
	while(!finish && curIndex >= 2) // �����治С��2��Ԫ��,����û�����������������ѭ��
	{
		if(m_points[curIndex/2-1]->totalLen >= m_points[curIndex-1]->totalLen) // ��>ʱ������Ľڵ�ȸ��ڵ������Ȩ�ߣ���Ҫ����
																	   // ��==ʱ������Ľڵ�϶������ĸ��ڵ����˳������Խ���
		{
			Point* temp = m_points[curIndex/2-1];
			m_points[curIndex/2-1] = m_points[curIndex-1];
			m_points[curIndex-1] = temp; 

			//���ϼ����Ƚ�����
			curIndex = curIndex/2;
		}
		
		else // ���ȸ��ڵ����������С���ѵ����ʣ�����ѭ����ֹͣ
		{
			// ��������������£���������ֵܣ�����Ҫ���ֵܽڵ�Ա�
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
	// �������ڵ��β�ڵ�
	m_points[0] = m_points[m_points.size()-1];
	// ɾ��β�ڵ�
	m_points.pop_back();

	// �Զ�����
	int curOrder = 1;
	int leftOrder = curOrder * 2;
	int rightOrder = curOrder * 2 + 1;
	int nextOrder = 0;
	bool finish = false;
	while(!finish && leftOrder <= m_points.size()) // ���������һ���ӽڵ㣬�ż���ѭ��
	{	
		Point **largerPoint;
		
		// һ���ӽڵ�
		if(rightOrder > m_points.size())
		{
			largerPoint = &m_points[leftOrder-1];
			nextOrder = leftOrder;
		}
		// �����ӽڵ�
		else 
		{
			// ѡ������Ȩ�ϸߵ��ӽڵ�
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
		
		// ������ڵ���ӽڵ�����Ȩ�ͣ��ͽ���
		if(m_points[curOrder-1]->totalLen > (*largerPoint)->totalLen)
		{
			Point *temp = m_points[curOrder-1];
			m_points[curOrder-1] = *largerPoint;
			*largerPoint = temp;
		}
		// ������ڵ�����Ȩ���ӽڵ�����ȣ�ѭ��ֹͣ
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
		// ����ҵ����޸����ֵ��������ѭ��
		curPoint = m_points[i];
		if(curPoint->x == x && curPoint->y == y)
		{
			curPoint->fstLen = len;
			curPoint->totalLen = curPoint->fstLen + curPoint->secLen;
			
			// �����µĻ��ݷ���
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

