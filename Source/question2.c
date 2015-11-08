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
void push(proc * process1){
	proc * new_process = process1;
    new_process = malloc(sizeof(proc));
/*
	if (head == NULL) {
		node_t * new_node;
		new_node = malloc(sizeof(node_t));

	    new_node->process = new_process;
	    new_node->next = NULL;
	    head = new_node; 
	}
	else {
		node_t * new_node;
		new_node = malloc(sizeof(node_t));

	    new_node->process = new_process;
	    new_node->next = head;
	    head = new_node; 
	}
*/

	if (head == NULL) {
		node_t * new_node;
		new_node = malloc(sizeof(node_t));
	    new_node->process = new_process;
	    new_node->next = NULL;
	    head = new_node;
	}
	else {
		node_t * current = head;
		//current = malloc(sizeof(node_t));
	    while (current->next != NULL) {
	        current = current->next;
	    }
	    
	    current->next = malloc(sizeof(node_t));
	    current->next->process = new_process;
	    current->next->next = NULL;

	}
	
}

proc pop() {
	/*
    node_t * next_node = NULL;

    if (*head == NULL) {
        return NULL;
    }

    next_node = (*head)->next;
    node_t * retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
    */
}

void print_list(){//node_t * head) {
    node_t * current = head;
	proc * new_process = current->process;

        //printf("%s,%d,%d,%d\n", new_process->name, new_process->priority, new_process->pid, new_process->runtime);
    while (current != NULL) {
    	new_process = current->process;
        printf("%s,%d,%d,%d\n", new_process->name, new_process->priority, new_process->pid, new_process->runtime);
        current = current->next;
    }
    printf("123123123");
}

int main(void)
{
	
	head = malloc(sizeof(node_t));

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





}