#include <airobot/c/SimpleRobot.h>
#define SEARCH_DIS 100
#define GAP toRadians(1)
#define MAX_BULLET 10
#define POWER 1.3
#define MATCH_LENGTH 20
#define HISTORY_LENGTH 2000
struct VirtualBullet
{
	int valid;
	double fireX,fireY;
	double x,y;
	double heading;
	double velocity;
};
struct VirtualBullet bullets[MAX_BULLET];
int getNextBullet(void);
int currentIndex;
int getMatchIndex();
void record(struct Bot* bot);
void updateBullets(void);
void updateBullet(struct VirtualBullet*);
void getDestination(double* x,double* y);
void getFirePoint (int matchIndex, double power, double* fireX, double* fireY);
double getRisk(double x,double y);
double velocityRecord[HISTORY_LENGTH];
double headingRecord[HISTORY_LENGTH];
/**
 * 每个单位时间都会触发
 */
void onTick(struct TickAction* action)
{
	double x,y;
	int matchIndex;
	double fireX, fireY;
	struct Bot* opponent=getFirstOpponent ();
	updateBullets();
	getDestination(&x,&y);
	moveTo(x,y);
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
void onRoundBegin(struct RoundBeginAction* action)
{
	int i;
	for(i=0;i<MAX_BULLET;i++)bullets[i].valid = 0;
	currentIndex = 0;
}
int getNextBullet()
{
	int i;
	for(i=0;i<MAX_BULLET;i++)
	{
		if(!bullets[i].valid) return i;
	}
	return -1;
}
void updateBullets(void)
{
	int i;
	for(i=0;i<MAX_BULLET;i++)
	{
		if(bullets[i].valid) updateBullet(&(bullets[i]));
	}
}
void updateBullet(struct VirtualBullet* bullet)
{
	double nextX,nextY;
	nextPoint(bullet->x,bullet->y,bullet->heading,bullet->velocity,&nextX,&nextY);
	bullet->x = nextX;
	bullet->y = nextY;
	if(distance(bullet->fireX,bullet->fireY,bullet->x,bullet->y)>distance(bullet->fireX,bullet->fireY,getX(),getY()))
	{
		bullet->valid = 0;
	}
}
void getDestination(double* x,double* y)
{
	double bestRist = 100000000;
	int i;
	for(i=0;i*GAP<2*PI;i++)
	{
		double rist,nextX,nextY;
		nextPoint(getX(),getY(),i*GAP,SEARCH_DIS,&nextX,&nextY);
		if(!inCourt(nextX,nextY,RADIUS)) continue;
		rist = getRisk(nextX,nextY);
		if(rist<bestRist)
		{
			*x = nextX;
			*y = nextY;
			bestRist = rist;
		}
	}
}
double getRisk(double x,double y)
{
	int i;
	double rist = 0;
	for(i=0;i<MAX_BULLET;i++)
	{
		double dis;
		if(!bullets[i].valid) continue;
		dis = distance(x,y,bullets[i].x,bullets[i].y);
		rist +=1/dis;
	}
	return rist;
}
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
void onFire(struct FireAction* action)
{
	int index;
	struct Bot* fireBot = getBotById(action->bullet.owner);
	if(isTeammate(fireBot)) return;
	index = getNextBullet();
	if(index<0) return;
	bullets[index].valid = 1;
	bullets[index].fireX = action->bullet.fireX;
	bullets[index].fireY = action->bullet.fireY;
	bullets[index].x = action->bullet.fireX;
	bullets[index].y = action->bullet.fireY;
	bullets[index].velocity = getBulletVelocity(action->bullet.power);
	bullets[index].heading = heading(bullets[index].fireX, bullets[index].fireY, getX(), getY());
}
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
	matchBeginHook = onMatchBegin;
	matchFinishHook = onMatchFinish;
	roundBeginHook = onRoundBegin;
	roundFinishHook = onRoundFinish;
	messageReceivedHook = onMessageReceived;
	hitRobotHook = onHitRobot;
	hitWallHook = onHitWall;
	robotDeathHook = onRobotDeath;
	fireHook = onFire;
	bulletHitHook = onBulletHit;
	hitedByBulletHook = onHitedByBullet;
	
	return startup(argC, argV);
}

																	
																																		
																																			