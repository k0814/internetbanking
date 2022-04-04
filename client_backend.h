#include<stdio.h>
#include<stdlib.h> // it sorted some typedef functions.... dark elixir magic.
#include "bank_server.h"

int current_account_index ; // current_account_index points to the account of the current client using the front end.
                            // all accounts are stored in the array_of_accounts array.
                            // this is the array_index of the account of the current client.  ... global to easen parameter calls.
                            // this index varies... it may be new or old.
                            // if user logins, we have to look for it using the function : find_account()
                            // if user signsup, we record the array index which we have inserted the new record.
                            // so this variable will be set in two functions:
                            // 1. call_server_to_login()
                            // 2. call_server_to_make_new_account()


void call_server_to_make_new_account();   // under the signup_page...
void call_server_to_inquire_balance();    // under the account_page
void call_server_to_inquire_statement();  // under the account_page
void call_server_to_allow_deposit();      // under the account_page
void call_server_to_conduct_withdrawal(); // under the account_page
void call_server_to_close_account();      // under the account_page
void log_out();  // makes the program to stop directly interacring with the account of the current user.

// special functions associated with the login page. that are not core functions //
void call_server_to_login(); // under the account_page
Bool take_login_input_and_validate();
Bool input_and_validate_pin(int account_number);
Bool check_if_pin_is_correct(int account_number, int pin);
Bool check_if_account_number_exists(int account_number);

// end of special functions that are not core functions //

int find_account(int some_account_number){  // finds the account that has the same account_number as the one in the parameter.
                                            // returns the array index of that account in the array_of_accounts
  int i;
  for (i=0 ; i<pointer_to_array_of_accounts+1; i++){
    if (array_of_accounts[i].account_number == some_account_number){
      return i;
    }
  }
}  // end of find_account(int some_account_number)



void call_server_to_make_new_account(){
  printf("\n Enter your first name ---> " );
  char name[1000]; // keyboard input variable.
  scanf("%s",&name );

  printf("\n Enter your email address ---> " );
  char mail[1000]; // keyboard input variable.
  scanf("%s",&mail );

  printf("\n Enter your new 4-digit pin ---> " );
  int pin; // keyboard input variable.
  scanf("%d",&pin );

  Account new_account = open_new_account(name, mail, pin);  // instantiate an acccount with default values.
  current_account_index = pointer_to_array_of_accounts;
  add_to_array_of_accounts(new_account);
}

void call_server_to_allow_deposit(){
  float some_money;
  printf("\n\n Enter the deposit amount ---> " );
  scanf("%f",&some_money );

  // execute deposition.
    Bool result = deposit_to_account(&array_of_accounts[current_account_index], some_money);
    if (result == true){
      printf("\n ----- You have successfully deposited %f Ksh. -----\n", some_money );
      printf("  ----- Your new balance is %f Ksh.  -----", array_of_accounts[current_account_index].balance );
    }

    else{
      printf("\n ----- The deposit was unsucessful, try checking if the account has already been closed -----\n", some_money );
    }
}

void call_server_to_conduct_withdrawal(){
  printf("\n Enter the withdrawal amount (current balance : %f )---> ", array_of_accounts[current_account_index].balance );
  float some_money;
  scanf("%f",&some_money );

  // execute withdrawal.
  Bool result = withdraw_from_account (&array_of_accounts[current_account_index], some_money);
  if (result == true){
    printf("\n ----- You have successfully withdrawn %f Ksh. -----\n", some_money );
    printf("  ----- Your new balance is %f Ksh.  -----", array_of_accounts[current_account_index].balance );
  }
}

void call_server_to_inquire_balance(){
  printf("\n Your current balance is : %f\n", array_of_accounts[current_account_index].balance );
}

void call_server_to_inquire_statement(){
  printf("\t---------------------------------------\n");
  printf("\t---------- Account statement ----------\n");

  if (array_of_accounts[current_account_index].transaction_list_pointer <= 0){ // if there are no transactions yet...
    printf("No transactions executed yet\n" );
  }

  else{   // print the statements
    // declaring variables used in the statement printing process
    int transaction_index ;
    int i;   // counter
    int length_of_statement = array_of_accounts[current_account_index].transaction_list_pointer;
    char* action;
    // end of declaring variables used in the statement printing process

    // looping thru the transaction_list
    for(i=0; i<length_of_statement; i++){
      transaction_index = i+1;

      // switch decides on the verb to describe the transaction.
      switch (array_of_accounts[current_account_index].transaction_list[i].type) {
        case withdrawal:
          action = "withdrew";
          break;

        case deposit:
          action = "deposited";
          break;

        case statement_check:
          action = "checked the statement ";
          break;

        case balance_check:
          action = "checked the statement ";
          break;

        case account_closing:
          action = "closed the account ";
          break;

        case account_opening:
          action = "opened the account ";
          break;
      } // end of switch

      if (array_of_accounts[current_account_index].transaction_list[i].type == withdrawal || array_of_accounts[current_account_index].transaction_list[i].type == deposit ){
        printf("%d . You %s %f on %s", transaction_index,  action, array_of_accounts[current_account_index].transaction_list[i].money_involved, array_of_accounts[current_account_index].transaction_list[i].time  );
      }

      else {
        printf("%d . You %s  on %s", transaction_index, action,  array_of_accounts[current_account_index].transaction_list[i].time  );
      }

    } // end of for loop looping through the transaction_list
  } // end of else stmt covering "If there were at least one transaction" condition
} // end of call_server_to_inquire_statement function.


// ---------- start of void call_server_to_close_account() -------- //
  void call_server_to_close_account(){
    Bool result = close_account(&array_of_accounts[current_account_index]);
    if (result == true){
      printf("\n ----- Account has been successfully closed -----\n" );
    }
  }
// ---------- end of void call_server_to_close_account() -------- //


// ----------- start of void log_out(); ---------------------------- //
void log_out(){
  current_account_index = -1;  // making the current account index to point at smth non-sensical (not within the bounds of array_of_accounts)
  printf("\n ------------  Logging out ....   -----\n" );
}
// ----------- end of void log_out(); ---------------------------- //

//---------- start of void call_server_to_login()----------------- //
Bool take_login_input_and_validate(){
  // this function :
  // - takes in login input from client.
  // - validates all inputs [account number, pin] and returns errors if possible.
  // - if login was successful... make client_backend directly access user's account info .... then return true Bool.
  // - else if login was unsuccessful... print specific error message and return false.

  // taking input from user:
  printf("\n Enter your Account number ---> " );
  int account_number; // keyboard input variable.
  scanf("%d",&account_number );



  // validate users input.
    // check if the account number exists in the array_of_accounts
    Bool account_number_exists = check_if_account_number_exists(account_number); // this funtion has been defined in client_backend.h

    Bool result = false;

    // if account_number_exists , then check if pin is correct. else.... tell user that the account number does not exist.
    switch (account_number_exists) {
      case true: // in case account number exists...input pin and validate
        result = input_and_validate_pin(account_number);

        if (result == true){ // if pin is correct, make frontend directly affect users account.
          current_account_index = find_account(account_number);
        }

        return result;

      case false:  // in case account number doesn't exist...print error message and return Bool false.
        printf("\n------------ The account number does not Exist -------------\n" );
        return result;
    }
}
//---------- end of void call_server_to_login()----------------- //

// ------- start of check_if_account_number_exists() ------------- //
// returns Bool true if account is found in the array_of_accounts
Bool check_if_account_number_exists(int account_number){
  int i;
  Bool result = false;

  for(i=0; i<pointer_to_array_of_accounts; i++){
    if(account_number == array_of_accounts[i].account_number){
      result = true;
      break;
    }
  }

  return result;
}
// ------- end of check_if_account_number_exists() ------------- //

// ------- start of check_if_pin_is_correct() ------------- //
// returns Bool true if pin is correct
Bool check_if_pin_is_correct(int account_number, int pin){
  Bool result = false;
  int current_temporary_index = find_account(account_number);

  if(array_of_accounts[current_temporary_index].client_pin == pin){
    result = true;
    return result;
  }

  else{ return result;}
}
// ------- end of check_if_pin_is_correct() ------------- //

Bool input_and_validate_pin(int account_number){
  int pin;
  printf("\n Enter your 4-digit pin ---> " );
  scanf("%d",&pin );

  Bool result = check_if_pin_is_correct(account_number, pin); // this funtion has been defined in client_backend.h
  return result;
}
