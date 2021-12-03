#include<stdio.h>

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


int main(int argc, char **argv){

    //Loop to run the shell
    lsh_loop();


    return EXIT_SUCCESS;
}