#include <airobot/c/SimpleRobot.h>
#define POWER 2.5
#define MATCH_LENGHT 20
#define PREFER_DISTANCE 300
#define SEARCH_DIS 100
#define GAP toRadians(12)
#define AWAY_FROM_WALL 50
#define HISTORY_LENGHT 2000
double velocityRecord[HISTORY_LENGHT];
double headingRecord[HISTORY_LENGHT];
//�ƶ��ķ���,���������ֵֻ��ȡ1��-1��1��ʾǰ����-1��ʾ����
int direction = 1;
//��һ�������������ƶ�ʱ��
long maxTime = 20;
//��ǰ��һ�������ϵ��ƶ�ʱ��
long moveTime = 0;
double lastHeading;
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
int currentIndex;
//�жϸ����ĵ��Ƿ���һ����Ч��Ŀ���
int isValid(double x, double y);
struct virtualBullet {
double originX;
double originY;
double x;
double y;
double heading;
double velocity;
}virtualBullet;
void record(struct Bot* bot);
//������ѵ�ƥ���
int getMatchIndex();
//�õ������λ��
void getFirePoint(int matchIndex, double power, double* fireX, double* fireY);

void onRoundBegin(struct RoundBeginAction* action)
{
	currentIndex = 0;
}

void onTick(struct TickAction* action)
{
	double nextX, nextY, dis, bea, nextHeading;
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
	
void setDirection(void)
{
	moveTime++;
	if(moveTime>=maxTime)
	{
		
		//�ı��˶�����
		direction *= -1;
		moveTime=0;
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
	headingTo = lineHeading + PI/2.65;
	//Ϊ�˱��־�����deltaAngle��headingTo��������
	deltaAngle = getDeltaAngle(centerX, centerY);
	headingTo -= PI*deltaAngle;
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
		*x = getCourtWidth()/2.5;
		*y = getCourtHeight()/2.5;
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
	deltaDistance = deltaDistance/1000;
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
	return startup(argC, argV);
}
