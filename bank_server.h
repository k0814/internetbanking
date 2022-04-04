#include<stdio.h>    // I/O operation functions.
#include<stdlib.h>
#include<string.h>
// #include<stdbool.h>
#include "clock.h"   // defines functions that extract time for the server.

// type definitions... apart from things like int, float, char and stuff.
typedef struct transaction Transaction;
typedef struct account Account;
typedef enum bool Bool;            // C does not have 'bool' type by default.
enum bool { true, false };


// transaction types defined here.
enum transaction_type{
	withdrawal,
	deposit,
	statement_check,
	balance_check,
	account_closing,
	account_opening
};


// transaction structure
struct transaction {
  char* time;
  enum transaction_type type;
  double money_involved;
};

// account status.
enum account_status{
  closed,
  open
};


// account blueprint.
struct account {
  enum   account_status status ;   // can be open or closed
  char  client_name[100];              // any length is possible... no '\n' or whitespace allowed.
	char  client_mail[100];              // any length is possible... no '\n' or whitespace allowed.
	int   client_pin;                    // a pin ha 4 digits.
  int account_number;    // no alphabet letters allowed
  double balance;                  // actual money in the bank
  Transaction transaction_list[1000];    // list of all the transactions, this is helpful in extracting a bank statement.
  int transaction_list_pointer;      // It helps us know the array_index of the next transaction. By default it begins at zero. Whenever a new transaction happens, it gets incremented.
                                     // Vectors or dynamic arrays like the ones in python were hard to implement (took time + bugs + old,poorly documented libraries). C meeh.
};


// functions that affect a single account.
void display_account_details(Account ac); //
Account open_new_account (char* client_name, char* client_mail, int client_pin); // returns an account filed with default data.
Bool add_new_transaction_to_transaction_list (Account* ac, Transaction some_transaction); // it just adds a record to the statement.
                                                                                          // it does not cause logic changes, ie. eg adding a deposit transaction does not mean the account balance will change
Bool deposit_to_account(Account* ac, double some_money); // now this causes logic changes to the account balance.
Bool withdraw_from_account (Account* ac, double some_money);  // if withdrawal cannot happen, it returns false.
Bool close_account(Account* ac); // an account can be closed only if the balance is zero.

//****************************************************************************//


// definition of Bool close_account(Account* ac)
  Bool close_account(Account* ac){
    Account temp = *ac;
		// you can only close an account that has 0.00 balance. We aint thievs...you see...
		if (temp.balance > 0){
			printf("\n Kindly withdraw all the money from the account before closing it...\n");
			return false;
		}
    switch (temp.status) {
      case closed:            // i mean you cannot close a cosed account.
        printf("\n The account has already been closed.\n" );
        return false;

      //chane account status to "closed", instantiate that transaction, record that transaction.
      case open:
        // causing changes to the account.
        temp.status = open;  // tnis line does not make sense.... fun fact... it fixes a bug <evil laugter >
                             // you can replace it with any valid line of code , as long as that line is not a declaration
        // creating a transaction instance
        Transaction closing_account_transaction;
        closing_account_transaction.time = get_transaction_time();
        closing_account_transaction.money_involved = 0.00;
        closing_account_transaction.type = account_closing;

        // recording the transaction in the transaction_list
        add_new_transaction_to_transaction_list(&temp, closing_account_transaction);

				// causing changes to the account.
        temp.status = closed;
        // returning ownership.
        *ac = temp;
        return true;
    }
  }
// end of the definition of Bool close_account(Account* ac)


// definition of the deposit_to_account()
  Bool deposit_to_account(Account* ac, double some_money){   // if deposit fails, it returns false. A deposit will fail if the account is currently closed
    Account temp = *ac;

    switch (temp.status) {
      case closed:
        return false;

      case open:
        temp.balance = temp.balance + some_money;  // incrementing the initial balance.

        // creating a transaction instance
        Transaction deposit_transaction;
        deposit_transaction.time = get_transaction_time();
        deposit_transaction.money_involved = some_money;
        deposit_transaction.type = deposit;

        // recording the transaction in the transaction_list
        add_new_transaction_to_transaction_list(&temp, deposit_transaction);

        // returning ownwership from temp to ac*
        *ac = temp;

        return true;
    }

  }
// end of the definition of the deposit_to_account()

// definition of Bool withdraw_from_account (double some_money);
  Bool withdraw_from_account (Account* ac, double some_money){
    Account temp = *ac;
    // only open accounts can be withdrawn from.
    // only accounts with sufficient money can be withdrawn from.
    // defining states....
      if (temp.status == closed) { // if account is closed...
				printf("\n Wait... withdrawal did not happen because the account is currently closed\n" );
        return false;
        // if we had great error handling, we would return "Account closed error...."
      }
      else if (temp.balance < some_money){ // if account has insufficient balance
				printf("\n Withdrawal did not happen because the account has insufficient balance\n" );
        return false;
        // if we had great error handling, we would return "Account has insufficient balance error...."
      }

      else{ // create transaction instance, cause changes in the balance, record the transaction in the transaction_list
        // creating a transaction instance
        Transaction withdrawal_transaction;
        withdrawal_transaction.time = get_transaction_time();
        withdrawal_transaction.money_involved = some_money;
        withdrawal_transaction.type = withdrawal;

        // recording the transaction in the transaction_list
        add_new_transaction_to_transaction_list(&temp, withdrawal_transaction);

        // causing changes to the balance.
        temp.balance = temp.balance - some_money;

        // returning ownership.
        *ac = temp;
        return true;
      }
  }
// End of definition of Bool withdraw_from_account (double some_money);


Bool add_new_transaction_to_transaction_list (Account* ac, Transaction some_transaction){  // adds transaction to account. works on open accounts only.
	                                                                     // returns true if operation was successful... else ... returns false.
  Account temp = *ac		; // am using temp so as to avoid using the confusing pointer notations in the operations below

	if (temp.status == closed){
		return false;
	}

	if (temp.status == open){
		temp.transaction_list[temp.transaction_list_pointer] = some_transaction;
		temp.transaction_list_pointer = temp.transaction_list_pointer + 1;
		*ac = temp;    // returning ownership
		return true;
	}
}

//****************************************************************************//

Account open_new_account (char* client_name, char* client_mail, int client_pin){ // what gets called when bank server opens a new a/c, fills default data.

	static int last_known_account_number = 9999; // this will be the account_number of the newly created account. It is static so that we can increment it afterwards
	                                             // we will increment it in order to avoid having two or more accounts having the same account number.
																							 // considering we will store the accounts in a file, we will look for the last_known_account_number in that file before we run our scripts
																							 // it is not supposed to always start at 9999  **** future_bug ****
																							 // the fix will be smth like : last_known_account_number = read_from_file()

	last_known_account_number = last_known_account_number + 1;
	int new_account_number = last_known_account_number;

	Account temp ;

  // instantiating the transaction "opening_account_transaction"
	Transaction opening_account_transaction;
	opening_account_transaction.time = get_transaction_time();
  opening_account_transaction.type = account_opening;
  opening_account_transaction.money_involved = 0.0;

	// setting the default values of a new account
	temp.account_number = new_account_number;
	strcpy(temp.client_name, client_name);
	strcpy(temp.client_mail, client_mail);
	temp.client_pin = client_pin;
	temp.balance = 0.00;
	temp.status = open;
	temp.transaction_list_pointer = 0;


	// recording the opening transaction into the transaction_list
	add_new_transaction_to_transaction_list(&temp, opening_account_transaction);

	//returning ownership back to the original a/c.
	return temp;
}

//************* display functions *********************** //
void display_account_details(Account ac){  // used for testing purposes
	char* account_status ;

	switch (ac.status) {
		case 0:
			account_status = "closed";
			break;

		case 1:
			account_status = "open";
			break;
	}

	printf("account_ status : %s\n", account_status);
  printf("client_name : %s\n", ac.client_name);
  printf("account_number : %d\n", ac.account_number);
  printf("balance : %f\n", ac.balance);

	printf("---------- statement ----------\n");

	if (ac.transaction_list_pointer <= 0){ // if there are no transactions yet...
		printf("No transactions executed yet\n" );
	}

	else{   // print the statements
		// declaring variables used in the statement printing process
		int transaction_index ;
		int i;   // counter
		int length_of_statement = ac.transaction_list_pointer;
		char* action;
		// end of declaring variables used in the statement printing process

		// looping thru the transaction_list
		for(i=0; i<length_of_statement; i++){
			transaction_index = i+1;

			// switch decides on the verb to describe the transaction.
			switch (ac.transaction_list[i].type) {
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
			}

			if (ac.transaction_list[i].type == withdrawal || ac.transaction_list[i].type == deposit ){
				printf("%d . %s %s %f on %s", transaction_index, ac.client_name, action, ac.transaction_list[i].money_involved, ac.transaction_list[i].time  );
			}

			else {
				printf("%d . %s %s  on %s", transaction_index, ac.client_name, action,  ac.transaction_list[i].time  );
			}

		}
	}
}

////  DARK MAGIC .... I present to you... BUGS /////
// Ok .... met a lot of bugs here....
/// I wanted to create an array that stores accounts.
// To counter the bugs.... i made the array and its pointer global
Account array_of_accounts[1000];
int pointer_to_array_of_accounts = 0;  // points to the array index where the next account will be stored in . It starts at zero by default.
void add_to_array_of_accounts(Account ac){  // helps you add an element at the end of the array...
	array_of_accounts[pointer_to_array_of_accounts] = ac;
	pointer_to_array_of_accounts = pointer_to_array_of_accounts + 1;
}
