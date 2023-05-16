#include <airobot/c/SimpleRobot.h>
#define POWER 1.5
//�������
#define PREFER_DISTANCE 300
//����Ŀ���ʱ���õ������뾶
#define SEARCH_DIS 100
//ÿ�ε����ĵ�����
#define GAP toRadians(5)
//��ǽ�ľ���
#define AWAY_FROM_WALL 50
double lastHeading;
//�ƶ��ķ���,���������ֵֻ��ȡ1��-1��1��ʾǰ����-1��ʾ����
int direction = 1;
//��һ�������������ƶ�ʱ��
long maxTime = 30;
//��ǰ��һ�������ϵ��ƶ�ʱ��
long moveTime = 0;

//�����ƶ��ķ���
void setDirection(void);
//ִ���ƶ�
void doMove(void);
//ִ��ת��
void doTurn(void);
//�õ�Բ��λ��
void getCenter(double* x, double* y);
//�õ����ڱ�����������������deltaAngle
double getDeltaAngle(double centerX, double centerY);
//�õ����������ƶ�������������ײǽ�ļн�needTurn
double getNeedTurn(double headingTo);
//�жϸ����ĵ��Ƿ���һ����Ч��Ŀ���
int isValid(double x, double y);

void onTick(struct TickAction* action)
{   double nextX, nextY, dis, bea, nextHeading;
long time;
struct Bot* bot = getFirstOpponent();
if(bot==NULL) return;
//���������������λʱ��ķ���ļн�
bea = bearing(bot->heading, lastHeading);
//��ʼ������
nextHeading = bot->heading;
nextX = bot->x; 
nextY = bot->y;
time = 0;

//�������
while(1)
{
//�ж��ӵ���timeʱ����ܷ����nextλ�õĶ���
double dis = distance(nextX, nextY, getX(), getY());
if(dis/getBulletVelocity(POWER)<=time)
{
//�ܴ򵽣���������
fireOnPoint(nextX, nextY, POWER); //����
break;
}
//������һ�ε���
time ++;
nextHeading += bea;
//�������¸�ʱ����ֵ�λ��
nextPoint(nextX, nextY, nextHeading, bot->velocity, &nextX, &nextY);
}
lastHeading = bot->heading;
setDirection();
doMove();
doTurn();
}
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


void setDirection(void)
{
moveTime++;
if(moveTime>=maxTime)
{
moveTime=0;
//�������������������ƶ���ʱ��
maxTime = rand()%60;
//�ı��˶�����
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
//Ϊ�˱��־�����deltaAngle��headingTo��������
deltaAngle = getDeltaAngle(centerX, centerY);
headingTo -= deltaAngle;
//Ϊ�˲�ײǽ��needTurn��headingTo��������
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
//�������
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

//���������˳���
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
