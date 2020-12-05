#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Stack.h"

#define MaxSize 102

char Map[MaxSize][MaxSize];/*��ͼ*/
int Mark[MaxSize][MaxSize] = {0};/*���ڱ���Ƿ��߹�*/
int x_size = 0, y_size = 0;/*���ڼ�¼�Թ���С*/
int x_start, y_start;/*�������*/
int x_end, y_end;/*�յ�����*/
int flag = 0;

Stack *SaveStack = NULL;/*��¼·��*/
Stack *SaveStackTop = NULL;/*��¼·��ջ��*/
/**��ʱջ**/
Stack *RecordStack = NULL;
Stack *RecordStackTop = NULL;/*��ʱջ��*/

int direction_x[4] = {0, -1, 0, 1};
int direction_y[4] = {-1, 0, 1, 0};/*�ĸ������������£�*/

enum directions {
    South = 0, West = 1, North = 2, East = 3
};/*�������*/

/*
 * ��csv��ȡ��ͼ��Ϣ
 */
void ReadMap() {

    char pass[2] = "";
    char *line = NULL, *record;
    FILE *fp = fopen(".\\map.csv", "r+");

    if (fp != NULL) {
        fgets(pass, 2, fp);/*������һ��,��һ���Ƕ�����ͷ*/
        while ((line = fgets(pass, sizeof(pass), fp)) != NULL)//��û�ж�ȡ���ļ�ĩβʱѭ������
        {
            record = strtok(line, ",");/*�Զ��Ž�ÿ�����ָ�*/
            x_size = 0;
            while (record != NULL)/*��ȡÿһ�е�����*/
            {
                Map[y_size][x_size] = record[0];/*��ÿ�����������*/
                x_size++;
                record = strtok(NULL, ",");/*Ѱ����һ�����ָ����ַ���*/
            }
            y_size++;
        }
        x_size = x_size - 2;/*��ȥ��һ�������һ�б߿�*/
        y_size = y_size - 2;/*��ȥ��һ�������һ�б߿�*/
    }

}

/*
 * int x ������
 *
 * int y ������
 */
void dfs(int x, int y) {
    if (x == x_end && y == y_end)/*��Ѱ�ɹ�ʱ������*/
    {
        flag = 1;
        Node *p1 = (Node *) malloc(sizeof(Node));
        p1->x = x;
        p1->y = y;
        p1->direction = "0";
        SaveStack = push(SaveStack, p1, &SaveStackTop);/*ѹ���յ�*/
        while (isEmpty(SaveStack)) {
            Node *p = SaveStackTop->data;                           /**֮ǰ�߹���·���������ջ�ף����յ���ջ��**/
            RecordStack = push(RecordStack, p, &RecordStackTop);    /**�ᵼ�������˳���Ǵ��յ㵽���**/
            SaveStack = pop(SaveStack, &SaveStackTop);              /**ʹ��һ����ʱջ�����˳���Ϊ�յ㵽���**/
        }
        while (isEmpty(RecordStack)) {
            Node *p = RecordStackTop->data;                            /**����ʱջѭ������,ѹ���¼·��ջ**/
            SaveStack = push(SaveStack, p, &SaveStackTop);/*������*/    /**����ѹ���¼·��ջ��Ϊ�˻��ݣ�**/
            RecordStack = pop(RecordStack, &RecordStackTop);           /**�������������·��**/
            printf("(%d,%d,%s)", p->x, p->y, p->direction);/*����ڵ�*/
            if (RecordStack != NULL)/*ʹ���һ���ڵ㲻���"->"*/
                printf("->");
        }
        SaveStack = pop(SaveStack, &SaveStackTop);/*����ѹ����յ�*/
        printf("\n");
        return;
    }
    if (x < 1 || x > x_size || y < 1 || y > y_size)/*Խ��ͻ���*/
        return;
    for (int i = 0; i < 4; i++) {
        int next_x = x + direction_x[i];/*����i��ֵ+1 +0 -1*/
        int next_y = y + direction_y[i];/*����i��ֵ+1 +0 -1*/
        if (0 < next_x && next_x <= x_size
            && 0 < next_y && next_y <= y_size
            && Map[next_y][next_x] == '0' && Mark[next_y][next_x] == 0) /*�ж��Ƿ�����*/
        {
            Mark[next_y][next_x] = 1;/*���߹��ĵط����б��*/
            Node *p = (Node *) malloc(sizeof(Node));
            p->x = x;
            p->y = y;
            switch (i) { //��һ���ķ���,������ڵ�
                case East:
                    p->direction = "��";
                    break;
                case North:
                    p->direction = "��";
                    break;
                case West:
                    p->direction = "��";
                    break;
                case South:
                    p->direction = "��";
                    break;
                default:
                    break;
            }
            SaveStack = push(SaveStack, p, &SaveStackTop);

            dfs(next_x, next_y); /*���ѵݹ�*/

            Mark[next_y][next_x] = 0;/*����,��ǳ�δ�߹�*/
            SaveStack = pop(SaveStack, &SaveStackTop);/*��ջ*/
        }
    }
}


int main() {

    ReadMap(); /*��ȡ��ͼ*/

    while (1) {
        printf("��������㣬�����ʽ������x ����y\n");
        scanf("%d %d", &x_start, &y_start);
        if ((x_start < 0 && x_start >= x_size) &&
            (y_start < 0 && y_start >= y_size)) /*�ж������Ƿ����*/
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
            (y_start < 0 && y_start >= y_size))  /*�ж������Ƿ����*/
            printf("�յ�Խ��\n");
        else if (Map[y_end][x_end] != '0')
            printf("�õ㲻����Ϊһ���յ�\n");
        else break;
    }
    dfs(x_start, y_start);

    if (flag == 0) { /*�ж��Ƿ��н�*/
        printf("�޽�");
    }
    return 0;

}
