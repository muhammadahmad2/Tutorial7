#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 256

typedef struct{
	char name[BUFFER_SIZE];
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
	if(head == NULL){
		return NULL;
	} else if (strcmp(head->process->name, name) == 0){
		return pop();
	} else { 
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
}

proc* delete_pid(int pid){
	if(head == NULL){
		return NULL;
	} else if (head->process->pid == pid){
		return pop();
	} else { 
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
}

void print_list(){
    node_t * current = head;	
	while (current != NULL) {
		proc * new_process = current->process;
        printf("%s,%d,%d,%d\n", new_process->name, 
			new_process->priority, new_process->pid, new_process->runtime);
        current = current->next;
    }
}

int main(void)
{ 
	//Open file for reading
	char filename[] = "processes_q5.txt";
	FILE *in_file = fopen(filename, "r");
	if(in_file == NULL){
		fprintf(stderr, "Unable to open file '%s'.\n", filename);
		return 1;
	}
	//Read file into queue
	char buffer[BUFFER_SIZE];
	while(fgets(buffer, BUFFER_SIZE, in_file) != NULL)
	{
		char *name = strtok(buffer, ",");
		char *priority = strtok(NULL, ",");
		char *runtime = strtok(NULL, ",");
		
		proc *new_process = NULL;
		new_process = malloc(sizeof(proc));
		strcpy(new_process->name, name);
		new_process->priority = atoi(priority);
		new_process->pid = 0;
		new_process->runtime = atoi(runtime);
		push(new_process);
	}
	fclose(in_file);

	//Execute processes with priority 0
	node_t * current_n = head;
	while(current_n != NULL){	
		if(current_n->process->priority == 0){
			proc *current_p = delete_name(current_n->process->name);
			pid_t pid = fork();
			if(pid == 0) {
				execl(current_p->name, 0);
				return 0;
			}
			current_p->pid = (int) pid;
			sleep(current_p->runtime);
			kill(pid, SIGINT);
			waitpid(pid, NULL, 0);
			printf("%s, %d, %d, %d\n", current_p->name, 
				current_p->priority, current_p->pid, current_p->runtime);
			free(current_p);
		}
		
		current_n = current_n->next;
	}

	//Execute all other processes
	current_n = head;
	while(current_n != NULL){
		proc *current_p = pop();
		pid_t pid = fork();
		if(pid == 0){
			execl(current_p->name, 0);
			return 0;
		}
		current_p->pid = (int) pid;
		sleep(current_p->runtime);
		kill(pid, SIGINT);
		waitpid(pid, NULL, 0);
		printf("%s, %d, %d, %d\n", current_p->name, 
			current_p->priority, current_p->pid, current_p->runtime);
		free(current_p);
		current_n = current_n->next;
	}
	free(head);
	return 0;
}
