#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


typedef struct{
	char name[256];
	int priority;
	int pid;
	int runtime;
}proc;

typedef struct queue{
	proc * process;
	struct queue *next;
}node_t;

node_t * head = NULL;
void push(proc * process){
	proc * new_process = process;
	node_t *new_node = malloc(sizeof(node_t));
	new_node->process = new_process;
	new_node->next = NULL;

	if (head == NULL) {
	    head = new_node;
	}
	else {
		node_t * current = head;
		//current = malloc(sizeof(node_t));
	    while (current->next != NULL) {
	        current = current->next;
	    }
	    
	    current->next = new_node;

	}
	
}

void print_list(){//node_t * head) {
    node_t * current = head;
	

        //printf("%s,%d,%d,%d\n", current->process->name, current->process->priority, current->process->pid, current->process->runtime);
    while (current != NULL) {
    	//new_process = current->process;
		proc * new_process = current->process;
        printf("%s,%d,%d,%d\n", new_process->name, new_process->priority, new_process->pid, new_process->runtime);
        current = current->next;
    }
}

int main(void)
{
	
	//head = malloc(sizeof(node_t));

    const char *filename = "processes.txt";
    FILE *input_file = fopen( filename, "r" );

    if( input_file == NULL ){

        fprintf( stderr, "Unable to open file %s\n", filename );

    }else{

        // Read each line into the buffer
        char  line[255];

	    //fp = fopen("file.txt" , "r");
	    while (fgets(line, sizeof(line), input_file) != NULL)
	    {
	        const char* val1 = strtok(line, ",");
	        const char* val2 = strtok(NULL, ",");
	        const char* val3 = strtok(NULL, ",");
	        const char* val4 = strtok(NULL, ",");

	        proc *new_process = NULL;
	        new_process = malloc(sizeof(proc));
	        strcpy(new_process->name,val1);
	        new_process->priority = atoi(val2);
	        new_process->pid = atoi(val3);
	        new_process->runtime = atoi(val4);

	        push(new_process);
	    }

        if( ferror(input_file) ){
            perror( "The following error occurred" );
        }

        fclose( input_file );

    }
    print_list();
	free(head);
}
