#include <airobot/c/SimpleRobot.h>
#define POWER 2.5
#define MATCH_LENGHT 20
#define PREFER_DISTANCE 300
#define SEARCH_DIS 100
#define GAP toRadians(12)
#define AWAY_FROM_WALL 50
#define HISTORY_LENGHT 2000
double velocityRecord[HISTORY_LENGHT];
double headingRecord[HISTORY_LENGHT];
//移动的方向,这个变量的值只能取1和-1，1表示前进，-1表示后退
int direction = 1;
//在一个方向上最大的移动时间
long maxTime = 20;
//当前在一个方向上的移动时间
long moveTime = 0;
double lastHeading;
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
int currentIndex;
//判断给定的点是否是一个有效的目标点
int isValid(double x, double y);
struct virtualBullet {
double originX;
double originY;
double x;
double y;
double heading;
double velocity;
}virtualBullet;
void record(struct Bot* bot);
//计算最佳的匹配点
int getMatchIndex();
//得到开火的位置
void getFirePoint(int matchIndex, double power, double* fireX, double* fireY);

void onRoundBegin(struct RoundBeginAction* action)
{
	currentIndex = 0;
}

void onTick(struct TickAction* action)
{
	double nextX, nextY, dis, bea, nextHeading;
	long time;
	struct Bot* bot = getFirstOpponent();
	if(bot==NULL) return;
	
	//计算机器人两个单位时间的方向的夹角
	bea = bearing(bot->heading, lastHeading);
	//初始化迭代
	nextHeading = bot->heading;
	nextX = bot->x; 
	nextY = bot->y;
	time = 0;
	
	//进入迭代
	while(1)
	{
		//判断子弹在time时间后能否打到在next位置的对手
		double dis = distance(nextX, nextY, getX(), getY());
		if(dis/getBulletVelocity(POWER)<=time)
		{
			//能打到，迭代结束
			fireOnPoint(nextX, nextY, POWER); //开火
			break;
		}
		//进入下一次迭代
		time ++;
		nextHeading += bea;
		//计算在下个时间对手的位置
		nextPoint(nextX, nextY, nextHeading, bot->velocity, &nextX, &nextY);
	}
	lastHeading = bot->heading;
	setDirection();
	doMove();
	doTurn();
}
	
void setDirection(void)
{
	moveTime++;
	if(moveTime>=maxTime)
	{
		
		//改变运动方向
		direction *= -1;
		moveTime=0;
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
	headingTo = lineHeading + PI/2.65;
	//为了保持距离用deltaAngle对headingTo进行修正
	deltaAngle = getDeltaAngle(centerX, centerY);
	headingTo -= PI*deltaAngle;
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
		*x = getCourtWidth()/2.5;
		*y = getCourtHeight()/2.5;
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
	deltaDistance = deltaDistance/1000;
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
int main(int argC, char* argV[])
{
	tickHook = onTick;
	return startup(argC, argV);
}
