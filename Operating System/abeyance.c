#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stack.h"

#define ROW 100
#define COL 100
#define SIZE 100

struct collisionVector{
	int arr[COL];
	int length;
};

struct state{ // Each state has its own collision vector, and an array of next states. In a One-One mapping each of these have a latency associated with it.
	int value;
	struct collisionVector* cv;
	struct state* next[SIZE];
	int latency[SIZE];
};

int traversedArray[500];
float latencyArray[SIZE];
int latencyIndex = 0;
struct state* adjacencyList[SIZE];
FILE *fp;
int presentInStack[SIZE];
int noOfVertices = 0;
int hashTable[500];
int backupHashTable[500];
int adjMatrix[SIZE][SIZE];

void initializeCollisionVector(struct collisionVector* c, int col);
struct collisionVector createInitialCollisionVector(int reservationTable[ROW][COL],int row,int col);
struct collisionVector* createNewCollisionVector(struct collisionVector* c1,struct collisionVector* c2, int latency);
struct state* createNewState(struct collisionVector* c);
void displayState(struct state* st);
void populateDiagram(struct state* firstState, struct collisionVector* initialVector);
void initializeTraversedArray(void);



void initializeTraversedArray()
{
//    initializeLatencyArray();
	int i;
	for(i = 0; i < 500; i++){
		traversedArray[i] = 0;
	}
}

void initializeCollisionVector(struct collisionVector* c, int col)
{
	int i;
	for(i = 0; i < col; i++){
		c->arr[i] = 0;
	}
}


 struct collisionVector createInitialCollisionVector(int reservationTable[ROW][COL],int row,int col)
 {
 	struct collisionVector cv;
 	initializeCollisionVector(&cv,col);
 	// iterate over each row and find the forbidden latencies
 	int i,j,k,l,latency,max;
 	latency = 0;
 	for(i = 0; i < row; i++){//check for every state
 		for(j = 0; j < col-1; j++){
 			if(reservationTable[i][j] == 1){ // i.e if the state has reserved some space in the reservation table
 				for(l = j+1; l < col; l++){
 					if(reservationTable[i][l] == 1){
 						latency = l - j;
 						if(!cv.arr[latency-1])
 							cv.arr[latency-1] = 1;
 					}
 				}	
 			}
 		}
 	}
 	
 	for(i = col-1; i > -1; i--){
 		if(cv.arr[i] == 1){
 			cv.length = i+1;
 			break;
 		}
 	}
 	return cv;
 }

struct state* createNewState(struct collisionVector* c)
{
	struct state* newState = (struct state*)malloc(sizeof(struct state));
	newState->cv = c;
	newState->value = 0;
	int i;
	for(i = 0; i < c->length; i++){
		int mult = 1;
		for(int j=1;j<=i;j++)
		{
			mult= mult*(2*j);
		}
		newState->value += c->arr[i] * mult;
		newState->latency[i] = -1;
	}
	return newState;
}

struct collisionVector* createNewCollisionVector(struct collisionVector* c1,struct collisionVector* c2, int latency)
{
	struct collisionVector* cv = (struct collisionVector*)malloc(sizeof(struct collisionVector));
	cv->length = c1->length;
 	initializeCollisionVector(cv,c1->length);
 	int i;
 	for(i = 0; i < c1->length-latency; i++){
 	    cv->arr[i] = c1->arr[i+latency];
 	}
 	for(i = 0; i < cv->length; i++){
 	    cv->arr[i] = cv->arr[i] || c2->arr[i];
 	}
 	return cv;
}
void displayState(struct state* st)
{
    int i=0;
    #ifdef DEBUG
    printf("%d\n",st->cv->length);
    for(i = st->cv->length-1; i>-1; i--){
	    printf("%d\t",st->cv->arr[i]);
	}
	printf("\n");
	printf("Value of Collision Vector is: %d\n",st->value);
	printf("Value of children are: \n");
	i=0;
	while(st->latency[i] != -1){
		printf("Latency of edge to this child node is: %d\n", st->latency[i]);
		printf("Value of Collision Vector of child node is: %d\n",st->next[i]->value);
		i++;
	}
	#endif
}
void populateDiagram(struct state* firstState, struct collisionVector* initialVector)
{
    //printf("\nInside populateDiagram!\n \n");
    
    
	int i,j,stateIndex = 0;
	traversedArray[firstState->value-1] = 1;
	
	
	for(i = 0; i<initialVector->length; i++){
	    firstState->latency[i] = -1;
	}
	for(i = 0; i< firstState->cv->length; i++){
 	    if(firstState->cv->arr[i] == 0){
 	        struct collisionVector* c = createNewCollisionVector(firstState->cv, initialVector, i+1); // creating a collision vector using initial collision vector and the latency
 	        struct state* nextState = createNewState(c); // creating the next state using
 	        firstState->next[stateIndex] = nextState; // updating the next state
 	        firstState->latency[stateIndex] = i+1; // updating the latency
 	        stateIndex++;

 		}
 	}
 	adjacencyList[noOfVertices] = firstState;
 	noOfVertices++;
 	displayState(firstState);
 	i=0;
	while(firstState->latency[i] != -1){
		if(traversedArray[firstState->next[i]->value - 1]){
		    i++;
		    continue;
		}
		populateDiagram(firstState->next[i], initialVector);
		i++;
	}
	return;
}


void populateHashTable()
{
	int i;
	for(i = 0; i<noOfVertices; i++){
		int x = adjacencyList[i]->value;
		hashTable[x] = i;
		//backupHashTable[x] = i;
		//printf("Hashtable for %d has value%d\n", adjacencyList[i]->value,i);
	}
}
void populateBackupHashTable()
{
	int i;
	for(i = 0; i<noOfVertices; i++){
		int x = adjacencyList[i]->value;
		//hashTable[x] = i;
		backupHashTable[x] = i;
		//printf("Hashtable for %d has value%d\n", adjacencyList[i]->value,i);
	}
}

void printCycle(struct Stack* stack, int stopValue, int originalValue)
{
	
	int element;
	fp = fopen("Cycles.txt", "a");// "w" means that we are going to write on this file
	struct Stack* temp = createStack(stack->capacity);
	while(!(isEmpty(stack))){
		element = pop(stack);
		
		push(temp,element);
	}
	#ifdef DEBUG
	printf("Printing Cycle:\n");
    #endif

	while(!(isEmpty(temp))){
		element = pop(temp);
		// printf("%d ",element);
		fprintf(fp,"%d ",element);
		push(stack,element);
	}
	element = originalValue;
	// printf("%d \n",element);
	fprintf(fp,"%d %d \n",element, -1);
	fclose(fp);
}

void findCycles(struct Stack* stack,int index, int originalValue)
{
	int i = index,j;

	push(stack, adjacencyList[i]->value);
	presentInStack[i] = 1;
	j = 0;
    //printf(" %d is pushed onto the stack\n",adjacencyList[i]->value);
    
    while(adjacencyList[i]->latency[j] != -1){
	    #ifdef DEBUG
		printf("A neighbour node of %d is %d, and its HashValue is %d. Present in stack: %d\n",adjacencyList[i]->value,adjacencyList[i]->next[j]->value, hashTable[adjacencyList[i]->next[j]->value],presentInStack[hashTable[adjacencyList[i]->next[j]->value]]);
		#endif

		if(adjacencyList[i]->next[j]->value == originalValue){
			#ifdef DEBUG
			printf("it is a cycle!\n");
			#endif

			printCycle(stack,adjacencyList[i]->value,originalValue);
			j++;
		}
		else if(presentInStack[hashTable[adjacencyList[i]->next[j]->value]]){
			#ifdef DEBUG
			printf("It is already present in the stack\n");
			#endif

			j++;
			continue;
		}
		else{
			#ifdef DEBUG
			printf("It has to be pushed onto the stack, and a dfs has to be conducted\n");
			#endif

			findCycles(stack,hashTable[adjacencyList[i]->next[j]->value],originalValue);
			
			j++;
		}
	}
	//printf("Problem after this line\n");
	//pop(stack);
	pop(stack);
	presentInStack[hashTable[adjacencyList[i]->value]] = 0;
	return;
}

void removeVertex(int vertexValue)
{
	#ifdef DEBUG
	printf("Removing vertex %d\n",vertexValue);
	#endif

	int i = 0,j;
	for(j = i+1; j<noOfVertices; j++){
		//printf("No problem with %d<noOfVertices %d\n",j,adjacencyList[j]->value);
		adjacencyList[i] = adjacencyList[j];
		i++;
	}
	//free(adjacencyList[j]);
	noOfVertices--;
	//Now removing the vertex from individual adjacency lists
	for(i = 0; i<noOfVertices; i++){
		j = 0;
		//printf("No problem with %d<noOfVertices %d\n",i,adjacencyList[i]->value);
 	    while(adjacencyList[i]->latency[j] != -1){
 	    	if(adjacencyList[i]->next[j]->value==vertexValue) 
 	    		adjacencyList[i]->latency[j] = -1;
			j++;
		}
	}
	
}

int cmpfunc (const void * a, const void * b)
{
   return ( *(float*)a - *(float*)b );
}


void calculateLatency(int v){
	/*int i,j;
	for(i = 0;i < v; i++){
 	   for(j = 0;j < v; j++){
 			printf("%d\n",adjMatrix[i][j]);	
 	   }
 	}
 	
 	printf("%d\n",backupHashTable[177]);
 	printf("%d\n",backupHashTable[189]);
 	printf("%d\n",backupHashTable[191]);
 	printf("%d\n",backupHashTable[183]);
 	printf("%d\n",backupHashTable[187]);
 	*/
 	
	FILE *f = fopen("Cycles.txt","r");
	latencyIndex = 0;
	float latency = 0;
	int sum = 0;
	int edge = 0;
	int i = -2,j = -2, k = -2, x = -2;
	fscanf(f,"%d %d",&i, &j);
	while(j != -11){
		if(j == -1){
			//either the old cycle can end or the whole process can end
			//printf("Sum of latencies at this point, and no of edges traversed: %d, %d\n",sum,edge);
			latency = (float)sum / edge;
			latencyArray[latencyIndex] = latency;
			latencyIndex++;
			#ifdef DEBUG
			printf("Latency for this cycle is: %f\n",latency);
			#endif

			sum = 0;
			edge = 0;
			fscanf(f,"%d",&x);
			if(x == -11) break; //whole process ends
			else{ // x is a vertex
				//sum += adjMatrix[backupHashTable[k]][backupHashTable[i]];
				//edge += 1;
				
				i = x;
				fscanf(f,"%d",&j);
			}
		}
		sum += adjMatrix[backupHashTable[i]][backupHashTable[j]];
		edge += 1;
		#ifdef DEBUG
		printf("Sum of latencies at this point, and no of edges traversed: %d, %d\n",sum,edge);
		#endif

		i = j;
		k = i; // used in case the old cycle ends
		fscanf(f,"%d",&j);
	}
	/*
	while(i != -11 || j!= -11){
		while(i != -1 || j != -1){
			printf("Vertices in the loop: %d-->%d\n",i,j);
			sum += adjMatrix[backupHashTable[i]][backupHashTable[j]];
			edge += 1;
			printf("Sum of latencies at this point, and no of edges traversed: %d, %d\n",sum,edge);
			fscanf(f,"%d %d",&i,&j);
			printf("Vertices in the loop: %d-->%d\n",backupHashTable[i],backupHashTable[j]);
		}
		latency = (float)sum / edge;
		printf("Latency for this cycle is: %f",latency);
		
	}
	*/
	
}

int main()
 {
 	fp = fopen("Cycles.txt","w");
 	fclose(fp);
 	int arr[ROW][COL], i, j, stateIndex;
 	int row, col;
 	FILE *input;
 	input = fopen("Reservation.txt","r");
 	fscanf(input,"%d %d",&row,&col);
 	for(i = 0; i < row; i++){
 		for(j = 0; j < col; j++){
 			
 			fscanf(input,"%d",&arr[i][j]);
 			// if(arr[i][j] == 1)
 				//sprintf("%d %d\n",i,j);
 		}
 	}
 	fclose(input);
 	struct collisionVector initialVector = createInitialCollisionVector(arr,row,col);
	struct state* firstState = createNewState(&initialVector);
	
 	
 	struct state* nullState = (struct state*)malloc(sizeof(struct state));
	nullState->next[0] = firstState;
	nullState->latency[0] = 0;
	for(i = 0; i < initialVector.length-1; i++){
		nullState->latency[i+1] = -1;
	}
	nullState->value = 0;
 	initializeTraversedArray();
 	populateDiagram(firstState,&initialVector);
 	initializeTraversedArray();
 	int cycles = noOfVertices;
 	populateHashTable();
 	populateBackupHashTable();
 	int k;
 	
 	
 	for(i = 0;i < noOfVertices; i++){
 	    j = 0;
 	    while(adjacencyList[i]->latency[j] != -1){
	
			adjMatrix[backupHashTable[adjacencyList[i]->value]][backupHashTable[adjacencyList[i]->next[j]->value]] = adjacencyList[i]->latency[j];
			j++;
		}
	
 	}
 	
 	//struct Stack* blockedStack = createStack(100);
 	int index = 0;
 	
 	for(index = 0; index < cycles; index++){
 	#ifdef DEBUG
 	printf("---------------Iteration--------------\n");
 	printf("Printing the current adjacencyList: \n");
	#endif

 	for(i = 0;i < noOfVertices; i++){
 			//presentInStack[i] = 0;
 	    	j = 0;
 	    	#ifdef DEBUG
 			printf("The current node is: %d\n",adjacencyList[i]->value);
 			#endif

 	    	while(adjacencyList[i]->latency[j] != -1){
		//		printf("%d,%d\n",adjacencyList[i]->next[j]->value,adjacencyList[i]->latency[j]);
				j++;
			}
 		}
 		//printf("\n");
 		struct Stack* stack = createStack(100);
 		findCycles(stack,0,adjacencyList[0]->value);
 		//printf("problem at 297\n");
 		removeVertex(adjacencyList[0]->value); // After removing the vertex, the whole array is shifted left so it always starts from 0.
 		//printf("problem at 299\n");
 		populateHashTable();
 		free(stack);
 	}
 	
 	fp = fopen("Cycles.txt","a");
 	fprintf(fp,"%d %d",-11,-12);
 	fclose(fp);
 	calculateLatency(cycles);
 	int demoLatencyArray[latencyIndex];
 	for(k = 0; k < latencyIndex - 1; k++){
 		demoLatencyArray[k] = latencyArray[k];
 	}
 	for(i = 0; i < latencyIndex-1; i++){
 		for(j = i+1; j < latencyIndex; j++){
 			if(latencyArray[i] > latencyArray[j]){
 				float temp = latencyArray[j];
 				latencyArray[j] = latencyArray[i];
 				latencyArray[i] = temp;
 			}
 		}
 		
 	}
 	for(k = 0; k < latencyIndex - 1; k++){
 		if(latencyArray[0] == demoLatencyArray[k])
 			break;
 	}
 	fp = fopen("Cycles.txt","r");
 	int count = -1;
 	while(count != (k-1)){
 		int elem;
 		fscanf(fp,"%d",&elem);
 		if(elem == -1) count++;
 	}
 	int elem = 0;
 	printf("\nThe minimum average latency for this pipeline is: %f\n",latencyArray[0]);
 	printf("The Cycle containing the minimum average latency is: ");
 	fscanf(fp,"%d",&elem);
 	while(elem !=-1){
 		printf("%d ",elem);
 		fscanf(fp,"%d",&elem);
 	}
 	printf("\n");
 	fclose(fp);
 	
 	
 }
 /*
//  Resultant collision vector is 1	0	1	1	0	0	0	1
 */
