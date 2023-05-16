#include <airobot/c/SimpleRobot.h>
//理想距离
#define PREFER_DISTANCE 300
//计算目标点时采用的搜索半径
#define SEARCH_DIS 100
//每次迭代的递增量
#define GAP toRadians(5)
//离墙的距离
#define AWAY_FROM_WALL 50
#define POWER 1.8
#define MATCH_LENGTH 20
#define HISTORY_LENGTH 2000
double velocityRecord[HISTORY_LENGTH];
double headingRecord[HISTORY_LENGTH];
int currentIndex;
int getMatchIndex();
void record(struct Bot* bot);
void getFirePoint (int matchIndex, double power, double* fireX, double* fireY);
//移动的方向,这个变量的值只能取1和-1，1表示前进，-1表示后退
int direction = 1;
//在一个方向上最大的移动时间
long maxTime = 30;
//当前在一个方向上的移动时间
long moveTime = 0;

//设置移动的方向
void setDirection(void);
//执行移动
void doMove(void);
//执行转动
void doTurn(void);
//得到圆心位置
void getCenter(double* x, double* y);
//得到用于保持理想距离的修正角deltaAngle
double getDeltaAngle(double centerX, double centerY);
//得到用来修正移动方向避免机器人撞墙的夹角needTurn
double getNeedTurn(double headingTo);
//判断给定的点是否是一个有效的目标点
int isValid(double x, double y);

void onTick(struct TickAction* action)
{
	int matchIndex;
	double fireX, fireY;
	struct Bot* opponent=getFirstOpponent ();
	setDirection();
	doMove();
	doTurn();
	if(opponent==NULL) return;
	record (opponent);
	matchIndex = getMatchIndex () ;
	getFirePoint (matchIndex,POWER,&fireX,&fireY) ;
	fireOnPoint (fireX, fireY, POWER);
}
void record(struct Bot* bot)
{
	velocityRecord[currentIndex] = bot->velocity;
	headingRecord[currentIndex] = bot->heading;
	currentIndex++;
}
int getMatchIndex()
{
	double beatSimilarity = 1000000;
	int matchIndex = 0,i,j;
	for(i=MATCH_LENGTH;i<currentIndex-MATCH_LENGTH;i++)
	{
		double similarity = 0;
		for(j=1;j<=MATCH_LENGTH;j++)
		{
			similarity += fabs(velocityRecord[i-j]-velocityRecord [currentIndex-j]);
			similarity += fabs(headingRecord[i-j]-headingRecord[currentIndex-j]) ;
		}
		if(similarity<beatSimilarity)
		{
			matchIndex = i;
			beatSimilarity = similarity;
		}
	}
	return matchIndex;
}
void getFirePoint(int matchIndex,double power,double* fireX,double* fireY)
{
	double x = getFirstOpponent()->x;
	double y = getFirstOpponent()->y;
	double dis;
	int time = 0;
	while(matchIndex+time<currentIndex)
	{
		dis = distance(getX(),getY(),x,y);
		if(dis/getBulletVelocity(power)<=time) break;
		nextPoint(x,y,headingRecord[matchIndex+time],velocityRecord[matchIndex+time],&x,&y);
		time++;
	}
	*fireX = x;
	*fireY = y;
}
void onRoundBegin(struct RoundBeginAction* action)
{
	currentIndex = 0;
}	
void setDirection(void)
{
	moveTime++;
	if(moveTime>=maxTime)
	{
		moveTime=0;
		//随机生成在这个方向上移动的时间
		maxTime = rand()%60;
		//改变运动方向
		direction *= -1;
	}
}

void doMove(void)
{
	move(10*direction);
}

void doTurn(void)
{
	double centerX, centerY, lineHeading, headingTo, deltaAngle, needTurn, bea;
	
	getCenter(&centerX, &centerY);
	lineHeading = heading(getX(), getY(), centerX, centerY);
	headingTo = lineHeading + PI/2;
	//为了保持距离用deltaAngle对headingTo进行修正
	deltaAngle = getDeltaAngle(centerX, centerY);
	headingTo -= deltaAngle;
	//为了不撞墙用needTurn对headingTo进行修正
	needTurn = getNeedTurn(headingTo);
	headingTo -= needTurn;
			
	bea = bearing(headingTo, getHeading());
	turn(bea);
}

void getCenter(double* x, double* y)
{
	struct Bot* bot = getFirstOpponent();
	if(bot==NULL)
	{
		*x = getCourtWidth()/2;
		*y = getCourtHeight()/2;
	}
	else
	{
		*x = bot->x;
		*y = bot->y;
	}
}

double getDeltaAngle(double centerX, double centerY)
{
	double dis = distance(centerX, centerY, getX(), getY());
	double deltaDistance = dis-PREFER_DISTANCE;
	//deltaDistance = deltaDistance/fmax(distance, PREFER_DISTANCE);
	deltaDistance = deltaDistance/500;
	return direction*PI/3*deltaDistance;
}

double getNeedTurn(double headingTo)
{
	double needTurn = 0, nextX, nextY;
	//进入迭代
	while(1)
	{
		nextPoint(getX(), getY(), headingTo-needTurn, 
			direction*SEARCH_DIS, &nextX, &nextY);
		if(isValid(nextX, nextY)) break;
		needTurn += direction*GAP;
	}
	return needTurn;
}

int isValid(double x, double y)
{
	return inCourt(x, y, AWAY_FROM_WALL);
}
		
//启动机器人程序