#include <airobot/c/SimpleRobot.h>
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
	double fHeading;
	struct bot* opponent = getFirstOpponent();
	if(opponent == NULL)return ;
	fHeading = heading(getX() , getY() , opponent->x , opponent->y);
	fire( fHeading , 3);
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

																	
																		