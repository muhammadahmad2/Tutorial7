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
	    while (current->next != NULL) {
	        current = current->next;
	    }
	    
	    current->next = new_node;

	}
	
}

proc* pop() {
	
    node_t * next_node = NULL;

    if (head == NULL) {
        return NULL;
    }

    next_node = head->next;
    proc * retval = head->process;
    free(head);
    head = next_node;

    return retval;
    
}

proc* delete_name(char *name){
	node_t * current = head;
	node_t * temp_node = NULL;

    while (current->next != NULL) {
    	if (strcmp(current->next->process->name,name)==0) {
    		temp_node = current->next;
		    current->next = temp_node->next;
		    free(temp_node);

		    return current->process;
    	}
        current = current->next;
    }
    return NULL;	
}

proc* delete_pid(int pid){
	node_t * current = head;
	node_t * temp_node = NULL;

    while (current->next != NULL) {
    	if (current->next->process->pid == pid) {
    		temp_node = current->next;
		    current->next = temp_node->next;
		    free(temp_node);

		    return current->process;
    	}
        current = current->next;
    }
    return NULL;
}

void print_list(){//node_t * head) {
    node_t * current = head;	
	while (current != NULL) {
		proc * new_process = current->process;
        printf("%s,%d,%d,%d\n", new_process->name, new_process->priority, new_process->pid, new_process->runtime);
        current = current->next;
    }
}

int main(void)
{
	

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

    delete_name("emacs");
    delete_pid(12235);

    node_t * current = head;	
	while (current != NULL) {
		proc * new_process = pop();
        printf("%s,%d,%d,%d\n", new_process->name, new_process->priority, new_process->pid, new_process->runtime);
        current = current->next;
    }

	free(head);
}