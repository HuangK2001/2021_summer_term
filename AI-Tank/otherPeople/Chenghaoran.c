#include <airobot/c/SimpleRobot.h> 
#define PREFER_DISTANCE 300
#define SEARCH_DIS 100
#define GAP toRadians(5)
#define AWAY_FROM_WALL 50
#define POWER 0.5
#define MAX_BULLET 100
#define BEARING_GAP 0.0523
#define DISTANCE_GAP 100
#define FACTOR_SIZE 30
#define STATIST_SIZE 15
#define WAVE_SIZE 60
#define MATCH_LENGTH 20
#define HISTORY_LENGTH 2000
double velocityRecord[HISTORY_LENGTH];
double headingRecord[HISTORY_LENGTH];
int currentIndex;
void record(struct Bot* bot);
int getMatchIndex();
void getFirePoint(int matchIndex,double power,double* fireX,double* fireY);

double lastHeading;
int direction = 1;
long maxTime = 30;
long moveTime = 0;
void setDirection(void);
void doMove(void);
void doTurn(void);
void getCenter(double* x, double* y);
double getDeltaAngle(double centerX, double centerY);
double getNeedTurn(double headingTo);
int getNextBullet(void);
void updateBullets(void);
void updateBullet(struct VirtualBullet*);
void getDestination(double* x, double* y);
double getRisk(double x, double y);

struct VirtualBullet
{
int valid; 
double fireX, fireY; 
double x, y; 
double heading;
double velocity; 
};
struct VirtualBullet bullets[MAX_BULLET];
struct Statist
{
double factors[FACTOR_SIZE];
};
struct Wave
{
int statist;
double center;
double startX,startY;
double velocity;
long startTime;
int active;
};
struct Statist statists[STATIST_SIZE];
struct Wave waves[WAVE_SIZE];
int getIndex(double bearing);
int getHeightestIndex(int statist);
double getBestFireBearing(int statist);
void rate(int statist,double bearing);
void addWave(struct Bot*);
int getNextWave(void);
int getStatist(struct Bot*);
void updateWaves(struct Bot*);
void updateWave(int wave,long time,double x,double y);
void doMove(void);
void doFire(struct Bot* bot);

int isValid(double x, double y);

void onTick(struct TickAction* action)
{
double nextX, nextY, dis, bea, nextHeading;
double x, y;
long time;
struct Bot* bot = getFirstOpponent();
struct Bot* opponent = getFirstOpponent();
int matchIndex;
double fireX,fireY;

setDirection();
doMove();
doTurn();
updateBullets();
getDestination(&x, &y);
moveTo(x, y);
if(bot==NULL) return;
bea = bearing(bot->heading, lastHeading);
nextHeading = bot->heading;
nextX = bot->x; 
nextY = bot->y;
time = 0;
while(1)
{
double dis = distance(nextX, nextY, getX(), getY());
if(dis/getBulletVelocity(POWER)<=time)
{
fireOnPoint(nextX, nextY, POWER); 
break;
}
time ++;
nextHeading += bea;
nextPoint(nextX, nextY, nextHeading, bot->velocity, &nextX, &nextY);
}
lastHeading = bot->heading;
if(opponent==NULL) return;

record(opponent);
matchIndex = getMatchIndex();
getFirePoint(matchIndex,POWER,&fireX,&fireY);
fireOnPoint(fireX,fireY,POWER);
addWave(opponent);
updateWaves(opponent);
doFire(opponent);
if(opponent==NULL) return;

record(opponent);
matchIndex = getMatchIndex( );
getFirePoint(matchIndex,POWER,&fireX,&fireY);
}
/** 
 * 当开始一场新的比赛时触发
 */
void setDirection(void)
{
moveTime++;
if(moveTime>=maxTime)
{
moveTime=0;
maxTime = rand()%60;
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
deltaAngle = getDeltaAngle(centerX, centerY);
headingTo -= deltaAngle;
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
deltaDistance = deltaDistance/500;
return direction*PI/3*deltaDistance;
}
double getNeedTurn(double headingTo)
{
double needTurn = 0, nextX, nextY;
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

void record(struct Bot* bot)
{
velocityRecord[currentIndex] = bot->velocity;
headingRecord[currentIndex] = bot->heading;
currentIndex++;
}

int getMatchIndex()
{
double beatSimilarity = 100000;
int matchIndex = 0,i,j;
for (i = MATCH_LENGTH;i<currentIndex-MATCH_LENGTH;i++)
{
double similarity = 0;
for(j=i;j<=MATCH_LENGTH;j++)
{
similarity+=fabs(velocityRecord[i-j]-velocityRecord[currentIndex-j]);
similarity+=fabs(headingRecord[i-j]-headingRecord[currentIndex-j]);
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
double x= getFirstOpponent()->x;
double y= getFirstOpponent()->y;
double dis;
int time = 0;
while (matchIndex+time<currentIndex)
{
dis=distance(getX(),getY(),x,y);
if(dis/getBulletVelocity(power)<=time) break;
nextPoint(x,y,headingRecord[matchIndex+time],
velocityRecord[matchIndex+time],&x,&y);
time++;
}
*fireX=x;
*fireY=y;
}

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
  currentIndex = 0;
  for(i=0; i<MAX_BULLET; i++) 
  bullets[i].valid = 0;
for(i = 0; i<WAVE_SIZE; i++)
waves[i].active = 0;
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
int getNextBullet()
{
int i;
for(i=0; i<MAX_BULLET; i++)
{
if(!bullets[i].valid) return i;
}
return -1;
}

void updateBullets(void)
{
int i;
for(i=0; i<MAX_BULLET; i++)
{
if(bullets[i].valid) updateBullet(&(bullets[i]));
}
}

void updateBullet(struct VirtualBullet* bullet)
{
double nextX, nextY;

nextPoint(bullet->x, bullet->y, bullet->heading, bullet->velocity, &nextX, &nextY);
bullet->x = nextX;
bullet->y = nextY;

if(distance(bullet->fireX, bullet->fireY, bullet->x, bullet->y)>
distance(bullet->fireX, bullet->fireY, getX(), getY()))
{
bullet->valid = 0;
}
}

void getDestination(double* x, double* y)
{
double bestRist = 100000000;
int i;
for(i=0; i*GAP<2*PI; i++)
{
double rist, nextX, nextY;
nextPoint(getX(), getY(), i*GAP, SEARCH_DIS, &nextX, &nextY);
if(!inCourt(nextX, nextY, RADIUS)) continue;
rist = getRisk(nextX, nextY);
if(rist<bestRist)
{
*x = nextX;
*y = nextY;
bestRist = rist;
}
}
}

double getRisk(double x, double y)
{
int i;
double rist = 0;
for(i=0; i<MAX_BULLET; i++)
{
double dis;
if(!bullets[i].valid) continue;
dis = distance(x, y, bullets[i].x, bullets[i].y);
rist += 1/dis;
}
return rist;
}

int getIndex(double bearing)
{
return FACTOR_SIZE/2 + (int)(bearing/BEARING_GAP);
}

int getHeightestIndex(int statist)
{
int heightestIndex = 0;
int i;
for(i = 0; i <FACTOR_SIZE;i++)
{
  if(statists[statist].factors[i]>statists[statist].
factors[heightestIndex])
{
heightestIndex = i;
}
}
return heightestIndex;
}

void rate(int statist, double bearing)
{
int i;
statists[statist].factors[getIndex(bearing)] += 0.01;
for(i = 0; i < FACTOR_SIZE; i++)
statists[statist].factors[i] *= 0.99;
}

double getBestFireBearing(int statist)
{
return (getHeightestIndex(statist) - FACTOR_SIZE/2)*BEARING_GAP;
}

void addWave(struct Bot* bot)
{
int wave = getNextWave();
if(wave < 0)return;
waves[wave].active = 1;
waves[wave].statist = getStatist(bot);
waves[wave].center = heading(getX(),getY(),bot->x,bot->y);
waves[wave].startX = getX();
waves[wave].startY = getY();
waves[wave].startTime = getTime();
waves[wave].velocity = getBulletVelocity(POWER);
}

void updateWaves(struct Bot* bot)
{
int i;
for(i= 0;i < WAVE_SIZE;i++)
{
if(waves[i].active)
{
updateWave(i, getTime(),bot->x,bot->y);
}
}
}

void updateWave(int wave, long time,double x,double y)
{
double d1 = distance(waves[wave].startX,waves[wave].startY,x,y);
double d2 = waves[wave].velocity*(time - waves[wave].startTime);
 if(d2 >= d1)
{
double h = heading(waves[wave].startX, waves[wave].startY,x,y);
double b = bearing(h,waves[wave].center);
rate(waves[wave].statist, b);
waves[wave].active = 0;
}
}

int getNextWave()
{
int i;
for(i = 0;i<WAVE_SIZE;i++)
{
if(!waves[i].active) return i;
}
return -1;
}

int getStatist(struct Bot* bot)
{
double d = distance(getX(),getY(),bot->x,bot->y);
return (int)(d/DISTANCE_GAP);
}

/*void doMove(void)
{
moveTo(getCourtWidth()/2,getCourtHeight()/2);
}*/

void doFire(struct Bot* bot)
{
if(getFirePrepareTime() <= 0)
{
int statist  = getStatist(bot);
double b = getBestFireBearing(statist);
double h = heading(getX(), getY(),bot->x,bot->y);
fire(h+b,POWER);
}
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


