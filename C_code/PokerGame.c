#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void dealer(struct deck *card); // ī�带 �Է¹ް� �����ִ� �Լ�
void start(struct deck *card, int, int, int); 
void select(struct deck **hand, struct deck **end, int player); 
//����ī������ 7���� ī���߿��� ����� �� 2���� ������ �Լ� 
void winner(struct deck **end, int player); //�������� ������ �ο��ϴ� �Լ�
void victory(struct deck **end, int *, int); //�ο����� ������ �������� ���ڸ� �Ǻ��ϴ� �Լ�
void bubble(int *, int *);
void bubble_struct(struct deck *, struct deck *);
void end_bubble(struct deck **, int); //���� ���и� ���� ������� �������ִ� �Լ�
void grade_bubble(struct deck **end, int player); //���� ���и� ��޺��� �������ִ� �Լ�
void open_hand(int a, struct deck **hand); //���� ������ ���и� ������ִ� �Լ� ++
void hidden_hand(int a, struct deck **hand); //���� ������ ���и� �˷��ִ� �Լ� ++
void seven_hand(int a, struct deck **hand); // 7���� ���и� ������ִ� �Լ�
void end_hand(int a, struct deck **end);   // ���� �����и� ������ִ� �Լ�
int call(int, int);    //�� �Լ� ++
int dadang(int, int); // ���� �Լ� ++
int half(int, int);  //  ���� �Լ� ++
int full(int, int); //   Ǯ �Լ� ++

struct deck {
	int number;     //ī���ȣ�� �Է�
	char name[20]; //ī�� ������ �Է�
	int grade;    //ī���� �켱������ �Է�
	int open;    //ī�尡 �ո����� �޸����� ����
};

int main(void) //�����Լ�
{
	int stack = 0;  //ī�带 ���������� ���޹ޱ� ���� �ʱ�ȭ ���� �����ϴ� �Լ�
	int player;    //�÷��̾��� ���� 
	int money;
	struct deck card[52] = { 0 }; //52���� ī�带 �Է¹ޱ� ���� �迭 ����
	printf("\n");
	printf("����������������������������������������������������������������������\n");
	printf("��                        ��Ŀ ���� start                           ��\n");
	printf("����������������������������������������������������������������������\n\n");
	printf("���������������������������������Ӽ���������������������������������\n");
	printf("������������������������������Ŀ ����   ������������������������������\n");
	printf("�� ī��3��        ������          LAST       ����         ������    ��\n");
	printf("���ް� ����  ->  ī��ް�  ->     ����  ->   ����ī��  -> ��  ��    ��\n");
	printf("��  ����           ����                      ����          �� ��    ��\n");
	printf("����������������������������������������������������������������������\n");
	printf("��");
	printf("     ������ ��� ī�带 ���� �� LAST ����                         ");
	printf("��\n");
	printf("����������������������������������������������������������������������\n");
	printf("\n �÷��̾� ���� �Է��ϼ��� : "); //�÷��̾� �� �Է�
	scanf_s("%d", &player);
	printf("\n �⺻ ���ñ� �Է� : "); //���� �⺻�� ++
	scanf_s("%d", &money);

	dealer(card); //ī���Է� �� ������
	start(card, player, stack,money); //���� ���� �Լ�

	return 0;
}

void dealer(struct deck *card)
{
	int a, b;                        //���� ���� ����
	int n;                          //������ ���� ���� �� ���� �Լ�
	int i = 0;                     // 52���� ������Ű�� ���� �ʱ�ȭ
	struct deck tmp = { 0 };      //������ ���� ���尪 ����
	srand((unsigned)time(NULL)); //���� ����

	for (a = 0; a < 4; a++) {
		for (b = 0; b < 13; b++) {

			card[i].number = b + 2; //ī�带 2���� 14���� �Է¹���

			if (a == 0) {
				strcpy(card[i].name, "��"); //���� 4���� �Է�
			}
			else if (a == 1) {
				strcpy(card[i].name, "��");
			}
			else if (a == 2) {
				strcpy(card[i].name, "��");
			}
			else if (a == 3) {
				strcpy(card[i].name, "��");
			}
			card[i].grade = (20 * a) + b + 1; 
			// 0-13(Ŭ�ι�), 20-33(��Ʈ), 40-53(���̾�), 60-73(�����̵�) �� ��� �� ����
			card[i].open = 0; // ī�� ���� ����(�޸����� ����)
			i++;
		}
	}

	for (i = 0; i < 52; i++) //���� ������ �̿��� 52���� ī�带 �������� ������
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
	int bat;               //���� ������ ���� ++
	int bat_money=money;  //���� ���� ���ñݾ��� ���� ++
	int dead[7] = { 0 }; // ���̸� ������ �÷��̾� ���� ++
	int open;            //������ ����ī��
	struct deck **hand; //�÷��̾ ���и� �����ϴ� ����ü 2���� ������
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

	for (a = 0; a < player; a++) {  //3���� ī�带 stack������ ���� ���������� ���޹���
		for (b = 0; b < 3; b++) {
			hand[a][b] = card[stack];
			stack++;
		}
	}
	for (a = 0; a < player; a++) 
	{
		system("cls");   //���ȭ���� �����ִ� ����
		printf("����������������������������������������������������������������������\n");
		printf("��                     %d player �����Դϴ�.                         ��\n",a+1);
		printf("����������������������������������������������������������������������\n\n");
		printf(" ī�� : ");
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

		printf("\n �� �� ī�带 �����Ͻðڽ��ϱ� : ");            //������ ī�� ����
		scanf_s("%d", &open);
		if (open > 3) {
			a--;
			continue;
		}
		hand[a][open - 1].open = 1; //�Է¹��� ī�带 ���»��·� ��ȯ
		hand[a][3] = card[stack];  // ����ī�带 ����
		stack++;
		hand[a][3].open = 1;     //4����ʹ� ���µ� ���·�
	}
	for (a = 0; a < player; a++) {
		for (b = 4; b < 7; b++) {
			hand[a][b] = card[stack]; //������ ī����� ���»��·� ���޹���
			stack++;
			hand[a][b].open = 1;
		}
	}
	system("cls");
	for (a = 0; a < player; a++) {
		for (b = 0; b < 7; b++) {
			for (c = 0; c < 6; c++) {          //�������и� ���� ���� ���ڼ����� �����Ͽ� ����
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
			printf("�� %d player : �� �� ", b + 1); //�÷��̾���� ���� ��Ȳ�� ���
			if (dead[b] == 1)
			{
				printf("�� �� �� �� ��  die. \n"); //dead ��ġ�� 1�̸� ���� ++
				continue;
			}
			open_hand(b, hand);
			if (a-1 == b) {              //�÷��̾ ���� ��� -> �ɸ��� ++
				switch (bat) {
				case 1:
					printf("��!\n");
					continue;
				case 2:
					printf("����!\n");
					continue;
				case 3:
					printf("����!\n");
					continue;
				case 4:
					printf("Ǯ!\n");
					continue;
				}
			}
			printf("\n");
		}
		printf("��---------------------------------------------\n");
		printf("��%d plqyer \n",a+1);
		printf("��\n");
		printf("���� ���ñ� : %d \n",money);  // �ѹ��ñ� ��� ++
		printf("��\n");
		hidden_hand(a, hand);
		printf("��\n");
		printf("��1.�� 2.���� 3.���� 4.Ǯ 5.���� \n"); //�� 5���� ������++
		printf("��\n");                      //���� ����ȭ, ���ڼ� ���� ++
		printf("������ �������� ������ : "); //�÷��̾ ���� �Է� ++
		scanf_s("%d", &bat); 
		if (bat > 5 || bat < 1) {        //������ ����� ���Է� ++
			a--;
			system("cls");
			continue;
		}
		if (bat == 1) {
			money = call(money, bat_money); //�� �� ��++
		}
		else if (bat == 2) {
			money = dadang(money, bat_money); //���� �� ��++
		}
		else if (bat == 3) {
			money = half(money, bat_money); // ���� �� ��++
		}
		else if (bat == 4) {
			money = full(money, bat_money); //Ǯ �� ��++
		}
		else if (bat == 5) {              // ���̸� ���� ���� �� ++
			dead[a]++;
			printf("��\n");
			printf("������.\n");
			printf("��\n");
			printf("���� �� �� �� �� �� �� \n"); 
			printf("��\n");
			printf("����ü ���ñ� -> %d\n", money);
			printf("��\n");
		}
		else {
			a--;
			continue;
		}
		printf("���ƹ����ڸ� �Է��ϼ��� : ");
		scanf_s("%d", &c);
		system("cls");
	}
	select(hand, end, player);
	winner(end, player);      //�������� ������ �ο��ϴ� �Լ�

	for (a = 0; a < (player + 2); a++) //�޸��Ҵ� ����
	{
		free(hand[a]);
	}
	free(hand);

	for (a = 0; a < (player + 2); a++) //�޸��Ҵ� ����
	{
		free(end[a]);
	}
	free(end);
}

void select(struct deck **hand, struct deck **end, int player)
{
	int a, b, i;
	int c = 0;
	int select[2] = { 0 };   //�÷��̾ ������ ī���ȣ�� ����

	for (a = 0; a < player; a++) {
		printf("����������������������������������������������������������������������\n");
		printf("��                     %d player �����Դϴ�.                         ��\n", a + 1);
		printf("����������������������������������������������������������������������\n\n");
		printf(" ������ ī�� 2�� ����.\n\n");  //���� ī�� ���� ������ �Բ� 7���� ī�带 �����Ͽ� ������
		seven_hand(a, hand);
		printf("\n\n");
		for (b = 0; b < 2; b++) {
			printf(" %d ��° ī�� : ", b + 1);  //2���� ���� ī�带 ���� 
			scanf_s("%d", &select[b]);
			if (select[b] > 7 || select[b] < 0) { // 7�̻� 0�̸��� �Է����� ��
				printf("------------------------------------------\n");
				printf(" �� ��° ī�������� �Է°����մϴ�. \n");
				printf("------------------------------------------\n");
				b--;
				continue;
			}
			if (select[0] == select[1]) { //���� ī�带 �ߺ� �������� ��
				printf("------------------------------------------\n");
				printf(" �ٽ� �Է����ּ���. (�ߺ��Ұ�) \n");
				printf("------------------------------------------\n");
				b--;
				continue;
			}
		}
		for (b = 0; b < 7; b++) {
			if ((b == select[0] - 1) || (b == select[1] - 1)) {
				continue;  //���� ī��� ���� �ʰ� ���� �������� �Ѿ
			}
			end[a][c] = hand[a][b];
			c++; //ī�� �й� ����
		}
		select[1] = 0; // ���� �ʱ�ȭ
		c = 0;        // ���� �ʱ�ȭ

		printf("\n ���� �� : ");
		end_hand(a, end);
		printf("\n");
		printf("\n ���� �ܰ�� �ƹ����ڸ� �Է��ϼ��� : "); //�����ܰ�� ����
		scanf_s("%d", &i);
		system("cls");
	}
	printf(" ----------------------------------------------\n");
	for (a = 0; a < player; a++) {
		printf("�� %d player card : ", a + 1);
		end_hand(a, end);
		printf("\n");
	}
	printf("��---------------------------------------------\n");
}

void winner(struct deck **end,int player)
{
	int a,b;
	int score[7] = { 0 }; //'������ ����'
	int same[7] = { 0 }; //'ī���� �ߺ� Ƚ��'�� �÷��̾�� ����

	end_bubble(end, player); //������ �Ǻ��ϱ� ���� ī���� ���ڼ������ ����

	for (a = 0; a<player; a++) {  //ī�尡 �� �� �ߺ��Ǿ����� ���
		for (b = 0; b < 4; b++) {
			if (end[a][b].number == end[a][b+1].number) {
				same[a] = same[a] + 1;
			}
		}
	}	 

	for (a = 0; a < player; a++) {
		printf("�� ");
		if (end[a][0].name == end[a][1].name && end[a][1].name == end[a][2].name && end[a][2].name == end[a][3].name && end[a][3].name == end[a][4].name) {
			if (end[a][0].number == 14 && end[a][1].number == 13 && end[a][2].number == 12 && end[a][3].number == 11 && end[a][4].number == 10) { 
				score[a] = 13; // �ξ� ��Ʈ����Ʈ �÷���
				printf("%d player - �ξ� ��Ʈ����Ʈ �÷���!\n", a + 1);
			}
			else if (end[a][0].number == 14 && end[a][1].number == 2 && end[a][2].number == 3 && end[a][3].number == 4 && end[a][4].number == 5) {
				score[a] = 12; // �齺Ʈ����Ʈ �÷���
				printf("%d player �� - ��Ʈ����Ʈ �÷���!\n", a + 1);
			}
			else if (end[a][0].number-1 == end[a][1].number && end[a][1].number -1 == end[a][2].number && end[a][2].number - 1 == end[a][3].number && end[a][3].number-1 == end[a][4].number) {
				score[a] = 11; // ��Ʈ����Ʈ �÷���
				printf("%d player - ��Ʈ����Ʈ �÷���!\n", a + 1);
			}
			else {
				score[a] = 8; // �÷���
				printf("%d player - �÷���!\n", a + 1);
			}
		}
		else if (same[a] + 1 == 4) {
			if (end[a][1].number == end[a][2].number && end[a][2].number == end[a][3].number) {
				score[a] = 10; //��ī��
				printf("%d player - ��ī��!\n", a + 1);
			}
			else if(end[a][0].number == end[a][2].number || end[a][2].number == end[a][4].number) {
				score[a] = 9; //Ǯ�Ͽ콺
				printf("%d player - Ǯ�Ͽ콺!\n", a + 1);
			}
		}
		else if (end[a][0].number == 14 && end[a][1].number == 13 && end[a][2].number == 12 && end[a][3].number == 11 && end[a][4].number == 10) {
			score[a] = 7; //����ƾ
			printf("%d player - ����ƾ!\n", a + 1);
		}
		else if (end[a][0].number == 14 && end[a][1].number == 2 && end[a][2].number == 3 && end[a][3].number == 4 && end[a][4].number == 5) {
			score[a] = 6; //�齺Ʈ����Ʈ
			printf("%d player �� - �齺Ʈ����Ʈ!\n", a + 1);
		}
		else if (end[a][0].number - 1 == end[a][1].number && end[a][1].number - 1 == end[a][2].number && end[a][2].number - 1 == end[a][3].number && end[a][3].number - 1 == end[a][4].number) {
			score[a] = 5; //��Ʈ����Ʈ
			printf("%d player - ��Ʈ����Ʈ!\n", a + 1);
		}
		else if (same[a] + 1 == 3) {
			if ((end[a][0].number == end[a][2].number) || (end[a][1].number == end[a][3].number) || (end[a][2].number == end[a][4].number)) {
				score[a] = 4; //Ʈ����
				printf("%d player - Ʈ����!\n", a + 1);
			}
			else {
				score[a] = 3; //�����
				printf("%d player - �����\n", a + 1);
			}
		}
		else if (same[a] + 1 == 2) {
			score[a] = 2; //�����
			printf("%d player - �����\n", a + 1);
		}
		else if (same[a] + 1 == 1) {
			score[a] = 1; //�����
			printf("%d player - �����\n", a + 1);
		}
	}
	grade_bubble(end, player); //ī�带 �ٽ� ��޼������ ���� topī�带 �Ǻ��ϱ� ����
	victory(end, score, player); //���ڸ� �����ϴ� �Լ�
}

void victory(struct deck **end, int *score, int player)
{
	int a, b;
	int number[7] = { 0 };
	int top = 0;
	int top_grade = 0;

	for (a = 0; a < player; a++) //������ ��ȣ�� number���� ����
	{
		number[a] = a;
	}

	for (a = 0; a < player; a++)
	{ //������ ���� ������ score�� ���� �������� ������������ ����
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
	printf("��\n�����ڴ� %d�� player�Դϴ� -> ���ñ� ����\n\n", top + 1);
}

void bubble(int *i, int *j)
{
	int t = *i;
	*i = *j;
	*j = t;
}
void bubble_struct(struct deck *i, struct deck *j) //������ �����ϱ����� 
{                                                 //����ü ��ü�� ��������
	struct deck t = { 0 };
	t = *i;
	*i = *j;
	*j = t;
}

void grade_bubble(struct deck **end, int player) //��� ���� ī�带 �����ϴ� �Լ�
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

	printf("������ ī�� : ");
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
	for (b = 0; b < 5; b++) {                 //���ĵ� �����и� ���
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
	printf("��\n");
	printf("����!\n");
	printf("��\n");
	printf("����ü ���ñ� -> %d\n",money);
	printf("��\n");

	return money;
}
int dadang(int money, int bat_money)
{
	bat_money = 2*bat_money ;
	money = money + bat_money;
	printf("��\n");
	printf("������!\n");
	printf("��\n");
	printf("����ü ���ñ� -> %d\n", money);
	printf("��\n");

	return money;
}
int half(int money, int bat_money)
{
	bat_money = money/2;
	money = money + bat_money;
	printf("��\n");
	printf("������!\n");
	printf("��\n");
	printf("����ü ���ñ� -> %d\n", money);
	printf("��\n");

	return money;
}
int full(int money, int bat_money)
{
	bat_money = money;
	money = 2*money;
	printf("��\n");
	printf("��Ǯ!\n");
	printf("��\n");
	printf("����ü ���ñ� -> %d\n", money);
	printf("��\n");

	return money;
}