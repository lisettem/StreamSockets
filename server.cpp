#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <fstream>
#include<iostream>
#include<cstring>
using namespace std;

void error(char *msg){
	perror(msg);
	exit(1);
}

struct fileInfo{
string majorName;
string avgCareerPay;
string midCareerPay;
};

int main()
{
string fileName;

cout<<"Enter the name of the file: ";
cin>> fileName;

fileInfo getInfo; //instance of the struct we will use to store the info in the file into the correct variable
vector<fileInfo> dataTable;
string lineInFile; //will store a line in the file

ifstream myfile(fileName.c_str());

if (myfile.is_open()){

	while (getline(myfile,lineInFile)){
	string emptyString=""; //temp string used to assign info to the designated variable

	for (int i=0; i< lineInFile.length(); i++){

		if(lineInFile[i] == '\t' && isalpha(lineInFile[i-1]) || lineInFile[i] == '\t' && lineInFile[i-1]== ')')
		getInfo.majorName = lineInFile.substr(0,i);

			if (!isalpha(lineInFile[i])) { //if its not a letter it will go in here

			if (lineInFile[i]=='\t'){
			getInfo.avgCareerPay = emptyString;
			emptyString.clear();
			}

			else if(i == lineInFile.length() -1 ){ //if its the last character
			getInfo.midCareerPay = emptyString;
			emptyString.clear();
			}

			else
			emptyString += lineInFile[i]; //else its a letter so add it to the string
		}
	}
		dataTable.push_back(getInfo);
}
}

else
cout<<"File did not open"<<endl;

	int sockfd, newSockFd, portNumber;
	socklen_t clientLength;

	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int storeSocket;

	cout<<"Enter server port number:";
	cin>> portNumber;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
		error((char*)"ERROR opening socket");

	memset((char *)&serv_addr,0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portNumber);

	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error((char*)"ERROR on binding");

	listen(sockfd, 5);
	cout<<"Waiting for client"<<endl;

	clientLength= sizeof(cli_addr);

while(true){ //put accept in a loop to accept multiple requests from the client

	newSockFd = accept(sockfd, (struct sockaddr *) &cli_addr, &clientLength);
	if (newSockFd < 0)
		error((char*)"ERROR on accept");
	memset(buffer,0, 256);

	storeSocket = read(newSockFd, buffer, 255);

	if (storeSocket < 0)
	 error((char*)"ERROR reading from socket");

	string bufferToString="";
	bufferToString=buffer;
	char avgpy[256];
	char midpy[256];
	string avgToString="";
	string midToString="";

	for (int i=0; i< dataTable.size(); i++){
	if (bufferToString.compare(dataTable[i].majorName) ==0){
		avgToString= "The average early career pay for " + dataTable[i].majorName + " is $" + dataTable[i].avgCareerPay +"\n";
		midToString= "The corresponding mid-career pay is $" + dataTable[i].midCareerPay +"\n";
}
}
	if(avgToString== "" && midToString == ""){ //if the major is not in the table
	avgToString="-1";
	midToString="-1";
	}

strcpy(avgpy, avgToString.c_str()); //convert them to c strings and copy them to the appropriate array
strcpy(midpy, midToString.c_str());
strncat(avgpy, midpy, 256); //copy everything into avgpy and that is what we will write

storeSocket = write(newSockFd, avgpy, 256);

if (storeSocket < 0) error((char*)"ERROR writing to socket");

close(newSockFd);
}

return 0;
}
