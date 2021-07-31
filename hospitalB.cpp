#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>

#include <set>
#include <iterator>
#include <map>
#include <typeinfo>

#include <limits.h>

// extern "C"{
// 	#include "scheduler_helper.h"
// }

using namespace std;

// int num_vertex = 0;

// void execute_djikstra(double graph[][num_vertex], int location);

int main(int argc, char* argv[]){

	//Phase 1: Boot-up
	set<int> s;
	map<int,int> mp;
	int location = 0;
	int totalCapacity = 0;
	int initialOccupancy = 0;

	// double shortest_distance = 100000; //This value is selected as max is only 10000
	
	string file = "map.txt"; //CHANGE TO map.txt for submission!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//Read in map.txt and construct the graph
	fstream map_file;
	fstream map_file2;
	// map_file.open("map.txt", ios::in); //Use this for submission!
	map_file.open(file, ios::in); //Use this for test case - simple map!
	// map_file.open("map_hard.txt", ios::in); //Use this for test case-hard map!

	if(!map_file){
		cout << "Map file does not exist" << endl;
		return -1;
	}
	string line;

	//Iterate through each line and in each line, add first and second vertex in set
	while(getline(map_file, line)){
		int n = line.length();
		//Declare char array
		char char_array[n+1];
		//Copy contents of string line to char array (C string)
		strcpy(char_array,line.c_str());
		char delim[30] = " \t\n";
		char* token = strtok(char_array,delim);
		int index = 0;
		while(token){
			//Insert first vertex and second vertex in set
			if(index==0 || index==1){
				// cout << token << endl;
				s.insert(stoi(token));
			}
			token = strtok(NULL,delim);
			index += 1;
		}
	}

	//Create adjacency matrix using set length
	int num_vertex = s.size();
	double adjMatrix[num_vertex][num_vertex]; //Adjacency matrix 

	//Create Reindex map containing original index as key and value as new index
	set<int>::iterator it = s.begin();
	int index = 0;
	while(it != s.end()){
		mp.insert({*it,index});
		it++;
		index += 1;
	}

	//Initialize adjacency matrix to 0
	for(int i=0;i<num_vertex;i++){
		for(int j=0;j<num_vertex;j++){
			adjMatrix[i][j] = 0;
			// cout << adjMatrix[i][j] << " ";
		}
		// cout << endl;
	}

	// for (auto itr = mp.begin(); itr != mp.end(); ++itr){
	// 	cout << itr->first << '\t' << itr->second << endl;
	// }

	//Iterate through each line and retrieve, first vertex, second vertex and distance
	//Insert distance in adjMatrix using reindexed map 

	// map_file2.open("map.txt", ios::in); //Use this for submission!
	map_file2.open(file, ios::in); //Use this for test case - simple map!
	// map_file2.open("map_hard.txt", ios::in); //Use this for test case-hard map!
	while(getline(map_file2, line)){
		// cout << line << endl;
		int n = line.length();
		//Declare char array
		char char_array[n+1];
		//Copy contents of string line to char array (C string)
		strcpy(char_array,line.c_str());
		char delim[30] = " \t\n";
		char* token = strtok(char_array,delim);
		int index = 0;

		int first_vertex = -1;
		int second_vertex = -1;

		while(token){
			//Insert first vertex and second vertex in set
			if(index==0){
				first_vertex = stoi(token);
			}
			if(index==1){
				second_vertex = stoi(token);
			}
			if(index==2){
				//Update distance in adjMatrix using reindexed map
				adjMatrix[mp.find(first_vertex)->second][mp.find(second_vertex)->second] = stod(token);
				adjMatrix[mp.find(second_vertex)->second][mp.find(first_vertex)->second] = stod(token);
			}
			token = strtok(NULL,delim);
			index += 1;
		}
	}

	//Testing - Print values in adjacency matrix
	// for(int i=0;i<num_vertex;i++){
	// 	for(int j=0;j<num_vertex;j++){
	// 		cout << adjMatrix[i][j] << " ";
	// 	}
	// 	cout << endl;
	// }

	//Initialize capacity and occupancy from command line arguments
	if(argv[1]!=NULL && argv[2]!=NULL && argv[3]!=NULL){
		location = atoi(argv[1]); //Location of hospital
		totalCapacity = atoi(argv[2]);
		initialOccupancy = atoi(argv[3]);
		// cout << "Location: " << location << ", totalCapacity: " << totalCapacity << ", initialOccupancy: " << initialOccupancy << endl;		
	}
	else{
		cout << "Invalid commandline parameters" << endl;
	}

	map_file.close();
	map_file2.close();

	//Boot-up
	int hospital_server_socket;
	struct sockaddr_in hospital_server_address;

	//Create UDP socket
	hospital_server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(hospital_server_socket < 0){
		printf("Error in creating socket hospital A socket\n");
		return -1;
	}
	// printf("Socket is created successfully\n");

	//Set port and IP:
	hospital_server_address.sin_family = AF_INET;
	hospital_server_address.sin_port = htons(31476);
	hospital_server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	//Bind to the set port and IP
	if(bind(hospital_server_socket, (struct sockaddr*)&hospital_server_address, sizeof(hospital_server_address)) <0){
		printf("Couldn't bind to the port\n");
		return -1;
	}
	// printf("Done with binding\n");

	//Boot-up complete
	printf("Hospital B is up and running using UDP on port %d.\nHospital B has total capacity %d and initial occupancy %d\n",ntohs(hospital_server_address.sin_port),totalCapacity,initialOccupancy);



	//Phase 2: Forward - Inform the scheduler
	// int client_socket;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	char server_message[2000], client_message[2000];
	socklen_t server_address_length = sizeof(server_address);
	socklen_t client_address_length = sizeof(client_address);

	// Clean buffers:
	memset(server_message, '\0', sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));

	// Create socket:
	// client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// if(client_socket < 0){
	//   printf("Error in creating socket\n");
	//   return -1;
	// }
	// printf("Socket is created successfully\n");
	//printf("Hospital A is up and running using UDP on port %d. Hospital A has total capacity %d and initial occupancy %d.\n",)

	// Set port and IP:
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(33476);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	// // Get input from the user:
	// printf("Enter message: ");
	// cin >> client_message;
	// gets(client_message);

	//Create client message
	string client_message_string = string("B:") + to_string(totalCapacity) + "," + to_string(initialOccupancy);
		//Convert string to c string
	strcpy(client_message,client_message_string.c_str());

	// cout << "wow1" << endl;
	// Send the message to server (Scheduler):
	if(sendto(hospital_server_socket, client_message, strlen(client_message), 0,
	   (struct sockaddr*)&server_address, server_address_length) < 0){
	  printf("Unable to send message to scheduler\n");
	  return -1;
	}

	// cout << "wow2" << endl;

	//Phase 3 - Scoring

	// Receive the server's response (Scheduler):
	if(recvfrom(hospital_server_socket, server_message, sizeof(server_message), 0,
	   (struct sockaddr*)&client_address, &client_address_length) < 0){
	  printf("Error while receiving scheduler's message\n");
	  return -1;
	}

	// cout << "wow3" << endl;
 
	//Extract client location from client message
	int clientLocation = -1;
	int clientLocation_reindexed = -1;

	string clientLocation_string = string(server_message).substr(7);

	clientLocation = atoi(clientLocation_string.c_str());
	// printf("Scheduler's response: %s\n", server_message);
	printf("Hospital B has received input from client at location %d\n",clientLocation);


	//Location finding
	map<int,int>::iterator it2 = mp.find(clientLocation);
	if(it2 !=mp.end()){
		//Since clientLocation exists in map, execute scoring via djikstra
		clientLocation_reindexed = it2->second;

		//Scoring - execute djikstra!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		//Output array - Holds the shortest distance from clientLocation (source) to i
		double shortestDistance[num_vertex];

		//shortestPathSet[i] is equal to true if vertex i is inside of shortest path tree
		bool shortestPathSet[num_vertex];

		//Initialization
		for(int i=0;i<num_vertex;i++){
			shortestDistance[i] = INT_MAX; //Initalize as Infinite
			shortestPathSet[i] = false;
		}

		//Distance of clientLocation vertex from itself is always 0
		shortestDistance[clientLocation_reindexed] = 0;

		//Obtain shortest path for all vertices - Required
		for(int i=0;i<num_vertex-1;i++){

			//Pick vertex with min distance from the set of vertices not yet processed

			//Find vertex with minimum distance value, from set of vertexes that are not included in shortest path tree

			//Initialize minimum value and current vertex
			int minimum = INT_MAX;
			int vertex_minDistValue = -1;

			for(int v=0;v<num_vertex;v++){
				if(shortestPathSet[v] == false && shortestDistance[v] <= minimum){
					minimum = shortestDistance[v];
					vertex_minDistValue = v;
				}
			}

			//Update the picked vertex as processed
			shortestPathSet[vertex_minDistValue] = true;

			//Update distance value of the adjacent vertices of the picked vertex
			for(int v=0;v<num_vertex;v++){
				//Update shortestDistance[v] if the vertex is not in shortestPathSet and
				//new distance which includes clientLocation to v through vertex_minDistValue is smaller than current amount in shortestDistance[v]
				if(!shortestPathSet[v] && adjMatrix[vertex_minDistValue][v] && shortestDistance[vertex_minDistValue] != INT_MAX
					&& shortestDistance[vertex_minDistValue] + adjMatrix[vertex_minDistValue][v] < shortestDistance[v]){

					shortestDistance[v] = shortestDistance[vertex_minDistValue] + adjMatrix[vertex_minDistValue][v];
				}
			}
		}

		//Print constructed distance array
		// printf("Vertex \t\t Distance from Source\n");
		// for (int i = 0; i < num_vertex; i++){
	 //    	printf("%d \t\t %f\n", i, shortestDistance[i]);
		// }

		//Get reindexed position of hospital location
		int hospital_location_reindexed = -1;
		map<int,int>::iterator hospital_location_iterator = mp.find(location);
		if(hospital_location_iterator!=mp.end()){
			hospital_location_reindexed = mp.find(location)->second;
		}
		else{
			printf("Invalid hospital B location\n");
			return -1;
		}

		//Calculate score
		double d = -1.0; //Shortest distance
		double a = -1.0; //Availability
		double score = -1.0;

		//Determine Availability
		a = (totalCapacity-initialOccupancy)/(double)totalCapacity;
		// cout << "totalCapacity:" << totalCapacity << endl;
		// cout << "initialOccupancy:" << initialOccupancy << endl;
		// cout << "a:" << a << endl;
		if(a<0 || a>1){
			a = -1;
		}
			//Check if location is same as hospital location
		if(clientLocation_reindexed==hospital_location_reindexed){
			//distance is NONE
			d = -1;
		}
		else{

			//Shortest distance
			d = shortestDistance[hospital_location_reindexed];
		}
		if(d!=-1 && a!=-1){
			//Compute score
			score = 1/(double)(d*(1.1-a));

		}
		//print statements
			//availability
		if(a!=-1){
			printf("Hospital B has capacity=%d,occupation=%d,availability=%f\n",totalCapacity,initialOccupancy,a);			
		}
		else{
			//availability none
			printf("Hospital B has capacity=%d,occupation=%d,availability=None\n",totalCapacity,initialOccupancy);
		}
			//shortest path - d
		if(d!=-1){
			printf("Hospital B has found the shortest path to client, distance=%f\n",d);			
		}
		else{
			//No shortest path
			printf("Hospital B has found the shortest path to client, distance=None\n");		
		}
			//score
		if(score!=-1){
			printf("Hospital B has the score=%f\n",score);
		}
		else{
			//score none
			printf("Hospital B has the score=None\n");
		}
		//For sending message to scheduler to inform score and distance
		char client_message2[2000];
		memset(client_message2, '\0', sizeof(client_message2));
		//Create client message
		if(score!=-1 && d!=-1){
			client_message_string = string("B-Score:") + to_string(score) + ",Distance:" + to_string(d);
		}
		else if(score==-1 && d==-1){
			client_message_string = string("B-None Score,None Distance");
		}
		else if(score==-1){
			client_message_string = string("B-None Score") + ",Distance:" + to_string(d);
		}
		else{
			//d==-1
			client_message_string = string("B-Score:") + to_string(score) + ",None Distance";
		}
		//Convert string to c string
		strcpy(client_message2,client_message_string.c_str());
		// printf("%s\n",client_message2);

		// Send the message to server (Scheduler):
		if(sendto(hospital_server_socket, client_message2, strlen(client_message2), 0,
		   (struct sockaddr*)&server_address, server_address_length) < 0){
		  printf("Unable to send message to scheduler\n");
		  return -1;
		}

		//Message to indicate score and distance have been sent to scheduler
		if(score!=-1 && d!=-1){
			printf("Hospital B has sent score=%f and distance=%f to the Scheduler\n",score,d);
		}
		else if(score==-1 && d==-1){
			printf("Hospital B has sent score=None and distance=None\n");
		}
		else if(score==-1){
			printf("Hospital B has sent score=None and distance=%f\n",d);
		}
		else{
			//d==-1
			printf("Hospital B has sent score=%f and distance=None\n",score);
		}

		//For receiving the result from the Scheduler
		memset(server_message, '\0', sizeof(server_message));
		if(recvfrom(hospital_server_socket, server_message, sizeof(server_message), 0,
		   (struct sockaddr*)&client_address, &client_address_length) < 0){
		  printf("Error while receiving scheduler's result\n");
		  return -1;
		}
		if(strcmp(server_message,"Assigned")==0){
			//Update occupancy and availability
			double newOccupancy = initialOccupancy + 1.0;
			double new_a = (totalCapacity-newOccupancy)/(double)totalCapacity;
			printf("Hospital B has been assigned to a client, occupation is updated to %d, availability is updated to %f\n",(int)newOccupancy,new_a);
		}
		
	}
	else{
		//Client location not found
		printf("Hospital B does not have the location %d in map\n",clientLocation);
		char client_message2[2000];
		memset(client_message2, '\0', sizeof(client_message2));
		//Create client message
		client_message_string = string("B:Invalid Client location");
			//Convert string to c string
		strcpy(client_message2,client_message_string.c_str());
		// printf("%s\n",client_message2);

		// Send the message to server (Scheduler):
		if(sendto(hospital_server_socket, client_message2, strlen(client_message2), 0,
		   (struct sockaddr*)&server_address, server_address_length) < 0){
		  printf("Unable to send message to scheduler\n");
		  return -1;
		}

		//Location finding - no location found message sent to scheduler
		printf("Hospital B has sent \"location not found\" to the Scheduler\n");
	}




	// Close the socket:
	close(hospital_server_socket);
	  
	return 0;


}

// void execute_djikstra(double graph[][num_vertex], int location){
// 	cout << "Num_vertex: " << num_vertex << endl;
// }