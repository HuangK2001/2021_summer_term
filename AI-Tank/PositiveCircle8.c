#include <airobot/c/SimpleRobot.h>
#include <math.h>
double MoveV = 5;
void waitForRadians(double radians);
/**
 * ÿ����λʱ�䶼�ᴥ��
 */
void waitForRadians(double radians)
{
	double TurnV = toRadians(5);
	int i;
	int time = (int) (radians/TurnV);
	for(i = 0 ; i < time ; i++) delay();
}
void onTick(struct TickAction* action){}
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
void onRoundBegin(struct RoundBeginAction* action){
	double TurnV = toRadians(5);
		move(MoveV);
		turn(TurnV);
		waitForRadians(PI/4);//����PI/2    б������->���£�PI/4     б������->���£�PI/4*3
	while (1){
		move(MoveV);
		turn(TurnV);
		waitForRadians(PI*2);
		turn(-TurnV);
		waitForRadians(PI*2);
	}
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

																	
																		
																			