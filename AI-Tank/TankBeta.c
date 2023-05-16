#include <airobot/c/SimpleRobot.h>
//�������
#define PREFER_DISTANCE 300
//����Ŀ���ʱ���õ������뾶
#define SEARCH_DIS 100
//ÿ�ε����ĵ�����
#define GAP toRadians(5)
//��ǽ�ľ���
#define AWAY_FROM_WALL 50
#define POWER 1.8
#define MATCH_LENGTH 20
#define HISTORY_LENGTH 2000
double velocityRecord[HISTORY_LENGTH];
double headingRecord[HISTORY_LENGTH];
int currentIndex;
int getMatchIndex();
void record(struct Bot* bot);
void getFirePoint (int matchIndex, double power, double* fireX, double* fireY);
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
{
	int matchIndex;
	double fireX, fireY;
	struct Bot* opponent=getFirstOpponent ();
	setDirection();
	doMove();
	doTurn();
	if(opponent==NULL) return;
	record (opponent);
	matchIndex = getMatchIndex () ;
	getFirePoint (matchIndex,POWER,&fireX,&fireY) ;
	fireOnPoint (fireX, fireY, POWER);
}
void record(struct Bot* bot)
{
	velocityRecord[currentIndex] = bot->velocity;
	headingRecord[currentIndex] = bot->heading;
	currentIndex++;
}
int getMatchIndex()
{
	double beatSimilarity = 1000000;
	int matchIndex = 0,i,j;
	for(i=MATCH_LENGTH;i<currentIndex-MATCH_LENGTH;i++)
	{
		double similarity = 0;
		for(j=1;j<=MATCH_LENGTH;j++)
		{
			similarity += fabs(velocityRecord[i-j]-velocityRecord [currentIndex-j]);
			similarity += fabs(headingRecord[i-j]-headingRecord[currentIndex-j]) ;
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
	double x = getFirstOpponent()->x;
	double y = getFirstOpponent()->y;
	double dis;
	int time = 0;
	while(matchIndex+time<currentIndex)
	{
		dis = distance(getX(),getY(),x,y);
		if(dis/getBulletVelocity(power)<=time) break;
		nextPoint(x,y,headingRecord[matchIndex+time],velocityRecord[matchIndex+time],&x,&y);
		time++;
	}
	*fireX = x;
	*fireY = y;
}
void onRoundBegin(struct RoundBeginAction* action)
{
	currentIndex = 0;
}	
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