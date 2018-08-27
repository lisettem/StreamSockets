#include<iostream>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include <netdb.h>
#include<cstring>
using namespace std;

void error(char *message)
{
	perror(message);
	exit(1);
}

int main()
{
char * hostName = new char[100];
int portNumber;
int sockfd;
int storeSocket;
struct sockaddr_in server_address;
struct hostent *server;
char buffer[256];
string buffdummy;

cout<<"Enter host name:";
cin>> hostName;

cout<<"Enter port number:";
cin>> portNumber;
cin.ignore();

while(true){

sockfd =socket(AF_INET, SOCK_STREAM, 0);
if (sockfd <0)
error((char*)"Error opening socket");

server = gethostbyname(hostName);
if(server == NULL){
cout<< "No such host"<<endl;
exit(0);
}

memset((char*)&server_address, 0, sizeof(server_address));
server_address.sin_family= AF_INET;
bcopy((char*)server->h_addr, (char*)&server_address.sin_addr.s_addr, server->h_length);
server_address.sin_port= htons(portNumber);

if(connect(sockfd,(struct sockaddr*) &server_address, sizeof(server_address)) <0)
error((char*)"Error connecting");

cout<<"Enter college major:";
getline(cin, buffdummy);

if (buffdummy == " "){ //this ends the loop if its an empty string
write(sockfd, buffer, strlen(buffer));
close(sockfd);
exit(1);
}

strcpy(buffer, buffdummy.c_str());

storeSocket= write(sockfd, buffer, strlen(buffer));

if(storeSocket <0)
	error((char*)"Error writting to socket");

memset(buffer, 0, 256);

storeSocket=read(sockfd, buffer, 255);

if(storeSocket<0)
error((char*)"Error reading from socket");

if(buffer[0] == '-') //if the buffer starts with - then we know its the -1-1 string so the major is not in the table
cout<<"The major is not in the table"<<endl;
else
cout <<buffer;
}
return 0;
}

