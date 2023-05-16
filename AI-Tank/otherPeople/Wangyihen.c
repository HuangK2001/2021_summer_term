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
 * 每个单位时间都会触发
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
 * 当开始一场新的比赛时触发
 */
void onMatchBegin(struct MatchBeginAction* action){}
/**
 * 当整场比赛结束时触发
 */
void onMatchFinish(struct MatchFinishAction* action){}
/** 
 * 当开始一轮新的比赛时触发
 */
void onRoundBegin(struct RoundBeginAction* action){}
/**
 * 当一轮比赛结束时触发。
 */
void onRoundFinish(struct RoundFinishAction* action){}

/**
 * 当有队友向自己发送消息时触发
 */
void onMessageReceived(struct MessageReceivedAction* action){}
/**
 * 当撞到其它机器人时触发
 */
void onHitRobot(struct HitRobotAction* action){}
/**
 * 当撞到墙时触发
 */
void onHitWall(struct HitWallAction* action){}
/**
 * 当任意一个机器人开火时触发
 */
void onFire(struct FireAction* action){}
/**
 * 当有机器人死亡时触发
 */
void onRobotDeath(struct RobotDeathAction* action){}
/**
 * 当自己的子弹击中敌人时触发
 */
void onBulletHit(struct BulletHitAction* action){}
/**
 * 当被别人的子弹击中时触发
 */
void onHitedByBullet(struct HitedByBulletAction* action){}


/**
 * 机器人程序入口
 */
int main(int argC, char* argV[])
{
	tickHook = onTick;
	roundBeginHook = onRoundBegin;
	return startup(argC, argV);
}
