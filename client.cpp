#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

// extern "C"{
// 	#include "scheduler_helper.h"
// }

using namespace std;

int main(int argc, char* argv[]){

	//Create client message from commandline
	string client_location = "";
	if(argv[1]!=NULL){
		client_location = argv[1];
	}
	else{
		printf("Invalid command line parameters for client\n");
		return -1;
	}	


	int client_socket;
	struct sockaddr_in server_address;
	char server_message[2000], client_message[2000];

	//Clean buffers
	memset(server_message, '\0', sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));

	//Create socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(client_socket < 0){
		printf("Unable to create client socket\n");
		return -1;
	}

	// printf("Socket created successfully\n");
	printf("The client is up and running\n");

	//Set port and IP the same as server-side (scheduler)
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(34476);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	//Send connection request to server
	if(connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
		printf("Unable to connect with scheduler\n");
		return -1;
	}
	// printf("Connected with scheduler successfully\n");

	// Get input from user
	// printf("Enter message: ");
	// cin >> client_message;

	
	string client_message_string = string("Client:") + client_location;
	//Convert string to c string
	strcpy(client_message,client_message_string.c_str());

	//Send the message to scheduler (server)
	if(send(client_socket, client_message, strlen(client_message), 0) < 0){
		printf("Unable to send query to Scheduler\n");
		return -1;
	}

	printf("The client has sent query to Scheduler using TCP: client location %s\n",client_location.c_str());

	//Receive the server's response
	strcpy(server_message,"");
	memset(server_message, '\0', sizeof(server_message)); //Clear buffer
	if(recv(client_socket, server_message, sizeof(server_message), 0) < 0){
		printf("Error while receiving server's message\n");
		return -1;
	}
	// cout << "server_message:" << server_message << endl;
	printf("The client has received results from the Scheduler: assigned to Hospital %s\n",server_message);
	// cout << "WOW" << endl;
	// cout << server_message << endl;
	// printf("Server's response: %s\n", server_message);

	// cout << "hello" << endl;

	//Output errors
	// if(strcmp("server_message","None")==0){
	// 	//Output error message
	// 	strcpy(server_message,"");
	// 	memset(server_message, '\0', sizeof(server_message)); //Clear buffer
	// 	if(recv(client_socket, server_message, sizeof(server_message), 0) < 0){
	// 		printf("Error while receiving server's message\n");
	// 		return -1;
	// 	}
	// 	cout << "Hello123" << endl;
	// 	printf("this is a test %s\n",server_message);					
	// }

	//Close the socket
	close(client_socket);

}