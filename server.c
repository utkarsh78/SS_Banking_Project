// Server File

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <pthread.h>

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

admin getAdmin(int ID){
	int i=ID-100;
	admin presentCust;
	int fd=open("Adminfile",O_RDONLY,0744);
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	   
	lock.l_len=sizeof(admin);	           
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);				//locking the selected record

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  			//Bringing the cursor to the selected record
	read(fd,&presentCust,sizeof(admin));

	lock.l_type=F_UNLCK;					// Unnloking the Record
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return presentCust;
}

normalUser getNormalUser(int ID){
	int i=ID-1000;
	normalUser presentCust;
	int fd=open("NormalUserfile",O_RDONLY,0744);
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    	   
	lock.l_len=sizeof(normalUser);	            
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);				//locking the selected record

	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  	//Bringing the cursor to the selected record
	read(fd,&presentCust,sizeof(normalUser));

	lock.l_type=F_UNLCK;				// Unlocking the Record
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return presentCust;
}

jointUser getJointUser(int ID){
	int i=ID-2000;
	jointUser presentCust;
	int fd=open("JointUserfile",O_RDONLY,0744);
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     
	lock.l_len=sizeof(jointUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);				//lock the selected record

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  				//Bringing the cursor to the selected record
	read(fd,&presentCust,sizeof(jointUser));

	lock.l_type=F_UNLCK;				// Unlocking the Record
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return presentCust;
}

bool checkAdmin(admin presentCust){
	int i=presentCust.userID-100;
	int fd=open("Adminfile",O_RDONLY,0744);
	bool result;
	admin temp;
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     
	lock.l_len=sizeof(admin);	           
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);			//locking the selected record

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  		//Bringing the cursor to the selected record
	read(fd,&temp,sizeof(admin));
	if(!strcmp(temp.password,presentCust.password))	
		result=true;
	else
		result=false;

	lock.l_type=F_UNLCK;				// Unlock The Record
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkNormalUser(normalUser presentCust){
	int i=presentCust.userID-1000;
	int fd=open("NormalUserfile",O_RDONLY,0744);
	bool result;
	normalUser temp;
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    	   
	lock.l_len=sizeof(normalUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);					//locking the selected record

	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  		//Bringing the Cursor to the selected record
	read(fd,&temp,sizeof(normalUser));
	if(!strcmp(temp.password,presentCust.password) && !strcmp(temp.status,"ACTIVE"))	
		result=true;
	else
		result=false;

	lock.l_type=F_UNLCK;						// Unlocking the Record
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkJointUser(jointUser presentCust){
	int i=presentCust.userID-2000;
	int fd=open("JointUserfile",O_RDONLY,0744);
	bool result;
	jointUser temp;
	
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	    
	lock.l_len=sizeof(jointUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);				//locking the selected record

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  		//Bringing the cursor to the selected record
	read(fd,&temp,sizeof(jointUser));
	if(!strcmp(temp.password,presentCust.password) && !strcmp(temp.status,"ACTIVE"))	
		result=true;
	else
		result=false;

	lock.l_type=F_UNLCK;					// Unlock the Record
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool withdrawl(int AccountCategory,int ID,float amt){			// Withdraw Money from Account
	if(AccountCategory==1){
		int i=ID-1000;
		int fd=open("NormalUserfile",O_RDWR,0744);
		bool result;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);   
		lock.l_len=sizeof(normalUser);	           
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);			//locking the selected record

		normalUser presentCust;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);      //Bringing the cursor to the selected record
		read(fd,&presentCust,sizeof(normalUser));
		
		if(!strcmp(presentCust.status,"ACTIVE") && presentCust.balance>=amt){
			presentCust.balance-=amt;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&presentCust,sizeof(normalUser));
			result=true;
		}
		else	
			result=false;
		printf("Press Enter for removing Lock from Withdrawl\n");
		getchar();
		lock.l_type=F_UNLCK;				// Unlock the Reoord
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
	}
	else if(AccountCategory==2){
		int i=ID-2000;
		int fd=open("JointUserfile",O_RDWR,0744);
		bool result;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    
		lock.l_len=sizeof(jointUser);	        
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);				//locking the selected record
		

		jointUser presentCust;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  			//Bringing the cursor to the selected record
		read(fd,&presentCust,sizeof(jointUser));
		
		if(!strcmp(presentCust.status,"ACTIVE") && presentCust.balance>=amt){
			presentCust.balance-=amt;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&presentCust,sizeof(jointUser));
			result=true;
		}
		else	
			result=false;
		
		printf("Press Enter for removing Lock from Withdrawl\n");
		getchar();
		lock.l_type=F_UNLCK;				// Unlock The Record
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

bool depositin(int AccountCategory,int ID,float amt){			// Deposit Money to Account
	if(AccountCategory==1){
		int i=ID-1000;
		int fd=open("NormalUserfile",O_RDWR,0744);
		bool result;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    
		lock.l_len=sizeof(normalUser);	            
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);				//locking the selected record
		normalUser presentCust;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  		//Bringing the cursor to the selected record
		read(fd,&presentCust,sizeof(normalUser));
		
		if(!strcmp(presentCust.status,"ACTIVE")){
			presentCust.balance+=amt;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&presentCust,sizeof(normalUser));
			result=true;
		}
		else	
			result=false;

		printf("Press Enter for removing Lock from Deposit\n");
		getchar();
		lock.l_type=F_UNLCK;				// Unlock the Record
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;		
	}
	else if(AccountCategory==2){
		int i=ID-2000;
		int fd=open("JointUserfile",O_RDWR,0744);
		bool result;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);  
		lock.l_len=sizeof(jointUser);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);			//locking the selected record

		jointUser presentCust;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  	//Bringing the cursor to the selected record
		read(fd,&presentCust,sizeof(jointUser));
		
		if(!strcmp(presentCust.status,"ACTIVE")){
			presentCust.balance+=amt;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&presentCust,sizeof(jointUser));
			result=true;
		}
		else	
			result=false;
		printf("Press Enter for removing Lock from Deposit\n");
		getchar();
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
	}
	return false;
}

float getBalance(int AccountCategory,int ID){				// Fetch Balance of Account
	float result;
	if(AccountCategory==1){
		int i=ID-1000;
		int fd=open("NormalUserfile",O_RDONLY,0744);
		normalUser temp;
	
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    	 
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);					//locking the selected record

		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);   	//Bringing the cursor to the selected record
		read(fd,&temp,sizeof(normalUser));
		if(!strcmp(temp.status,"ACTIVE"))	result=temp.balance;
		else					
			result=0;

		lock.l_type=F_UNLCK;		// Unlock the Record
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(AccountCategory==2){
		int i=ID-2000;
		int fd=open("JointUserfile",O_RDONLY,0744);
		jointUser temp;
	
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    	     
		lock.l_len=sizeof(jointUser);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);					//locking the selected record

		lseek(fd,(i)*sizeof(jointUser),SEEK_SET); 				//Bringing the cursor to the selected record
		read(fd,&temp,sizeof(jointUser));
		if(!strcmp(temp.status,"ACTIVE"))	result=temp.balance;
		else					
			result=0;

		lock.l_type=F_UNLCK;					// Unlock the Record
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return 0;
}

bool Passwordchange(int AccountCategory,int ID,char newPwd[10]){			// Update Password for Account
	
	if(AccountCategory==1){
		int i=ID-1000;
		int fd=open("NormalUserfile",O_RDWR,0744);
		bool result;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);   
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);				//locking the selected record

		normalUser presentCust;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  		//Bringing the cursor to the selected record
		read(fd,&presentCust,sizeof(normalUser));
		
		if(!strcmp(presentCust.status,"ACTIVE")){
			strcpy(presentCust.password,newPwd);
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&presentCust,sizeof(normalUser));
			result=true;
		}
		else	
			result=false;

		printf("Press Enter to Unlock From Critical Section of Password Change \n");
		getchar();
		lock.l_type=F_UNLCK;			// Unlock the Record
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(AccountCategory==2){
		int i=ID-2000;
		int fd=open("JointUserfile",O_RDWR,0744);
		bool result;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser); 
		lock.l_len=sizeof(jointUser);	            
		lock.l_pid=getpid();
	
		fcntl(fd,F_SETLKW,&lock);				//locking the selected record
		//getchar();

		jointUser presentCust;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  		//Bringing the cursor to the selected record
		read(fd,&presentCust,sizeof(jointUser));
		
		if(!strcmp(presentCust.status,"ACTIVE")){
			strcpy(presentCust.password,newPwd);
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&presentCust,sizeof(jointUser));
			result=true;
		}
		else	
			result=false;
		printf("Press Enter to Unlock From Critical Section of Password Change \n");
		getchar();
		lock.l_type=F_UNLCK;			// Unlock the Record
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

bool addNormalUser(normalUser record){					// Adding Normal USer Account
	int fd=open("NormalUserfile",O_RDWR,0744);
	bool result;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(normalUser);    
	lock.l_len=sizeof(normalUser);	           
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	//locking the selected record

	normalUser endUser;
	lseek(fd,(-1)*sizeof(normalUser),SEEK_END);  		//Bringing the cursor to the selected record
	read(fd,&endUser,sizeof(normalUser));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(normalUser));
	if(j!=0)	
		result=true;
	else	
		result=false;
	
	lock.l_type=F_UNLCK;			// Unlock the Record
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool addJointUser(jointUser record){
	int fd=open("JointUserfile",O_RDWR,0744);					// Adding Joint USer Account
	bool result;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(jointUser);   
	lock.l_len=sizeof(jointUser);	            
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	//locking the selected record

	jointUser endUser;
	lseek(fd,(-1)*sizeof(jointUser),SEEK_END);  		//	Bringing the cursor to the selected record
	read(fd,&endUser,sizeof(jointUser));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(jointUser));
	if(j!=0)	
		result=true;
	else	
		result=false;
	
	lock.l_type=F_UNLCK;				// Unlock the Record
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteNormalUser(int ID){						// Deleting Normal User Account
	int i=ID-1000;
	int fd=open("NormalUserfile",O_RDWR,0744);
	bool result;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    
	lock.l_len=sizeof(normalUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	
	//getchar();

	normalUser presentCust;
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  
	read(fd,&presentCust,sizeof(normalUser));
	
	if(!strcmp(presentCust.status,"ACTIVE")){	
		strcpy(presentCust.status,"CLOSED");
		presentCust.balance=0;
		
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR); 
		int j=write(fd,&presentCust,sizeof(normalUser));
		if(j!=0)	
			result=true;
		else		
			result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteJointUser(int ID){					// Deleting Joint Account User
	int i=ID-2000;
	int fd=open("JointUserfile",O_RDWR,0744);
	bool result;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    
	lock.l_len=sizeof(jointUser);	           
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	
	//getchar();

	jointUser presentCust;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  
	read(fd,&presentCust,sizeof(jointUser));
	
	if(!strcmp(presentCust.status,"ACTIVE")){	
		strcpy(presentCust.status,"CLOSED");
		presentCust.balance=0;
		
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR); 
		int j=write(fd,&presentCust,sizeof(jointUser));
		if(j!=0)	
			result=true;
		else		
			result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modifyNormalUser(normalUser modUser){				// Modifying Normal User Account
	int i=modUser.userID-1000;
	int fd=open("NormalUserfile",O_RDWR,0744);
	bool result=false;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    
	lock.l_len=sizeof(normalUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	

	normalUser presentCust;
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  
	read(fd,&presentCust,sizeof(normalUser));
	
	if(!strcmp(presentCust.status,"ACTIVE") && (modUser.account_no==presentCust.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(normalUser));
		if(j!=0)	
			result=true;
		else		
			result=false;
	}
	printf("Press Enter to Unlock for Modifying for Normal User \n");
	getchar();
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modifyJointUser(jointUser modUser){			// Modifying Joint User Account
	int i=modUser.userID-2000;
	int fd=open("JointUserfile",O_RDWR,0744);
	bool result=false;
	
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);   	
	lock.l_len=sizeof(jointUser);	             
	lock.l_pid=getpid();
	
	fcntl(fd,F_SETLKW,&lock);	//locking the selected record

	jointUser presentCust;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&presentCust,sizeof(jointUser));
	
	if(!strcmp(presentCust.status,"ACTIVE")  && (modUser.account_no==presentCust.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(jointUser));
		if(j!=0)	
			result=true;
		else		
			result=false;
	}
	
	printf("Press Enter to Unlock for Modifying for Joint User Account \n");
	getchar();
	lock.l_type=F_UNLCK;			// Unlock the Record
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

void serverTask(int nsd){
	int messageLength,response,type,Category,AccountCategory,presentUserID;
	bool result;
	while(1){
		read(nsd,&Category,sizeof(Category));
		printf("Category : %d\n",Category);

		if(Category==1){
			normalUser presentUser1;
			AccountCategory=1;
			messageLength=read(nsd,&presentUser1,sizeof(normalUser));
			printf("Username : %d\n",presentUser1.userID);
			printf("Password : %s\n",presentUser1.password);
			presentUserID=presentUser1.userID;
			result=checkNormalUser(presentUser1);
			write(nsd,&result,sizeof(result));
		}
		else if(Category==2){
			jointUser presentUser2;
			AccountCategory=2;
			messageLength=read(nsd,&presentUser2,sizeof(jointUser));
			presentUserID=presentUser2.userID;
			printf("Username : %d\n",presentUser2.userID);
			printf("Password : %s\n",presentUser2.password);
			result=checkJointUser(presentUser2);
			write(nsd,&result,sizeof(result));
		}
		else if(Category==3){
			admin presentUser3;
			AccountCategory=3;
			messageLength=read(nsd,&presentUser3,sizeof(admin));
			presentUserID=presentUser3.userID;
			printf("Username : %d\n",presentUser3.userID);
			printf("Password : %s\n",presentUser3.password);
			result=checkAdmin(presentUser3);
			write(nsd,&result,sizeof(result));
		}
		else{
			result=false;
			write(nsd,&result,sizeof(result));
		}
		if(result)	break;		
	}

	while(1){
		read(nsd,&response,sizeof(int));
		if(Category==1 || Category==2){
			if(response==1){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=depositin(AccountCategory,presentUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(response==2){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=withdrawl(AccountCategory,presentUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(response==3){
				float amt;
				amt=getBalance(AccountCategory,presentUserID);
				write(nsd,&amt,sizeof(float));
			}
			else if(response==4){
				char pwd[10];
				read(nsd,pwd,sizeof(pwd));
				result=Passwordchange(AccountCategory,presentUserID,pwd);
				write(nsd,&result,sizeof(result));
			}
			else if(response==5){
				if(Category==1){
					normalUser user1=getNormalUser(presentUserID);
					write(nsd,&user1,sizeof(normalUser));
				}
				else if(Category==2){
					jointUser user2=getJointUser(presentUserID);
					write(nsd,&user2,sizeof(jointUser));
				}
			}
			else if(response==6)	break;
		}
		else if(Category==3){
			read(nsd,&type,sizeof(int));
			if(response==1){
				if(type==1){
					normalUser newUser1;
					read(nsd,&newUser1,sizeof(normalUser));
					result=addNormalUser(newUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					jointUser newUser2;
					read(nsd,&newUser2,sizeof(jointUser));
					result=addJointUser(newUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(response==2){
				if(type==1){
					int delUserID1;
					read(nsd,&delUserID1,sizeof(int));
					result=deleteNormalUser(delUserID1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
					read(nsd,&delUserID2,sizeof(int));
					result=deleteJointUser(delUserID2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(response==3){
				if(type==1){
					normalUser modUser1;
					read(nsd,&modUser1,sizeof(normalUser));
					result=modifyNormalUser(modUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					jointUser modUser2;
					read(nsd,&modUser2,sizeof(jointUser));
					result=modifyJointUser(modUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(response==4){
				if(type==1){
					normalUser searchUser1;
					int userID1;
					read(nsd,&userID1,sizeof(int));
					searchUser1=getNormalUser(userID1);
					write(nsd,&searchUser1,sizeof(normalUser));
				}
				else if(type==2){
					jointUser searchUser2;
					int userID2;
					read(nsd,&userID2,sizeof(int));
					searchUser2=getJointUser(userID2);
					write(nsd,&searchUser2,sizeof(jointUser));
				}
			}
			else if(response==5)	break;
		}
	}
	close(nsd);
	printf("Client Session Ended \n");
	return;
}

void *connection_handler(void *nsd) {
	int nsfd = *(int*)nsd;
	serverTask(nsfd);
}


int main(){
	struct sockaddr_in server,client;
	int sd,nsd,clientLen;
	pthread_t threads;
	bool result;
	sd=socket(AF_INET,SOCK_STREAM,0);

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(7245);

	bind(sd,(struct sockaddr *)&server,sizeof(server));
	listen(sd,15);			//15 clients per connection
	
	printf("Connecting to the Client\n");
	while(1){
		clientLen=sizeof(client);
		nsd=accept(sd,(struct sockaddr *)&client,&clientLen);

		printf("Connected Successfully to the Client\n");
		if(pthread_create(&threads,NULL,connection_handler,(void*) &nsd)<0){
			perror("Thread could not be Created");
			return 1;
		}	
	}
	pthread_exit(NULL);
	close(sd);
	return 0;
}