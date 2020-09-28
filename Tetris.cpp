#define UNICODE
#include <iostream>
using namespace std;
#include <windows.h>
#include <thread>
#include <chrono>
#include <vector>

wstring tetromino[7];
int WidthF=12;
int HeigthF=20;
int WidthS=80;
int HeigthS=50;
unsigned char *Field=nullptr;
bool Ingame;
bool BKey[4];
bool HoldG;
bool Fall;
int Piece=rand()%7;
int Giro=0;
int XM=WidthF/2;
int YM=0;
int Speed=20;
int Times=0;
int Given;
vector<int> Deline;

int Rotation(int X, int Y, int G){
    switch( G % 4)
    {
        case 0: return Y*4+X;
        case 1: return 12+Y-X*4;
        case 2: return 15-Y*4-X;
        case 3: return 3-Y+X*4;
    }
    return 0;
}

bool Valid(int T, int R, int X, int Y){
    for (int PX=0;PX<4;PX++)
        for (int PY=0;PY<4;PY++){
            int PI=Rotation(PX,PY,R);
            int FI=(Y+PY)*WidthF+X+PX;
            if (X+PX >=0 && X+PX<WidthF)
            {
                if (Y+PY >=0 && Y+PY<HeigthF)
                {
                    if((tetromino[T][PI]) == (L'X') && Field[FI]!=0)return false; 
                }     
            }   
        }
    return true;
}

int main(){

    //Tetrominos
    tetromino[0]+=(L"..X.");
    tetromino[0]+=(L"..X.");
    tetromino[0]+=(L"..X.");
    tetromino[0]+=(L"..X.");

    tetromino[1]+=(L".X..");
    tetromino[1]+=(L".XX.");
    tetromino[1]+=(L"..X.");
    tetromino[1]+=(L"....");

    tetromino[2]+=(L"....");
    tetromino[2]+=(L".XX.");
    tetromino[2]+=(L".XX.");
    tetromino[2]+=(L"....");

    tetromino[3]+=(L"..X.");
    tetromino[3]+=(L".XX.");
    tetromino[3]+=(L".X.");
    tetromino[3]+=(L"....");

    tetromino[4]+=(L"..X.");
    tetromino[4]+=(L".XX.");
    tetromino[4]+=(L"..X.");
    tetromino[4]+=(L"....");

    tetromino[5]+=(L"..X.");
    tetromino[5]+=(L"..X.");
    tetromino[5]+=(L".XX.");
    tetromino[5]+=(L"....");

    tetromino[6]+=(L".X..");
    tetromino[6]+=(L".X..");
    tetromino[6]+=(L".XX.");
    tetromino[6]+=(L"....");

    Field=new unsigned char[HeigthF*WidthF];
    for(int X=0;X<WidthF;X++)
        for(int Y=0;Y<HeigthF;Y++)
            Field[Y*WidthF + X]=(X==0||X==WidthF-1||Y==HeigthF-1)?9:0;

    wchar_t *Screen=new wchar_t[WidthS*HeigthS];
    for(int I=0;I<WidthS*HeigthS;I++)Screen[I]=L' ';
    HANDLE hConsole=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,CONSOLE_TEXTMODE_BUFFER,NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten=0;
     
    Ingame=true;

    while(Ingame){
        Sleep(60);
        Times=Times+1;
        Fall=(Times==Speed);
        if(Fall){
            if (Valid(Piece,Giro,XM,YM+1)){
              YM+=1;
              Times=0;
            }
            else{
                for (int PX=0;PX<4;PX++)
                    for (int PY=0;PY<4;PY++)
                        if(tetromino[Piece][Rotation(PX,PY,Giro)]==L'X')
                            Field[(YM+PY)*WidthF+PX+XM]=Piece+1;

                Given++;
                if(Given&10==0)
                    if(Speed>=10) Speed--;

                for (int PY=0;PY<4;PY++)
                    if(YM+PY<HeigthF-1){
                        bool Line=true;
                        for(int PX=1;PX<WidthF;PX++)
                            Line&=(Field)[(YM+PY)*WidthF+PX]!=0;
                        if (Line){
                            for(int PX=1;PX<WidthF-1;PX++)
                                Field[(YM+PY)*WidthF+PX]=8;
                            Deline.push_back(YM+PY);
                        }
                    }
                XM=WidthF/2;
                YM=0;
                Giro=0;
                Piece=rand()%7;
                Ingame=(Valid(Piece,Giro,XM,YM));
                Times=0;
            }
        }

        for (int K=0;K<4;K++)
            BKey[K]=(0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28\x26"[K])))!=0;
        
        if(BKey[1]){
            if(Valid(Piece,Giro,XM-1,YM)){
                XM=XM-1;
            }
        }
        if(BKey[0]){
            if(Valid(Piece,Giro,XM+1,YM)){
                XM=XM+1;
            }
        }
        if(BKey[2]){
            if(Valid(Piece,Giro,XM,YM+1)){
                YM=YM+1;
            }
        }
        if(BKey[3]){
            if(Valid(Piece,Giro+1,XM,YM) && !HoldG){
                Giro=Giro+1;
                HoldG=true;
            }
        }else{
            HoldG=false;
        }
        

        for(int X=0;X<WidthF;X++)
            for(int Y=0;Y<HeigthF;Y++)
                Screen[(Y+5)*WidthS+X+34]=L" ABCDEFG=#"[Field[Y*WidthF+X]];
        
        for (int PX=0;PX<4;PX++)
            for (int PY=0;PY<4;PY++)
                if((tetromino[Piece][Rotation(PX,PY,Giro)])==L'X')
                    Screen[(YM+PY+5)*WidthS+(XM+PX+34)]=Piece+65;
        if (!Deline.empty()){
            WriteConsoleOutputCharacter(hConsole,Screen,HeigthS*WidthS,{0,0},&dwBytesWritten);
            Sleep(300);
            for(auto &V : Deline)
                for(int PX=1;PX<WidthF-1;PX++)
                {
                    for(int PY=V;PY>0;PY--)
                        Field[PY*WidthF+PX]=Field[(PY-1)*WidthF+PX];
                    Field[PX]=0;
                 }
            Deline.clear();    
        }
        
        WriteConsoleOutputCharacter(hConsole,Screen,HeigthS*WidthS,{0,0},&dwBytesWritten);
     }

    return 0;
}