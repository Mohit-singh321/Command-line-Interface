#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

#define SPACE " "

/**
 * Malloc - Allocates memory with error handling
 * @size: Number of bytes to allocate
 * Return: Pointer to allocated memory
 * Corner cases:
 * - Zero size: returns NULL
 * - Allocation failure: prints error and exits
 */
void	*Malloc(size_t size)
{
	void	*ptr;

	if (size == 0)
		return (NULL);
	ptr = malloc(size);
	if (!ptr)
	{
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}


/**
 * Realloc - Reallocates memory with error handling
 * @ptr: Pointer to previously allocated memory
 * @size: New size in bytes
 * Return: Pointer to reallocated memory
 * Corner cases:
 * - NULL ptr: acts as malloc
 * - Zero size: acts as free
 * - Allocation failure: prints error and exits
 */
void	*Realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	new_ptr = realloc(ptr, size);
	if (!new_ptr && size != 0)
	{
		perror("Realloc failed");
		exit(EXIT_FAILURE);
	}
	return (new_ptr);
}

char	**cell_split_line(char *line)
{
	size_t			bufsize;
	unsigned long	position;
	char			**arr;

	bufsize = BUFSIZ; // default size in stdio.h
	position = 0;
	arr = Malloc(bufsize * sizeof (*arr));

	for (char *token = strtok(line, SPACE); token; token = strtok(NULL, SPACE))
	{
		arr[position++] = token;	
		if (position >= bufsize)
		{
			bufsize *= 2;
			arr = Realloc(arr, bufsize * sizeof (*arr));
		}
	}
	arr[position] = NULL;
	return (arr);
}

void chDir(const char *path){

    if(path == NULL){
        printf("cd: path argument required\n");
        return;
    }
    int x = chdir(path);
    if(x == -1){
        printf("cd: failed \n");
    }
}

void cellLaunch(char **args){
    pid_t id = fork();

    if(id == 0){

        execvp(args[0] , args);
    }
    else{
        wait(NULL);
    }
}

char *readline(void){

	char *line = NULL;
	size_t n =0;

	char cwd[BUFSIZ];

	if(isatty(fileno(stdin))){
		printf("C:%s $> ", getcwd(cwd , BUFSIZ));
	}


	getline(&line , &n , stdin);
	return line;
}



int main(int argc , char *argv[]){

    // read -> eval -> execute -> loop
	char *buffer ;

	// read
    while(buffer = readline()){

        
        
       
        buffer[strcspn(buffer, "\n")] = 0;
    
		
		char **args;
		// reading token
        args = cell_split_line(buffer);

        // checking for cd
        if(args[0] && !strcmp(args[0] , "cd")){

            chDir(args[1]);

        }
		// evaluate / execute
        cellLaunch(args);

    }


}