#include <airobot/c/SimpleRobot.h>
double MoveV = 5;
void waitForRadians(double radians);
void waitForDistance(double distance);
/**
 * ÿ����λʱ�䶼�ᴥ��
 */
void waitForRadians(double radians){
	double TurnV = toRadians(3);
	double R = MoveV/TurnV;
	int i;
	int time = (int) (radians/TurnV);
	for(i = 0 ; i < time ; i++) delay();
}
void waitForDistance(double distance){
	double TurnV = toRadians(3);
	double R = MoveV/TurnV;
	int i;
	int time = (int) (distance/MoveV);
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
	double TurnV = toRadians(3);
	double R = MoveV/TurnV;
		move(MoveV);
		turn(TurnV);
		waitForRadians(PI/4*3);//���� PI/4       ����PI/4*3
	while(1){
		move(MoveV);
		turn(TurnV);
		waitForRadians(toRadians(270));
		turn(0);
		waitForDistance(R*2);
		turn(-TurnV);
		waitForRadians(toRadians(270));
		turn(0);
		waitForDistance(R*2);
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

																	
																		
																			