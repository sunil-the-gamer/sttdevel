#include<graphics.h>
#include<stdio.h>

#define VERTICAL_INDEX        480
#define HORIZONTAL_INDEX      640

void window_size(void)
{
   int gd = DETECT,gm,left=100,top=100,right=200,bottom=200,x= 300,y=150,radius=50;
   initgraph(&gd,&gm,NULL);
   rectangle(left, top, right, bottom);
   circle(x, y, radius);
   bar(left + 300, top, right + 300, bottom);
   line(left - 10, top + 150, left + 410, top + 150);
   ellipse(x, y + 200, 0, 360, 100, 50);
   outtextxy(left + 100, top + 325, "C Graphics Program");
}

void window_box(void)
{
	int x1 = 0, x2 = VERTICAL_INDEX;
	int y1 = 0, y2 = HORIZONTAL_INDEX;
	line(x1, y1, x1, y2);
	line(x2, y1, x2, y2);
	line(x1, y1, x1, y2);
	line(x2, y1, x2, y2);

}


int main(void)
{
	int gd=DETECT,gm;
	int i,x,y;
	initgraph(&gd,&gm,NULL);
	window_size();
	window_box();
	for(i=0;i<=1000;i++)
	{
		x=rand()%639;
		y=rand()%480;
		putpixel(x,y,15);
	}
	getch();
	closegraph();
}

