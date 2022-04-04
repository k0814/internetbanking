#include<time.h>
#include<string.h>
#include<stdlib.h>

// removes the nwwline character froma string. If string does not have '\n', it is left alone
// kiarie
char* remove_new_line (char* string){
  int i;
  int len = strlen(string);
  char* newstring ;
  newstring = (char *)malloc(len-1);


  if (string[len-1] == '\n'){
    for (i = 0; i < len-1; i++) {
      newstring[i] = string[i];
    }

    return newstring;
  }

  else{
    return string;
  }
}


// this function returns the current server time ---> as a string
// kiarie
char* get_transaction_time(){
	struct tm* ptr;   // declare a time struct
    time_t t;
    t = time(NULL);
    ptr = gmtime(&t);
    char* time_str;
    time_str = asctime(ptr);  // asctime() returns a full time description of the ptr.
	                          // ptr is a tm struct instance
    char* time_str_without_new_line = remove_new_line(time_str);   // this line is experimental , you see... asctime returns a string with  newline... its a pain in the ass
	  return time_str ;
}
