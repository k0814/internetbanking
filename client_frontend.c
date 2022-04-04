#include<stdio.h>  // for basic input output functionalities eg scanf and printf
#include<stdlib.h> // it sorted some typedef functions.... dark elixir magic.
#include<time.h>  // contains delay(milliseconds) function
#include "client_backend.h"

int sleep_time = 2; // page loading time.... think of it how javascript does things

//***** client dashboard functions
void homepage();      // user can login or signup or quit application
void login_page();
void signup_page();
void account_page();  // user can
void ask_user_to_exit_statement_inquiry_page();

//***** end of client dashboard functions

//***** start of client dashboard functions actual definitions

//******************* homepage **********************//
void homepage () {
  printf("***** Bank Home Page*****\n\n" );
  printf("Choose an option....\n" );
  printf("1. Log in\n" );
  printf("2. Sign up\n" );
  printf("3. Quit application\n\n" );

  int choice;
  char input[20];  // string input.
  printf("Choice ---> " );
  scanf("%s",&input );
  choice = atoi(input); // convert input to int if possible. else it returns zero or some random number.
                        // C language is mysterious... what if I wanted to convert "0"???

 // making choices .... aagh life.
  switch (choice) {
    case 1:
     login_page();
     break;

    case 2:
     signup_page();
     break;

    case 3:
     exit(1);
     break;

    default:
     printf("\nYou entered an invalid choice... try again ....\n" );
     sleep(sleep_time);    // wait for 2 seconds
     system("cls");  // clear screen
     homepage();     // re-write the homepage
  }
}
//******************* end of homepage **********************//

//******************* start of login_page **********************//
 void login_page(){
   system ("cls"); // clear the screen.... to emulate ... opening a new clean browser tab
   printf("\n----------------------------- ACCOUNT LOG IN PAGE -----------------------------------\n" );

   Bool login_details_were_valid = take_login_input_and_validate();


   switch (login_details_were_valid) {
     case true:
       account_page();
      break;

     case false:
       sleep(sleep_time);    // wait for 2 seconds
       system("cls");  // clear screen
       homepage();     // re-write the homepage
   }

 }
//******************* end of login_page **********************//

//******************* start of signup_page **********************//
  void signup_page(){
    system ("cls"); // clear the screen.... to emulate ... opening a new clean browser tab
    printf("\n----------------------------- ACCOUNT SIGN UP PAGE -----------------------------------\n" );

    // call server to make new account.
    call_server_to_make_new_account();
    sleep(sleep_time-1);
    account_page();
  }
//******************* end of signup_page **********************//

//******************* start of account_page **********************//
  void account_page(){
    // int index = find_account(ac); // access the array_index of the account, so we can obtain info from it directly
    system ("cls"); // clear the screen.... to emulate ... opening a new clean browser tab

    printf("\n\n---------------------------------------- USER ACCOUNT PAGE -------------------------------------------------\n\n" );
    printf("------- Username : %s ----------", array_of_accounts[current_account_index].client_name );
    printf("------- Account number : %d ----------", array_of_accounts[current_account_index].account_number );
    printf("------- Balance : %f ----------\n", array_of_accounts[current_account_index].balance );
    printf("\nChoose an option : \n" );
    printf("1. Deposit Cash\n" );
    printf("2. Withdraw Cash\n" );
    printf("3. Check balance\n" );
    printf("4. Check statement\n" );
    printf("5. Close account\n" );
    printf("6. Log out\n" ); // calls the log_out() function.

    int choice;
    char input[20];  // string input.
    printf("Choice ---> " );
    scanf("%s",&input );
    choice = atoi(input); // convert input to int if possible. else it returns zero or some random number.
                          // C language is mysterious... what if I wanted to convert "0"???

   // making choices .... aagh life.
    switch (choice) {
      case 1:
       call_server_to_allow_deposit();
       sleep(sleep_time + 2);
       break;

      case 2:
       call_server_to_conduct_withdrawal();
       sleep(sleep_time + 2);
       break;

      case 3:
       call_server_to_inquire_balance();
       sleep(sleep_time + 2);
       break;

      case 4:
       call_server_to_inquire_statement();
       ask_user_to_exit_statement_inquiry_page();
       break;

      case 5:
       call_server_to_close_account();
       sleep(sleep_time + 2);
       break;

      case 6:
       log_out();
       sleep(sleep_time);
       system("cls");
       homepage();
       break;

      default:
       printf("\nYou entered an invalid choice... try again ....\n" );
       sleep(sleep_time);
       account_page();
    }
    account_page();

  }
//******************* end of account_page **********************//

int main(int argc, char const *argv[]) {
  homepage();
  return 0;
}


void ask_user_to_exit_statement_inquiry_page(){
  printf("\n --- Press X or any key if you have finished viewing the statement ---> " );
  char input[20];  // string input.
  scanf("%s",&input );

  if (input == true){
    account_page();
  }
}
