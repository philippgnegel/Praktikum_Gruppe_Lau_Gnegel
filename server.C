/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
#include "TASK1.H"

#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>

#include <cstdio> // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <ctime> //contains various functions for manipulating date and time

#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include "SHA256.H"

#include "SIMPLESOCKET.H"

using namespace TASK1;



class MyServer : public TCPserver{
public:
	MyServer(int portNmb, int maxDataRecv) : TCPserver(portNmb, maxDataRecv){;};
    string rueckgabeSet(int laenge, int zeichen);
    string CharToString(char myChar[]);

protected:
	string myResponse(string input);
	BlackBoxSafe *bbus = nullptr;

};





int main(){
	//using namespace TASK1;
	srand(time(nullptr));


	MyServer srv(2025,64);

	srv.run();
}




string MyServer::myResponse(string input){

	/*
    string input2 = input;

    int i = 0;
    int ssize;
    bool abbruch = false;


    char zeichen = input[0];
    int ascii =  int(zeichen);
    cout << "ascii: " << ascii;

	while (i < input.size() && !abbruch ){
		if (input2[i] ==')'){
			ssize = i;
			abbruch = true;
			cout << "abbruch!!!!" << endl;
		}
		cout << "aktuelles i: " << i << endl;
	 i++;
	}

	input = input2.substr(0,ssize+1);

	cout << "erase2: #" << input << "#    Stringlänge gekürzt;" << input.size() << endl;
    */
	if( input.compare(0,12,"ServerStatus") == 0){
		return string("ready");
        }



	if (input.compare(0,8,"ServerPw") == 0){

		int x; int y;
		int res;
		res = sscanf(input.c_str(),"ServerPw(%d,%d)",&x,&y);
		printf("res = %d, ",res);


             if (res != 2) return string("Error: Argument von ServerPw(i_length,i_Symbols) nicht erkannt");
             if (x < 1) return string("Error: Passwortlänge zu kurz");
             if (x > 30) return string ("Error: Passwortlänge zu groß");
             if (y < 1) return string("Error: Zeichensatz zu kurz");
             if (y > 62) return string ("Error: Zeichensatz zu lang");

		     if(bbus == nullptr){
		         bbus = new BlackBoxSafe(x,y);
		         printf("ServerPw Eingabewerte x: %d y: %d\n",x,y);
		         cout << "Paswort: " << bbus->pwd_ << endl;
		         return string("Eingabe i.o., neues Serverpasswort wurde initial gesetzt");
		     } else {
		    	 delete bbus;

		    	 bbus = new BlackBoxSafe(x,y);
		    	 printf("ServerPw Eingabewerte x: %d y: %d\n",x,y);
		    	 		    	 cout << "Paswort: " << bbus->pwd_<< endl;
		    	 return string("Eingabe i.o., ServerPasswort wurde überschrieben");

		     }





	}

	if (input.compare(0,5,"PwTry") == 0){
        if (bbus == nullptr) return string ("Error: kein passwort initialisiert");

        cout << "PwTry wird durchlaufen" << endl;

        const char *tempPasswortChar = input.c_str();
        char PasswortChar[input.size()];
        int res2;
		res2 = sscanf(tempPasswortChar,"PwTry(%99[^)])", PasswortChar);

		     if (res2 != 1){
		    	 cout << "gelesenes Pw: " << PasswortChar <<"     res2= " << res2 << input << endl;
		         return string("Error: Argument von PwTry(s) wurde nicht erkannt");
		         }
		     else if (res2 == 1){
		    	 cout << "erkanntes Passwort: " << PasswortChar << "     res2= " << res2 << " char size" << input.size() << "  #" << input << "#" << endl;
		         return(bbus -> input(string(PasswortChar)));
		         }
	     }


    if(input.compare(0,9,"PwSymbols") == 0){
    	if (bbus == nullptr) return string ("Error: Kein Passwort initialisiert, Zeichensatz konnte nicht gefunden werden");
   return (bbus -> getSymbols());
    }

	else return ("Error: Befehl wurde nicht erkannt");



};




string MyServer::rueckgabeSet(int laenge, int zeichen){                      //Wandelt die Integer aus der Eingabe in einen
                                                                             //String für die Rückmeldung an den Client um
		string newresponse;
		string satz = "Passwort wurde gesetzt: Länge: ";
		string satz2 = ", Anzahl Zeichen: ";
		stringstream slaenge;
		stringstream szeichen;
		slaenge << laenge;
		szeichen << zeichen;
		newresponse = satz + slaenge.str() + satz2 + szeichen.str();
		return(newresponse);
	}

string CharToString(char myChar[]){
	    string newString(myChar);
	    return(newString);
}






