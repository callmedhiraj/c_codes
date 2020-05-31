/**Necessary header files */
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<dos.h>
#include<time.h>
#include<windows.h>
#include<malloc.h>
///global settings
int isAdmin = 0, idOfVoter;
///define the data as structures
typedef struct{
    int tm_year;
    int tm_mon;
    int tm_mday;
}Date;

typedef struct{
    int constituencyID;
    char constituencyName[20];
    int totalCandidates;
    char candidateNames[10][150];
}Constituency;
typedef struct{
    int contestantID;
    char contestantName[150];
    Date dob;
    char gender;
    int totalVotes;
    int constituencyID;
}Contestant;
// data structure of a voter
typedef struct{
int voterID;
char username[150];
char password[150];
int constituencyID;
int hasAlreadyCastedVote;
}Voter;

int calculateAge(Date *d)
{
int temp1, temp2, age = 0 ;
time_t now = time(0);
struct tm *local = localtime(&now);
temp1 = (1900+local->tm_year)*365 + (int)(1+local->tm_mon)*30 + (int)(local->tm_mday);
temp2 =  (d->tm_year)*365 + (int)(1+d->tm_mon)*30 + (int)(d->tm_mday);
age= (int)(temp1-temp2)/365;
return age;
}

int adminLogin(char *uname, char *pass);
/**
reads the name of all the constituencies and stores it on the disk
Displays the successful and error message
**/
void addConstituency();
/// function that lists all the constituencies from the disk.
void viewAllConstituencies();
/// function that adds contestants in any constituency
/// first of all, the list of constituencies will be listed and the
/// admin will be asked to read the total number of contestants
/// accordingly, the details of all the contestants of that constituencies
/// will be read and stored on the disk.
void addConstestant();
/// function to view all contestant in any constituencies
void viewAllContestants();
void viewContestantsByConstituency();
/// function to add a voter
void addVoter(int constituencyID);
/// function to view the voter's voting status
int viewVoterVotingStatus(int voterID);
/// function to delete a Voter
void deleteVoter(int voterID);
/// function to view total votes received
void viewTotalVoteCountOfAllContestant();
///function to announce the winner from each constituencies
void viewWinnerOfEachConstituency();
///function to calculate total number of registered constituencies till date
int getTotalConstituencies();
///main implementation
int main(){
///declare necessary variables
char systemUserName[150], systemPassword[150];
int i = 0;
char ch;

///verify if the user is admin or not
LOGIN:
system("cls"); ///clear screen
printf("\n Enter the username:\t");
fflush(stdin);
gets(systemUserName);
///hide password
printf("\nEnter the password :\t");
i = 0; ///for next password reading
while((ch =getch())!='\r'){
    systemPassword[i] = ch;
    printf("*");
    ++i;
}
fflush(stdin);
if(strcmp(systemUserName,"admin") ==0 && strcmp(systemPassword, "admin!@") ==0){
    ///the user is an admin, set the global variable to  1
    isAdmin = 1;
    goto ADMIN_MENU;
}else{
///read the system user_password file to read the matching values
//@todo variable declared
FILE *fptr;
Voter v;
fptr = fopen("voters.bin", "r+b");
if(!fptr){
    printf("\nThere was some error validating the user. \n Try again later.");
    getch();
    exit(-1);
}
while(fread(&v, sizeof(v), 1, fptr)){
    if(strcmp(systemUserName, v.username) == 0 ||
       strcmp(systemPassword, v.password) == 0){
           system("cls");
        printf("\n Welcome %s, Logged in as a voter", systemUserName);
       }
}
fclose(fptr);

getch();
exit(0);
}
///show the ADMIN menu
ADMIN_MENU:
    system("cls");
    printf("\n Welcome Admin,\n\n\n");
    Sleep(2000);
    printf("1. Add new constituency (Currently, %d)\n", getTotalConstituencies());
    printf("2. View all constituency\n");
    printf("3. Edit Constituency\n");
    printf("4. Delete Constituency\n");
    printf("5. Search Constituency\n");
    printf("6. Add Contestant\n");
    printf("7. View Contestants By Constituency\n");
    printf("8. Add Voter\n");
    printf("9. Logout\n");
    printf("10. Exit\n");
///@todo variable declared
int choice;
scanf("%d", &choice);
system("cls");
int cid;
switch(choice){
case 1:
    addConstituency();
    break;
case 2:
    viewAllConstituencies();
    break;
case 6:
    addConstestant();
    break;
case 7:
    viewContestantsByConstituency();
    break;
case 8:
    printf("\nVoter for which Constituency ? ");
    scanf("%d",&cid );
    addVoter(cid);
    break;
case 9://logout
    printf("\nSending terminating signal");
    for(i = 0; i<6; i++){
        Sleep(500);
        printf(".");
    }
    goto LOGIN;
    break;
case 10://exit
     printf("\nSending terminating signal");
    for(i = 0; i<6; i++){
        Sleep(500);
        printf(".");
    }
    exit(0);
    system("exit");
break;
default:
    printf("\n Wrong choice. Try again.");
    system("cls");
    goto ADMIN_MENU;
    break;
}
goto ADMIN_MENU;
getch();
return 0;
}
void addConstituency(){
    char fileName[] = "constituency.bin";
    FILE *fptr;
/*
see if the file is already present or not
->if it is already present open it in
    "r+b": read and write within the file
->Do not use "a+b" mode because you won't be able to modify earlier records
->Otherwise, open the file in
    "w+b": a new file is created if not found
    for reading and writing by removing all the previous contents in it.
**/

fptr = fopen(fileName, "r+");
if(fptr == NULL){
    printf("\n File not found .....trying to create one");
    fptr = fopen(fileName, "w+");
        if(fptr == NULL){
         printf("\n File not found .....trying to create one failed too...");
         return;
        }
}
printf("\n All good to go...");
///@todo variable declared
Constituency *c;
int totalConstituency, i, j;
printf("\nHow many constituencies are there? \t ");
scanf("%d", &totalConstituency);
///dynamically reserve the memory
c = (Constituency*) malloc(sizeof(Constituency)*totalConstituency);
for(i = 0; i < totalConstituency; i++){
    system("cls");
    ///read the details of each constituency
    /*int constituencyID;
    char constituencyName[20];
    int totalCandidates;
    */
    printf("\nEnter constituencyID:\t");
    scanf("%d", &c[i].constituencyID);
    printf("\nEnter Constituency name:\t");
    fflush(stdin);
    gets(c[i].constituencyName);
    printf("\nEnter total candidates (<=10):\t");
    scanf("%d", &c[i].totalCandidates);
    while(c[i].totalCandidates > 10 || c[i].totalCandidates <= 0 ){
        printf("\n candidates in each constituency should be minimum 1 and should not exceed 10 .");
        scanf("%d", &c[i].totalCandidates);
    }//add the candidates



}/// for loop ends

///take the file pointer after the last record i.e to the end of the file
///or else you will end up over writing the earlier records.
fseek(fptr, 0, 2);
/// now the data is ready, write to the disk file
if(fwrite(c, sizeof(Constituency), totalConstituency, fptr)){
    printf("\nThe constituencies has been registered.");
    //return;
}else{
printf("\n The constituencies could not be created.");
}
///close the file
fclose(fptr);
}/// function ends
void viewAllConstituencies(){
char fileName[] = "constituency.bin";
    FILE *fptr;
/**
see if the file is already present or not
->if it is already present open it in
    "r+b": read and write within the file
->Do not use "a+b" mode because you won't be able to modify earlier records
->Otherwise, open the file in
    "w+b": a new file is created if not found
    for reading and writing by removing all the previous contents in it.
**/

fptr = fopen(fileName, "r+");
if(fptr == NULL){
    printf("\n File not found .....trying to create one");
    fptr = fopen(fileName, "w+");
        if(fptr == NULL){
         printf("\n File not found .....trying to create one failed too...");
         return;
        }
}
printf("\n All good to go...");
///@todo variable declaration
Constituency c;
printf("\nConstituency ID |\t Constituency Name |\t Total Candidates\n");
//printf("_______\n");
while(fread(&c, sizeof(Constituency), 1, fptr)==1){
   // printf("________________________________\n");
    printf("\n%d|\t\t", c.constituencyID);
    printf("%s|\t\t", c.constituencyName);
    printf("%d\t\t", c.totalCandidates);
}
///close the file
fclose(fptr);
getche();
}
int getTotalConstituencies(){
    int total = 0;
char fileName[] = "constituency.bin";
   FILE *fptr;
fptr = fopen(fileName, "r+b");
if(fptr == NULL){
    printf("\n File not found .....trying to create one");
    fptr = fopen(fileName, "w+b");
    if(fptr== NULL){
    printf("\n Sorry the constituency could not be added.");
    exit;
    }
}
fseek(fptr, 0, 2);//take to the end of the file
Constituency c;
while(fread(&c, sizeof(Constituency), 1, fptr)==1){
   ++total;
}
fclose(fptr);
return total;
}
void addVoter(int constituencyID ){
    system("cls");
    Voter v;
    int i;
    char ch;
    printf("\nEnter the voterID: ");
    scanf("%d", &v.voterID);
    printf("\nEnter the username: ");
    fflush(stdin);
    gets(v.username);
    printf("\nEnter the password:");
    i = 0; ///for next password reading
while((ch =getch())!='\r'){
    v.password[i] = ch;
    printf("*");
    ++i;
}
fflush(stdin);
v.constituencyID = constituencyID;
v.hasAlreadyCastedVote=0;
/// now the record is ready to be stored on the disk;
FILE *fptr;
fptr = fopen("voters.bin", "r+b");
if(fptr == NULL){
    printf("\n File not found .....trying to create one");
    fptr = fopen("voters.bin", "w+b");
    if(fptr== NULL){
    printf("\n Sorry the voter could not be added.");
    exit;
    }
}
fseek(fptr, 0, 2);//take to the end of the file
if(fwrite(&v,sizeof(Voter), 1, fptr)==1){
    printf("\nThe voter has been added.");
}
fclose(fptr);
getch();

}
void viewContestantsByConstituency(){


}
void addConstestant(){
Contestant c;
int cid;
printf("\nEnter constituency ID: ");
scanf("%d", &c.constituencyID);
printf("\nEnter contestant ID:");
scanf("%d", &c.contestantID);
printf("\nEnter the contestant name:");
fflush(stdin);
gets(c.contestantName);
printf("\nEnter the gender(M/F): ");
fflush(stdin);
c.gender = getchar();
printf("\nEnter date of birth (yyyy/mm/dd):");
scanf("%d/%d/%d", &c.dob.tm_year, &c.dob.tm_mon, &c.dob.tm_mday);
c.totalVotes = 0;
if(calculateAge(&c.dob) <18){
    printf("\nSorry not eligible for candidacy, age is %d", calculateAge(&c.dob));
    exit(-1);
}

//valid age
//write to the disk
FILE *fptr;
fptr = fopen(c.contestantName, "r+b");
if(fptr == NULL){
    printf("\n File not found .....trying to create one");
    fptr = fopen(c.contestantName, "w+b");
    if(fptr== NULL){
    printf("\n Sorry the candidate could not be added.");
    exit;
    }
}
fseek(fptr, 0, 2);//take to the end of the file
if(fwrite(&c,sizeof(c), 1, fptr)==1){
    printf("\nThe candidate has been added.");
}
fclose(fptr);
getch();
}

