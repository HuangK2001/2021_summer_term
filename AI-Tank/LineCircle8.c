#include <airobot/c/SimpleRobot.h>
double MoveV = 5;
void waitForRadians(double radians);
void waitForDistance(double distance);
/**
 * 每个单位时间都会触发
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
void onRoundBegin(struct RoundBeginAction* action){
	double TurnV = toRadians(3);
	double R = MoveV/TurnV;
		move(MoveV);
		turn(TurnV);
		waitForRadians(PI/4*3);//竖向 PI/4       横向PI/4*3
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

																	
																		
																			