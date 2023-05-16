#include <airobot/c/SimpleRobot.h>
#define PREFER_DISTANCE 300
#define MATCH_LENGHT 20
#define SEARCH_DIS 100
#define GAP toRadians(5)
#define HISTORY_LENGHT 2000
#define AWAY_FROM_WALL 50
#define POWER 2.5
double velocityRecord[HISTORY_LENGHT];
double headingRecord[HISTORY_LENGHT];	
double lastHeading;
struct bot
{
	int id;
	char *name;
	double x,y;
	double heading;
	double v;
	double energy;	
	double score;
	long firePrepareTime;
	int memberID;
};
int direction = 1;
long maxTime = 30;
long moveTime = 0;
void setDirection(void);
void doMove(void);
void doTurn(void);	
void getCenter(double* x, double* y);
double getDeltaAngle(double centerX, double centerY);
double getNeedTurn(double headingTo);
int isValid(double x, double y);
/**
 * ÿ����λʱ�䶼�ᴥ��
 */
void onTick(struct TickAction* action){
	double nextX , nextY,Dis , bea ,nextHeading;
	long time;
	struct bot *bot = getFirstOpponent();
	setDirection();
	doMove();
	doTurn();
	if(bot == NULL) return;
	bea = bearing(bot->heading , lastHeading);
	nextHeading = bot->heading;
	nextX = bot->x;
	nextY = bot->y;
	time = 0;
	while(1)
	{
		double Dis = distance(nextX,nextY,getX(),getY());
		if(Dis/getBulletVelocity(POWER)<=time)
		{
			fireOnPoint(nextX , nextY , POWER);
			break;
		}
		time++;
		nextHeading += bea;
		nextPoint(nextX,nextY,nextHeading,bot->v,&nextX,&nextY);
	}
	lastHeading = bot->heading;
}
void setDirection(void)
{
	moveTime++;
	if(moveTime>=maxTime)
	{
		moveTime=0;
		maxTime = rand()%60;
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
	deltaAngle = getDeltaAngle(centerX, centerY);
	headingTo -= deltaAngle;
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
/** 
 * ����ʼһ���µı���ʱ����
 */
void onMatchBegin(struct MatchBeginAction* action){}
/**
 * ��������������ʱ����
 */
void onMatchFinish(struct MatchFinishAction* action){}
/** 
 * ����ʼһ���µı���ʱ����
 */
void onRoundBegin(struct RoundBeginAction* action){}
/**
 * ��һ�ֱ�������ʱ������
 */
void onRoundFinish(struct RoundFinishAction* action){}

/**
 * ���ж������Լ�������Ϣʱ����
 */
void onMessageReceived(struct MessageReceivedAction* action){}
/**
 * ��ײ������������ʱ����
 */
void onHitRobot(struct HitRobotAction* action){}
/**
 * ��ײ��ǽʱ����
 */
void onHitWall(struct HitWallAction* action){}
/**
 * ������һ�������˿���ʱ����
 */
void onFire(struct FireAction* action){}
/**
 * ���л���������ʱ����
 */
void onRobotDeath(struct RobotDeathAction* action){}
/**
 * ���Լ����ӵ����е���ʱ����
 */
void onBulletHit(struct BulletHitAction* action){}
/**
 * �������˵��ӵ�����ʱ����
 */
void onHitedByBullet(struct HitedByBulletAction* action){}


/**
 * �����˳������
 */
int main(int argC, char* argV[])
{
	tickHook = onTick;
	roundBeginHook = onRoundBegin;
	return startup(argC, argV);
}
