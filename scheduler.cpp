// #include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <limits.h>

// extern "C"{
// 	#include "scheduler_helper.h"
// }

using namespace std;

int main(){

	//UDP with Hospital
	int server_socket;
	//Client_address are variable to store the addresses of Hospital A, B and C
	struct sockaddr_in server_address, client_address1, client_address2, client_address3;
	char server_message[2000], client_message[2000];
	socklen_t client_address_length1 = sizeof(client_address1);
	socklen_t client_address_length2 = sizeof(client_address2);
	socklen_t client_address_length3 = sizeof(client_address3);

	//Clean buffers
	memset(server_message, '\0', sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));

	//Create UDP socket
	server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if(server_socket < 0){
		printf("Error in creating socket\n");
		return -1;
	}
	// printf("Socket is created successfully\n");

	//Set port and IP:
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(33476);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	//Bind to the set port and IP
	if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) <0){
		printf("Couldn't bind to the port\n");
		return -1;
	}
	// printf("Done with binding\n");

	// printf("Listening for incoming messages..\n\n");
	printf("The Scheduler is up and running.\n");

	//Initalize total capacity and initial capacity variables
	int totalCapacity_A = -1;
	int totalCapacity_B = -1;
	int totalCapacity_C = -1;
	int initialOccupancy_A = -1;
	int initialOccupancy_B = -1;
	int initialOccupancy_C = -1;


	//Receive client's message (Hospital A )
	if(recvfrom(server_socket, client_message, sizeof(client_message),0,
		(struct sockaddr*)&client_address1, &client_address_length1) < 0){
		printf("Hospital A's message cannot be sent\n");
		return -1;
	}
	string type = string(client_message).substr(0,1);
	if(type=="A"){
		int index = 2;
		string totalCapacity_A_string = "";
		//Add string characters as long ","" is not found to ensure single or double integers are catered for
		while(string(client_message)[index]!=','){
			totalCapacity_A_string += string(client_message)[index];
			index += 1;
		}
		totalCapacity_A = atoi(totalCapacity_A_string.c_str());
		initialOccupancy_A = atoi((string(client_message).substr(index+1)).c_str());
		// cout << typeid(initialCapacity_A).name() << endl;
		printf("The Scheduler has received information from Hospital %s:total capacity is %d and initial occupancy is %d\n",type.c_str(),totalCapacity_A,initialOccupancy_A);
	}

	//Receive client's message (Hospital B )
	memset(client_message, '\0', sizeof(client_message));
	if(recvfrom(server_socket, client_message, sizeof(client_message),0,
		(struct sockaddr*)&client_address2, &client_address_length2) < 0){
		printf("Hospital B's message cannot be sent\n");
		return -1;
	}
	type = string(client_message).substr(0,1);
	if(type=="B"){
		int index = 2;
		string totalCapacity_B_string = "";
		//Add string characters as long ","" is not found to ensure single or double integers are catered for
		while(string(client_message)[index]!=','){
			totalCapacity_B_string += string(client_message)[index];
			index += 1;
		}
		totalCapacity_B = atoi(totalCapacity_B_string.c_str());
		initialOccupancy_B = atoi((string(client_message).substr(index+1)).c_str());
		printf("The Scheduler has received information from Hospital %s:total capacity is %d and initial occupancy is %d\n",type.c_str(),totalCapacity_B,initialOccupancy_B);
	}	

	//Receive client's message (Hospital C )
	memset(client_message, '\0', sizeof(client_message));
	if(recvfrom(server_socket, client_message, sizeof(client_message),0,
		(struct sockaddr*)&client_address3, &client_address_length3) < 0){
		printf("Hospital C's message cannot be sent\n");
		return -1;
	}
	type = string(client_message).substr(0,1);
	if(type=="C"){
		int index = 2;
		string totalCapacity_C_string = "";
		//Add string characters as long ","" is not found to ensure single or double integers are catered for
		while(string(client_message)[index]!=','){
			totalCapacity_C_string += string(client_message)[index];
			index += 1;
		}
		totalCapacity_C = atoi(totalCapacity_C_string.c_str());
		initialOccupancy_C = atoi((string(client_message).substr(index+1)).c_str());
		printf("The Scheduler has received information from Hospital %s:total capacity is %d and initial occupancy is %d\n",type.c_str(),totalCapacity_C,initialOccupancy_C);
	}



	

	//TCP with client

	//Create socket to communicate with client
	int parent_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(parent_socket<0){
		printf("Error while creating socket to communicate with client\n");
		return -1;
	}

	struct sockaddr_in server_addressess;

	//Set Port and IP
	server_addressess.sin_family = AF_INET;
	server_addressess.sin_port = htons(34476);
	server_addressess.sin_addr.s_addr = inet_addr("127.0.0.1");	
	int bind_result = bind(parent_socket, (struct sockaddr*)&server_addressess, sizeof(server_addressess));
	if(bind_result<0){
		printf("Couldn't bind to the TCP port\n");
		return -1;
	}

	//Listen for clients
	if(listen(parent_socket,1) < 0){
		printf("Error while listening for client\n");
		return -1;
	}
	// printf("Listening for incoming connections....\n");

	//Accept client connection from client
	struct sockaddr_in client_addressess;
	socklen_t client_size = sizeof(client_addressess);
	int client_socket = accept(parent_socket,(struct sockaddr*)&client_addressess, &client_size);

	if(client_socket < 0){
		printf("Client connection cannot be accepted\n");
		return -1;
	}
	// printf("Client connects at IP: %s and port: %d\n", inet_ntoa(client_addressess.sin_addr), ntohs(client_addressess.sin_port));

	//Receive client's message
	// char client_message[2000];
	strcpy(client_message,"");
	memset(client_message, '\0', sizeof(client_message));

	if(recv(client_socket, client_message, sizeof(client_message), 0) < 0){
		printf("Could not receive client's message\n");
		return -1;
	}
	// printf("Message from client:%s\n", client_message);
	int clientLocation = atoi(string(client_message).substr(7).c_str());
	printf("The Scheduler has received client at location %d from the client using TCP over port %d\n",clientLocation,ntohs(server_addressess.sin_port));

	//Send client location to Hospitals via UDP - Last step of phase 2 forward
	double availability_A = (totalCapacity_A - initialOccupancy_A)/(double)totalCapacity_A;
	double availability_B = (totalCapacity_B - initialOccupancy_B)/(double)totalCapacity_B;
	double availability_C = (totalCapacity_C - initialOccupancy_C)/(double)totalCapacity_C;

	//Hospital A
	if(availability_A>0){
		// Clean buffers:
		memset(client_message, '\0', sizeof(client_message));

		string clientLocationString = string("Client:") + to_string(clientLocation);
		// string clientLocationString = "Client:";
		strcpy(client_message, clientLocationString.c_str());
		// client_address_length = sizeof(client_message);

		if(sendto(server_socket,client_message,strlen(client_message),0,
			(struct sockaddr*)&client_address1, client_address_length1) < 0){
			printf("Can't send client location to hospital A\n");
			return -1;
		}
		printf("The Scheduler has sent client location to Hospital A using UDP over port %d\n",ntohs(server_address.sin_port));		
	}
	//Hospital B
	if(availability_B>0){
		// Clean buffers:
		memset(client_message, '\0', sizeof(client_message));

		string clientLocationString = string("Client:") + to_string(clientLocation);
		// string clientLocationString = "Client:";
		strcpy(client_message, clientLocationString.c_str());

		if(sendto(server_socket,client_message,strlen(client_message),0,
			(struct sockaddr*)&client_address2, client_address_length2) < 0){
			printf("Can't send client location to hospital B\n");
			return -1;
		}
		printf("The Scheduler has sent client location to Hospital B using UDP over port %d\n",ntohs(server_address.sin_port));		
	}
	//Hospital C
	if(availability_C>0){
		//Clean buffers:
		memset(client_message, '\0', sizeof(client_message));

		string clientLocationString = string("Client:") + to_string(clientLocation);
		// string clientLocationString = "Client:";
		strcpy(client_message, clientLocationString.c_str());

		if(sendto(server_socket,client_message,strlen(client_message),0,
			(struct sockaddr*)&client_address3, client_address_length3) < 0){
			printf("Can't send client location to hospital C\n");
			return -1;
		}
		printf("The Scheduler has sent client location to Hospital C using UDP over port %d\n",ntohs(server_address.sin_port));	
	}
	// Receive the client's response (Hospital):
	double score_A = -1.0;
	double distance_A = double(INT_MAX);
	double score_B = -1.0;
	double distance_B = double(INT_MAX);
	double score_C = -1.0;
	double distance_C = double(INT_MAX);
	bool invalid = false;	

	//Hospital A
	if(availability_A>0){
		strcpy(client_message,"");
		memset(client_message, '\0', sizeof(client_message));
		if(recvfrom(server_socket, client_message, sizeof(client_message), 0,
		   (struct sockaddr*)&client_address1, &client_address_length1) < 0){
		  printf("Error while receiving Hospital A's message\n");
		  return -1;
		}
		//Check if invalid client location
		int invalidIndex = string(client_message).find("Invalid");
		if(invalidIndex!=-1){
			//Invalid is found - Print error message
			printf("The Scheduler has received map information from Hospital A, the score=None and the distance=None\n");
			invalid = true;
		}
		else{
			// printf("%s\n",client_message);
			string score = ""; //reset
			string distance = ""; //reset
			//Update score
			int scoreIndex = string(client_message).find(":") + 1;
			while(client_message[scoreIndex]!=','){
				score += client_message[scoreIndex];
				scoreIndex += 1;
			}
			score_A = strtod(score.c_str(),NULL);
			//Update distance
			int distanceIndex = string(client_message).rfind(":") + 1;
			distance = string(client_message).substr(distanceIndex);
			distance_A = strtod(distance.c_str(),NULL);
			printf("The Scheduler has received map information from Hospital A, the score=%f and the distance=%f\n",score_A,distance_A);
		}
		
	}
	//Hospital B
	if(availability_B>0){
		strcpy(client_message,"");
		memset(client_message, '\0', sizeof(client_message));
		if(recvfrom(server_socket, client_message, sizeof(client_message), 0,
		   (struct sockaddr*)&client_address2, &client_address_length2) < 0){
		  printf("Error while receiving Hospital B's message\n");
		  return -1;
		}
		//Check if invalid client location
		int invalidIndex = string(client_message).find("Invalid");
		if(invalidIndex!=-1){
			//Invalid is found - Print error message
			printf("The Scheduler has received map information from Hospital B, the score=None and the distance=None\n");
			invalid = true;
		}
		else{
			// printf("%s\n",client_message);
			string score = ""; //reset
			string distance = ""; //reset
			//Update score
			int scoreIndex = string(client_message).find(":") + 1;
			while(client_message[scoreIndex]!=','){
				score += client_message[scoreIndex];
				scoreIndex += 1;
			}
			score_B = strtod(score.c_str(),NULL);
			//Update distance
			int distanceIndex = string(client_message).rfind(":") + 1;
			distance = string(client_message).substr(distanceIndex);
			distance_B = strtod(distance.c_str(),NULL);	
			printf("The Scheduler has received map information from Hospital B, the score=%f and the distance=%f\n",score_B,distance_B);
		}
		
	}
	//Hospital C
	if(availability_C>0){
		strcpy(client_message,"");
		memset(client_message, '\0', sizeof(client_message));
		if(recvfrom(server_socket, client_message, sizeof(client_message), 0,
		   (struct sockaddr*)&client_address3, &client_address_length3) < 0){
		  printf("Error while receiving Hospital C's message\n");
		  return -1;
		}
		//Check if invalid client location
		int invalidIndex = string(client_message).find("Invalid");
		if(invalidIndex!=-1){
			//Invalid is found - Print error message
			printf("The Scheduler has received map information from Hospital C, the score=None and the distance=None\n");
			invalid = true;
		}
		else{
			// printf("%s\n",client_message);
			string score = ""; //reset
			string distance = ""; //reset
			//Update score
			int scoreIndex = string(client_message).find(":") + 1;
			while(client_message[scoreIndex]!=','){
				score += client_message[scoreIndex];
				scoreIndex += 1;
			}
			score_C = strtod(score.c_str(),NULL);
			//Update distance
			int distanceIndex = string(client_message).rfind(":") + 1;
			distance = string(client_message).substr(distanceIndex);
			distance_C = strtod(distance.c_str(),NULL);	
			printf("The Scheduler has received map information from Hospital C, the score=%f and the distance=%f\n",score_C,distance_C);
		}
	}

	//Assignment of Hospital
	//Compare scores and distance
	string assignedHospital = "";

	//Check if all scores are none
	if(score_A==-1 && score_B==-1 && score_C==-1){
		assignedHospital = "None";
	}
	//Check if all scores are equal
	else if(score_A==score_B && score_B==score_C){
		if(distance_A<distance_B && distance_A<distance_C){
			assignedHospital = "A";
		}
		else if(distance_B<distance_A && distance_B<distance_C){
			assignedHospital = "B";
		}
		else{
			assignedHospital = "C";
		}
	}
	//Check if score A is highest
	else if(score_A>score_B && score_A>score_C){
		assignedHospital = "A";
	}
	else if(score_A==score_B || score_A==score_C){
		if(score_A==score_B){
			if(distance_A<distance_B){
				assignedHospital = "A";
			}
		}
		if(score_A==score_C){
			if(distance_A<distance_C){
				assignedHospital = "A";
			}
		}
	}
	//Check if score B is highest
	else if(score_B>score_A && score_B>score_C){
		assignedHospital = "B";
	}
	else if(score_B==score_A || score_B==score_C){
		if(score_B==score_A){
			if(distance_B<distance_A){
				assignedHospital = "B";
			}
		}
		if(score_B==score_C){
			if(distance_B<distance_C){
				assignedHospital = "B";
			}
		}
	}
	//Check if score C is highest
	else if(score_C>score_B && score_C>score_A){
		assignedHospital = "C";
	}
	//else if(score_C==score_B || score_C==score_A){
	else{
		if(score_C==score_B){
			if(distance_C<distance_B){
				assignedHospital = "C";
			}
		}
		if(score_C==score_A){
			if(distance_C<distance_A){
				assignedHospital = "C";
			}
		}
	}

	//Print message
	printf("The Scheduler has assigned Hospital %s to the client\n",assignedHospital.c_str());

	//Send result to client
	//Check if assigned hospital is none. If so, send additional error message
	if(assignedHospital=="None"){
		if(invalid){
			//Invalid client location
			// cout << "Invalid client!" << endl;
			char server_message3[2000];
			memset(server_message3, '\0', sizeof(server_message3));
			string error_message = "None\nLocation " + to_string(clientLocation) + " not found";
			strcpy(server_message3,error_message.c_str());
			if(send(client_socket, server_message3, strlen(server_message3), 0) < 0){
				printf("Can't send message to client!\n");
				return -1;
			}
		}
		else{
			//None assignment
			// cout << "None assignment" << endl;
			char server_message3[2000];
			memset(server_message3, '\0', sizeof(server_message3));
			string error_message = "\nScore=None,No assignment";
			strcpy(server_message3,error_message.c_str());
			if(send(client_socket, server_message3, strlen(server_message3), 0) < 0){
				printf("Can't send message to client!\n");
				return -1;
			}
		}

	}
	else{
		//Send assigned result to client
		//Respond to client
		char server_message2[2000];
		memset(server_message2, '\0', sizeof(server_message2));

		strcpy(server_message2, assignedHospital.c_str());
		//Send assignment. Assignment can be none
		if(send(client_socket, server_message2, strlen(server_message2), 0) < 0){
			printf("Can't send message to client!\n");
			return -1;
		}
	}
	printf("The Scheduler has sent the result to client using TCP over port %d\n",ntohs(server_addressess.sin_port));
	// if(strcmp(server_message2,"None")==0){
	// 	char server_message3[2000];
	// 	memset(server_message3, '\0', sizeof(server_message3));

	// 	if(invalid==true){
	// 		string temp_message = string("Location ") + to_string(clientLocation) + " not found";
	// 		cout << temp_message << endl;
	// 		strcpy(server_message3,temp_message.c_str());
	// 	}
	// 	else{
	// 		strcpy(server_message3,"Score=None,No assignment\n");
	// 	}
	// 	cout << "server Message3:" << server_message3 << endl;
	// 	if(send(client_socket, server_message3, strlen(server_message3), 0) < 0){
	// 		printf("Can't send message to client!\n");
	// 		return -1;
	// 	}

	// 	return -1;
	// }

	//Phase 4 - reply to hospital and update availability and score
	if(assignedHospital=="A"){
		initialOccupancy_A += 1; //Update initalOccupancy of hospital in scheduler
		// Clean buffers:
		memset(client_message, '\0', sizeof(client_message));

		string clientLocationString = string("Assigned");
		// string clientLocationString = "Client:";
		strcpy(client_message, clientLocationString.c_str());
		// client_address_length = sizeof(client_message);

		if(sendto(server_socket,client_message,strlen(client_message),0,
			(struct sockaddr*)&client_address1, client_address_length1) < 0){
			printf("Can't send assignment to hospital A\n");
			return -1;
		}
		// printf("The Scheduler has sent the result to Hospital A using UDP over port %d\n",ntohs(client_address1.sin_port));
		printf("The Scheduler has sent the result to Hospital A using UDP over port %d\n",ntohs(server_address.sin_port));
		
	}
	else if(assignedHospital=="B"){
		initialOccupancy_B += 1; //Update initalOccupancy of hospital in scheduler
		// Clean buffers:
		memset(client_message, '\0', sizeof(client_message));

		string clientLocationString = string("Assigned");
		// string clientLocationString = "Client:";
		strcpy(client_message, clientLocationString.c_str());
		// client_address_length = sizeof(client_message);

		if(sendto(server_socket,client_message,strlen(client_message),0,
			(struct sockaddr*)&client_address2, client_address_length2) < 0){
			printf("Can't send assignment to hospital B\n");
			return -1;
		}
		// printf("The Scheduler has sent the result to Hospital B using UDP over port %d\n",ntohs(client_address2.sin_port));
		printf("The Scheduler has sent the result to Hospital B using UDP over port %d\n",ntohs(server_address.sin_port));
		
	}
	else if(assignedHospital=="C"){
		//AssignedHospital == "C"
		initialOccupancy_C += 1; //Update initalOccupancy of hospital in scheduler
		// Clean buffers:
		memset(client_message, '\0', sizeof(client_message));

		string clientLocationString = string("Assigned");
		// string clientLocationString = "Client:";
		strcpy(client_message, clientLocationString.c_str());
		// client_address_length = sizeof(client_message);

		if(sendto(server_socket,client_message,strlen(client_message),0,
			(struct sockaddr*)&client_address3, client_address_length3) < 0){
			printf("Can't send assignment to hospital C\n");
			return -1;
		}
		// printf("The Scheduler has sent the result to Hospital C using UDP over port %d\n",ntohs(client_address3.sin_port));
		printf("The Scheduler has sent the result to Hospital C using UDP over port %d\n",ntohs(server_address.sin_port));
	}
	else{
		//AssignedHospital == "None"
		//Do nothing
	}

	//Close the socket with hospital
	close(server_socket);

	//Closing the socket
	close(client_socket);
	close(parent_socket);

	return 0;
}