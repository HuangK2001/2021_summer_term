#include <airobot/c/SimpleRobot.h>
double velocity[100];
double x[100][100];
double y[100][100];
double headingTo;
//double energy;
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


/*
 * ÿ����λʱ�䶼�ᴥ��
 */
void onTick(struct TickAction* action)
{
	struct bot* fireBot = getFirstOpponent();
	int time = (int)(getTime()%100);
	int round = getCurrentRound()%10;
	double angle = bearing(headingTo,getHeading());
	if(fabs(angle)>0.00001)
	{
		move(0);
		turn(angle);
	}
	else
	{
		turn(0);
		move(5);
	}
	velocity[time] = getVelocity();
	x[round][time] = getX();
	y[round][time] = getY();
	printf("MyRobotLocation--x=%lf  y=%lf\n",x[round][time],y[round][time]);
	printf("MyRobotVelocity--%lf\n",velocity[time]);
	//printf("MyRobotEnergy--%lf\n",energy);
	printf("AnemyRobotLocation--x=%lf  y=%lf\n",fireBot->x,fireBot->y);
	printf("AnemyRobotVelocity--%lf\n",fireBot->v);
	printf("AnemyRobotEnergy--%lf\n",fireBot->energy);
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
void onHitWall(struct HitWallAction* action)
{
	headingTo += PI/2;
}
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

																	
																																										