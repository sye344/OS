#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
char cwd[1000];
char prevWorkingDirectory[1000];
void callChdir(char *add)
{
    int x = chdir(add);
    if (x == -1)
    {
        printf("Error in changing directory to %s\n", add);
    }
    else
    {
        strcpy(prevWorkingDirectory, cwd);
        strcpy(cwd, add);
    }
}

void echo(char *token)
{
    if (token == NULL)
    {
        printf("\n");
    }
    else
    {
        while (token != NULL)
        {
            printf("%s ", token);
            token = strtok(NULL, " ");
        }
        printf("\n");
    }
}
void cd(char *argument)
{
    if (argument == NULL || strcmp(argument, "-") == 0)
    {
        callChdir(prevWorkingDirectory);
    }
    else if (strcmp("..", argument) == 0)
    {
        callChdir("../");
    }
    else if (strcmp("~", argument) == 0)
    {
        callChdir(getenv("HOME"));
    }
    else
    {
        if (strcmp("-L", argument) == 0 || strcmp("-P", argument) == 0)
        {
            argument = strtok(NULL, " ");
            if (argument == NULL)
            {
                callChdir(getenv("HOME"));
            }
            else
            {
                callChdir(argument);
            }
        }
        else
        {
            callChdir(argument);
        }
    }
}
void pwd(char *argument)
{
    if (argument != NULL)
    {
        if (strcmp(argument, "-L") == 0)
        {
            printf("%s\n", cwd);
        }
        else if (strcmp(argument, "-P") == 0)
        {
            char tmp2[1000];
            if (getcwd(tmp2, sizeof(tmp2)) != NULL)
            {	
                printf("%s\n", tmp2);
            }
        }
        else
        {
            printf("Command not found \n");
        }
    }
    else
    {
        printf("%s\n", cwd);
    }
}

int main(int arg, char const *argc[])
{
    printf("Welcome to Mahir's shell\n");
    getcwd(cwd, sizeof(cwd));
    strcpy(prevWorkingDirectory,cwd);
    
    while (1)
    {
        getcwd(cwd, sizeof(cwd));

        printf("%s ~$ ", cwd);
        char input[100];
        char nakli[105];
        scanf("%[^\n]%*c", input);
		strcat(nakli,input);
		strcat(nakli," ");
        char *token;
        token = strtok(input, " ");

        if (strcmp(token, "cd") == 0)
        {
            token = strtok(NULL, " ");
            cd(token);
        }
        else if (strcmp(token, "echo") == 0)
        {
            token = strtok(NULL, " ");
            echo(token);
        }
        else if (strcmp(token, "pwd") == 0)
        {
            token = strtok(NULL, " ");
            pwd(token);
        }
        else{
            pid_t pid;
			pid=fork();

			if(pid==0){
                if(strcmp(token,"ls")==0){
                    token=strtok(NULL," ");

                    execl("/Users/mahiryadav/repos/OS_assignment_2/ls",token,NULL);
                }
                else if(strcmp(token,"cat")==0){
                    token=strtok(NULL," ");
                    execl("/Users/mahiryadav/repos/OS_assignment_2/cat",token,nakli,NULL);
                }
                else if(strcmp(token,"date")==0){
                    token=strtok(NULL," ");
                    
                    execl("/Users/mahiryadav/repos/OS_assignment_2/date",token,NULL);
                }
                else if(strcmp(token,"rm")==0){
                    token=strtok(NULL," ");
                    execl("/Users/mahiryadav/repos/OS_assignment_2/rm",token,nakli,NULL);
                    
                }
                else if(strcmp(token,"mkdir")==0){
                    token=strtok(NULL," ");
                    execl("/Users/mahiryadav/repos/OS_assignment_2/mkdir",token,nakli,NULL);
                    
                }
				exit(0);
			}
			else if(pid>0){
				waitpid(0,NULL,0);
			}
			else{
				printf("Error in forking\n");
			}
        }
    }

}