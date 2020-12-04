#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack.h"

#define MaxSize 102

char Map[MaxSize][MaxSize];
int Mark[MaxSize][MaxSize] = {0};
int x_size = 0, y_size = 0;//���ڼ�¼�Թ���С
int x_start, y_start;//�������
int x_end, y_end;//�յ�����
Stack *SaveStack = NULL;//��¼·��
Stack *SaveStackTop = NULL;//��¼·��ջ��

Stack *RecordStack = NULL;//��ʱջ
Stack *RecordStackTop = NULL;//��ʱջ��

int direction_x[4] = {0, -1, 0, 1};
int direction_y[4] = {-1, 0, 1, 0};//�ĸ������������£�

enum directions{
    South = 0, West, North, East
};

/*
 * ��csv��ȡ��ͼ��Ϣ
 */
void ReadMap() {

    char pass[256] = "";
    char *line = NULL, *record;
    FILE *fp = fopen(".\\map.csv", "r+");

    if (fp != NULL) {
        fgets(pass, 255, fp);//������һ��,��һ���Ƕ�����ͷ
//        fgets(pass, 101, fp);//�����ڶ���,�ڶ�����map�߿�
//        fseek(fp, 2, SEEK_CUR);//������һ�У���һ����map�߿�
        while ((line = fgets(pass, sizeof(pass), fp)) != NULL)//��û�ж�ȡ���ļ�ĩβʱѭ������
        {
            //fseek(fp, 2, SEEK_CUR);//������һ�У���һ����map�߿�
            record = strtok(line, ",");//�Զ��Ž�ÿ�����ָ�
            x_size = 0;
            while (record != NULL)//��ȡÿһ�е�����
            {
                Map[y_size][x_size] = record[0];//��ÿ�����������
                x_size++;
                record = strtok(NULL, ",");//Ѱ����һ�����ָ����ַ���
            }
            y_size++;
        }
        x_size = x_size - 2;//��ȥ��һ�������һ�б߿�
        y_size = y_size - 2;//��ȥ��һ�������һ�б߿�
    }

}

/*
 * int x ������
 * int y ������
 * int direction ���� ��� directions
 * */
void dfs(int x, int y) {

    if (x == x_end && y == y_end)//��Ѱ�ɹ�ʱ������
    {
        Node *p1 = (Node *) malloc(sizeof(Node));
        p1->x = x;
        p1->y = y;
        p1->direction = 0;
        SaveStack = push(SaveStack, p1, &SaveStackTop);
        while (isEmpty(SaveStack)) {
            Node *p = SaveStackTop->data;
            RecordStack = push(RecordStack, p, &RecordStackTop);
            SaveStack = pop(SaveStack, &SaveStackTop);
        }
        while (isEmpty(RecordStack)) {
            Node *p = RecordStackTop->data;
            SaveStack = push(SaveStack, p, &SaveStackTop);//������
            RecordStack = pop(RecordStack, &RecordStackTop);
            printf("(%d,%d,%s)->", p->x, p->y, p->direction);
        }
        SaveStack = pop(SaveStack, &SaveStackTop);
        printf("\n");
        return;
    }
    if (x < 1 || x > x_size || y < 1 || y > y_size)//Խ��ͻ���
        return;
    for (int i = 0; i < 4; i++) {
        int next_x = x + direction_x[i];//����i��ֵ+1 +0 +
        int next_y = y + direction_y[i];
        if (0 < next_x && next_x <= x_size
            && 0 < next_y && next_y <= y_size
            && Map[next_y][next_x] == '0' && Mark[next_y][next_x] == 0) {

            Mark[next_y][next_x] = 1;
            Node *p = (Node *) malloc(sizeof(Node));
            p->x = x;
            p->y = y;
            switch (i) {
                case East:
                    p->direction = "��";
                    break;
                case South:
                    p->direction = "��";
                    break;
                case West:
                    p->direction = "��";
                    break;
                case North:
                    p->direction = "��";
                    break;

            }
            SaveStack = push(SaveStack, p, &SaveStackTop);

            dfs(next_x, next_y);

            Mark[next_y][next_x] = 0;
            SaveStack = pop(SaveStack, &SaveStackTop);
        }
    }
}


int main() {

    ReadMap();

    while (1) {
        printf("��������㣬�����ʽ������x ����y\n");
        scanf("%d %d", &x_start, &y_start);
        if ((x_start < 0 && x_start >= x_size) &&
            (y_start < 0 && y_start >= y_size))
            printf("���Խ��\n");
        else if (Map[y_start][x_start] != '0')
            printf("�õ㲻����Ϊһ�����\n");
        else break;
    }
    Mark[y_start][x_start] = 1;
    while (1) {
        printf("�������յ㣬�����ʽ������x ����y\n");
        scanf("%d %d", &x_end, &y_end);
        if ((x_end < 0 && x_end >= x_size) &&
            (y_start < 0 && y_start >= y_size))
            printf("�յ�Խ��\n");
        else if (Map[y_end][x_end] != '0')
            printf("�õ㲻����Ϊһ���յ�\n");
        else break;
    }
    dfs(x_start, y_start);

    return 0;

}
