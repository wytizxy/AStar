/********************************************************************
*����������A*�㷨��ʵ�ְ����������
*
*
*�㷨�Ĳ������£�
*1����ʼ����������open��closed������ʼ״̬����open����
*2���ظ����й��̣�ֱ���ҵ�Ŀ����Ϊֹ�����open��Ϊ�գ���
*   ô����ʧ�ܣ�
*3����open�����ó�������Сfֵ�Ľ��(����һ����ΪBESTNODE)��
*   ������closed���У�
*4�����BESTNODEΪĿ���㣬�ɹ���ý�,�˳�ѭ����
*5�����BESTNODE����Ŀ���㣬��ô�������ĺ�̽��(�˺�̽�
*   ���������ȵ�״̬��ͬ)����̽�����һ������
*6����ÿ����̽��������¹��̣�
*7����������BESTNODE��parentָ�룻
*8������˽����open���У����Ƚ�open���еĽ����ӽ�BESTNODE
*   �ĺ�̽�����У�Ȼ�������������gֵ������˽���gֵС
*   ��open���еĽ��ʱ��open���еĽ��ı�parentָ�룬ͬʱ��
*   �˽��ɾ����
*9������˽����closed���У����Ƚ�closed���еĽ����ӽ�BESTNODE
*   �ĺ�̽���У�Ȼ�������������gֵ������˽���gֵС��
*   closed���еĽ��ʱ��closed���еĽ��ı�parentָ�룻��
*   closed���еĽ�����·���open���У�ͬʱ���˽��ɾ����
*10������˽��Ȳ���open����Ҳ����closed���У���ô��Ӵ˽����
*   BESTNODE�ĺ�̽�����С�
*
*
*
********************************************************************/
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <cmath>
#define size 3
using namespace std;
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
void gotoxy(const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout, coord);
}
void setcolor(const int bg_color = 0, const int fg_color = 7)
{
	SetConsoleTextAttribute(hout, bg_color * 16 + fg_color);
}
//�����ά�������洢���ݱ�ʾĳһ���ض�״̬
typedef int status[size][size];
struct SpringLink;
//����״̬ͼ�еĽ�����ݽṹ
typedef struct Node
{
	status data;//������洢��״̬
	struct Node *parent;//ָ����ĸ��׽��
	struct SpringLink *child;//ָ����ĺ�̽��
	struct Node *next;//ָ��open����closed���еĺ�һ�����
	int fvalue;//�����ܵ�·��
	int gvalue;//����ʵ��·��
	int hvalue;//���ĵ���Ŀ��Ŀ��ѳ̶�
}NNode, *PNode;
//����洢ָ�����̽���ָ��ĵ�ַ
typedef struct SpringLink
{
	struct Node *pointData;//ָ�����ָ��
	struct SpringLink *next;//ָ���ֵڽ��
}SPLink, *PSPLink;
PNode open;
PNode closed;
//��ʼ״̬��Ŀ��״̬
status startt = { 8, 7, 6, 5, 4, 3, 2, 1, 0 };
status target = { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
//��ʼ��һ��������
void initLink(PNode &Head)
{
	Head = (PNode)malloc(sizeof(NNode));
	Head->next = NULL;
}
//�ж������Ƿ�Ϊ��
bool isEmpty(PNode Head)
{
	if (Head->next == NULL)
		return true;
	else
		return false;
}
//���������ó�һ������
void popNode(PNode &Head, PNode &FNode)
{
	if (isEmpty(Head))
	{
		FNode = NULL;
		return;
	}
	FNode = Head->next;
	Head->next = Head->next->next;
	FNode->next = NULL;
}
//��������պ�̽������������µ��ӽ��
void addSpringNode(PNode &Head, PNode newData)
{
	PSPLink newNode = (PSPLink)malloc(sizeof(SPLink));
	newNode->pointData = newData;

	newNode->next = Head->child;
	Head->child = newNode;
}
//�ͷ�״̬ͼ�д�Ž���̽���ַ�Ŀռ�
void freeSpringLink(PSPLink &Head)
{
	PSPLink tmm;

	while (Head != NULL)
	{
		tmm = Head;
		Head = Head->next;
		free(tmm);
	}
}
//�ͷ�open����closed���е���Դ
void freeLink(PNode &Head)
{
	PNode tmn;

	tmn = Head;
	Head = Head->next;
	free(tmn);

	while (Head != NULL)
	{
		//�����ͷŴ�Ž���̽���ַ�Ŀռ�
		freeSpringLink(Head->child);
		tmn = Head;
		Head = Head->next;
		free(tmn);
	}
}
//����ͨ���������һ�����
void addNode(PNode &Head, PNode &newNode)
{
	newNode->next = Head->next;
	Head->next = newNode;
}
//��ǵݼ����е����������һ�����
void addAscNode(PNode &Head, PNode &newNode)
{
	PNode P;
	PNode Q;

	P = Head->next;
	Q = Head;
	while (P != NULL && P->fvalue < newNode->fvalue)
	{
		Q = P;
		P = P->next;
	}
	//�����жϺ�λ��֮��������Ǽ򵥵Ĳ�����
	newNode->next = Q->next;
	Q->next = newNode;
}
//ȷ��ĳ������λ��
int Locate(status data, int num)
{
	for (int i = 0; i < size; i += 1)
	{
		for (int j = 0; j < size; j += 1)
		{
			if (data[i][j] == num)
			{
				return 3 * i + j;
			}
		}
	}
}
//�������hֵ
int computeHValue(PNode theNode)
{
	int h = 0;
	for (int i = 0; i < size; i += 1)
	{
		for (int j = 0; j < size; j += 1)
		{
			int location_target = Locate(target, theNode->data[i][j]);
			int location_now = 3 * i + j;
			h += abs(location_target / 3 - location_now / 3) + abs(location_target % 3 - location_now % 3);
		}
	}

	return h;
}
//�������f��g��hֵ
void computeAllValue(PNode &theNode, PNode parentNode)
{
	if (parentNode == NULL)
		theNode->gvalue = 0;
	else
		theNode->gvalue = parentNode->gvalue + 1;

	theNode->hvalue = computeHValue(theNode);
	theNode->fvalue = theNode->gvalue + theNode->hvalue;
}
//��ʼ�������������㷨��ʼ����������
void initial()
{
	//��ʼ��open�Լ�closed��
	initLink(open);
	initLink(closed);

	//��ʼ����ʼ��㣬���ʼ���ĸ��ڵ�Ϊ�ս��
	PNode NULLNode = NULL;
	PNode Start = (PNode)malloc(sizeof(NNode));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Start->data[i][j] = startt[i][j];
		}
	}
	Start->parent = NULL;
	Start->child = NULL;
	Start->next = NULL;
	computeAllValue(Start, NULLNode);

	//��ʼ������open��
	addAscNode(open, Start);
}
//��B�ڵ��״̬��ֵ��A���
void statusAEB(PNode &ANode, PNode BNode)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ANode->data[i][j] = BNode->data[i][j];
		}
	}
}
//��������Ƿ�����ͬ��״̬
bool hasSameStatus(PNode ANode, PNode BNode)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (ANode->data[i][j] != BNode->data[i][j])
				return false;
		}
	}
	return true;
}
//����������Ƚ���Ƿ�����ͬ��״̬
bool hasAnceSameStatus(PNode OrigiNode, PNode AnceNode)
{
	while (AnceNode != NULL)
	{
		if (hasSameStatus(OrigiNode, AnceNode))
			return true;
		AnceNode = AnceNode->parent;
	}
	return false;
}
//ȡ�÷����пյĸ��ӵ�λ��
void getPosition(PNode theNode, int &row, int &col)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (theNode->data[i][j] == 0)
			{
				row = i;
				col = j;
				return;
			}
		}
	}
}
//�����������ֵ�ֵ
void changeAB(int &A, int &B)
{
	int C;
	C = B;
	B = A;
	A = C;
}
//�����Ӧ��״̬�Ƿ���ĳһ��������
bool inLink(PNode spciNode, PNode theLink, PNode &theNodeLink, PNode &preNode)
{
	preNode = theLink;
	theLink = theLink->next;

	while (theLink != NULL)
	{
		if (hasSameStatus(spciNode, theLink))
		{
			theNodeLink = theLink;
			return true;
		}
		preNode = theLink;
		theLink = theLink->next;
	}
	return false;
}
//�������ĺ�̽��(������״̬��ͬ)����
void SpringLink(PNode theNode, PNode &spring)
{
	int row;
	int col;

	getPosition(theNode, row, col);

	//�յĸ����ұߵĸ��������ƶ�
	if (col != 2)
	{
		PNode rlNewNode = (PNode)malloc(sizeof(NNode));
		statusAEB(rlNewNode, theNode);
		changeAB(rlNewNode->data[row][col], rlNewNode->data[row][col + 1]);
		if (hasAnceSameStatus(rlNewNode, theNode->parent))
		{
			free(rlNewNode);//�븸����ͬ�����������
		}
		else
		{
			rlNewNode->parent = theNode;
			rlNewNode->child = NULL;
			rlNewNode->next = NULL;
			computeAllValue(rlNewNode, theNode);
			//�����������̽������
			addNode(spring, rlNewNode);
		}
	}
	//�յĸ�����ߵĸ��������ƶ�
	if (col != 0)
	{
		PNode lrNewNode = (PNode)malloc(sizeof(NNode));
		statusAEB(lrNewNode, theNode);
		changeAB(lrNewNode->data[row][col], lrNewNode->data[row][col - 1]);
		if (hasAnceSameStatus(lrNewNode, theNode->parent))
		{
			free(lrNewNode);//�븸����ͬ�����������
		}
		else
		{
			lrNewNode->parent = theNode;
			lrNewNode->child = NULL;
			lrNewNode->next = NULL;
			computeAllValue(lrNewNode, theNode);
			//�����������̽������
			addNode(spring, lrNewNode);
		}
	}
	//�յĸ����ϱߵĸ��������ƶ�
	if (row != 0)
	{
		PNode udNewNode = (PNode)malloc(sizeof(NNode));
		statusAEB(udNewNode, theNode);
		changeAB(udNewNode->data[row][col], udNewNode->data[row - 1][col]);
		if (hasAnceSameStatus(udNewNode, theNode->parent))
		{
			free(udNewNode);//�븸����ͬ�����������
		}
		else
		{
			udNewNode->parent = theNode;
			udNewNode->child = NULL;
			udNewNode->next = NULL;
			computeAllValue(udNewNode, theNode);
			//�����������̽������
			addNode(spring, udNewNode);
		}
	}
	//�յĸ����±ߵĸ��������ƶ�
	if (row != 2)
	{
		PNode duNewNode = (PNode)malloc(sizeof(NNode));
		statusAEB(duNewNode, theNode);
		changeAB(duNewNode->data[row][col], duNewNode->data[row + 1][col]);
		if (hasAnceSameStatus(duNewNode, theNode->parent))
		{
			free(duNewNode);//�븸����ͬ�����������
		}
		else
		{
			duNewNode->parent = theNode;
			duNewNode->child = NULL;
			duNewNode->next = NULL;
			computeAllValue(duNewNode, theNode);
			//�����������̽������
			addNode(spring, duNewNode);
		}
	}
}
//�����������״̬
void outputStatus(PNode stat)
{
	for (int i = 0; i < 3; i += 1)
	{
		for (int j = 0; j < 3; j += 1)
		{
			cout << stat->data[i][j]<<" ";
		}
		cout << endl;
	}

}
void outputStatus2(PNode stat)
{
	for (int i = 0; i < 3; i += 1)
	{
		for (int j = 0; j < 3; j += 1)
		{
			gotoxy(j * 20 + 51, i * 9 + 9);
			if (stat->data[i][j] != 0)
			{
				setcolor(15, stat->data[i][j] % 7);
				cout << stat->data[i][j];
				setcolor(15, 0);
			}
			else
			{
				cout << " ";
			}
		}
	}
}
//�����ѵ�·��
void outputBestRoad(PNode goal)
{
	int deepnum = goal->gvalue;

	if (goal->parent != NULL)
	{
		outputBestRoad(goal->parent);
	}
	gotoxy(20, 3);
	cout << "��" << deepnum-- << "��" << endl;
	outputStatus2(goal);
	Sleep(1000);
}
void AStar()
{
	PNode tmpNode=NULL;//ָ���open�����ó����ŵ�closed���еĽ���ָ��
	PNode spring;//tmpNode�ĺ�̽����
	PNode tmpLNode;//tmpNode��ĳһ����̽��
	PNode tmpChartNode;
	PNode thePreNode;//ָ��Ҫ��closed�����Ƶ�open���еĽ���ǰһ������ָ��
	bool getGoal = false;//��ʶ�Ƿ�ﵽĿ��״̬
	long numcount = 1;//��¼��open�����ó��������

	initial();//�Ժ������г�ʼ��
	initLink(spring);//�Ժ������ĳ�ʼ��
	tmpChartNode = NULL;

	//cout << "��open�����ó��Ľ���״̬����Ӧ��ֵ" << endl;
	while (!isEmpty(open))
	{
		//��open�����ó�fֵ��С��Ԫ��,�����ó���Ԫ�ط���closed����
		popNode(open, tmpNode);
		addNode(closed, tmpNode);

		//Sleep(10);
		cout << "��" << numcount++ << "��״̬�ǣ�" << endl;
		outputStatus(tmpNode);
		cout << "��fֵΪ��" << tmpNode->fvalue << endl;
		cout << "��gֵΪ��" << tmpNode->gvalue << endl;
		cout << "��hֵΪ��" << tmpNode->hvalue << endl;


		//����ó���Ԫ����Ŀ��״̬������ѭ��
		if (computeHValue(tmpNode) == 0)
		{
			getGoal = true;
			break;
		}

		//������ǰ�����ĺ��(�����Ȳ�ͬ)����б������ĺ�̽���parent����ָ��ǰ���Ľ��
		SpringLink(tmpNode, spring);

		//���������ĺ�̽������
		while (!isEmpty(spring))
		{
			popNode(spring, tmpLNode);
			//״̬��open�����Ѿ����ڣ�thePreNode���������ﲢ��������
			if (inLink(tmpLNode, open, tmpChartNode, thePreNode))
			{
				addSpringNode(tmpNode, tmpChartNode);
				if (tmpLNode->gvalue < tmpChartNode->gvalue)
				{
					tmpChartNode->parent = tmpLNode->parent;
					tmpChartNode->gvalue = tmpLNode->gvalue;
					tmpChartNode->fvalue = tmpLNode->fvalue;
				}
				free(tmpLNode);
			}
			//״̬��closed�����Ѿ�����
			else if (inLink(tmpLNode, closed, tmpChartNode, thePreNode))
			{
				addSpringNode(tmpNode, tmpChartNode);
				if (tmpLNode->gvalue < tmpChartNode->gvalue)
				{
					PNode commu;
					tmpChartNode->parent = tmpLNode->parent;
					tmpChartNode->gvalue = tmpLNode->gvalue;
					tmpChartNode->fvalue = tmpLNode->fvalue;
					freeSpringLink(tmpChartNode->child);
					tmpChartNode->child = NULL;
					popNode(thePreNode, commu);
					addAscNode(open, commu);
				}
				free(tmpLNode);
			}
			//�µ�״̬����״̬�Ȳ���open����Ҳ����closed����
			else
			{
				addSpringNode(tmpNode, tmpLNode);
				addAscNode(open, tmpLNode);
			}
		}
	}
	cout << "5��֮��������·����ʾ";
	Sleep(5000);
	system("cls");
	//Ŀ��ɴ�Ļ��������ѵ�·��
	if (getGoal)
	{
		gotoxy(20, 2);
		cout << "·������Ϊ��" << tmpNode->gvalue << endl;
		int x = 40;
		int y = 4;
		gotoxy(x, y);
		cout << "���������������������С������������������С�������������������";
		gotoxy(x, y + 1);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 2);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 3);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 4);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 5);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 6);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 7);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 8);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 9);
		cout << "���������������������ࡪ�����������������ࡪ������������������";
		gotoxy(x, y + 10);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 11);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 12);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 13);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 14);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 15);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 16);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 17);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 18);
		cout << "���������������������ࡪ�����������������ࡪ������������������";
		gotoxy(x, y + 19);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 20);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 21);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 22);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 23);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 24);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 25);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 26);
		cout << "ح                  ح                  ح                  ح";
		gotoxy(x, y + 27);
		cout << "���������������������ء������������������ء�������������������";
		outputBestRoad(tmpNode);
	}

	//�ͷŽ����ռ���ڴ�
	freeLink(open);
	freeLink(closed);
	//getch();
}
int main()
{
	system("mode con cols=150 lines=38");
	SetConsoleTitleA("���������� by�������");
	setcolor(15,0);
	system("cls");
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hout, &cci);
	cci.bVisible = false;
	SetConsoleCursorInfo(hout, &cci);
	cout << "��ʼ״̬��" << endl;
	for (int i = 0; i < 3; i += 1)
	{
		for (int j = 0; j < 3; j += 1)
		{
			cout << startt[i][j] << " ";
		}
		cout << endl;
	}
	cout << "Ŀ��״̬��" << endl;
	for (int i = 0; i < 3; i += 1)
	{
		for (int j = 0; j < 3; j += 1)
		{
			cout << target[i][j] << " ";
		}
		cout << endl;
	}
	cout << "5��֮�����A*����";
	Sleep(5000);
	AStar();
	gotoxy(120, 37);
	system("pause");
	return 0;
}
