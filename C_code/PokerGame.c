#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void dealer(struct deck *card); // 카드를 입력받고 섞어주는 함수
void start(struct deck *card, int, int, int); 
void select(struct deck **hand, struct deck **end, int player); 
//히든카드포함 7장의 카드중에서 사용할 패 2장을 버리는 함수 
void winner(struct deck **end, int player); //족보별로 점수를 부여하는 함수
void victory(struct deck **end, int *, int); //부여받은 점수를 바탕으로 승자를 판별하는 함수
void bubble(int *, int *);
void bubble_struct(struct deck *, struct deck *);
void end_bubble(struct deck **, int); //최종 손패를 숫자 순서대로 나열해주는 함수
void grade_bubble(struct deck **end, int player); //최종 손패를 등급별로 나열해주는 함수
void open_hand(int a, struct deck **hand); //오픈 상태의 손패만 출력해주는 함수 ++
void hidden_hand(int a, struct deck **hand); //히든 상태의 손패를 알려주는 함수 ++
void seven_hand(int a, struct deck **hand); // 7장의 손패를 출력해주는 함수
void end_hand(int a, struct deck **end);   // 최종 결정패를 출력해주는 함수
int call(int, int);    //콜 함수 ++
int dadang(int, int); // 따당 함수 ++
int half(int, int);  //  하프 함수 ++
int full(int, int); //   풀 함수 ++

struct deck {
	int number;     //카드번호를 입력
	char name[20]; //카드 문양을 입력
	int grade;    //카드의 우선순위를 입력
	int open;    //카드가 앞면인지 뒷면인지 구분
};

int main(void) //메인함수
{
	int stack = 0;  //카드를 순차적으로 지급받기 위해 초기화 없이 증가하는 함수
	int player;    //플레이어의 숫자 
	int money;
	struct deck card[52] = { 0 }; //52장의 카드를 입력받기 위한 배열 선언
	printf("\n");
	printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
	printf("┃                        포커 게임 start                           ┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
	printf("┏━━━━━━━━━━━━━━게임설명━━━━━━━━━━━━━━━┓\n");
	printf("┣━━━━━━━━━━━━━포커 게임   ━━━━━━━━━━━━━━┫\n");
	printf("┃ 카드3장        나머지          LAST       최종         족보로    ┃\n");
	printf("┃받고 한장  ->  카드받고  ->     배팅  ->   히든카드  -> 승  부    ┃\n");
	printf("┃  오픈           오픈                      받음          결 정    ┃\n");
	printf("┣━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫\n");
	printf("┃");
	printf("     배팅은 모든 카드를 받은 후 LAST 배팅                         ");
	printf("┃\n");
	printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
	printf("\n 플레이어 수를 입력하세요 : "); //플레이어 수 입력
	scanf_s("%d", &player);
	printf("\n 기본 배팅금 입력 : "); //배팅 기본금 ++
	scanf_s("%d", &money);

	dealer(card); //카드입력 및 섞어줌
	start(card, player, stack,money); //게임 진행 함수

	return 0;
}

void dealer(struct deck *card)
{
	int a, b;                        //루프 제어 변수
	int n;                          //셔플을 위한 랜덤 값 저장 함수
	int i = 0;                     // 52까지 증가시키기 위해 초기화
	struct deck tmp = { 0 };      //셔플을 위한 저장값 저장
	srand((unsigned)time(NULL)); //랜덤 변수

	for (a = 0; a < 4; a++) {
		for (b = 0; b < 13; b++) {

			card[i].number = b + 2; //카드를 2부터 14까지 입력받음

			if (a == 0) {
				strcpy(card[i].name, "♣"); //문양 4가지 입력
			}
			else if (a == 1) {
				strcpy(card[i].name, "♥");
			}
			else if (a == 2) {
				strcpy(card[i].name, "◆");
			}
			else if (a == 3) {
				strcpy(card[i].name, "♠");
			}
			card[i].grade = (20 * a) + b + 1; 
			// 0-13(클로버), 20-33(하트), 40-53(다이아), 60-73(스페이드) 의 등급 값 저장
			card[i].open = 0; // 카드 오픈 변수(뒷면으로 저장)
			i++;
		}
	}

	for (i = 0; i < 52; i++) //랜덤 변수를 이용해 52장의 카드를 무작위로 섞어줌
	{
		n = rand() % 52;
		tmp = card[i];
		card[i] = card[n];
		card[n] = tmp;
	}
}

void start(struct deck *card, int player, int stack, int money) 
{
	int a, b, c; 
	int bat;               //배팅 선택을 저장 ++
	int bat_money=money;  //이전 턴의 배팅금액을 저장 ++
	int dead[7] = { 0 }; // 다이를 선택한 플레이어 저장 ++
	int open;            //선택한 오픈카드
	struct deck **hand; //플레이어별 손패를 저장하는 구조체 2차원 포인터
	struct deck **end;
	struct deck tmp = { 0 };

	hand = (struct deck **)malloc((player + 2) * sizeof(struct deck *));
	for (a = 0; a < player + 2; a++)
	{
		hand[a] = (struct deck *)malloc(7 * sizeof(struct deck));
	}

	end = (struct deck **)malloc((player + 2) * sizeof(struct deck *));
	for (a = 0; a < player + 2; a++)
	{
		end[a] = (struct deck *)malloc(5 * sizeof(struct deck));
	}

	for (a = 0; a < player; a++) {  //3장의 카드를 stack변수를 통해 순차적으로 지급받음
		for (b = 0; b < 3; b++) {
			hand[a][b] = card[stack];
			stack++;
		}
	}
	for (a = 0; a < player; a++) 
	{
		system("cls");   //출력화면을 지워주는 역할
		printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("┃                     %d player 차례입니다.                         ┃\n",a+1);
		printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf(" 카드 : ");
		for (b = 0; b < 3; b++) {
			switch (hand[a][b].number) {
			case 11:
				printf("%sJ ", hand[a][b].name);
				continue;
			case 12:
				printf("%sK ", hand[a][b].name);
				continue;
			case 13:
				printf("%sQ ", hand[a][b].name);
				continue;
			case 14:
				printf("%sA ", hand[a][b].name);
				continue;
			}
			printf("%s%d ", hand[a][b].name, hand[a][b].number);
		}
		printf("\n");

		printf("\n 몇 번 카드를 오픈하시겠습니까 : ");            //오픈할 카드 선택
		scanf_s("%d", &open);
		if (open > 3) {
			a--;
			continue;
		}
		hand[a][open - 1].open = 1; //입력받은 카드를 오픈상태로 변환
		hand[a][3] = card[stack];  // 다음카드를 받음
		stack++;
		hand[a][3].open = 1;     //4장부터는 오픈된 상태로
	}
	for (a = 0; a < player; a++) {
		for (b = 4; b < 7; b++) {
			hand[a][b] = card[stack]; //나머지 카드들을 오픈상태로 지급받음
			stack++;
			hand[a][b].open = 1;
		}
	}
	system("cls");
	for (a = 0; a < player; a++) {
		for (b = 0; b < 7; b++) {
			for (c = 0; c < 6; c++) {          //최종손패를 보기 쉽게 숫자순으로 나열하여 정렬
				if (hand[a][c].number < hand[a][c + 1].number) {
					tmp = hand[a][c];
					hand[a][c] = hand[a][c + 1];
					hand[a][c + 1] = tmp;
				}
			}
		}
	}
	for (a = 0; a < player; a++) {
		printf(" ----------------------------------------------\n");
		for (b = 0; b < player; b++) {
			printf("┃ %d player : ▩ ▩ ", b + 1); //플레이어들의 손패 상황을 출력
			if (dead[b] == 1)
			{
				printf("▩ ▩ ▩ ▩ ▩  die. \n"); //dead 수치가 1이면 다이 ++
				continue;
			}
			open_hand(b, hand);
			if (a-1 == b) {              //플레이어별 배팅 출력 -> 심리전 ++
				switch (bat) {
				case 1:
					printf("콜!\n");
					continue;
				case 2:
					printf("따당!\n");
					continue;
				case 3:
					printf("하프!\n");
					continue;
				case 4:
					printf("풀!\n");
					continue;
				}
			}
			printf("\n");
		}
		printf("┃---------------------------------------------\n");
		printf("┃%d plqyer \n",a+1);
		printf("┃\n");
		printf("┃총 배팅금 : %d \n",money);  // 총배팅금 출력 ++
		printf("┃\n");
		hidden_hand(a, hand);
		printf("┃\n");
		printf("┃1.콜 2.따당 3.하프 4.풀 5.다이 \n"); //총 5가지 선택지++
		printf("┃\n");                      //배팅 간소화, 도박성 낮춤 ++
		printf("┃배팅 선택지를 고르세요 : "); //플레이어별 배팅 입력 ++
		scanf_s("%d", &bat); 
		if (bat > 5 || bat < 1) {        //선택지 벗어나면 재입력 ++
			a--;
			system("cls");
			continue;
		}
		if (bat == 1) {
			money = call(money, bat_money); //콜 일 때++
		}
		else if (bat == 2) {
			money = dadang(money, bat_money); //따당 일 때++
		}
		else if (bat == 3) {
			money = half(money, bat_money); // 하프 일 때++
		}
		else if (bat == 4) {
			money = full(money, bat_money); //풀 일 때++
		}
		else if (bat == 5) {              // 다이를 선택 했을 때 ++
			dead[a]++;
			printf("┃\n");
			printf("┃다이.\n");
			printf("┃\n");
			printf("┃▩ ▩ ▩ ▩ ▩ ▩ ▩ \n"); 
			printf("┃\n");
			printf("┃전체 배팅금 -> %d\n", money);
			printf("┃\n");
		}
		else {
			a--;
			continue;
		}
		printf("┃아무숫자를 입력하세요 : ");
		scanf_s("%d", &c);
		system("cls");
	}
	select(hand, end, player);
	winner(end, player);      //족보별로 점수를 부여하는 함수

	for (a = 0; a < (player + 2); a++) //메모리할당 해제
	{
		free(hand[a]);
	}
	free(hand);

	for (a = 0; a < (player + 2); a++) //메모리할당 해제
	{
		free(end[a]);
	}
	free(end);
}

void select(struct deck **hand, struct deck **end, int player)
{
	int a, b, i;
	int c = 0;
	int select[2] = { 0 };   //플레이어가 선택한 카드번호를 저장

	for (a = 0; a < player; a++) {
		printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("┃                     %d player 차례입니다.                         ┃\n", a + 1);
		printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf(" 버리는 카드 2장 선택.\n\n");  //버릴 카드 선택 물음과 함께 7장의 카드를 나열하여 보여줌
		seven_hand(a, hand);
		printf("\n\n");
		for (b = 0; b < 2; b++) {
			printf(" %d 번째 카드 : ", b + 1);  //2장의 버릴 카드를 선택 
			scanf_s("%d", &select[b]);
			if (select[b] > 7 || select[b] < 0) { // 7이상 0미만을 입력했을 때
				printf("------------------------------------------\n");
				printf(" 몇 번째 카드인지만 입력가능합니다. \n");
				printf("------------------------------------------\n");
				b--;
				continue;
			}
			if (select[0] == select[1]) { //같은 카드를 중복 선택했을 때
				printf("------------------------------------------\n");
				printf(" 다시 입력해주세요. (중복불가) \n");
				printf("------------------------------------------\n");
				b--;
				continue;
			}
		}
		for (b = 0; b < 7; b++) {
			if ((b == select[0] - 1) || (b == select[1] - 1)) {
				continue;  //버린 카드는 주지 않고 다음 과정으로 넘어감
			}
			end[a][c] = hand[a][b];
			c++; //카드 분배 스택
		}
		select[1] = 0; // 선택 초기화
		c = 0;        // 스택 초기화

		printf("\n 최종 패 : ");
		end_hand(a, end);
		printf("\n");
		printf("\n 다음 단계는 아무숫자를 입력하세요 : "); //다음단계로 진행
		scanf_s("%d", &i);
		system("cls");
	}
	printf(" ----------------------------------------------\n");
	for (a = 0; a < player; a++) {
		printf("┃ %d player card : ", a + 1);
		end_hand(a, end);
		printf("\n");
	}
	printf("┃---------------------------------------------\n");
}

void winner(struct deck **end,int player)
{
	int a,b;
	int score[7] = { 0 }; //'족보의 점수'
	int same[7] = { 0 }; //'카드의 중복 횟수'를 플레이어별로 저장

	end_bubble(end, player); //족보를 판별하기 위해 카드의 숫자순서대로 나열

	for (a = 0; a<player; a++) {  //카드가 몇 번 중복되었는지 계산
		for (b = 0; b < 4; b++) {
			if (end[a][b].number == end[a][b+1].number) {
				same[a] = same[a] + 1;
			}
		}
	}	 

	for (a = 0; a < player; a++) {
		printf("┃ ");
		if (end[a][0].name == end[a][1].name && end[a][1].name == end[a][2].name && end[a][2].name == end[a][3].name && end[a][3].name == end[a][4].name) {
			if (end[a][0].number == 14 && end[a][1].number == 13 && end[a][2].number == 12 && end[a][3].number == 11 && end[a][4].number == 10) { 
				score[a] = 13; // 로얄 스트레이트 플러쉬
				printf("%d player - 로얄 스트레이트 플러쉬!\n", a + 1);
			}
			else if (end[a][0].number == 14 && end[a][1].number == 2 && end[a][2].number == 3 && end[a][3].number == 4 && end[a][4].number == 5) {
				score[a] = 12; // 백스트레이트 플러쉬
				printf("%d player 의 - 스트레이트 플러쉬!\n", a + 1);
			}
			else if (end[a][0].number-1 == end[a][1].number && end[a][1].number -1 == end[a][2].number && end[a][2].number - 1 == end[a][3].number && end[a][3].number-1 == end[a][4].number) {
				score[a] = 11; // 스트레이트 플러쉬
				printf("%d player - 스트레이트 플러쉬!\n", a + 1);
			}
			else {
				score[a] = 8; // 플러쉬
				printf("%d player - 플러쉬!\n", a + 1);
			}
		}
		else if (same[a] + 1 == 4) {
			if (end[a][1].number == end[a][2].number && end[a][2].number == end[a][3].number) {
				score[a] = 10; //포카드
				printf("%d player - 포카드!\n", a + 1);
			}
			else if(end[a][0].number == end[a][2].number || end[a][2].number == end[a][4].number) {
				score[a] = 9; //풀하우스
				printf("%d player - 풀하우스!\n", a + 1);
			}
		}
		else if (end[a][0].number == 14 && end[a][1].number == 13 && end[a][2].number == 12 && end[a][3].number == 11 && end[a][4].number == 10) {
			score[a] = 7; //마운틴
			printf("%d player - 마운틴!\n", a + 1);
		}
		else if (end[a][0].number == 14 && end[a][1].number == 2 && end[a][2].number == 3 && end[a][3].number == 4 && end[a][4].number == 5) {
			score[a] = 6; //백스트레이트
			printf("%d player 의 - 백스트레이트!\n", a + 1);
		}
		else if (end[a][0].number - 1 == end[a][1].number && end[a][1].number - 1 == end[a][2].number && end[a][2].number - 1 == end[a][3].number && end[a][3].number - 1 == end[a][4].number) {
			score[a] = 5; //스트레이트
			printf("%d player - 스트레이트!\n", a + 1);
		}
		else if (same[a] + 1 == 3) {
			if ((end[a][0].number == end[a][2].number) || (end[a][1].number == end[a][3].number) || (end[a][2].number == end[a][4].number)) {
				score[a] = 4; //트리플
				printf("%d player - 트리플!\n", a + 1);
			}
			else {
				score[a] = 3; //투페어
				printf("%d player - 투페어\n", a + 1);
			}
		}
		else if (same[a] + 1 == 2) {
			score[a] = 2; //원페어
			printf("%d player - 원페어\n", a + 1);
		}
		else if (same[a] + 1 == 1) {
			score[a] = 1; //노페어
			printf("%d player - 노페어\n", a + 1);
		}
	}
	grade_bubble(end, player); //카드를 다시 등급순서대로 나열 top카드를 판별하기 위함
	victory(end, score, player); //승자를 결정하는 함수
}

void victory(struct deck **end, int *score, int player)
{
	int a, b;
	int number[7] = { 0 };
	int top = 0;
	int top_grade = 0;

	for (a = 0; a < player; a++) //선수의 번호를 number값에 저장
	{
		number[a] = a;
	}

	for (a = 0; a < player; a++)
	{ //점수가 높은 선수와 score를 높은 점수부터 내림차순으로 나열
		for (b = 0; b < player - 1; b++)
		{
			if (score[b] < score[b + 1])
			{
				bubble_struct(&end[b][0], &end[b+1][0]);
				bubble(&score[b], &score[b + 1]);
				bubble(&number[b], &number[b + 1]);
			}
		}
	}
	top_grade = end[0][0].grade;
	top = number[0];


	for (b = 0; b < player - 1; b++) { 
		if (score[0] == score[b + 1]) {
			if (top_grade < end[b + 1][0].grade) {
				top_grade = end[b + 1][0].grade;
				top = number[b + 1];
			}
		}
	}
	printf("┃\n┃승자는 %d번 player입니다 -> 배팅금 지급\n\n", top + 1);
}

void bubble(int *i, int *j)
{
	int t = *i;
	*i = *j;
	*j = t;
}
void bubble_struct(struct deck *i, struct deck *j) //순위를 나열하기위해 
{                                                 //구조체 자체를 버블정렬
	struct deck t = { 0 };
	t = *i;
	*i = *j;
	*j = t;
}

void grade_bubble(struct deck **end, int player) //등급 별로 카드를 나열하는 함수
{
	int a, b, c;
	struct deck tmp;

	for (a = 0; a < player; a++) {
		for (b = 0; b < 5; b++) {
			for (c = 0; c < 4; c++) {
				if (end[a][c].grade < end[a][c + 1].grade) {
					tmp = end[a][c];
					end[a][c] = end[a][c + 1];
					end[a][c + 1] = tmp;
				}
			}
		}
	}
}

void end_bubble(struct deck **end, int player)
{
	int a, b, c;
	struct deck tmp;

	for (a = 0; a < player; a++) {
		for (b = 0; b < 5; b++) {
			for (c = 0; c < 4; c++) {
				if (end[a][c].number < end[a][c + 1].number) {
					tmp = end[a][c];
					end[a][c] = end[a][c + 1];
					end[a][c + 1] = tmp;
				}
			}
		}
	}
}

void open_hand(int a, struct deck **hand)
{
	int b;

	for (b = 0; b < 7; b++) {
		if (hand[a][b].open == 1) {
			switch (hand[a][b].number) {
			case 11:
				printf("%sJ ", hand[a][b].name);
				continue;
			case 12:
				printf("%sK ", hand[a][b].name);
				continue;
			case 13:
				printf("%sQ ", hand[a][b].name);
				continue;
			case 14:
				printf("%sA ", hand[a][b].name);
				continue;
			}
			printf("%s%d ", hand[a][b].name, hand[a][b].number);
		}
	}
}

void hidden_hand(int a, struct deck **hand)
{
	int b;

	printf("┃히든 카드 : ");
	for (b = 0; b < 7; b++) {
		if (hand[a][b].open == 0) {
			switch (hand[a][b].number) {
			case 11:
				printf("%sJ ", hand[a][b].name);
				continue;
			case 12:
				printf("%sK ", hand[a][b].name);
				continue;
			case 13:
				printf("%sQ ", hand[a][b].name);
				continue;
			case 14:
				printf("%sA ", hand[a][b].name);
				continue;
			}
			printf("%s%d ", hand[a][b].name, hand[a][b].number);
		}
	}
	printf("\n");
}

void seven_hand(int a, struct deck **hand)
{
	int b;
	printf(" ");
	for (b = 0; b < 7; b++) {
		switch (hand[a][b].number) {
		case 11:
			printf("%sJ ", hand[a][b].name);
			continue;
		case 12:
			printf("%sK ", hand[a][b].name);
			continue;
		case 13:
			printf("%sQ ", hand[a][b].name);
			continue;
		case 14:
			printf("%sA ", hand[a][b].name);
			continue;
		}
		printf("%s%d ", hand[a][b].name, hand[a][b].number);
	}
}

void end_hand(int a, struct deck **end)
{
	int b;
	for (b = 0; b < 5; b++) {                 //정렬된 최종패를 출력
		switch (end[a][b].number) {
		case 11:
			printf("%sJ ", end[a][b].name);
			continue;
		case 12:
			printf("%sK ", end[a][b].name);
			continue;
		case 13:
			printf("%sQ ", end[a][b].name);
			continue;
		case 14:
			printf("%sA ", end[a][b].name);
			continue;
		}
		printf("%s%d ", end[a][b].name, end[a][b].number);
	}
}

int call(int money, int bat_money)
{
	money = money + bat_money;
	printf("┃\n");
	printf("┃콜!\n");
	printf("┃\n");
	printf("┃전체 배팅금 -> %d\n",money);
	printf("┃\n");

	return money;
}
int dadang(int money, int bat_money)
{
	bat_money = 2*bat_money ;
	money = money + bat_money;
	printf("┃\n");
	printf("┃따당!\n");
	printf("┃\n");
	printf("┃전체 배팅금 -> %d\n", money);
	printf("┃\n");

	return money;
}
int half(int money, int bat_money)
{
	bat_money = money/2;
	money = money + bat_money;
	printf("┃\n");
	printf("┃하프!\n");
	printf("┃\n");
	printf("┃전체 배팅금 -> %d\n", money);
	printf("┃\n");

	return money;
}
int full(int money, int bat_money)
{
	bat_money = money;
	money = 2*money;
	printf("┃\n");
	printf("┃풀!\n");
	printf("┃\n");
	printf("┃전체 배팅금 -> %d\n", money);
	printf("┃\n");

	return money;
}