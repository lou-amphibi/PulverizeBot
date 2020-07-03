#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#define HEALS 1
#define INTERVAL 3
#define FIELDS 13
#define MAXGEN 100
// Длина поля по умолчанию
int flen=13;
int score=0;
// Информация о юнитах
struct units {
    int hp;
    char *name;
    char body;
    char *stun;
    struct units *left;
    struct units *right;
};
typedef struct units Units;
// Статусы юнитов
char stunned[]="\x1b[32;1mStun";
char unstun[]="Free";
char dead[]="\x1b[31;1mDead";

Units *enemyfactory(Units *pu, char s[], int heals);
void treeprint(Units *pu);
void genpulverize(char s[], int dist, Units *pu);
int getword(char s[]);

int main(int argc, char *argv[])
{
    int smash;
    // Для секции сreate
    int create=0;
    int number=0;
    char **cunitname=(char**)malloc(MAXGEN*sizeof(char *)); // Выделяем память для массива указателей
    // Базовая секция
    Units *tree=NULL;
    char printfield[]="\e[4m\x1b[33;1mC\x1b[37;1m  \x1b[31;1mB\x1b[37;1m \x1b[33;1mY\x1b[37;1m   \x1b[31;1mR\x1b[37;1m  \x1b[33;1mM\e[0m\n";
    char field[MAXGEN]="C  B Y   R  M\n";
    char *unitname[]={"Carlay", "Brre", "Ymo", "Roma", "Meeeh"};

    if (argc>1)
    {
        if (strcmp(argv[1], "See")==0)
            printf("%s\n", printfield);
        else if (strcmp(argv[1], "Moo")==0)
            printf("\aCOW SAY\n");
        else
            puts("Stampede!");
    }
    else
        puts("Nothing can hold me back!");

    if (argc==3)
    {
    if (strcmp(argv[2], "Create")==0)
    {
        cunitname[0]=(char*)malloc(MAXGEN*sizeof(char)); // Выделяем память для первого элемента cunitname
        puts("They is:");
        while(getword(cunitname[number]))
        {
            number++;
            cunitname[number]=(char*)malloc(MAXGEN*sizeof(char)); // Последовательно выделяем память для новых строк, которые вводит пользователь.
        }
        if (*cunitname[0]=='\0')
        {
            puts("Nothing! Can't now");
            exit(2);
        }
        create=1;
    }
    else
        puts("CREATE OR LIVE! CHOOSE ONE");
    }
    // Если выбран кастомный режим
    if (create)
    {
        time_t h;
        srand(time(&h));
        int sp;
        int sd=number;
        int j=0;
        for (int i=0; i<MAXGEN; i++) // Генерируем поле со случайным числом пробелов между юнитами
        {
            sp=(rand()%3);
            while(sp!=0)
            {
                field[i++]=' ';
                sp--;
            }
            if (sd==0)
            {
                flen=i;
                field[i]='\0';
                break;
            }
            field[i]=cunitname[j][0];
            j++;
            sd--;
        }
    }
    // Цикл работы
    int on=1;
    while (on)
    {
    if (on==5)
        puts("\x1b[35;1mAmphibi's respect\x1b[0m");
    else if(on==76)
        puts("\x1b[35;1m888\x1b[31;1m//\x1b[0m");
    // Заполнение информации о юнитах в кастомном и дефолтном вариантах
    if (create)
    {
        for (int i=0; i<number; i++)
        {
            tree=enemyfactory(tree, cunitname[i], HEALS+i);

        }
    }
    else
    {
        for (int i=0; i<5; i++)
        {
            tree=enemyfactory(tree, unitname[i], HEALS+i);
        }
    }

    puts("Place to smash (1-13 defolt):");
    scanf("%d", &smash);
    if (smash==42)
    {
        puts("Oh dear. It s a man who read a Jennings book");
        puts("Go away");
        exit(0);
    }
    while (smash<=0 || smash>flen)
    {
        puts("WHERE YOU WANT TO SMASH (1-13 defolt):");
        scanf("%d", &smash);
    }
    smash-=1;
    int dist=smash-(INTERVAL/2); // Начальная точка удара
    putchar('\n');
    genpulverize(field, dist, tree); // Матлогика
    putchar('\n');
    treeprint(tree);
    printf("\x1b[32;1mCurrently scorehead - \x1b[36;1m%d\x1b[0m\n", score);
    printf("\x1b[37;\e[0mSome Pulverize? (1+/0)");
    scanf("%d", &on);
    }
    free(tree);
    free(cunitname);
}

void treesearch(Units *pu, char c, int t[], int i);
void triplesort(int s[], int t[], char b[], int start, int end);
void treedamage(Units *pu, char c);
// Движок
void genpulverize(char s[], int dist, Units *pu)
{
    int index[MAXGEN];
    int heals[MAXGEN];
    char bodyes[MAXGEN];
    int damages[MAXGEN];
    int stats=0;
    int smashing=dist;
    // Убираем лишнюю границу, если удар пришелся в левый край поля
    if(smashing<0)
        smashing=0;
    // Ищем юнитов, которые попали под удар
    for (int i=0; i<INTERVAL; i++, dist++)
    {
        if ( isalpha(s[dist]))
        {
            index[stats]=dist;
            treesearch(pu, s[dist], heals, stats);
            bodyes[stats]=s[dist];
            stats++;
        }
    }
    // Закрываем массив с хп для возможности завершения цикла выставления \n
    if (stats!=MAXGEN-1)
        heals[stats]=0;
    else
    {
        puts("Can't smash");
        exit(1);
    }
    //Параллельно сортируем массивы со статистикой
    triplesort(heals, index, bodyes, 0, stats);
    // Рассчитываем положение юнитов, которые попали под удар и уменьшаем хп
    for (int y=0; y<stats; y++)
    {
        for (int a=0; a<index[y]; a++)
            putchar(' ');
        if (bodyes[y]%2==1)
        {
            treedamage(pu, bodyes[y]);
            treesearch(pu, bodyes[y], damages, y);
            if (damages[y]>0)
                printf("\x1b[33;1m%c", bodyes[y]);
            else
                printf("\x1b[34;1m#");
        }
        else
        {
            treedamage(pu, bodyes[y]);
            treesearch(pu, bodyes[y], damages, y);
            if (damages[y]>0)
                printf("\x1b[31;1m%c", bodyes[y]);
            else
                printf("\x1b[34;1m#");
        }
        while (heals[y]!=heals[y+1])
        {
            putchar('\n');
            (heals[y])--;
        }
    }
    // Пишем основное поле с индикатором удара
    for (int j=0; j<flen; j++)
    {
        if (j==smashing && smashing<dist)
        {
            printf("\e[4m\x1b[35;1m8");
            smashing++;
        }
        else if (isalpha(s[j]))
        {
            if (s[j]%2==1)
                printf("\e[4m\x1b[33;1m%c", s[j]);
            else
                printf("\e[4m\x1b[31;1m%c", s[j]);
        }
        else
             printf("\e[4m\x1b[37;1m%c", s[j]);
    }
    putchar('\n');

}

// Утилиты
Units *enemyfactory(Units *pu, char s[], int heals)
{
    int how;
    if(pu==NULL)
    {
        pu=(Units *) malloc(sizeof(Units));
        pu->hp=heals;
        pu->name=s;
        pu->body=*s;
        pu->stun=unstun;
        pu->left=NULL;
        pu->right=NULL;
    }
    else
    {
        if ( (how=strcmp(s, pu->name))<0)
            pu->left=enemyfactory(pu->left, s, heals);
        else if (how>0)
            pu->right=enemyfactory(pu->right, s, heals);
        else
        {
            pu->hp=heals;
            pu->name=s;
            pu->body=*s;
            pu->stun=unstun;
            pu->left=NULL;
            pu->right=NULL;
        }
    }
    return pu;
}




void treeprint(Units *pu)
{
    if (pu!=NULL)
    {
        treeprint(pu->left);
        printf("\x1b[37;\e[0mAbout %s, body %c, hp %d, status-%s\n", pu->name, pu->body, pu->hp, pu->stun);
        treeprint(pu->right);
    }
}

void treesearch(Units *pu, char c, int t[], int i)
{
    if (pu!=NULL)
    {
        treesearch(pu->left, c, t, i);
        if (c==pu->body)
            t[i]=pu->hp;
        treesearch(pu->right, c, t, i);
    }
}

void triplesort(int s[], int t[], char b[], int start, int end)
{
    int index;
    int tindex;
    int bindex;
    int min;
    int tab;
    for (int j=start; j<end-1; j++)
    {
        index=s[j];
        for (int i=j+1; i<end; i++)
        {
            if (s[i]>index)
            {
                index=s[i];
                tindex=t[i];
                bindex=b[i];
                min=i;
            }
        }
        if (index!=s[j])
        {
            tab=s[j];
            s[j]=index;
            s[min]=tab;

            tab=t[j];
            t[j]=tindex;
            t[min]=tab;

            tab=b[j];
            b[j]=bindex;
            b[min]=tab;
        }
    }
}

void treedamage(Units *pu, char c)
{
    if (pu!=NULL)
    {
    treedamage(pu->left, c);
    if (c==pu->body)
    {
        pu->hp-=2;
        score+=1;
        if (pu->hp>0)
        {
            pu->stun=stunned;
            score+=2;
        }
        else
        {
            pu->stun=dead;
            score+=4;
        }

    }
    treedamage(pu->right, c);
    }
}

int getword(char s[])
{
    int c;
    int i=0;
    while((c=getchar())!='\n' && c!=EOF)
        s[i++]=c;
    if (c=='\n')
        s[i]='\0';
    return i;
}