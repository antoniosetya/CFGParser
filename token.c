#include "token.h"

void assign_symbol(Token *T){
/*I.S. Token sembarang yang dikenali stringnya */
/*F.S. Symbol token akan diassign sesuai string token tersebut*/
	char c;

	if(strcmp(String(*T),"Program") == 0){
		strcpy(T->symbol,"P");
	}
	else if(strcmp(String(*T),"..") == 0){
		strcpy(T->symbol,"DP");
	}
	else if(strcmp(String(*T),"integer") == 0){
		strcpy(T->symbol,"INT");
	}
	else if(strcmp(String(*T),"char") == 0){
		strcpy(T->symbol,"CH");
	}
	else if(strcmp(String(*T),"array") == 0){
		strcpy(T->symbol,"AR");
	}
	else if(strcmp(String(*T),"real") == 0){
		strcpy(T->symbol,"RE");
	}
	else if(strcmp(String(*T),"Var") == 0){
		strcpy(T->symbol,"V");
	}
	else if(strcmp(String(*T),"begin") == 0){
		strcpy(T->symbol,"B");
	}
	else if(strcmp(String(*T),"end") == 0){
		strcpy(T->symbol,"E");
	}
	else if(strcmp(String(*T),":=") == 0){
		strcpy(T->symbol,"A");
	}
	else if(strcmp(String(*T),"input") == 0){
		strcpy(T->symbol,"I");
	}
	else if(strcmp(String(*T),"output") == 0){
		strcpy(T->symbol,"O");
	}
	else if(strcmp(String(*T),"do") == 0){
		strcpy(T->symbol,"D");
	}
	else if(strcmp(String(*T),"while") == 0){
		strcpy(T->symbol,"W");
	}
	else if(strcmp(String(*T),"if") == 0){
		strcpy(T->symbol,"IF");
	}
	else if(strcmp(String(*T),"then") == 0){
		strcpy(T->symbol,"TH");
	}
	else if(strcmp(String(*T),"else") == 0){
		strcpy(T->symbol,"S");
	}
	else if(strcmp(String(*T),"repeat") == 0){
		strcpy(T->symbol,"R");
	}
	else if(strcmp(String(*T),"until") == 0){
		strcpy(T->symbol,"U");
	}
	else if(strcmp("<=",String(*T)) == 0 || strcmp("=",String(*T)) == 0 || strcmp("<",String(*T)) == 0 || strcmp(">=",String(*T)) == 0 || strcmp(">",String(*T)) == 0 || strcmp("<>",String(*T)) == 0){
		strcpy(T->symbol,"C");
	}
	else if(strcmp(String(*T),"for") == 0){
		strcpy(T->symbol,"F");
	}
	else if(strcmp(String(*T),"to") == 0){
		strcpy(T->symbol,"T");
	}
	else if(strcmp(String(*T),"downto") == 0){
		strcpy(T->symbol,"DT");
	}
	else if(strcmp(String(*T),"step") == 0){
		strcpy(T->symbol,"ST");
	}
	else if(strcmp(String(*T),"and") == 0){
		strcpy(T->symbol,"AN");
	}
	else if(strcmp(String(*T),"or") == 0){
		strcpy(T->symbol,"OR");
	}
	else if(strcmp(String(*T),"not") == 0){
		strcpy(T->symbol,"N");
	}
	else if(strcmp(String(*T),"div") == 0) {
		strcpy(T->symbol,"IDV");
	}
	else if(strcmp(String(*T),"mod") == 0) {
		strcpy(T->symbol,"MD");
	}
	else if(strlen(String(*T)) == 1 ){
		c = String(*T)[0];
		if( c == '+' ) {
			strcpy(T->symbol,"PL");
		}
		else if (c == '-') {
			strcpy(T->symbol,"SB");
		}
		else if (c == '*') {
			strcpy(T->symbol,"MP");
		}
		else if (c =='/' ) {
			strcpy(T->symbol,"DV");
		}
		else if( c=='.'){
			strcpy(T->symbol,"PR");
		}
		else if( c==';'){
			strcpy(T->symbol,"SC");
		}
		else if( c==':'){
			strcpy(T->symbol,"C");
		}
		else if( c==','){
			strcpy(T->symbol,"CM");
		}
		else if( c=='"'){
			strcpy(T->symbol,"K");
		}
		else if( c=='['){
			strcpy(T->symbol,"OB");
		}
		else if( c==']'){
			strcpy(T->symbol,"CB");
		}
		else if( c=='('){
			strcpy(T->symbol,"OP");
		}
		else if( c==')'){
			strcpy(T->symbol,"CP");
		}
	} else {
		strcpy(T->symbol,"Z");
	}
}
