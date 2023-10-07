// File for Adding 1 Normal User and 1 Joint User to Database

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

typedef struct normalUser{
	int userID;
	char name[30];
	char password[8];
	int account_no;
	float balance;
	char status[20];
}normalUser;

typedef struct jointUser{
	int userID;
	char name1[30];
	char name2[30];
	char password[8];
	int account_no;
	float balance;
	char status[20];
}jointUser;

int NormalUserID();
int JointUserID();

int NormalUserID(){
	int fd=open("NormalUserfile",O_RDONLY,0744);
	normalUser record;
	lseek(fd,-sizeof(normalUser),SEEK_END);
	read(fd,&record,sizeof(normalUser));
	close(fd);
	return record.userID+1;
}

int JointUserID(){
	int fd=open("JointUserfile",O_RDONLY,0744);
	jointUser record;
	lseek(fd,-sizeof(jointUser),SEEK_END);
	read(fd,&record,sizeof(jointUser));
	close(fd);
	return record.userID+1;
}

int main(){
    printf("Enter Atleast 1 Normal User \n");
	int fd=open("NormalUserfile",O_RDWR | O_CREAT,0744);
	int choice=1;
	normalUser nuser;
	printf("Please enter the name of the normal user: ");
	scanf(" %[^\n]",nuser.name);
	printf("Enter password (Upto 8 chars Allowed): ");
	scanf(" %[^\n]",nuser.password);
	nuser.userID=1000;
	nuser.balance=0;
	nuser.account_no=(nuser.userID-1000)+10000;
	printf("Your userID is : %d\n",nuser.userID);
	strcpy(nuser.status,"ACTIVE");
	write(fd,&nuser,sizeof(normalUser));
	printf("Add Another Normal User?, Press 1 for Yes & 0 for No ");
	scanf("%d",&choice);
	while(choice){
		printf("Please enter the name of the normal user: ");
		scanf(" %[^\n]",nuser.name);
		printf("Enter password (Upto 8 chars Allowed): ");
		scanf(" %[^\n]",nuser.password);
		nuser.userID=NormalUserID();
		nuser.balance=0;
		nuser.account_no=(nuser.userID-1000)+10000;
		printf("Your userID is : %d\n",nuser.userID);
		strcpy(nuser.status,"ACTIVE");
		write(fd,&nuser,sizeof(normalUser));
		printf("Add Another Normal User?, Press 1 for Yes & 0 for No ");
		scanf("%d",&choice);
	}
	close(fd);

	printf("Enter Atleast 1 Joint Account User \n");
	fd=open("JointUserfile",O_RDWR | O_CREAT,0744);
	choice=1;
	jointUser juser;
	printf("Enter First Name of the Joint Account: ");
	scanf(" %[^\n]",juser.name1);
	printf("Enter Second Name of the Joint Account: ");
	scanf(" %[^\n]",juser.name2);
	printf("Enter password (Upto 8 chars Allowed): ");
	scanf(" %[^\n]",juser.password);
	juser.userID=2000;
	juser.balance=0;
	juser.account_no=(juser.userID-2000)+20000;
	printf("Your userID is : %d\n",juser.userID);
	strcpy(juser.status,"ACTIVE");
	write(fd,&juser,sizeof(juser));
	printf("Add Another Joint Account User?, Press 1 for Yes & 0 for No ");
	scanf("%d",&choice);
	while(choice){
		printf("Enter First Name of the Joint Account: ");
		scanf(" %[^\n]",juser.name1);
		printf("Enter Second Name of the Joint Account: ");
		scanf(" %[^\n]",juser.name2);
		printf("Enter password (Upto 8 chars Allowed): ");
		scanf(" %[^\n]",juser.password);
		juser.userID=JointUserID();
		juser.balance=0;
		juser.account_no=(juser.userID-2000)+20000;
		printf("Your userID is : %d\n",juser.userID);
		strcpy(juser.status,"ACTIVE");
		write(fd,&juser,sizeof(jointUser));
		printf("Add Another Joint Account User?, Press 1 for Yes & 0 for No ");
		scanf("%d",&choice);
	}
	close(fd);
	return 0;
}