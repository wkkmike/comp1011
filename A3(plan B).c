#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#define MAXN 100006
#define root 1

void wait(int sec)
{	// a function let the program pause for 1 second. 
	Sleep(sec * 1000);
}

struct Elevator
{ 
	int floor; // The range of floor is postive integers in range of [1,9]
	int dir;   // 1 for up, -1 for down, 0 for stop.
};

struct WaitingList
{   //a list of strucure represent request in each floor, 
	//[1]-[9], represent going down request in corresponding floor
	//[10]-[18], represent going up request for 1-9 floor repectively
	int tot;  // the total number of people ask for this particular request
	int no[MAXN]; //serial number of the people 
	int time[MAXN];//the time of the request for each people. 
};

struct Request
{
	int floor; //the floor which people ask for 1 request
	int goal; //destination
	int arrive; //the time ask 1 request
	int get_in; //time ask 2 request
	int get_out; //time the people get out the left
};

struct ServingList
{   // a linked list store serving request. 
	int no;//serial number of the person
	struct ServingList *next;
}*Serv;

typedef struct ServingList SL;
typedef struct Elevator ELE;
typedef struct Request REQ;

REQ r[MAXN];
ELE ele;
struct WaitingList w[19];  // 1-9 to store the waiting list of going down request, and 10-18 for going up request.

int req[MAXN];
int tot_ele = 0;
int cnt = 0, time = 0, serv_num=0;
//cnt: count the number of people ask 1 request
//time: a timer
//serv_num: counter(number of people in the elevator)
char op[100];


void ready()
{ // welcoming information
	printf("Welcome to the elevator simulation progream.\n");
	printf("\n");
	wait(1);
	printf("The Version of the progream is V 1.06\n");
	printf("\n");
	wait(1);
	printf("The designger of this Program is Wang Qifan, Yan Haozhe and Jiang Yifan.\n");
	printf("\n");
	wait(1);
	printf("All Rights Reserved.\n");
	printf("\n");
	wait(2);
	printf("The maximum of requests the program supported is 100,000 and please do NOT input requests over 100,000.\n");
}



void init()
{	//function initialize the elevator
	memset(r, 0, sizeof(r)); // initialize
	ele = (struct Elevator){ 1,0 }; // initialize the elevator at floor one, direction 'stop'
	Serv = malloc(sizeof(SL)); // create the first element in serving list
	Serv->no = 1;
	Serv->next = NULL;
}  

void addToWaiting(struct WaitingList *A, int floor, int dir, int time, int no)
{	// adding 1 request to waiting list
	r[no].arrive = time;
	r[no].floor = floor;
	struct WaitingList *B = A + dir*9 + floor;
	//since in the function 'askforrequest1', if user want to go down, input 0;
	//therefore, (dir*9 + floor) = the floor, A is the address of the first element of the waiting list
	//so, A + dir*9 + floor is the adress of the first element of waitlist[floor];
	//in a simliar way, for user want to go down, B is point to the first element of waitlist[floor + 9]
	(B->tot)++; // increase the number of people ask for the particular request
	int t = B->tot; 
	B->no[t] = no; // set waitlist[floor +(0 or 1)].no[t] to the serial number of the person
	B->time[t] = time; // set to the time of first request being made
}

void out_state1(ELE e)
{   //print the current information of the elevator.
	printf("The elevator arrive at %dF.\n", e.floor);
	printf("\n");
}

int check()
{// return 1 indicate no one the servelist and waitlist
	int v = 1;
	if (serv_num > 0)return 0; 
	int i;
	for (i = 1; i <= 18; i++)
	{
		if (w[i].tot > 0)return 0;
	}

	

	return 1;
}

void askForRequest1()
{ // ask people for the first kind of request
	printf("New Request? ('Y' for yes and 'N' for no) : ");
	scanf("%s", op);

	printf("\n");

	if (op[0] == 'Y' || op[0] == 'y')
	{
		cnt++;//increase the counter(total people ask for 1 request)
		printf("Request #%d\n", cnt);

		int valid = 0;

		int floor, dir;

		while (!valid)
		{
			printf("Please input which floor have the request (1-9): ");
			scanf("%d", &floor);

			if (floor > 0 && floor < 10)
			{//if the input is valid
				if (floor == 1)dir = 1;//Floor 1, set the direction to 1
				if (floor == 9)dir = 0;//floor 9, set the direction to 0
				if (floor > 1 && floor < 9)
				{
					int valid = 0;
					
					printf("Please input the direction (1 for up and 0 for down): ");
					scanf("%d", &dir);
					
					while (!valid)
					{//repeat ask until correct input
						if (dir == 0 || dir == 1)valid = 1;
						else
						{
							printf("Invalid input. The input of direction can only be 1 (for up) or 0 (for down). Please try again.\n");
							printf("Please input the direction (1 for up and 0 for down): ");
							scanf("%d", &dir);
						}
					}
					
				}

				if (dir == 1 || dir == 0)valid = 1;
			}

			if (!valid)printf("Invalid input, please try again.\n");
		}

		struct WaitingList *p;

		p = w;

		addToWaiting(p, floor, dir, time, cnt); // add the request to the waitlist
	}
}

void addToServing(SL *p, int no, int time)
{	//add to the servelist
	SL *p1 = p->next,*p2;
	p->next = malloc(sizeof(SL));
	p2 = p->next;
	p2->no = no;
	p2->next = p1;
}

void completeServing(ELE e, SL *head)
{	//traversal the servelist, find if any request can be completed
	int f = e.floor;
	SL *p1 = head, *p2 = head->next;

	while (p2 != NULL)
	{
		int no = p2->no;
		if (r[no].goal == f)//if #n's request can be completed 
		{
			r[no].get_out = time;
			printf("The request #%d is completed. With processing %d minutes.\n",no,r[no].get_out-r[no].get_in);
			printf("\n");
			p1->next = p2->next;
			p2 = p2->next;//set the pointer to the next element 
			serv_num--;
		}
		
		else
		{
			p1 = p2;
			p2 = p2->next;
		}
	}

}

void getin_ele(struct Elevator *e)
{//test whether any 1 request can be completed
 //if any can, add it to the servelist
	int dir = e->dir, floor = e->floor;
	if (dir == -1)dir++;
	int kth = dir * 9 + floor; 
	if (w[kth].tot == 0)return;
	//if no people' request in the waitlist have the same direction with elevator's, no people get in.
	 
	serv_num += w[kth].tot;//else, increase the amount of servse 
	int i;
	for (i = 1; i <= w[kth].tot; i++)
	{	//repeat n times equal to the amount of people whose 1 request can be fullfilled.
		int no = w[kth].no[i]; // the serial number of people
		r[no].get_in = time; //the time second request make
		addToServing(Serv,no,time); //add to the serving list

		printf("The request of #%d has been fulfilled, and was walked in the elevator, with waiting for %d minute(s).\n", no, r[no].get_in - r[no].arrive);
		printf("\n");
		
		printf("Please input press the button of floor which request #%d wanna go.",no);

		printf("\n");

		int valid = 0;

		while (!valid) //input a valid target floor
		{
			int x;
			scanf("%d", &x);
			if (!(x > 0 && x < 10 && x != r[no].floor))
			{
				printf("Invalid input. The target floor must be in range of [1,9] and not same as the floor it started.\n");
				printf("Please input press the button of floor which request #%d wanna go. \n", no);
			}

			else
			{
				r[no].goal = x;//set the target floor
				valid = 1;//reset flag
			}
		}
	}

	w[kth].tot = 0;//clear the request list in that floor and direction.
}

void output_end()
{
	printf("This elevator works for %d minute(s).\n", time);
	printf("\n");
	wait(2);
	printf("It severed for %d peoples.\n", cnt);
	printf("\n");
	wait(3);

	double wait_avg = 0.0;
	double wait_sum = 0.0;

	if (cnt != 0)
	{
		int i;
		for (i = 1; i <= cnt; i++)
			wait_sum += r[i].get_in - r[i].arrive;//compute the total time of waitting 

		wait_avg = wait_sum / cnt;
	}

	printf("The average waiting time of requests is %.2lf minutes.\n", wait_avg);
	printf("\n");
	wait(4);

	printf("Thank You for using this program.\n");
}

int main()
{
	ready();//print welcome information
	init();//initialize 


	while (1)
	{
		ele.floor += ele.dir; //refresh elevator floor  

		out_state1(ele); //print information of elevator

		int nowdir = ele.dir;

		


		if (serv_num) //if there is people in elevator
		{
			completeServing(ele, Serv); //try if any request can be complete, if can then complete it 
		}
		
		askForRequest1(); // ask user input the first kind of request

		if (ele.floor == 9)ele.dir = -1; //if the elevator is in the top floor, force the direction state to 'go down' 
		if (ele.floor == 1)ele.dir = 1; //if the elevator is in the bottom floor, force the direction state to 'go up' 

		if (!serv_num) // if there isn't any people inside elevator
		{
			int up_request, down_request;  //Check if there is any request on the same direction, otherwise, the elevator will turn.
			up_request = down_request = 0;

			int i;
			for (i = ele.floor + 1; i <= 9; i++) //check if any people ask 1 request in upper floor
				if (w[i + 9].tot || w[i].tot)up_request = 1;

			
			for (i = ele.floor - 1; i > 0; i--) // check if any people ask 1 request in lower floor
				if (w[i].tot || w[i + 9].tot)down_request = 1;

			if (!up_request && !down_request) // if no one ask 1 request in other floor
			{
				if (!w[ele.floor].tot && !w[ele.floor + 9].tot)nowdir = 0;
				//if no one ask 1 request, change the direction to 'stop'
				else if (!w[ele.floor].tot)nowdir = 1;
				//if people in this floor ask for up, change the direction state to 'up'
				else if (!w[ele.floor + 9].tot)nowdir = -1;
				//if people in this floor ask for down, change the direction state to 'down'
			}	
			else if (!up_request)nowdir = -1;
			//if people in lower floor ask the elevator, change the direction to 'down'
			else if (!down_request)nowdir = 1;
			//if no people in lower floor ask the elevator and people in upper ask, change teh direction to 'up'
			ele.dir = nowdir;

		}

		getin_ele(&ele); 
		//if the 1 request can be fullfilled, compelete it and ask for the 2 request.
		if (ele.dir == 1)printf("The elevator is going up.\n");
		if (ele.dir == 0)printf("The elecator stops at %dF.\n", ele.floor);
		if(ele.dir == -1)printf("The elevator is going down.\n");

		printf("\n");

		wait(1);

		if (check()) // if no one in servelist and wait list
		{

			printf("All of the request are fulfilled. Input 'q' for end the simulation, otherwise, the simulation will continue.\n");
			scanf("%s", op);

			if (op[0] == 'q' || op[0] == 'Q')break;//if user want to end simulation, jump out the loop

			ele.dir = 0;

		}


		time++;//increase the timer
	}

	output_end();//output corresponding information

	return 0;
}
