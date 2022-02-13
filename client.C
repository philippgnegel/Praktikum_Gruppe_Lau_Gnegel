/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h> //contains various constants


#include "SIMPLESOCKET.H"

using namespace std;

bool PwShift(TCPclient objekt, unsigned long long *pzaehler, int array[] , int stelle, int Buchstaben, int maxStelle);
string BefehlServerPw(int laenge, int zeichen);
string BefehlPwTry(string Pw);
string NumbertoString(int *array, int sizePw);

int main(int argc, char* argv[]) {
	srand(time(NULL));

	int Modus;
	int StatisticTrys;
	int MaxPwLength;
	int MaxPwSymbols;


	if (argc != 5){
		cout << "Error: Kommandoparameter nicht erkannt: eingabe erfordert"
				<< endl <<"./client <modus> <statistikversuche> <Passwortlänge> <Symbolanzahl>"
				<< endl <<"Modus 1: Einzelprüfung" << endl << "Modus 2: Gruppenprüfung" << endl;

		return 0;
	}
    sscanf(argv[1], "%i", &Modus);
	sscanf(argv[2], "%i", &StatisticTrys);
	sscanf(argv[3], "%i", &MaxPwLength);
	sscanf(argv[4], "%i", &MaxPwSymbols);

	cout << "Programmausführung mit " << StatisticTrys << " Versuchen, einer Maximalen Passwortlänge von " << MaxPwLength <<
			" Zeichen und einer Zeichensatzlänge von " << MaxPwSymbols << endl;


	string host = "localhost";                                    //starte Server
    string msg;
	TCPclient c;

	c.conn(host , 2025);

	    string initmsg = "ServerStatus";                           //Verbindung prüfen
	    cout << "Serverstatus wird abgefragt, Serverstatus: ";
	    c.sendData(initmsg);
	    string statusmsg = c.receive(64);
	    if (statusmsg.compare(0,5,"ready") != 0){
	    cout << "nicht online" << endl;
	    msg = string ("BYEBYE");
	                c.sendData(msg);
	                msg = c.receive(64);
	                return 0;
	    }












    cout << "online" << endl;
	//bool goOn=1;
	//while(goOn){ // send and receive data
   if (Modus == 1){
        int zaehlergesamt = 0;

        for(int i = 0; i < StatisticTrys; i++){

			unsigned long long zaehler = 0;
				int arrayOfNum[MaxPwLength] = {0};
				int arrSize = MaxPwLength -1;
				int arrayMaxPwSymbols = MaxPwSymbols - 1;



				   msg = BefehlServerPw(MaxPwLength,MaxPwSymbols);                //Befehl zum Initialisieren des Passworts
				   cout << "client sends:" << msg << endl;
                   c.sendData(msg);
                   msg = c.receive(64);
                   cout << "got response:" << msg << endl;                         //PW.Initialisieren
			       PwShift (c, &zaehler, arrayOfNum, 0, arrayMaxPwSymbols, arrSize);
			       zaehlergesamt = zaehlergesamt + zaehler;
				}

            int avgzaehler = zaehlergesamt / StatisticTrys;
            cout << "Durchschnitt von " << StatisticTrys << " Versuchen: " << avgzaehler << " Attacken bis zum richtigen Passwort" << endl;

            msg = string ("BYEBYE");
            c.sendData(msg);
            msg = c.receive(64);
            return 0;
   }

   if (Modus == 2 ){

	   unsigned long long Ausgabe[MaxPwSymbols][MaxPwLength];

	   for (int SymbolsZaehler = 1; SymbolsZaehler <= MaxPwSymbols; SymbolsZaehler ++){
         for (int NumZaehler = 1; NumZaehler <= MaxPwLength; NumZaehler ++){
        	 unsigned long long zaehlergesamt = 0;

	           for(int z = 0; z < StatisticTrys; z++){

	        	    unsigned long long zaehler = 0;
	   				int arrayOfNum[NumZaehler] = {0};
	   				int arrSize = NumZaehler -1;
	   				int arrayMaxPwSymbols = SymbolsZaehler - 1;



	   				   msg = BefehlServerPw(NumZaehler,SymbolsZaehler);                //Befehl zum Initialisieren des Passworts
	   				   //cout << "client sends:" << msg << endl;
	                      c.sendData(msg);
	                      msg = c.receive(64);
	                      //cout << "got response:" << msg << endl;                         //PW.Initialisieren
	   			       PwShift (c, &zaehler, arrayOfNum, 0, arrayMaxPwSymbols, arrSize);
	   			       zaehlergesamt = zaehlergesamt + zaehler;
	   				}



	               unsigned long long avgzaehler = zaehlergesamt / StatisticTrys;
	               Ausgabe [SymbolsZaehler - 1][NumZaehler - 1] = avgzaehler;          //Ausgabe[Zeile][Spalte];    Werte im Array Speichern
	               cout << "PW-Länge: " << NumZaehler << " Zeichensatz: " << SymbolsZaehler << endl;
	               cout << "Durchschnitt von " << StatisticTrys << " Versuchen: " << avgzaehler << " Attacken bis zum richtigen Passwort" << endl;
	               avgzaehler = 0;

         }
	   }

	   cout << endl << endl << "Ausgabe der Statistik nach " << StatisticTrys << " Versuchen "<< endl
			   <<"zeilen: Anzahl der Symbole: " << MaxPwSymbols << endl << "Spalten: Anzahl maximaler PW-Länge: " << MaxPwLength << endl << endl;


       cout << "PW-Länge:  ";
       for (int i = 1; i <= MaxPwLength; i++){
    	   cout << i << " ";
       }

       cout << endl << "___________";
              for (int i = 1; i <= MaxPwLength; i++){
           	   cout << "__";
              }
       cout << endl;



	   for (int SymbolsZaehler = 0; SymbolsZaehler < MaxPwSymbols; SymbolsZaehler ++){
		   cout << SymbolsZaehler + 1 << " Symbole: ";
	            for (int NumZaehler = 0; NumZaehler < MaxPwLength; NumZaehler ++){

	            	cout << Ausgabe[SymbolsZaehler][NumZaehler] << " " ;

	            }
	            cout << endl ;

	   }


	               msg = string ("BYEBYE");
	               c.sendData(msg);
	               msg = c.receive(64);
	               return 0;
   }


	//}
}

bool PwShift(TCPclient objekt, unsigned long long *pzaehler, int array[] , int stelle, int Buchstaben, int maxStelle){

	int fest = stelle;

	for (int i= 0; i <= Buchstaben; i++){
		stelle = fest;
		array[stelle] = i;
		if (stelle == maxStelle){
			string PwAttack = NumbertoString(array, maxStelle);
			//cout << "Passwort wird versucht:" << PwAttack << endl;
			string PwTry = BefehlPwTry(PwAttack);
			//cout << "client sends:" << PwTry << endl;
			objekt.sendData(PwTry);
			string msg = objekt.receive(64);
			//cout << msg << endl;
            string test = msg;

			(*pzaehler)++;

			if (test.compare(0,15,"ACCESS ACCEPTED") == 0){
				cout << "Server Successful Attacked, needed Trys: " << *pzaehler << endl << "PW is: " << PwAttack << endl ;

				return true;
			}


		}

		/*if (stelle == 0){
			int status = i * 10;
			cout << "mit " << status << "% der moeglichen Passwoerter angegriffen << endl";
		}*/

		if (stelle < maxStelle){
			stelle = stelle + 1;
			bool abbruch = PwShift(objekt, pzaehler, array, stelle, Buchstaben, maxStelle);
			if (abbruch == true){
				return true;
			}
		}
	}
return false;

}



string NumbertoString (int *array, int sizePw){
	sizePw = sizePw + 1;
	const string SYMBOLS = "ABCDEFGHIJKLMNOPQRTSTUVWXYZabcdefghijklmopqrstuvwxyz0123456789";
	string pwd("");

	for (int i = 0; i < sizePw; i++){
		pwd += SYMBOLS[array[i]];
	}

	return string (pwd);

}

string BefehlServerPw(int laenge, int zeichen){                      //Wandelt die Integer aus der Eingabe in einen
                                                                             //String für die Rückmeldung an den Client um
		string newresponse;
		string Befehl1 = "ServerPw(";
		string Befehl2 = ",";
		string Befehl3 = ")";
		stringstream slaenge;
		stringstream szeichen;
		slaenge << laenge;
		szeichen << zeichen;
		newresponse = Befehl1 + slaenge.str() + Befehl2 + szeichen.str() + Befehl3;
		return(newresponse);
}

string BefehlPwTry(string Pw){
	    string newresponse;
	    string Befehl1 = "PwTry(";
	    string Befehl2 = ")";
	    newresponse = Befehl1 + Pw + Befehl2;
	    return(newresponse);
}














