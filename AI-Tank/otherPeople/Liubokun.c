#include <airobot/c/SimpleRobot.h>
#define POWER 0.5
#define MATCH_LENGHT 20
#define HISTORY_LENGHT 2000
#define MOVE_TIME 25
int direction = 1;
long moveTime = 0;
void setDirection(void);
void doMove(void);
void doTurn(void);
void getCenter(double* x, double* y);
double velocityRecord[HISTORY_LENGHT];
double headingRecord[HISTORY_LENGHT];
int currentIndex;
void record(struct Bot* bot);
int getMatchIndex();
void getFirePoint(int matchIndex, double power, double* fireX, double* fireY);
void onRoundBegin(struct RoundBeginAction* action)
{
	currentIndex = 0;
}
void onTick(struct TickAction* action)
{
	int matchIndex;
	double fireX, fireY;
	struct Bot* opponent = getFirstOpponent();
	if(opponent==NULL) return;
	setDirection();
	doMove();
	doTurn();
	record(opponent);
	matchIndex = getMatchIndex();
	getFirePoint(matchIndex, POWER, &fireX, &fireY);
	fireOnPoint(fireX, fireY, POWER);
}
void record(struct Bot* bot)
{
	velocityRecord[currentIndex] = bot->velocity;
	headingRecord[currentIndex] = bot->heading;
	currentIndex++;
}
int getMatchIndex()
{
	double beatSimilarity=1000000;
	int matchIndex=0, i, j;
	for(i=MATCH_LENGHT; i<currentIndex-MATCH_LENGHT; i++)
	{
		double similarity=0;
		for(j=1; j<=MATCH_LENGHT; j++)
		{
			similarity+=fabs(velocityRecord[i-j]-velocityRecord[currentIndex-j])/8;
			similarity+=fabs(headingRecord[i-j]-headingRecord[currentIndex-j])/PI;
		}
	   	if(similarity<beatSimilarity)
	   	{
		    matchIndex=i;
		   	beatSimilarity=similarity;
		}
	}
	return matchIndex;
}
void getFirePoint(int matchIndex, double power, double* fireX, double* fireY)
{
   	double x = getFirstOpponent()->x;
   	double y = getFirstOpponent()->y;
   	double dis;
    int time = 0;
    while(matchIndex+time<currentIndex)
    {
    	dis = distance(getX(), getY(), x, y);
    	if(dis/getBulletVelocity(power)<=time) break;
    	nextPoint(x, y, headingRecord[matchIndex+time], 
    		velocityRecord[matchIndex+time], &x, &y);
    	time++;
    }
	*fireX = x;
	*fireY = y;
}
void setDirection(void)
{
	moveTime++;
	if(moveTime>MOVE_TIME)
	{
		direction *= -1;
		moveTime = 0;
	}
}
void doMove(void)
{
	move(10*direction);
}
void doTurn(void)
{
	double centerX, centerY, lineHeading, headingTo, bea;
	getCenter(&centerX, &centerY);
	lineHeading = heading(getX(), getY(), centerX, centerY);
	headingTo = lineHeading + PI/2;
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
int main(int argC, char* argV[])
{
	tickHook = onTick;
	roundBeginHook = onRoundBegin;
	return startup(argC, argV);
}


