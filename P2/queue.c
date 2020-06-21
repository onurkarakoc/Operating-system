#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Struct for list nodes 
typedef struct lst_node_s {
int data;
struct lst_node_s* next;
}listnode; // listnode has properties of lst_node_s 



 
// Struct for task nodes 
typedef struct tsk_node_s {
	int task_num; //starting from 0
	int task_type; // insert:0, delete:1, search:2
	int value;
struct tsk_node_s* next;
}tasknode; // tasknode has properties of tsk_node_s



tasknode * taskroot = NULL; // Beginning of the task
tasknode * taskending = NULL; // End of the task
listnode * listroot = NULL; // Beginning of the list

// Methods are here 
void Task_queue(int n);
void Task_enqueue(int task_num, int task_type, int value);
int Task_dequeue();
int Insert(int value);
int Delete(int value);
int Search(int value);
void Traverse(listnode * move);
void Task_type_check(int number);



void Task_queue(int n){
	srand(time(NULL));
	int numberoftask = 0; // This will be our task_num
	while (numberoftask<n){
		
		int task_type = (rand()%(3)); // 0 or 1 or 2
		int value = (rand()%(101)); // limited to 100 
		Task_enqueue(numberoftask, task_type, value); // add to task list
		numberoftask++;
		
		
}
	printf("Generated %d random list tasks...\n", n);
}

void Task_enqueue(int task_num, int task_type, int value){
	if(taskroot == NULL){ // Task queue is empty 
		taskroot = malloc(sizeof(tasknode));
		taskroot ->task_num = task_num;
		taskroot -> task_type = task_type ;
		taskroot -> value = value;
		taskroot -> next = NULL;
		taskending = taskroot;
	}else { // If task queue is not empty , add to queue.
		taskending -> next = malloc(sizeof(tasknode));
		taskending -> next -> task_num = task_num;
		taskending -> next -> task_type = task_type;
		taskending -> next -> value = value;
		taskending = taskending -> next ;
		taskending -> next = NULL;
	
}
}

int Task_dequeue(){ // I canceled arguments because of my design of my code.
	if(taskroot == NULL){ // If task queue is empty , we cannot dequeue anything from queue.
		printf("%s\n","Linked list is empty");
		return -1;
	}
	int returnvalue = taskroot -> value;	
	tasknode * temp = taskroot;
	taskroot = taskroot -> next;
	free(temp);
	return returnvalue; // Returns us value the process.
}

int Insert(int value){ 
	listnode * temp, * newnode = malloc(sizeof(listnode)); // newnode is created for inserting box.
	
	
	if(Search(value) == 0){ // For no duplicate
		if(newnode){
			newnode -> data = value; // Now inserting box has value.
			if(listroot == NULL){ // İf list is empty add directly.
				listroot = newnode;
				newnode -> next = NULL;
				printf("%d%s\n", value, " is inserted");
				return 1;
			}
		else{
			
			if (listroot -> data > value){ // Root data value more than new inserted
				newnode -> next = listroot ;
				listroot = newnode;
				printf("%d%s\n", value, " is inserted");
				return 1;
			}
			else{
				temp = listroot;
				// We are trying to find appropriate position to add new created node.
				while(temp -> next != NULL && temp -> next -> data < value) {
				temp = temp -> next;
				}
				if(temp -> next == NULL){ // Insert new node in last position.
				newnode -> next = NULL;
				temp -> next = newnode;
				printf("%d%s\n", value, " is inserted");
				return 1;
				}	
				else { //Insert node in middle of linked list.
				
					newnode -> next = temp -> next;
					temp -> next = newnode;
					printf("%d%s\n", value, " is inserted");
					return 1;
				}
			
			
			}
		}
}
	
	}else{ // here search returns us 1; so it means value is already in the list.
		printf("%d%s\n",value, " cannot be inserted");
		return 0;
}
		

}
	
int Delete(int value){
	listnode * temp;
	temp = listroot;
	listnode * freeitem; // will be deleted
	if(listroot == NULL){ // If list is empty, we cannot delete anything from the list.
		printf("%d%s\n",value, " cannot be deleted");
		return -1;
		
	}
	else if(listroot -> data == value){ // what luck! Root will be delete. 
		freeitem = listroot;
		listroot = listroot -> next;
		free(freeitem);
		printf("%d%s\n", value, " is deleted");
		
		return value;
	}
	while(temp -> next != NULL && temp -> next -> data != value){ //moving from beginnig to searched item which will be deleted
		temp = temp -> next ;
	}
	if ( temp -> next == NULL){ // We cannot find the value in the list.
		printf("%d%s\n",value," cannot be deleted");
	}
	else if(temp -> next -> data == value){ // Okay we found.
		freeitem = temp -> next;
		temp -> next = freeitem -> next;
		free(freeitem);
		printf("%d%s\n", value, " is deleted");
		
		return value;
	}

}

int Search(int value){
	listnode * temp = listroot;
	while(temp!=NULL){
  		if(temp -> data == value){
			
			return 1; // Yes, we found.
  		}
		
		temp = temp -> next;
 	}
	return 0; // We cannot find.
}

		
void Traverse(listnode * move) // We are using to print all values in the list.
{
 while(move != NULL){
		printf("%d\t", move -> data);
		move = move -> next;
	}
}

void Task_type_check(int number){ // For type checking which process ? Insert , delete or search.
	for(int i=0; i< number; i++){
		tasknode * temp = taskroot;
		if(temp -> task_type == 0){
			printf("%s%d%s%d%s", "task ",i,"-insert ",temp -> value,":");
			Insert(Task_dequeue());
			
		}
		else if(temp -> task_type == 1){
			printf("%s%d%s%d%s", "task ",i,"-delete ",temp -> value,":");
			Delete(Task_dequeue());
			
		}
		else if(temp -> task_type == 2){
			printf("%s%d%s%d%s", "task ",i,"-search ",temp -> value,":");
			int searchcheck = Search(Task_dequeue());
			if(searchcheck ==1){
				printf("%d%s\n",temp -> value," is found");
			}
			else if(searchcheck == 0){
				printf("%d%s\n",temp -> value," is not found");
			}
			
		}
	}
}
	




int main(int argc, char *argv[]){
	int argument = atoi (argv[1]); // command line argument.
	Task_queue(argument); // created and enqueue to the taskqueue.
	Task_type_check(argument); // Now last thing, insert delete and search process.
	printf("%s\n", "Final list: ");	
	Traverse(listroot);
	
}
