/* C Program Example - C Program to Print Date 
*  This C program will print the current date */

#include<stdio.h>
#include<conio.h>
#include<dos.h>
void main()
{
	clrscr();
	struct date d;
	int n;
	getdate(&d);
	printf("1:dd-mm-yyyy\n");
	printf("2:mm-dd-yyyy\n");
	printf("3:yyyy-mm-dd\n");
	printf("Entire your format\n");
	scanf("%d",&n);
	if(n==1)
	printf("Current system date is %d/%d/%d",d.da_day, d.da_mon, d.da_year);
	else if(n==2)
	printf("Current system date is %d/%d/%d",d.da_mon, d.da_day, d.da_year);
	else
	printf("Current system date is %d/%d/%d",d.da_year, d.da_mon, d.da_day);
	getch();
}
