#include<stdio.h>
#include<graphics.h>
#include<conio.h>
#include<pthread.h>
#include<iostream>
#include<windows.h>

using namespace std;

int lives=3;
int level = 1;
int uflag=0;

struct user
{
    char name[20];
    int score;
};
struct user u[5];

void leaderboard(int sc)
{
    int i;
    char str[20];
    FILE *fp;

    fp = fopen("leaderboard.txt", "r");

    if(fp == NULL)
    {
        printf("File cannot open");
        exit(1);
    }

    for(i=0; i<5; i++)
    {
        fscanf(fp,"%s%d\n",u[i].name,&u[i].score);
        //printf("\n%s     %d",u[i].name,u[i].score);
    }


    printf("\nEnter your name:- ");
    gets(u[5].name);
    u[5].score = sc;



    int j, temp1;
    char temp[20];

    for(i=0; i<6-1; i++)
    {
        for(j=0; j<6-i-1; j++)
        {
            if(u[j].score < u[j+1].score)
            {
                strcpy(temp,u[j+1].name);
                strcpy(u[j+1].name,u[j].name);
                strcpy(u[j].name,temp);
                temp1 = u[j+1].score;
                u[j+1].score = u[j].score;
                u[j].score = temp1;
            }
        }
    }

    fp = fopen("leaderboard.txt", "w");

    if(fp == NULL)
    {
        printf("File cannot open");
        exit(1);
    }

    cleardevice();
    settextstyle(1,0,4);
    setcolor(LIGHTGREEN);
    outtextxy(170,40,"=========");
    outtextxy(170,80,"LeaderBoard");
    outtextxy(170,120,"=========");
    settextstyle(8,0,3);
    setcolor(YELLOW);
    for(i=0; i<5; i++)
    {
        sprintf(str,"%d)%s %d",i+1,u[i].name,u[i].score);
        outtextxy(170,150+(i*40),str);

        fprintf(fp,"\n%s %d",u[i].name,u[i].score);
    }
    fclose(fp);
}

class utility{
public:
    char name[30],temp[6][30];
    int highscore;
    void display_lives_levels(){
        settextstyle(3,0,1);
        setcolor(WHITE);
        char str[10], str1[10];
        sprintf(str,"Lives left:- %d",lives);
        sprintf(str1,"Level:- %d",level);
        outtextxy(30,40,str);
        outtextxy(30,20,str1);
    }

    int getscore(int score){
        int a = (1000*(level+1) + 10*lives) - score;
        return(a);
    }

    void display_score(int score){
        settextstyle(3,0,2);
        setcolor(YELLOW);
        char str[10];
        sprintf(str,"Your score:- %d",score);
        outtextxy(230,230,str);
    }

    void draw(int x,int y)
    {
        setcolor(LIGHTMAGENTA);
        circle(x+18,y+18,14);
        setfillstyle(1,LIGHTMAGENTA);
        floodfill(x+18,y+18,LIGHTMAGENTA);
        line(x,y,x+36,y+36);
        line(x+36,y,x,y+36);
        line(x,y+18,x+36,y+18);
        line(x+18,y,x+18,y+36);
    }

    void drawmonster(){
        draw(240,200);
        delay(100);
        draw(340,300);
        delay(100);
        draw(440,100);
        delay(100);
        draw(440,350);
        delay(100);
    }

    void generatefence(){
        for(int i=200;i<=220;i++)
        {
            for(int j=00;j<=480;j++)
                putpixel(i,j,LIGHTGREEN);
        }
        for(int i=300;i<=320;i++)
        {
            for(int j=00;j<=480;j++)
                putpixel(i,j,LIGHTGREEN);
        }
        for(int i=400;i<=420;i++)
        {
            for(int j=00;j<=480;j++)
                putpixel(i,j,LIGHTGREEN);
        }
        for(int i=500;i<=520;i++)
        {
            for(int j=00;j<=480;j++)
                putpixel(i,j,LIGHTGREEN);
        }
    }
};

class block : public utility{   //20*20
int i;
public:
    int x,y;
    int height;
    int p_colour;
    int s_colour;
    block()
    {
        p_colour=CYAN;
        s_colour=LIGHTGREEN;
    }
    int fig()
    {
        if(y>=100)
        {
            for(i=y;i<=y+height;i++)
            {
                setcolor(p_colour);
                line(x,i,x+20,i);
            }
        }
    }

    int move_forward(int count)  //count by how may steps/speed
    {//depends on x value
        setcolor(p_colour);
        for(i=x+20;i<=x+20+count;i++)//to draw the forward part
        {
            line(i,y,i,y+height);
        }

        setcolor(s_colour);
        for(i=0;i<count;i++)//to erase the behind part
        {
            line(x+i,y,x+i,y+height);//to erase the behind part
        }
        x=x+count;
    }

    int move_up(int count)
    {
        setcolor(p_colour);
        for(i=y-count;i<=y;i++)//to draw the above part
        {
            line(x,i,x+20,i);
        }

        setcolor(s_colour);
        for(i=y+height-count;i<=y+height;i++)//to remove lower part
        {
            line(x,i,x+20,i);
        }
        y=y-count;
    }

    int move_down(int count)
    {
        setcolor(p_colour);
        for(i=y+height;i<=y+height+count;i++)//to draw the lower part
        {
            line(x,i,x+20,i);
        }

        setcolor(s_colour);
        for(i=y;i<=y+count;i++)//to erase the upper part
        {
            line(x,i,x+20,i);
        }
        y=y+count;
    }
    int move_back(int count)
    {
        setcolor(p_colour);
        for(i=x-count;i<=x;i++)//to draw the behind part
        {
            line(i,y,i,y+height);
        }

        setcolor(s_colour);
        for(i=x+20-count+1;i<=x+20;i++)//to erase the forawrd part
        {
            line(i,y,i,y+height);
        }
        x=x-count;
    }
};

int get_status(block *b,int *flag,int *count)
{
    if(b->y+b->height>400)
	{
	    *flag=1;
	}
	if(b->y<100)
	{
	    *flag=0;
	}

	if(*flag==0)
        b->move_down(*count);
    else if(*flag==1)
        b->move_up(*count);
}

int check_crash(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int x_user,int y_user,int height)
{
    if((x_user>=x1&&x_user<=x1+20)||(x_user+20>=x1&&x_user+20<=x1+20))
    {
        if(y1+height>=y_user+20&&y1<=y_user)
            return(0);
        else
        {
            return(1);
        }
    }

    else if((x_user>=x2&&x_user<=x2+20)||(x_user+20>=x2&&x_user+20<=x2+20))
    {
        if(y2+height>=y_user+20&&y2<=y_user)
            return(0);
        else
        {
            return(1);
        }
    }

    else if((x_user>=x3&&x_user<=x3+20)||(x_user+20>=x3&&x_user+20<=x3+20))
    {
        if(y3+height>=y_user+20&&y3<=y_user)
            return(0);
        else
        {
            return(1);
        }
    }

    else if((x_user>=x4&&x_user<=x4+20)||(x_user+20>=x4&&x_user+20<=x4+20))
    {
        if(y4+height>=y_user+20&&y4<=y_user)
            return(0);
        else
        {
            return(1);
        }
    }
}

int check_crashmonster(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,int x_user,int y_user)
{
    if(x_user>=x1-20 && x_user<=x1+40)
    {
        if(y1>=y_user && y1-60<=y_user)
            return(1);
        else
            return(0);
    }
    else if(x_user>=x2-20 && x_user<=x2+40)
    {
        if(y2>=y_user && y2-60<=y_user)
            return(1);
        else
            return(0);
    }
    else if(x_user>=x3-20 && x_user<=x3+40)
    {
        if((y3>=y_user && y3-60<=y_user) || (y4>=y_user && y4-60<=y_user)) //y3&y4 in same loop because x3 and x4 are same, hence the if condition for x3 and x4 will be same always.
            return(1);
        else
            return(0);
    }
}

int moving()
{
    setbkcolor(CYAN);
    int score=0,sum=0;
    block b1,b2,b3,b4;
    block user_block;
    int crash=0,speed=2,crash1=0;
    user_block.x=100;
    user_block.y=300;
    user_block.height=20;
    user_block.p_colour=YELLOW;
    user_block.s_colour=CYAN;
    user_block.fig();//initial x,y of user_block

    int flag1=0,flag2=0,flag3=1,flag4=0;//0 means up to down

    b1.x=200;
    b1.y=100;//y for block 1
    b1.height=80;

    b2.x=300;
    b2.y=150;
    b2.height=80;

    b3.x=400;
    b3.y=400;
    b3.height=80;

    b4.x=500;
    b4.y=320;
    b4.height=80;

    int c1=1,c2=1,c3=1,c4=1;
    char ask;

    b1.fig();
    b2.fig();
    b3.fig();
    b4.fig();

    user_block.generatefence();
    user_block.drawmonster();
    user_block.display_lives_levels();

    while(level<4 && lives>0)//change this condition
    {
        score++;
        delay(10);

        crash=check_crash(b1.x,b1.y,b2.x,b2.y,b3.x,b3.y,b4.x,b4.y,user_block.x,user_block.y,b1.height); //crash with walls
        crash1=check_crashmonster(240,238,340,338,440,138,440,388,user_block.x,user_block.y); //crash with monster // The values were taken through hit and trial for monsters so dont try to find meaning

        if((crash==1||crash1==1) && lives>0)
        {
            crash=0;
            lives--;
            user_block.x=100;
            user_block.y=300;
            cleardevice();
            setbkcolor(CYAN);
            user_block.fig();

            if(lives==0)
                    break;

            user_block.generatefence();
            user_block.drawmonster();
            user_block.display_lives_levels();
        }

        if(GetKeyState(VK_ESCAPE) & 0x8000)
        {
                exit(0);
        }

        if(GetKeyState(VK_UP) & 0x8000)
        {
            user_block.move_up(speed);
        }

        if(GetKeyState(VK_DOWN) & 0x8000)
        {
            user_block.move_down(speed);
        }

        if(GetKeyState(VK_RIGHT) & 0x8000)
        {
            user_block.move_forward(speed);
        }

        if(GetKeyState(VK_LEFT) & 0x8000)
        {
            user_block.move_back(speed);
        }

        get_status(&b1,&flag1,&c1);
        get_status(&b2,&flag2,&c2);
        get_status(&b3,&flag3,&c3);
        get_status(&b4,&flag4,&c4);

        if(user_block.x>=600)
        {
            cleardevice();
            settextstyle(8,0,5);
            setcolor(YELLOW);
            outtextxy(120,180,"Level Completed");
            sum += user_block.getscore(score);

            speed++;
            lives++;
            level++;

            if(level>=4)
                break;
            user_block.display_score(sum);
            delay(1000);

            c1++; c2++; c3++; c4++;
            b1.height=b1.height-5;
            b2.height=b2.height-5;
            b3.height=b3.height-5;
            b4.height=b4.height-5;
            cleardevice();
            setbkcolor(CYAN);

            user_block.generatefence();
            user_block.drawmonster();
            user_block.display_lives_levels();

            user_block.x = 100;
            user_block.y=300;
            user_block.fig();
        }
    }//end of inner while

    cleardevice();
    setbkcolor(CYAN);

    if(lives==0){
        cleardevice();
        settextstyle(8,0,5);
        setcolor(YELLOW);
        outtextxy(100,180,"Sorry, try again.");
        user_block.display_score(sum);
        delay(1000);
    }
    else{
        cleardevice();
        settextstyle(8,0,5);
        setcolor(YELLOW);
        outtextxy(120,180,"You've won");
        user_block.display_score(sum);
        delay(1000);
    }
    settextstyle(3,0,1);
    outtextxy(115,260,"Enter your name in the cmd window to see the leaderboard");
    leaderboard(sum);
}

int main()
{
    int gd,gm,i;
    gd=DETECT;
    initgraph(&gd,&gm,"");
    cleardevice();
    setbkcolor(CYAN);
    setcolor(CYAN);
    for(i=0;i<=640;i++)
        line(0,i,640,i);

    settextstyle(1,0,5);
    setcolor(YELLOW);
    outtextxy(20,100,"Welcome to Blocked");
    setcolor(LIGHTGREEN);
    settextstyle(3,0,3);
    outtextxy(30,160,"Instructions:- ");
    settextstyle(3,0,1);
    outtextxy(30,190,"1)Avoid the walls, obstacles and reach the other side to clear the level.");
    outtextxy(30,210,"2)There are 3 levels in the game. Complete them all to finish the game.");
    outtextxy(30,230,"3)You will get an extra life after completing each level.");
    outtextxy(30,250,"4)Complete the game in minimum time to find your name in the leaderboard.");
    outtextxy(30,270,"5)Press Esc to exit the game anytime.");
    delay(6000);

    cleardevice();
    moving();
    getchar();
    closegraph();
}
