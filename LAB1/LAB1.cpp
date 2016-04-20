// LAB1.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#define Size 200
struct cdfg
{
	int op;
	int op1;
	int op2;
	int result;
} sample1[Size];

struct readylist
{
	int state;
	int op;
	int num;
	int dis;
} list1[Size], temp;

struct alu
{
	int mult;
	int add;
} alulist[Size];

#pragma warning(disable : 4996)	//visual studio only

int readIn()
{
	FILE *fptr_in;

	int step = 0, op = 0, op1 = 0, op2 = 0, result = 0, count = 0, end;
	char  In_FileName[20];
	int i;


	printf("\n欲選擇圖1，請輸入 p1.txt \n欲選擇圖2，請輸入 p2.txt\n");
	printf("\n Enter Input filename : ");
	scanf("%s", In_FileName);

	if ((fptr_in = fopen(In_FileName, "r")) == NULL)
	{
		printf("Error!!can not open file!! \nPress any key to exit. \n");
		system("pause");
		exit(1);
	}

	fscanf(fptr_in, "%d", &end);


	while (fscanf(fptr_in, " %d %d %d %d ", &op, &op1, &op2, &result))
	{


		sample1[count].op = op;
		//                        printf("op=%d",sample1[count].op);

		sample1[count].op1 = op1;
		//                        printf("op1=%d",sample1[count].op1);

		sample1[count].op2 = op2;
		//                        printf("op2=%d",sample1[count].op2);

		sample1[count].result = result;
		//                        printf("result=%d\n\n",sample1[count].result);

		count++;
		if (end == count)   //終止用
			break;

	}
	//                  sample1[count].step=-1;
	//                         printf("count=%d step=%d\n",count,sample1[count].step);



	fclose(fptr_in);
	return end;
}

int op_search(int state, int op, int list_num)
{
	int i;
	for (i = list_num+1; i >= 0; i--)
	{
		if (sample1[list1[i].num].result == op  && list1[i].state<=state)
			return list1[i].state;
	}
	return 0;
}


void list_Scheduling(int end)
{
	int opmult = 0, opadd = 0;
	int signal = 0, state = 0, opstate = 0, opstate1 = 0, opstate2 = 0;
	int list_num = 0;
	int i = 0, j = 0;

	FILE *fptr_out;
	fptr_out = fopen("Scheduling_outcome.txt", "w");

	printf("\n 輸入限制的乘法器個數...");
	scanf("%d", &opmult);
	printf("\n 輸入限制的加法器個數...");
	scanf("%d", &opadd);
	printf("*****************************\n");
	printf("*     Resource Constraint\n");
	printf("* \n");
	printf("*      Mult Constraint : %2d\n", opmult);
	printf("*      Add Constraint  : %2d\n",opadd);
	printf("*\n");
	printf("*****************************\n");

	fprintf(fptr_out, "*****************************\n");
	fprintf(fptr_out, "*     Resource Constraint\n");
	fprintf(fptr_out, "* \n");
	fprintf(fptr_out, "*      Mult Constraint : %2d\n", opmult);
	fprintf(fptr_out, "*      Add Constraint  : %2d\n", opadd);
	fprintf(fptr_out, "*\n");
	fprintf(fptr_out, "*****************************\n");



	for (i = 0; i<Size; i++)                              //alulist 初始化
	{
		alulist[i].add = opadd;
		alulist[i].mult = opmult;
	}

	for (i = 0; i<end; i++)
	{
		//printf("\n\nstate %d\n", state);
		opstate1 = op_search(state, sample1[i].op1, list_num);
		opstate2 = op_search(state, sample1[i].op2, list_num);
		//printf("V%d  is at state %d \t V%d  is at state %d\n", sample1[i].op1, opstate1,sample1[i].op2, opstate2);
		if (opstate1>opstate2)
			opstate = opstate1;
		else
			opstate = opstate2;
		state = opstate + 1;
		if (sample1[i].op == 1)
		{
			while (1)
			{
				if (alulist[state].add)
				{
					list1[list_num].num = i;
					list1[list_num].op = sample1[i].op;
					list1[list_num].state = state;
					//printf("V%d  is at state %d \n", sample1[i].result, state);
					alulist[state].add--;
					break;
				}
				else
				{
					state++;
				}
			}
		}
		else    if (sample1[i].op == 2)
		{
			while (1)
			{
				if (alulist[state].mult)
				{
					list1[list_num].num = i;
					list1[list_num].op = sample1[i].op;
					list1[list_num].state = state;
					//printf("V%d  is at state %d \n", sample1[i].result, state);
					alulist[state].mult--;
					break;
				}
				else
				{
					state++;
				}
			}

		}
		list_num++;
	}


	for (i = 0; i<end - 1; i++)
	{
		for (j = i + 1; j<end; j++)
		{
			if (list1[i].state>list1[j].state)
			{
				temp.state = list1[i].state;
				temp.op = list1[i].op;
				temp.num = list1[i].num;
				temp.dis = list1[i].dis;

				list1[i].state = list1[j].state;
				list1[i].op = list1[j].op;
				list1[i].num = list1[j].num;
				list1[i].dis = list1[j].dis;

				list1[j].state = temp.state;
				list1[j].op = temp.op;
				list1[j].num = temp.num;
				list1[j].dis = temp.dis;
			}
		}
	}


	for (i = 0; i < end; i++)
	{
		if (list1[i].op == 1)
		{
			fprintf(fptr_out, "state%3d\t v%3d = v%3d + v%3d\n", list1[i].state, sample1[list1[i].num].result, sample1[list1[i].num].op1, sample1[list1[i].num].op2);
			printf("state%3d\t v%3d = v%3d + v%3d\n", list1[i].state, sample1[list1[i].num].result, sample1[list1[i].num].op1, sample1[list1[i].num].op2);
		}
		else if (list1[i].op == 2)
		{
			fprintf(fptr_out, "state%3d\t v%3d = v%3d * v%3d\n", list1[i].state, sample1[list1[i].num].result, sample1[list1[i].num].op1, sample1[list1[i].num].op2);
			printf("state%3d\t v%3d = v%3d * v%3d\n", list1[i].state, sample1[list1[i].num].result, sample1[list1[i].num].op1, sample1[list1[i].num].op2);
		}
	}

	fclose(fptr_out);

}//list_Scheduling





int main()
{

	int end,i=1;
	while (i!=2)
	{
		end = readIn();
		list_Scheduling(end);
		printf("如果要繼續的話，請輸入 1 ，離開請輸入 2  : ");
		scanf("%d", &i);
	}




	system("pause");
	return 0;
}



