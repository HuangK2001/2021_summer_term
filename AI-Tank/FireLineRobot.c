#include <airobot/c/SimpleRobot.h>
#define POWER 0.5
/*ֱ�ߵ�����׼*/
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
/**
 * ÿ����λʱ�䶼�ᴥ��
 */
void onTick(struct TickAction* action){
	double nextX,nextY,Dis;
	long time;
	struct bot *bot = getFirstOpponent();
	if(bot == NULL) return;
	time = 1;
	while(1)
	{
		nextPoint(bot->x,bot->y,bot->heading,
			bot->v*time,&nextX,&nextY);
			Dis = distance(nextX , nextY,getX() , getY());
			if(Dis/getBulletVelocity(POWER)<=time)
			{
				fireOnPoint(nextX , nextY , POWER);
				return;
			}
			else time++;
	}
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

																	
																		