#include <airobot/c/SimpleRobot.h> 
#define POWER 0.7
#define MATCH_LENGTH 20
#define HISTORY_LENGTH 2000
	
#define PREFER_DISTANCE 450
#define SEARCH_DIS 300
#define GAP toRadians(6)
#define AWAY_FROM_WALL 30
double velocityRecord[HISTORY_LENGTH];  //射击
double headingRecord[HISTORY_LENGTH];
int currentIndex;
void record(struct Bot* bot);
int getMatchlndex();
void getFirePoint (int matchindex, double power, double* fireX, double* fireY);
void onRoundBegin(struct RoundBeginAction* action)
{
	currentIndex = 0;
}

int direction = 1;                      //运动
long maxTime = 25;
long moveTime = 0;
void setDirection(void);
void doMove(void);
void doTurn(void);
void getCenter(double* x, double* y);
double getDeltaAngle(double centerX, double centerY);
double getNeedTurn(double headingTo);
int isValid(double x, double y);   //判定目标点

void onTick(struct TickAction* action)
{
	int matchIndex;
	double fireX, fireY;
	struct Bot* opponent = getFirstOpponent();
	if(opponent==NULL)
		return;
	record(opponent);
	matchIndex = getMatchlndex();
	getFirePoint(matchIndex, POWER, &fireX, &fireY);
	fireOnPoint(fireX, fireY, POWER);
	setDirection();
	doMove();
	doTurn();
}
void record(struct Bot* bot)
{
	velocityRecord[currentIndex] = bot->velocity;
	headingRecord[currentIndex] = bot->heading;
	currentIndex++;
}
int getMatchlndex()
{
	double beatSimilarity=1000000;
	int matchIndex=0, i, j;
	for(i=MATCH_LENGTH; i < currentIndex-MATCH_LENGTH; i++)
	{
		double similarity=0;
		for(j=1; j<=MATCH_LENGTH; j++)
		{
			similarity+=fabs(velocityRecord[i-j]-velocityRecord[currentIndex-j]);
			similarity+=fabs(headingRecord[i-j]-headingRecord[currentIndex-j]);
		}
		if (similarity<beatSimilarity)
		{
			matchIndex=i;
			beatSimilarity=similarity;
		}
	}
	return matchIndex;
}
//得到开火的位置
void getFirePoint(int matchIndex, double power, double* fireX, double* fireY)
{//预测位置
	double x = getFirstOpponent()->x;
	double y = getFirstOpponent()->y;
	double dis;
	int time = 0;
	while (matchIndex + time < currentIndex)
	{
	dis = distance(getX(), getY(), x, y);
	if(dis/getBulletVelocity(power)<=time)
		break;
	nextPoint(x, y, headingRecord[matchIndex+time], velocityRecord[matchIndex+time], &x, &y);
	time++;
	}
	*fireX = x;
	*fireY = y;
}

void setDirection(void)  //随机运动
{
	int a=20, b=30;
	moveTime++;
	if(moveTime>=maxTime)
	{
		moveTime=0;
		//随机生成在这个方向上移动的时间
		maxTime = rand()%(b-a+1)+a;
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
	deltaAngle = getDeltaAngle(centerX, centerY);   //保持距离
	headingTo -= deltaAngle;
	needTurn = getNeedTurn(headingTo);   //不撞墙
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
	deltaDistance = deltaDistance/500;
	return direction*PI/3*deltaDistance;
}
double getNeedTurn(double headingTo)
{
	double needTurn = 0, nextX, nextY;
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

int main(int argC, char* argV[])
{
	tickHook = onTick;
	roundBeginHook = onRoundBegin;
	return startup(argC, argV);
}
