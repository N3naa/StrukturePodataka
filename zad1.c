#define _CTR_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define BUFFER_SIZE 1024
#define NAME_SIZE 256
#define FILENAME_MAX 1024
#define ERROR_OF -1
#pragma pack(1)

typedef struct student
{
    char name[NAME_SIZE];
    char surname[NAME_SIZE];
    unsigned score;
    float relative_score;

}_student;

int count_rows(char *);
int fill_array(_student *,int ,char * );

int main()
{
    _student *ps;
    int n;
    char filename[FILENAME_MAX];

    printf("Enter file name: ");
    scanf("%[^\n]", filename);
    fflush(stdin);

    n = count_rows(filename);

    if(n == ERROR_OF) return EXIT_FAILURE;

    ps = (_student *)malloc(n*sizeof(_student));

    if (!ps) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    fill_array(ps,n,filename);

    puts("List of students and their scores: \n");
    puts("************************************\n");

    for(int i = 0; i < n; i++)
    {
        printf("%d.Student: \n", i+1);
        printf("%s %s\n", ps[i].name, ps[i].surname);
        printf("Absolute score: %u\n", ps[i].score);
        printf("Relative score: %.2f\n", ps[i].relative_score);
    }

    free(ps);

    return EXIT_SUCCESS;
}

int count_rows(char *filename)
{
    FILE *fp = fopen(filename, "r");
    char buffer[BUFFER_SIZE];

    if(fp == NULL)
    {
        printf("Error opening file!\n");
        return ERROR_OF;
    }

    int i = 0;
    while(!feof(fp))
    {
        fgets(buffer,BUFFER_SIZE,fp);
        i++;
    }

    fclose(fp);

    return i;
}

int fill_array(_student *ps,int n,char *filename )
{
    FILE *fp = fopen(filename,"r");
    const float max_score = 50;

    if(fp == NULL)
    {
        perror("Error opening file: ");
        errno = 0;
        return EXIT_FAILURE;
    }

    for(int i = 0; i < n; i++)
    {
        fscanf(fp,"%s %s %u", ps[i].name, ps[i].surname, &ps[i].score);
        ps[i].relative_score = (ps[i].score/max_score)*100;
    }

    fclose(fp);

    return EXIT_SUCCESS;
}