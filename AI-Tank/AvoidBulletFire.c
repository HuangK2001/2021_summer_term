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
 * ÿ����λʱ�䶼�ᴥ��
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

																	
																																		
																																			