// Including Necesarry Headerfiles

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>

typedef struct admin{				// Structure of Administartor
	int userID;
	char username[30];
	char password[8];
}admin;

typedef struct normalUser{			// Structure of Normal USer
	int userID;
	char name[30];
	char password[8];
	int account_no;
	float balance;
	char status[20];
}normalUser;

typedef struct jointUser{			// Structure of Joint User
	int userID;
	char name1[30];
	char name2[30];
	char password[8];
	int account_no;
	float balance;
	char status[20];
}jointUser;

void LaunchMenu(int sd);
void AdminLogin(int sd);
void NormalUserLogin(int sd);
void JointUserLogin(int sd);
void deposit(int sd);
void withdraw(int sd);
void balanceEnquiry(int sd);
void passwordChange(int sd);
void viewDetails(int sd);
void addAccount(int sd);
void deleteAccount(int sd);
void modifyAccount(int sd);
void searchAccount(int sd);
int category,presentUserID;

void inputchoice(int sd){
		printf("1)Normal User Login\n2)Joint Account User Login\n3)Administrator Login\n4)Exit\n");
		
		printf("Enter Category Index you want to Proceed with ");
		scanf("%d",&category);
		switch(category)
		{
		case 1: NormalUserLogin(sd); 		// Redirect to Normal USer Login
			break;

		case 2: JointUserLogin(sd);			// Redirect to Joint User Login
			break;
		
		case 3: AdminLogin(sd);				// Redirect to Adminstrator Login
			break;
		case 4: exit(0);
				break;
		}
}

void LaunchMenu(int sd){
	int choice;
	if(category ==1 || category==2){				// Default Menu for Normal User & Joint Account User
		printf("1)Deposit\n2)Withdraw\n3)Balance Enquiry\n4)Password Change\n5)View Details\n6)Exit\n");
	
		printf("Please Enter your choice ");
		scanf("%d",&choice);
		
		switch(choice){
		case 1 :
			deposit(sd);
			break;
		case 2 :
			withdraw(sd);
			break;
		case 3 :
			balanceEnquiry(sd);
			break;
		case 4 :
			passwordChange(sd);
			break;
		case 5 :
			viewDetails(sd);
			break;
		case 6 :
			write(sd,&choice,sizeof(int));
			printf("Have a Good Day\n");
			exit(0);
		default :
			printf("Invalid Input!!\n\n");
			LaunchMenu(sd);
			break;
		}
	}
	else if(category==3){				// Default Menu for Administrator User

		printf("1)Add Account\n2)Delete  Account\n3)Modify Account\n4)Search Account\n5)Exit\n");
		
		printf("Please Enter your choice ");
		scanf("%d",&choice);
		
		switch(choice){
		case 1 :
			addAccount(sd);
			break;
		case 2 :
			deleteAccount(sd);
			break;
		case 3 :
			modifyAccount(sd);
			break;
		case 4 :
			searchAccount(sd);
			break;
		case 5 :
			write(sd,&choice,sizeof(int));
			printf("Have a Good Day\n");
			exit(0);
		default :
			printf("Invalid Input \n");
			LaunchMenu(sd);
			break;
		}
	}
}

void AdminLogin(int sd){
	bool result;
	admin presentUser;
	printf("User ID : ");
	scanf("%d",&presentUser.userID);
	presentUserID=presentUser.userID;
	printf("Password : ");
	scanf("%s",presentUser.password);

	write(sd,&category,sizeof(int));					// Connect to the Server
	write(sd,&presentUser,sizeof(admin));

	read(sd,&result,sizeof(result)); 					// Reply from Server

	if(!result){
		printf("Invalid login!!\n");
		inputchoice(sd);
	}
	else{
		printf("Login Successfull\n\n");
	}
	return;
}

void NormalUserLogin(int sd){					// Login for Normal User
	bool result;
	normalUser presentUser;
	printf("User ID : ");
	scanf("%d",&presentUser.userID);
	presentUserID=presentUser.userID;
	printf("Password : ");
	scanf("%s",presentUser.password);

	write(sd,&category,sizeof(int));			// Connect to Server
	write(sd,&presentUser,sizeof(normalUser));

	read(sd,&result,sizeof(result)); 			//Reply from the server

	if(!result){
		printf("Invalid login!!\n");
		inputchoice(sd);
	}
	else{
		printf("Login Successfull\n\n");
	}
	return;
}

void JointUserLogin(int sd){
	bool result;
	jointUser presentUser;
	printf("User ID : ");
	scanf("%d",&presentUser.userID);
	presentUserID=presentUser.userID;
	printf("Password : ");
	scanf("%s",presentUser.password);

	write(sd,&category,sizeof(int));				// Connect to Server
	write(sd,&presentUser,sizeof(jointUser));

	read(sd,&result,sizeof(result)); 				//Reply from the server

	if(!result){
		printf("Invalid login!!\n");
		inputchoice(sd);
	}
	else{
		printf("Login Successfull\n\n");
	}
	return;
}

void deposit(int sd){
	float amt;
	int choice=1;
	bool result;

	printf("Amount to Deposit : Rs ");
	scanf("%f",&amt);

	while(amt<=0){
		printf("Please Enter a valid amount!!\n");
		printf("Rs ");
		scanf("%f",&amt);
	}

	write(sd,&choice,sizeof(int));			// Connect to Server
	write(sd,&amt,sizeof(float));

	read(sd,&result,sizeof(result)); 		// Reply from the server

	if(!result){
		printf("Something Went Wrong, Money not Deposited \n");
	}
	else{
		printf("Amount Credited Successfully\n");
	}
	LaunchMenu(sd);
	return;
}

void withdraw(int sd){
	float amt;
	int choice=2;
	bool result;

	printf("Amount to withdraw ");
	scanf("%f",&amt);

	while(amt<=0){
		printf("Please Enter a valid amount!!\n");
		printf("Rs ");
		scanf("%f",&amt);
	}

	write(sd,&choice,sizeof(int));			// Connect to Server
	write(sd,&amt,sizeof(float));

	read(sd,&result,sizeof(result)); 		// Reply from the server

	if(!result){
		printf("Something Went Wrong, Money not withdrawn \n");
	}
	else{
		printf("Amount Debited Successfully \n");
	}
	LaunchMenu(sd);
	return;
}

void balanceEnquiry(int sd){
	float amt;
	int choice=3;
	int len;

	write(sd,&choice,sizeof(int));			// Connect to Server
	
	len=read(sd,&amt,sizeof(float));		// Reply from the server

	printf("Available Balance : Rs ");
	printf("%0.2f\n\n",amt);

	LaunchMenu(sd);
	return;
}

void passwordChange(int sd){
	int choice=4;
	char newpass[8];
	bool result;
	
	printf("Enter your New Banking Password\n");
	scanf("%s",newpass);

	write(sd,&choice,sizeof(int));				// Connect to Server
	write(sd,newpass,sizeof(newpass));

	read(sd,&result,sizeof(result)); 			// Reply from the server

	if(!result){
		printf("Error while changing your Password\n");
	}
	else{
		printf("Password updated Successfully\n");
	}
	LaunchMenu(sd);
	return;
}

void viewDetails(int sd){
	int choice=5;

	write(sd,&choice,sizeof(int));			// Connect to Server

	if(category==1){						// Normal User Details
		normalUser presentUser1;
		read(sd,&presentUser1,sizeof(normalUser));
		
		printf("User ID : %d\n",presentUser1.userID);
		printf("Account Holder's Name : %s\n",presentUser1.name);
		printf("Account Number : %d\n",presentUser1.account_no);
		printf("Available Balance : Rs.%0.2f\n",presentUser1.balance);
		printf("Status : %s\n\n",presentUser1.status);
	}
	else if(category==2){					// Joint User Details
		jointUser presentUser2;
		read(sd,&presentUser2,sizeof(jointUser));
		
		printf("User ID : %d\n",presentUser2.userID);
		printf("First Account Holder's Name : %s\n",presentUser2.name1);
		printf("Second Account Holder's Name : %s\n",presentUser2.name2);
		printf("Account Number : %d\n",presentUser2.account_no);
		printf("Available Balance : Rs.%0.2f\n",presentUser2.balance);
		printf("Status : %s\n\n",presentUser2.status);
	}
	LaunchMenu(sd);
	return;
}

void addAccount(int sd){
	int choice=1;
	int type;
	bool result;

	write(sd,&choice,sizeof(int));
	printf("Enter the type account\n 1} Normal Account\n 2} Joint Account\n:");
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){					// Add Normal User 
		normalUser addUser1;
		printf("Name of the account holder : ");
		scanf(" %[^\n]",addUser1.name);
		printf("Password (Upto 8 chars Allowed)");
		scanf("%s",addUser1.password);
		printf("Initial Deposit ");
		scanf("%f",&addUser1.balance);
		write(sd,&addUser1,sizeof(normalUser));
	}

	if(type==2){					// Add Joint Account User
		jointUser addUser2;
		printf("Primary Account Holder's Name : ");
		scanf(" %[^\n]",addUser2.name1);
		printf("Secondary Account Holde's Name : ");
		scanf(" %[^\n]",addUser2.name2);
		printf("Password (Upto 8 chars Allowed)");
		scanf("%s",addUser2.password);
		printf("Initial Deposit ");
		scanf("%f",&addUser2.balance);
		write(sd,&addUser2,sizeof(jointUser));
	}
	
	read(sd,&result,sizeof(result)); // Acknowledgement from the server

	if(!result){
		printf("Error while Processing the request for Adding Account!!\n\n");
	}
	else{
		printf("Account Added Successfully \n\n");
	}
	LaunchMenu(sd);
	return;
}

void deleteAccount(int sd){
	int choice=2;
	int type,userID;
	bool result;

	write(sd,&choice,sizeof(int));

	printf("Enter Account type\n 1} Normal Account 2} Joint Account)");
	scanf("%d",&type);
	
	write(sd,&type,sizeof(int));

	printf("User ID : ");
	scanf("%d",&userID);
	write(sd,&userID,sizeof(int));
	
	read(sd,&result,sizeof(result)); 			//Acknowledgement from the server

	if(!result){
		printf("Error while Deleting the Account!!\n\n");
	}
	else{
		printf("Account Deleted Successfully \n\n");
	}
	LaunchMenu(sd);
	return;
}

void modifyAccount(int sd){
	int choice=3;
	int type;
	bool result;

	write(sd,&choice,sizeof(int));

	printf("Enter Account type\n 1} Normal Account 2} Joint Account)");
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){					// Modify Normal Account User
		normalUser modUser1;
		printf("User ID ");
		scanf("%d",&modUser1.userID);
		printf("Account Number : ");
		scanf("%d",&modUser1.account_no);
		printf("Update Account Holders Name ");
		scanf(" %[^\n]",modUser1.name);
		printf("Update Password(Upto 8 chars Allowed)");
		scanf("%s",modUser1.password);
		printf("Updated Balance ");
		scanf("%f",&modUser1.balance);
		write(sd,&modUser1,sizeof(normalUser));
	}

	if(type==2){					// Modify Joint Account User
		jointUser modUser2;
		printf("User ID ");
		scanf("%d",&modUser2.userID);
		printf("Account Number : ");
		scanf("%d",&modUser2.account_no);
		printf("Updated Name of Primary Account Holder ");
		scanf(" %[^\n]",modUser2.name1);
		printf("Updated Name of Secondary Account Holder ");
		scanf(" %[^\n]",modUser2.name2);
		printf("Update Password(Upto 8 chars Allowed)");
		scanf("%s",modUser2.password);
		printf("Updated Balance ");;
		scanf("%f",&modUser2.balance);
		write(sd,&modUser2,sizeof(jointUser));
	}
	
	read(sd,&result,sizeof(result)); //from the server

	if(!result){
		printf("Error while Modifying the Account!!\n\n");
	}
	else{
		printf("Account Modified Successfully \n");
	}
	LaunchMenu(sd);
	return;
}

void searchAccount(int sd){
	int choice=4;
	int type,len;
	bool result;

	write(sd,&choice,sizeof(int));

	printf("Enter Account type\n 1} Normal Account 2} Joint Account)");
	scanf("%d",&type);

	write(sd,&type,sizeof(int));

	if(type==1){						// Search for Normal User
		normalUser searchUser1;
		int userID;
		printf("User ID ");
		scanf("%d",&userID);
		write(sd,&userID,sizeof(int));
		
		len=read(sd,&searchUser1,sizeof(normalUser));
		if(len==0){
			printf("Invalid User ID \n\n");
		}
		else{
			printf("User ID : %d\n",searchUser1.userID);
			printf("Name : %s\n",searchUser1.name);
			printf("Account Number : %d\n",searchUser1.account_no);
			printf("Available Balance : Rs.%0.2f\n",searchUser1.balance);
			printf("Status : %s\n\n",searchUser1.status);
		}
	}

	if(type==2){
		jointUser searchUser2;					// Search for Joint Account User
		int userID1;
		printf("User ID ");
		scanf("%d",&userID1);
		write(sd,&userID1,sizeof(int));
		
		len=read(sd,&searchUser2,sizeof(jointUser));
		if(len==0){
			printf("Invalid User ID \n\n");
		}
		else{
			printf("User ID : %d\n",searchUser2.userID);
			printf("Main Account Holder's Name : %s\n",searchUser2.name1);
			printf("Other Account Holder's Name : %s\n",searchUser2.name2);
			printf("Account Number : %d\n",searchUser2.account_no);
			printf("Available Balance : Rs.%0.2f\n",searchUser2.balance);
			printf("Status : %s\n\n",searchUser2.status);
		}
	}
	LaunchMenu(sd);
	return;
}

int main(){
	struct sockaddr_in server;
	int sd,msgLength;
	char buff[50];
	char result;

	sd=socket(AF_INET,SOCK_STREAM,0);				// Connect to Server
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1"); //same machine
	server.sin_port=htons(7245);
	connect(sd,(struct sockaddr *)&server,sizeof(server));

	inputchoice(sd);
	LaunchMenu(sd);	

	close(sd);

	return 0;
}