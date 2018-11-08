#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include "res.h"
#include <ctype.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>

#define LEVELSC 5
#define LOG_KEYBORD(a,str) if(a!=NULL){fwrite(str,1,strlen(str),a);fwrite("\r\n",1,2,a);fflush(a);}

extern char g_alpha[26];

int getRandom(int lev,char* word)
{
	SYSTEMTIME tmp;
	GetLocalTime(&tmp);
	char tmpWord[128];
	for(int i=0;i<lev;i++)
	{
		srand(tmp.wMilliseconds*(i+1));
		tmpWord[i]=g_alpha[rand()%26];
	}
	tmpWord[lev]='\0';
	strncpy(word,tmpWord,lev+1);
	return 0;
}
typedef union
{
	float a;
	char b[4];
}dm;
void printStart()
{
	printf("****************************************\n");
	printf("keyboard excise\n");
	printf("level one to level one hundred\n");
	printf("****************************************\n");
	printf("please input your level number\n");
}
void startThread(void *p)
{
	bool* pflag = (bool*)p;
	int Ctrl = 0;
	while(1)
	{
		if(*pflag)
		{
			Ctrl++;
			Sleep(9);
			if(Ctrl >= 110)
			{
				*pflag = false;
				Ctrl = 0;
			}
		}
		else
		{
			if(Ctrl != 0)
			{
				Ctrl = 0;
			}
			Sleep(10);
		}
	}
}
int startLevel()
{
	char levelstr[16];
	char* retchr=gets(levelstr);
	int levelnum=atoi(levelstr);
	if(levelnum!=0)
	{
		if(levelnum<0)
			levelnum=1;
		else if(levelnum>100)
			levelnum=100;
	}else
	{
		printf("please input a number between 1 and 100\n");
		startLevel();
	}
	return levelnum;
}
void doExcise(int lev, int* psc, char* showord, char *inputWord, bool* flag,FILE* fp)
{
	LOG_KEYBORD(fp,__FUNCTION__);
	char logLev[16]="level ";
	sprintf(logLev+6,"%d",lev);
	LOG_KEYBORD(fp,logLev);
	int score=*psc;
	char logStr[128]={0};
	char* pLog=NULL;
	if(lev>0 && lev <=10)
	{
		while(1)
		{
			system("cls");
			memset(logStr,0,128);
			memset(showord,0,64);
			memset(inputWord,0,64);
			pLog = logStr;
			SYSTEMTIME tmpTime;
			GetLocalTime(&tmpTime);
			srand(tmpTime.wMilliseconds);
			int tmpLv = rand()%lev+1;
			int a = getRandom(tmpLv,showord);
			printf("%s\n",showord);
			strncpy(pLog,"showword is:",12);
			pLog += 12;
			strncpy(pLog,showord,tmpLv);
			pLog+=lev;
			pLog[0]='\t';
			pLog++;
			
			//gets(inputWord);
			fflush(stdin);
			a = 0;
			*flag = true;
			while(*flag)
			{
				if((inputWord[a] = getch()) == showord[a])
				{
					a++;
					if(a == tmpLv)
					{
						*flag = false;
					}
				}
				else
				{
					*flag = false;
				}
			}
			//*flag = false;
			if(a < tmpLv)
			{
				printf("you lose\n");
			}
			strncpy(pLog,"inputWord is:",13);
			pLog += 13;
			//a = tmpLv;
			//while(a > 0)
			//{
			//	fflush(stdin);
			//	char tmpChar=getchar();
			//	if(tmpChar == showord[tmpLv-a])
			//	{
			//		inputWord[a]=tmpChar;
			//		a--;
			//	}
			//	else
			//	{
			//		break;
			//	}
			//}
			strncpy(pLog,inputWord,sizeof(inputWord));
			pLog+=sizeof(inputWord);
			LOG_KEYBORD(fp,logStr);
			if(' '==inputWord[0])
			{
				break;
			}
			else if(strncmp(inputWord,showord,tmpLv)==0)
			{
				score ++;
			}
			else
			{
				score --;
			}
			if(score >= LEVELSC || score < 0)
			{
				break;
			}
		}
	}
	else if(lev>10 && lev <=20)
	{

	}
	*psc=score;
}
void main(int args,char** argv)
{
	time_t curtime=NULL;
	char* path=NULL;
	if(args>1)
	{
		path=argv[1];
	}
	else
	{
		path="keybord.txt";
	}
	FILE* flog=fopen(path,"a+");
	time(&curtime);
	struct tm* logtime=localtime(&curtime);
	char* plogtime=asctime(logtime);
	LOG_KEYBORD(flog,plogtime);
	printStart();
	int lvnum = startLevel();
	printf("you chose level %d,during the excise press 'esr' to stop\n",lvnum);
	printf("if you are ready, press any key to start\n");
	system("pause");
	char showord[64] = {0};
	char inputWord[64] = {0};
	bool ffff=false;
	bool* flag = &ffff;
	HANDLE handle = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)startThread,(LPVOID)flag ,1,0);
	DWORD ret = ResumeThread(handle);
	int sc=0;

	while(1)
	{
		if(lvnum<=100)
		{
			sc = 0;
			doExcise(lvnum, &sc, showord, inputWord, flag,flog);
			if(sc >= LEVELSC)
			{
				printf("Level up!\nIf you are ready,press any key to start level %d.\n",++lvnum);
				system("pause");
			}
			else
			{
				printf("do you want continue? press y/n.\n");
				fflush(stdin);
				char tt = getchar();
				if(tt == 'y' || tt == 'Y')
				{
					printf("Level down!\nIf you are ready,press any key to start level %d.\n",--lvnum);
					system("pause");
					continue;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			printf("Your level is .\nYou are graduate!\n",lvnum);
			break;
		}
	}
	if(flog)
	{
		fclose(flog);
	}
	printf("Your score is %d\n",sc);
	CloseHandle(handle);
	system("pause");
	return;
}