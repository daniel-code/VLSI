#include <stdio.h>
#include <stdlib.h>
#define Size 25

struct node
{
    int label;
    int start;
    int endtime;

} sample1[Size],temp;

int readIn()
{
    FILE *fptr_in;

    int op = 0, op1 = 0, op2 = 0, count = 0, end = 0;
    char  In_FileName[20];
    printf("Enter Input filename : ");
    scanf("%s", In_FileName);

    if ((fptr_in = fopen(In_FileName, "r")) == NULL)
    {
        printf("Error!!can not open file!! \nPress any key to exit. \n");
        system("pause");
        exit(1);
    }

    fscanf(fptr_in, "%d", &end);


    while (fscanf(fptr_in, " %d %d %d", &op, &op1, &op2))
    {


        sample1[count].label = op;
        printf("label=%d\t",sample1[count].label);

        sample1[count].start = op1;
        printf("start=%d\t",sample1[count].start);

        sample1[count].endtime = op2;
        printf("end=%d\n",sample1[count].endtime);

        count++;
        if (end == count)   //²×¤î¥Î
            break;

    }
    fclose(fptr_in);
    return end;
}

void sort_sample(int end)
{
    int i,j;
    for (i = 0; i<end - 1; i++)
    {
        for (j = i + 1; j<end; j++)
        {
            if (sample1[i].start > sample1[j].start)
            {
                temp.label = sample1[i].label;
                temp.start = sample1[i].start;
                temp.endtime = sample1[i].endtime;

                sample1[i].label = sample1[j].label;
                sample1[i].start = sample1[j].start;
                sample1[i].endtime = sample1[j].endtime;

                sample1[j].label = temp.label;
                sample1[j].start = temp.start;
                sample1[j].endtime = temp.endtime;
            }
        }
    }
}

void Scheduling(int end)
{

    int i=0,j=0,end_time=0,count=0;
    for(j=0; j < end ; j++)
    {
        printf("label=%d\t",sample1[j].label);
        printf("start=%d\t",sample1[j].start);
        printf("end=%d\n",sample1[j].endtime);
    }
    while(1)
    {
        i++;
        end_time=0;
        printf("R%d :",i);
        for(j=0; j < end ; j++)
        {
            if(sample1[j].label!=0&&sample1[j].start>=end_time)
            {
                printf(" %d",sample1[j].label);
                end_time=sample1[j].endtime;
                sample1[j].label=0;
                count++;
            }
        }
        printf("\n");


        if(count==end)
            break;
    }

}


int main()
{
    int num = 0;
    num = readIn();
    sort_sample(num);
    Scheduling(num);


    return 0;
}
