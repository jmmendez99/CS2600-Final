#include<stdio.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

//Main loop
void lsh_loop(void){
    char *line;
    char **args;
    int status;

    do{
        printf("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);

        free(line);
        free(args);

    } while(status);
}

//Read line function
char *lsh_read_line(void){
    int bufsize = LSH_RL_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        //Read a character
        c = getchar();

        //If EOF, replace with NULL
        if (c == EOF || c == '\n'){
            buffer[position] = '\0';
            
            return buffer;
        } else {
            buffer[position] = c;
        }

        position++;

        //If exceeded, reallocate
        if (position >= bufsize){
            bufsize = bufsize + LSH_RL_BUFSIZE;
            
            buffer = realloc(buffer,bufsize);

            if (!buffer){
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

    }
}

//Parsing the line function
char **lsh_split_line(char *line){
    int bufsize = LSH_TOK_BUFSIZE;
    int position = 0;

    char **tokens = malloc(bufsize * sizeof(char));
    char *token;

    if (!token){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize){
            bufsize = bufsize + LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));

            if (!tokens){
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL,LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

//Launching our program
int lsh_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();

    if (pid == 0){
        //child process
        if (excecvp(args[0], args) == -1){
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    
    } else if (pid < 0){
        //Error forking
        perror("lsh");
    
    } else {
        //parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}


int main(int argc, char **argv){

    //Loop to run the shell
    lsh_loop();


    return EXIT_SUCCESS;
}