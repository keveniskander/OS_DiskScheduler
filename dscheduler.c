/*
DScheduler.c

Student Name : Keven Iskander
Student ID # : 160634540

*/

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "dscheduler.h"


/*
Any required standard libraries and your header files here
*/

struct schedulerInput loadRequest(){
    struct schedulerInput results;
    int numRequests;
    char line_buffer[MAX_LINE_LENGTH];
    char direction;
    char *token;
    
    //Process simulation input line by line
    fgets(line_buffer, MAX_LINE_LENGTH, stdin);

    token = strtok(line_buffer, " ");
    sscanf(token, "%d",&numRequests);
    
    token = strtok(NULL, " ");
    sscanf(token, "%d",&results.startTrack);

    token = strtok(NULL, " ");
    sscanf(token, "%c",&direction);
    results.direction = direction == 'u' ? 1 : -1;
    
    results.requests.elements = numRequests;
    results.requests.data = (int *)malloc(sizeof(int)*numRequests);
    if ( results.requests.data == NULL ){
        fprintf( stderr, "Was unable to allocate space for requests data.\n");
        exit( BAD_MALLOC );
    }
    
    for (int i = 0; i < numRequests; i++){
        token = strtok(NULL, " ");
        sscanf(token, "%d", &results.requests.data[i]);
    }
    
    return results;
}

void printResults(struct schedulerResult results){
    for (int i = 0; i < results.requests.elements; i++){
        printf("%4d ", results.requests.data[i]);
    }
    printf("Total Head Movement %5d\n", results.totalHeadMovement);
}

struct schedulerResult processRequest(enum POLICIES policy, struct schedulerInput request){
    struct schedulerResult results;
    
    switch(policy){
        case FCFS:
            return process_FCFS_request(request);
        case SSTF:
            return process_SSTF_request(request);
        case SCAN:
            return process_SCAN_request(request);
        case C_SCAN:
            return process_C_SCAN_request(request);
    }
    return results;
}



/* Fill in the following functions */
struct schedulerResult process_FCFS_request(struct schedulerInput request){
    struct schedulerResult results;
    results.requests.elements =  request.requests.elements;


    int numRequests;
    numRequests = request.requests.elements;
    results.requests.elements = numRequests;
    results.requests.data = (int *)malloc(sizeof(int)*numRequests);
    results.totalHeadMovement = 0;
    int current = request.startTrack;


    printf("FCFS ->");


    for (int i = 0; i<request.requests.elements; i++){

    	results.totalHeadMovement += abs(current - request.requests.data[i]);
    	current = request.requests.data[i];
    	results.requests.data[i] = request.requests.data[i];


    }

    return results;
}

struct schedulerResult process_SSTF_request(struct schedulerInput request){

	struct schedulerResult results;
	results.requests.elements =  request.requests.elements;

	int numRequests;
	numRequests = request.requests.elements;
	results.requests.data = (int *)malloc(sizeof(int)*numRequests);
	results.totalHeadMovement = 0;
	results.requests.elements = numRequests;
	int j = 0;
	int current = request.startTrack;
	int smallest_dif = abs(current-(request.requests.data[j]));
	int p = 0;
	int smallest_index = 0;
	int visited[numRequests];

	for(int b = 0; b < numRequests; b++){
		visited[b] = 0;
	}


	while(p<numRequests){
		smallest_dif = 1023;
		for (j = 0; j < numRequests; j++){

			if(abs(current - request.requests.data[j]) < smallest_dif && visited[j] != 1){
				smallest_dif = abs(current - request.requests.data[j]);
				smallest_index = j;

			}
		}


		visited[smallest_index] = 1;


		results.totalHeadMovement += abs(current - request.requests.data[smallest_index]);
		current = request.requests.data[smallest_index];
		results.requests.data[p] = request.requests.data[smallest_index];

		p++;
	}
	printf("SSTF ->");
	return results;

}

struct schedulerResult process_SCAN_request(struct schedulerInput request){
    struct schedulerResult results;
    results.requests.elements =  request.requests.elements;

    int numRequests;
	numRequests = request.requests.elements;
	results.requests.data = (int *)malloc(sizeof(int)*numRequests);
	results.totalHeadMovement = 0;
	results.requests.elements = numRequests;

	int current = request.startTrack;
	int visited[numRequests];
	int q = 0;
	int k = 0;
	int smallest_distance = abs(current - request.requests.data[k]);
	int large_count = 0, higher = 0;
	int small_count = 0, lower = 0;
	int smallest_index = 0;
	int max_adder = 0, min_adder = 0;

	for(int b = 0; b < numRequests; b++){

		if(request.requests.data[b] > current){
			large_count++;
		}else{
			small_count++;
		}
		visited[b] = 0;
	}
	printf("SCAN ->");

	if(request.direction == 1){

		while(q<numRequests){


			if(large_count == higher && max_adder == 0 && large_count!=0){

				current = 1023;
				results.totalHeadMovement += abs(current-request.requests.data[smallest_index]);

				max_adder = 1;

			}else if(max_adder == 0 && large_count==0){

				current = 1023;
				results.totalHeadMovement += abs(current-request.startTrack);

				max_adder = 1;
			}

			smallest_distance = 1023;


			if(q<large_count){

				higher++;

				for (k = 0; k < numRequests; k++){


					if((visited[k] != 1 ) && (request.requests.data[k] > request.startTrack) && smallest_distance > abs(current - request.requests.data[k])){

						smallest_distance = abs(request.startTrack - request.requests.data[k]);
						smallest_index = k;


					}
				}

			}else{

				for (k = 0; k < numRequests; k++){

					if((visited[k] != 1 ) && (request.requests.data[k] < request.startTrack) && smallest_distance > abs(current - request.requests.data[k])){

						smallest_distance = abs(current - request.requests.data[k]);
						smallest_index = k;

					}
				}
			}

			visited[smallest_index] = 1;

			results.totalHeadMovement += abs(current - request.requests.data[smallest_index]);
			current = request.requests.data[smallest_index];
			results.requests.data[q] = request.requests.data[smallest_index];

			q++;
		}
	}else{


		while(q<numRequests){


			if(small_count == lower && min_adder == 0 && small_count!=0){


				current = 0;
				results.totalHeadMovement += abs(current-request.requests.data[smallest_index]);

				min_adder = 1;

			}else if(min_adder == 0 && small_count==0){


				current = 0;
				results.totalHeadMovement += abs(current-request.startTrack);

				min_adder = 1;
			}

			smallest_distance = 1023;

			if(q<small_count){
				lower++;
				for (k = 0; k < numRequests; k++){

					if((visited[k] != 1 ) && (request.requests.data[k] < request.startTrack) && smallest_distance > abs(current - request.requests.data[k])){

						smallest_distance = abs(request.startTrack - request.requests.data[k]);
						smallest_index = k;


					}
				}
			}else{


				for (k = 0; k < numRequests; k++){

					if((visited[k] != 1 ) && (request.requests.data[k] > request.startTrack) && smallest_distance > abs(current - request.requests.data[k])){

						smallest_distance = abs(current - request.requests.data[k]);
						smallest_index = k;

					}
				}
			}

			visited[smallest_index] = 1;

			results.totalHeadMovement += abs(current - request.requests.data[smallest_index]);
			current = request.requests.data[smallest_index];
			results.requests.data[q] = request.requests.data[smallest_index];

			q++;
		}



	}
    return results;
}

struct schedulerResult process_C_SCAN_request(struct schedulerInput request){

    struct schedulerResult results;
    results.requests.elements =  request.requests.elements;

    int numRequests;
	numRequests = request.requests.elements;
	results.requests.data = (int *)malloc(sizeof(int)*numRequests);
	results.totalHeadMovement = 0;
	results.requests.elements = numRequests;

	int current = request.startTrack;
	int visited[numRequests];
	int q = 0;
	int k = 0;

	int smallest_distance = abs(current - request.requests.data[k]);
	int large_count = 0, higher = 0;
	int small_count = 0, lower = 0;
	int smallest_index = 0;
	int max_adder = 0;

	for(int b = 0; b < numRequests; b++){

		if(request.requests.data[b] > current){
			large_count++;
		}else{
			small_count++;
		}
		visited[b] = 0;
	}
	printf("C-SCAN ->");

	if(request.direction == 1){

		while(q<numRequests){


			if(large_count == higher && max_adder == 0 && large_count!=0){
				current = 1023;
				results.totalHeadMovement += abs(current-request.requests.data[smallest_index]);
				results.totalHeadMovement += abs(current-0);
				current = 0;

				max_adder = 1;

			}else if(max_adder == 0 && large_count==0){
				current = 1023;
				results.totalHeadMovement += abs(current-request.startTrack);
				results.totalHeadMovement += abs(current-0);
				current = 0;

				max_adder = 1;
			}

			smallest_distance = 1023;

			if(q<large_count){

				higher++;

				for (k = 0; k < numRequests; k++){


					if((visited[k] != 1 ) && (request.requests.data[k] > request.startTrack) && smallest_distance > abs(current - request.requests.data[k])){

						smallest_distance = abs(request.startTrack - request.requests.data[k]);
						smallest_index = k;


					}
				}

			}else{

				for (k = 0; k < numRequests; k++){

					if((visited[k] != 1 ) && (request.requests.data[k] < request.startTrack) && smallest_distance > abs(current - request.requests.data[k])){

						smallest_distance = abs(current - request.requests.data[k]);
						smallest_index = k;

					}
				}
			}

			visited[smallest_index] = 1;

			results.totalHeadMovement += abs(current - request.requests.data[smallest_index]);
			current = request.requests.data[smallest_index];
			results.requests.data[q] = request.requests.data[smallest_index];

			q++;
		}
	}else{

		while(q<numRequests){


			if(max_adder == 0 && large_count!=0){
				current = 0;
				results.totalHeadMovement += abs(current-request.startTrack);


				max_adder = 1;

			}else if(max_adder == 0 && large_count==0){
				current = 0;
				results.totalHeadMovement += abs(current-request.startTrack);


				max_adder = 1;
			}

			smallest_distance = 1023;

			if(lower == small_count && higher!= large_count){

				higher++;

				for (k = 0; k < numRequests; k++){


					if((visited[k] != 1 ) && (request.requests.data[k] > request.startTrack) && smallest_distance > abs(current - request.requests.data[k])){

						smallest_distance = abs(current - request.requests.data[k]);
						smallest_index = k;


					}
				}

			}else{

				for (k = 0; k < numRequests; k++){

					if((visited[k] != 1 ) && (request.requests.data[k] < request.startTrack) && smallest_distance > abs(current - request.requests.data[k])){

						smallest_distance = abs(current - request.requests.data[k]);
						smallest_index = k;

					}
				}

				lower++;
			}

			visited[smallest_index] = 1;

			results.totalHeadMovement += abs(current - request.requests.data[smallest_index]);
			current = request.requests.data[smallest_index];
			results.requests.data[q] = request.requests.data[smallest_index];

			q++;
		}

	}

    return results;
}
