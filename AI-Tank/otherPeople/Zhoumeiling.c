#include <airobot/c/SimpleRobot.h>
#define POWER 1.5
//理想距离
#define PREFER_DISTANCE 300
//计算目标点时采用的搜索半径
#define SEARCH_DIS 100
//每次迭代的递增量
#define GAP toRadians(5)
//离墙的距离
#define AWAY_FROM_WALL 50
double lastHeading;
//移动的方向,这个变量的值只能取1和-1，1表示前进，-1表示后退
int direction = 1;
//在一个方向上最大的移动时间
long maxTime = 30;
//当前在一个方向上的移动时间
long moveTime = 0;

//设置移动的方向
void setDirection(void);
//执行移动
void doMove(void);
//执行转动
void doTurn(void);
//得到圆心位置
void getCenter(double* x, double* y);
//得到用于保持理想距离的修正角deltaAngle
double getDeltaAngle(double centerX, double centerY);
//得到用来修正移动方向避免机器人撞墙的夹角needTurn
double getNeedTurn(double headingTo);
//判断给定的点是否是一个有效的目标点
int isValid(double x, double y);

void onTick(struct TickAction* action)
{   double nextX, nextY, dis, bea, nextHeading;
long time;
struct Bot* bot = getFirstOpponent();
if(bot==NULL) return;
//计算机器人两个单位时间的方向的夹角
bea = bearing(bot->heading, lastHeading);
//初始化迭代
nextHeading = bot->heading;
nextX = bot->x; 
nextY = bot->y;
time = 0;

//进入迭代
while(1)
{
//判断子弹在time时间后能否打到在next位置的对手
double dis = distance(nextX, nextY, getX(), getY());
if(dis/getBulletVelocity(POWER)<=time)
{
//能打到，迭代结束
fireOnPoint(nextX, nextY, POWER); //开火
break;
}
//进入下一次迭代
time ++;
nextHeading += bea;
//计算在下个时间对手的位置
nextPoint(nextX, nextY, nextHeading, bot->velocity, &nextX, &nextY);
}
lastHeading = bot->heading;
setDirection();
doMove();
doTurn();
}
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


void setDirection(void)
{
moveTime++;
if(moveTime>=maxTime)
{
moveTime=0;
//随机生成在这个方向上移动的时间
maxTime = rand()%60;
//改变运动方向
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
//为了保持距离用deltaAngle对headingTo进行修正
deltaAngle = getDeltaAngle(centerX, centerY);
headingTo -= deltaAngle;
//为了不撞墙用needTurn对headingTo进行修正
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
//deltaDistance = deltaDistance/fmax(distance, PREFER_DISTANCE);
deltaDistance = deltaDistance/500;
return direction*PI/3*deltaDistance;
}

double getNeedTurn(double headingTo)
{
double needTurn = 0, nextX, nextY;
//进入迭代
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

//启动机器人程序
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
