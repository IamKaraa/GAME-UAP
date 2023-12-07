#include <iostream>
#include <conio.h>
#include <dos.h> 
#include <windows.h>
#include <time.h>
#include <fstream>
#include "MMSystem.h"

#define SCREEN_WIDTH 110 
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70
#define MENU_WIDTH 20
#define GAP_SIZE 7
#define Enemy_DIF 45

using namespace std; 


 
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char bird[3][5] = { ' ',' ','^',' ',' ',
					'|','T','H','T','|',
					'#','=','=','=','#' }; 
int birdPos = WIN_WIDTH/2;
int score = 0;
int bullets[20][4];
int bulletsLife[20];
int bIndex = 0;

void gotoxy(int x, int y){
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}
void setcursor(bool visible, DWORD size) {
	if(size == 0)
		size = 20;	
	
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}
void drawBorder(){ 
	
	for(int i=0; i<SCREEN_WIDTH; i++){ 
		gotoxy(i,SCREEN_HEIGHT); cout<<"-";
	}
	
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(0,i); cout<<"|";
		gotoxy(SCREEN_WIDTH,i); cout<<"|";
	}
	for(int i=0; i<SCREEN_HEIGHT; i++){
		gotoxy(WIN_WIDTH,i); cout<<"|";
	}
}
void genEnemy(int ind){
	enemyX[ind] = 3 + rand()%(WIN_WIDTH-10);  
}
void drawEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<".[].";  
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"[OO]"; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"[II]"; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<".[]."; 
		 
	} 
}
void eraseEnemy(int ind){
	if( enemyFlag[ind] == true ){
		gotoxy(enemyX[ind], enemyY[ind]);   cout<<"    ";  
		gotoxy(enemyX[ind], enemyY[ind]+1); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind]+2); cout<<"    "; 
		gotoxy(enemyX[ind], enemyY[ind]+3); cout<<"    "; 
	} 
}
void resetEnemy(int ind){
	eraseEnemy(ind);
	enemyY[ind] = 4;
	genEnemy(ind);
}
void genBullet(){
	bullets[bIndex][0] = 22;
	bullets[bIndex][1] = birdPos;
	bullets[bIndex][2] = 22; 
	bullets[bIndex][3] = birdPos+4; 
	bIndex++;
	if( bIndex == 20)
		bIndex = 0;
}
void moveBullet(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 2 )
			bullets[i][0]--;
		else
			bullets[i][0] = 0;
		
		if( bullets[i][2] > 2 )
			bullets[i][2]--;
		else
			bullets[i][2] = 0;
	} 
}
void drawBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] > 1){
			gotoxy(bullets[i][1],bullets[i][0]); cout<<"."; 
			gotoxy(bullets[i][3],bullets[i][2]); cout<<".";
		}
	}
}
void eraseBullets(){
	for(int i=0; i<20; i++){
		if( bullets[i][0] >= 1 ){
			gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
			gotoxy(bullets[i][3],bullets[i][2]); cout<<" ";
		}
	}
}
void eraseBullet(int i){ 
	gotoxy(bullets[i][1],bullets[i][0]); cout<<" ";
	gotoxy(bullets[i][3],bullets[i][2]); cout<<" "; 
}
void drawBird(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+birdPos, i+22); cout<<bird[i][j];
		}
	}
}
void eraseBird(){
	for(int i=0; i<3; i++){
		for(int j=0; j<5; j++){
			gotoxy(j+birdPos, i+22); cout<<" ";
		}
	}
}
 
int collision(){
	if( enemyY[0]+4 >= 23 ){
		if( enemyX[0] + 4 - birdPos >= 0 && enemyX[0] + 4 - birdPos < 8  ){
			return 1;
		}
	}
	return 0;
}
int bulletHit(){
	for(int i=0; i<20; i++){
		for(int j=0; j<4; j+=2){
			if( bullets[i][j] != 0 ){
				if( bullets[i][j] >= enemyY[0] && bullets[i][j] <= enemyY[0]+4 ){
					if( bullets[i][j+1] >= enemyX[0] && bullets[i][j+1] <= enemyX[0]+4 ){
						eraseBullet(i);
						bullets[i][j] = 0;
						resetEnemy(0);
						return 1;
					}
				}
				if( bullets[i][j] >= enemyY[1] && bullets[i][j] <= enemyY[1]+4 ){
					if( bullets[i][j+1] >= enemyX[1] && bullets[i][j+1] <= enemyX[1]+4 ){
						eraseBullet(i);
						resetEnemy(1); 
						bullets[i][j] = 0;
						return 1;
					}
				}
			}
		}
	}
	return 0;
}
void gameover(){
	system("cls");
	cout<<endl;
	cout<<"\t\t----------------------------"<<endl;
	cout<<"\t\t-------- KAMU CUPU! --------"<<endl;
	cout<<"\t\t----------------------------"<<endl<<endl;
	cout<<"\t\tLANJUT MAIN?.";
	PlaySound(TEXT("5.wav"),NULL, SND_SYNC);
	getch();
}
void updateScore(){
	gotoxy(WIN_WIDTH + 7, 5);cout<<"JUMLAH YANG KAMU BUNUH: "<<score<<endl;
}

void cara_main(){
	
	PlaySound(TEXT("3.wav"),NULL, SND_SYNC);
	
	system("cls");
	
	ifstream myfile;
	char sv_text;
	
	myfile.open("pesawatkecil.txt", ios::app);
	
	if (!myfile.fail()){
		
		while(!myfile.eof()){
			myfile.get(sv_text);
			cout << sv_text;
		}
		myfile.close();
	}
	
	cout<<"     CARA MENGALAHKAN KEBATILAN     ";
	cout<<"\n ---------------------------------";
	cout<<"\n TEKAN SPACEBAR DAN KUATKAN IMANMU";
	cout<<"\n\n   AYO TAKLUKKAN KEBATILAN!!";
	getch();
}

void bio1(){
	system("cls");
	
	ifstream myfile;
	char sv_text;
	
	myfile.open("bima.txt", ios::app);
	
	if (!myfile.fail()){
		
		while(!myfile.eof()){
			myfile.get(sv_text);
			cout << sv_text;
		}
		myfile.close();
	}
	
	gotoxy(0,8); cout<<"SATRIA BIMA BAGASKARA";
	gotoxy(0,9); cout<<"BISA DI PANGGIL BIMA";
	gotoxy(0,10); cout<<"MERUPAKAN CALON PRESIDEN BERIKUTNYA";
	gotoxy(0,11); cout<<"KLIK APAPUN UNTUK CABUT";
	getch();
	return;
}

void bio2(){
	system("cls");
	
	ifstream myfile;
	char sv_text;
	
	myfile.open("husnul.txt", ios::app);
	
	if (!myfile.fail()){
		
		while(!myfile.eof()){
			myfile.get(sv_text);
			cout << sv_text;
		}
		myfile.close();
	}
	
	gotoxy(0,8); cout<<"HUSNUL KHOTAMI";
	gotoxy(0,9); cout<<"BISA DI PANGGIL HUSNUL";
	gotoxy(0,10); cout<<"MERUPAKAN CALON PENGHUNI SURGA";
	gotoxy(0,11); cout<<"KLIK APAPUN UNTUK CABUT";
	getch();
	return;
}

void bio3(){
	system("cls");
	
	ifstream myfile;
	char sv_text;
	
	myfile.open("ipit.txt", ios::app);
	
	if (!myfile.fail()){
		
		while(!myfile.eof()){
			myfile.get(sv_text);
			cout << sv_text;
		}
		myfile.close();
	}
	
	gotoxy(0,8); cout<<"FITRIA NURAINI";
	gotoxy(0,9); cout<<"BISA DI PANGGIL IPIT";
	gotoxy(0,10); cout<<"MERUPAKAN GHIBLI LOVERS";
	gotoxy(0,11); cout<<"KLIK APAPUN UNTUK CABUT";
	getch();
	return;
}

void credit(){
	
	do{
	
	system("cls");
	
	gotoxy(10,5); cout<< "GAME PENAKLUK KEBATILAN";
	gotoxy(10,6); cout<< "-------------------------------------";
	gotoxy(10,7); cout<< "ANGGOTA : KELOMPOK 1";
	gotoxy(10,8); cout<< "-------------------------------------";
	gotoxy(10,9); cout<< "1. Satria Bima Bagaskara (2317051045)";
	gotoxy(10,10); cout<< "2. Husnul Khotami (2317051030)";
	gotoxy(10,11); cout<< "3. Fitria Nuraini (2317051021)";
	gotoxy(10,12); cout<< "4. Kembali";
	gotoxy(10,13); cout<< "-------------------------------------";
	
	gotoxy(10,15); cout<< "Source dari game yang kami kembangkan";
	gotoxy(10,16); cout<< "---------------------------------------------------";
	gotoxy(10,17); cout<< "Refrensi        : https://github.com/AngularsCoding";
	gotoxy(10,18); cout<< "Gambar/Struktur : https://www.asciiart.eu/";
	gotoxy(10,19); cout<< "Tulisan/Font    : https://patorjk.com/";
	
	
	char pilih = getche();
	
	if( pilih=='1') bio1();
		else if( pilih=='2') bio2();
		else if( pilih=='3') bio3();
		else if( pilih=='4') return;
}while(1);

}

void game(){
	
	PlaySound(TEXT("1.wav"),NULL, SND_SYNC);
	
	birdPos = -1 + WIN_WIDTH/2;
	score = 0;
	enemyFlag[0] = 1;  
	enemyFlag[1] = 1;
	enemyY[0] = enemyY[1] = 4;
	
	for(int i=0; i<20; i++){
		bullets[i][0] = bullets[i][1] = 0; 
	}
	
	system("cls"); 
	drawBorder();
	genEnemy(0);
	genEnemy(1);
	updateScore();
	
	gotoxy(WIN_WIDTH + 5, 2);cout<<"    PENAKLUK KEBATILAN    ";
	gotoxy(WIN_WIDTH + 6, 4);cout<<"--------------------------";
	gotoxy(WIN_WIDTH + 6, 6);cout<<"--------------------------";
	gotoxy(WIN_WIDTH + 7, 12);cout<<"    *PANDUAN SINGKAT*        ";
	gotoxy(WIN_WIDTH + 7, 13);cout<<"-------------------------  ";
	gotoxy(WIN_WIDTH + 2, 14);cout<<"             A - KIRI         ";
	gotoxy(WIN_WIDTH + 2, 15);cout<<"             D - KANAN    ";
	gotoxy(WIN_WIDTH + 2, 16);cout<<"          SPASI - TEMBAK";
	
	gotoxy(10, 5);cout<<"SIAP MELAWAN KEBATILAN?";
	getch();
	gotoxy(10, 5);cout<<"                       ";
	
	while(1){
		if(kbhit()){
			char ch = getch();
			if( ch=='a' || ch=='A' ){
				if( birdPos > 2 )
					birdPos -= 2;
			}
			if( ch=='d' || ch=='D' ){
				if( birdPos < WIN_WIDTH-7 )
					birdPos += 2;
			}
			if(ch==32){
				genBullet();
			} 
			if(ch==27){
				break;
			}
		}
		
		drawBird();
		drawEnemy(0);
		drawEnemy(1);
		drawBullets();
		if( collision() == 1  ){
			gameover();
			return;
		}
		if(  bulletHit() == 1 ){
			score++;
			updateScore(); 
		} 
		Sleep(200);
		eraseBird();
		eraseEnemy(0);
		eraseEnemy(1);
		eraseBullets();
		moveBullet();   
		
		if( enemyFlag[0] == 1 )
			enemyY[0] += 1;
		
		if( enemyFlag[1] == 1 )
			enemyY[1] += 1;
		 
		if( enemyY[0] > SCREEN_HEIGHT-5 ){ 
			resetEnemy(0); 
		}
		if( enemyY[1] > SCREEN_HEIGHT-5 ){ 
			resetEnemy(1); 
		}
	}
}

int main()
{
	
	system ("color F4");
	setcursor(0,0); 
	srand( (unsigned)time(NULL)); 
	 
	ifstream myfile;
	char sv_text;
	
	myfile.open("opening.txt", ios::app);
	
	if (!myfile.fail()){
		
		while(!myfile.eof()){
			myfile.get(sv_text);
			cout << sv_text;
		}
		myfile.close();
	}
	
	
	PlaySound(TEXT("2.wav"),NULL, SND_SYNC);
	gotoxy(0,28); cout<<" Tunggu sebentar ya... ";
	gotoxy(0,29); cout<<" ------------------------------- ";
	for(int i=0; i <32;i++){
		gotoxy(i,29); cout<<"=";
		Sleep(500);
	}
	
	Sleep(500); 
	
	do{
		
		system("cls");
		
	ifstream myfile;
	char sv_text;
	
	myfile.open("kastil.txt", ios::app);
	
	if (!myfile.fail()){
		
		while(!myfile.eof()){
			myfile.get(sv_text);
			cout << sv_text;
		}
		myfile.close();
	}
		
		gotoxy(10,5); cout<<" ------------------------------- "; 
		gotoxy(10,6); cout<<" |     PENAKLUK KEBATILAN      | "; 
		gotoxy(10,7); cout<<" -------------------------------";
		gotoxy(10,8); cout<<"                                 ";
		gotoxy(10,9); cout<<"1. SIAP MENAKLUKKAN KEBATILAN";
		gotoxy(10,10); cout<<"2. CARA MENAKLUKKAN KEBATILAN";	 
		gotoxy(10,11); cout<<"3. KABUR UNTUK ORANG CUPU";
		gotoxy(10,12); cout<<"4. KREDIT";
		gotoxy(10,13); cout<<"MANA YANG KAMU PILIH?  ";
		char op = getche();
		
		if( op=='1') game();
		else if( op=='2') cara_main();
		else if( op=='4') credit();
		else if( op=='3') PlaySound(TEXT("4.wav"),NULL, SND_SYNC),exit(0);
		
	}while(1);
	
	return 0;
}
