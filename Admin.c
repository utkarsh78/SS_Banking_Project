// File for Adding Admin Users in the Database.

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

typedef struct admin{
	int userID;
	char username[30];
	char password[8];
}admin;

int AdminUserID();

int AdminUserID(){
	int fd=open("Adminfile",O_RDONLY,0744);
	admin record;
	lseek(fd,-sizeof(admin),SEEK_END);
	read(fd,&record,sizeof(admin));
	close(fd);
	return record.userID+1;
}

int main(){
	int fd=open("Adminfile",O_RDWR | O_CREAT,0744);
	int choice=0;
	admin addAdmin;
	printf("Username of admin: ");
	scanf(" %s",addAdmin.username);
	printf("Enter password (Upto 8 chars Allowed): ");
	scanf(" %s",addAdmin.password);
	addAdmin.userID=100;
	printf("Your userID is : %d\n",addAdmin.userID);
	write(fd,&addAdmin,sizeof(addAdmin));
	printf("Add Another Admin?, Press 1 for Yes & 0 for No ");
	scanf("%d",&choice);
	while(choice){
		printf("Please enter the name of the admin: ");
		scanf(" %[^\n]",addAdmin.username);
		printf("Enter password (Upto 8 chars Allowed): ");
		scanf(" %[^\n]",addAdmin.password);
		addAdmin.userID=AdminUserID();
		printf("Your userID is : %d\n",addAdmin.userID);
		write(fd,&addAdmin,sizeof(admin));
		printf("Add Another Admin?, Press 1 for Yes & 0 for No ");
		scanf("%d",&choice);
	}
	close(fd);
	return 0;
}