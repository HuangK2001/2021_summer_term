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
 * 每个单位时间都会触发
 */
void onTick(struct TickAction* action){
	double fHeading;
	struct bot* opponent = getFirstOpponent();
	if(opponent == NULL)return ;
	fHeading = heading(getX() , getY() , opponent->x , opponent->y);
	fire( fHeading , 3);
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

																	
																		