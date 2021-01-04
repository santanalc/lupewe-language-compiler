#include <boost/regex.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cstring>
#include <algorithm>
#include <sstream>

#define V_TERMINAIS 4// define quantidade de vertices que será terminais no automato da analise sintática

using namespace std;
using namespace boost;

// Define uma tripla para um Token do codigo  'TIPOTOKEN , TOKEN , Linha que o token se encontra'
struct triple{            
    string first,second;
    int third;
    triple(){}
    triple(string a, string b, int c){
        first = a;
        second = b;
        third =c;
    }
};

vector<pair<string,regex> > expressoes;  // Vetor com cada regex que sera utilizado para a identificação dos tokens
map<int,map<string,int> > Automato; // Automato que sera criado para a analise sintatica que contem todas as regras da linguagem
bool vterminal[1000]; // Determina os vertices terminais do Automat
map<string,bool> variaveis[1000]; // Variaveis que ja foram utilizadas.
int tokenAtual; 
vector<triple> SequenciaTokens; // É um vetor que guarda cada token do código durante a análise léxica.
        
int line; // Variavel que indica a linha do codigo fonte que esta sendo processada durante a analise lexica.
          
// Função que transforma um numero do tipo int para uma string
string intToString(int Number){ 
    string Result;         
    ostringstream convert; 
    convert << Number;      
    return convert.str();     
}

// Função para resetar o contador do token que esta sendo analizado durante a análise sintática
void resetContTokens(){ 
    tokenAtual = 0;
}

// Função que recebe um triple como referencia, e retorna true caso for possível pegar o próximo token.
bool nextLineToken(triple &aux){  
    if(tokenAtual==SequenciaTokens.size())
        return false;
    aux =  SequenciaTokens[tokenAtual++];
    return true;
}

// Funcao responsável pelo o carregamento de todo o automato  apartir de um txt 
void carregaAutomato(){ 
    memset(vterminal,0,sizeof vterminal);

    // Vertices terminais Pre atribuidos
    int termina[] = {5,40,53,59}; 

    for(int i = 0; i<V_TERMINAIS;i++)
        vterminal[termina[i]] = true;

    char str[1000];
    FILE *arq = fopen ("automato.txt","r");
    while(fgets(str,1000,arq)!=NULL){
       if(strlen(str)<4)continue;
       string n[3];
       int aux = 0;
       for(int i = 0;i<strlen(str);i++){
         if(str[i] == ' '){aux++;continue;}
         n[aux].push_back(str[i]);
       } 
       int v = atoi(n[0].c_str()), u = atoi(n[2].c_str());
       Automato[v][n[1]] = u;
    }

    fclose(arq);
}

// Funcao responsavel por todo o carregamento das expressões regulares.
void carregaExpressoes(){ 
    
   
   expressoes.push_back(make_pair("INICIO_PROGRAMA",regex("(inicioPrograma)(.*)")));
    expressoes.push_back(make_pair("FIM_PROGRAMA",regex("(fimPrograma)(.*)")));
    
    expressoes.push_back(make_pair("INTEIRO",regex("(inteiro)(.*)")));
    expressoes.push_back(make_pair("CARACTER",regex("(caracter)(.*)")));
    expressoes.push_back(make_pair("REAL",regex("(real)(.*)")));
    expressoes.push_back(make_pair("CADEIA",regex("(cadeia)(.*)")));
    expressoes.push_back(make_pair("BOOLEANO",regex("(booleano)(.*)")));

    expressoes.push_back(make_pair("ENTRADA",regex("(entra)(.*)")));
    expressoes.push_back(make_pair("SAIDA",regex("(sai)(.*)")));

    expressoes.push_back(make_pair("ABREPARENTESES",regex("(\\()(.*)")));
    expressoes.push_back(make_pair("FECHAPARENTESES",regex("(\\))(.*)")));
    expressoes.push_back(make_pair("INIBLOCO",regex("(\\{)(.*)")));
    expressoes.push_back(make_pair("FIMBLOCO",regex("(\\})(.*)")));
    
    expressoes.push_back(make_pair("ENQUANTO",regex("(enquanto)(.*)")));
    expressoes.push_back(make_pair("PARA",regex("(para)(.*)")));
    expressoes.push_back(make_pair("SENAO",regex("(senao)(.*)")));
    expressoes.push_back(make_pair("SE",regex("(se)(.*)")));

    expressoes.push_back(make_pair("LOGICO_E",regex("(e)(.*)")));
    expressoes.push_back(make_pair("LOGICO_OU",regex("(ou)(.*)")));
    expressoes.push_back(make_pair("LOGICO_NAO",regex("(nao)(.*)")));
    expressoes.push_back(make_pair("TRUEFALSE",regex("(true)(.*)")));
    expressoes.push_back(make_pair("TRUEFALSE",regex("(false)(.*)")));

    expressoes.push_back(make_pair("DIFERENTE",regex("(~=)(.*)")));
    expressoes.push_back(make_pair("IGUAL",regex("(==)(.*)")));
    expressoes.push_back(make_pair("MAIOR_IGUAL",regex("(>=)(.*)")));
    expressoes.push_back(make_pair("MENOR_IGUAL",regex("(<=)(.*)")));
    expressoes.push_back(make_pair("MAIOR",regex("(>)(.*)")));
    expressoes.push_back(make_pair("MENOR",regex("(<)(.*)")));


    expressoes.push_back(make_pair("ATRIBUICAO",regex("(=)(.*)")));

    expressoes.push_back(make_pair("ADD",regex("(\\+)(.*)")));
    expressoes.push_back(make_pair("SUB",regex("(\\-)(.*)")));
    expressoes.push_back(make_pair("MULT",regex("(\\*)(.*)")));
    expressoes.push_back(make_pair("DIV",regex("(\\/)(.*)")));
    expressoes.push_back(make_pair("RESTO",regex("(resto)(.*)")));

    expressoes.push_back(make_pair("PVIRGULA",regex("(\\;)(.*)")));


    expressoes.push_back(make_pair("NUM",regex("(((\\d)+(\\.)(\\d)+)|((\\d)+))(.*)")));
    // expressoes.push_back(make_pair("NUM_REAL",regex("((\\-)?)(((\\d)+(\\.)(\\d)+)(.*)")));
    expressoes.push_back(make_pair("LETRA",regex("('(.*)')(.*)")));
    expressoes.push_back(make_pair("VAR",regex("((([A-Z]+[0-9]*)|([a-z]+[0-9]*))+)(.*)")));
     expressoes.push_back(make_pair("STRING",regex("(\\\"([A-Z]|[a-z]|[0-9]|\\s|\\+|\\-|\\*|\\=|\\&|\\$|\\\\%|\\.|\\,|\\\\)*\\\")(.*)")));
    
}

// Analise Lexica
bool analiseLexica(string str){ 
    // A variavel str, é cada linha do codigo contendo uma sequencia de tokens
    bool flag = true;
    if(str=="")return true;
    if(str[str.size()-1] == '\n')str = str.substr(0,str.size()-1);
    if(str=="")return true;
    
    while(str!=""){
        while(str!="" && (str[0]==' ' || str[0]=='\t'))
            str = str.substr(1,str.size()-1);
        if(str=="")break;

        flag = false;

        for(int i = 0;i<expressoes.size();i++){
            smatch m;
            if(regex_match(str,m,expressoes[i].second)){ 
                SequenciaTokens.push_back(triple(expressoes[i].first,m[1],line)); 
                str = m[m.size()-1];
                flag = true;
                break;
            }
        }

        if(!flag)break;
    }
    return flag;
}


//Analise Sintatica
vector<string> AnaliseSintatica(){

    //No final do programa a variável tem que ser igual a 0 indicando que tudo que abre, fecha
    int P=0,B=0;

    //É um vetor que guarda todos os erros de sintaxe
    vector<string> err;

    int atual = 1;
    triple aresta;
    bool vemvariavel = false;

    while(nextLineToken(aresta)){
        if(Automato[atual].count(aresta.first)<=0){
            err.push_back("'" + aresta.second+"'" + " não esperado na linha  "+intToString(aresta.third)+"\n");
            vemvariavel = false;
            continue;
        }

        if(B<0){
            err.push_back("'}' não esperado na linha "+intToString(aresta.third)+"\n");
            atual = 1; 
            vemvariavel = false;
            continue;
        }
        if(P<0){
            err.push_back("')' não esperado na linha  "+intToString(aresta.third)+"\n");
            atual = 1;
            vemvariavel = false;
            continue;
        }

        atual = Automato[atual][aresta.first];

        if( aresta.first == "INIBLOCO")B++;
        if(aresta.first == "FIMBLOCO" || aresta.first == "FIM_PROGRAMA"){
            variaveis[B].clear();
            B--;
        }
        if(aresta.first == "ABREPARENTESES")P++;
        if(aresta.first == "FECHAPARENTESES")P--;
        if(aresta.first == "INTEIRO" || aresta.first == "REAL" || aresta.first == "CARACTER" || aresta.first == "CADEIA" || aresta.first == "BOOLEANO"){
            vemvariavel = true;
        }
        if(aresta.first == "VAR"){
            if(vemvariavel){
                bool ok = false;
                for(int i = 0;i<=B && !ok;i++)
                    ok = variaveis[i][aresta.second];
                if(ok){
                    err.push_back("'" + aresta.second + "'" + " variável já declarada "+intToString(aresta.third)+"\n");
                    vemvariavel = false;
                }
                vemvariavel = false;
                variaveis[B][aresta.second] = true;
               
            }else{

                bool ok = false;
                for(int i = 0;i<=B && !ok;i++)
                    ok = variaveis[i][aresta.second];
                
                if(!ok){
                    err.push_back("Variável '"+aresta.second+"' não declarado na linha "+intToString(aresta.third)+"\n");
                    vemvariavel = false;
                }

            }
        }
        if(vterminal[atual]){
            if(P!=0)
                err.push_back("Erro no abre ou fecha parenteses"+intToString(aresta.third)+"\n");
            atual = 1;
            P  = 0;
        }
    }

    if(P || B){
        err.push_back("Error\n");
    }

    return err;
}

//Funcao para traduzir o codigo para c++
void traducao(string x){
    FILE *arq = fopen ("traducao.txt","r");
    map<string,string> trad;
    char str[1000];

    while(fgets(str,1000,arq)!=NULL){
        string aux;
        string aux2;
        bool flag = false;
        for(int i = 0 ;i<strlen(str);i++){
            if(str[i] == ' ' || str[i]=='\n'){flag = true;continue;}
            if(!flag)aux.push_back(str[i]);
            else aux2.push_back(str[i]);
        }
        
        trad[aux] = aux2;
    }
    
    fclose(arq);
    resetContTokens();

    arq = fopen(x.c_str(),"w");
    
    bool in = false,out = false;
    
    triple tk;

    fputs("#include<iostream>\n#include<cstdio>\n#include<string>\nusing namespace std;\n\n",arq);
    
    //Tradução do token para c++
    while(nextLineToken(tk)){
        if(trad.count(tk.second)>0){       
            if (tk.second == "inicioPrograma"){
                fputs(string("int ").c_str(), arq);
            }
            if(tk.second == "sai" && out == false){
                out = true ;
                fputs(string("cout").c_str(),arq);
            }
            if(tk.second == "entra" && in == false){
                in = true ;
                fputs(string("cin").c_str(),arq);
            }
            fputs((trad[tk.second]+" ").c_str(),arq);
        }else{
        if(tk.first == "INTEIRO" || tk.first == "REAL" || tk.first == "CARACTER" || tk.first == "CADEIA" || tk.first == "BOOLEANO")continue;
             fputs((tk.second+" ").c_str(),arq);
            
        }
        if(tk.second == "{" ||tk.second == ";" || tk.second == "}")
            fputs(string("\n").c_str(),arq);

        if(tk.second == ";" ){
            in = false;
            out = false;
        }

    }
    fclose(arq);
    
}

int main(int argc, char* argv[]) {

    int len = strlen(argv[1]);
    //Confere se a extensao do arquivo possui .lp
    if(len<4 || tolower(argv[1][len-1])!='p' || tolower(argv[1][len-2])!='l' || argv[1][len-3]!='.'){
        cout<<"Extenção do arquivo deve ser .lp"<<endl;
        return -1;
    }
    
    //Carrega o código .lp
    FILE *arq;
    arq = fopen (argv[1],"r");
    if(arq == NULL){
        cout<<"Arquivo não existe"<<endl;
        return -1;
    }
    
    //Carrega as expressoes regulares (REGEX)
    carregaExpressoes();
    
    //Le linha por linha do codigo para realizar a analise lexica
    char str[1000];
    line = 1;
    bool error = false;
    while(fgets(str,1000,arq)!=NULL){

       if(!analiseLexica(string(str))){
           error = true;
           cout<<"Erro na linha: "<<line<<endl;
       }
       line++;
    }
    fclose(arq);

    //Caso a análise lexica retorne um erro, já para o programa 
    if(error)return -1;

    //Gera o arquivo resultada da Analise Lexica
    arq = fopen((string(argv[1],argv[1]+len-2)+"o").c_str(), "w");
    for(int i = 0;i<SequenciaTokens.size();i++){
        fputs((SequenciaTokens[i].first + " " + SequenciaTokens[i].second+"\n").c_str(),arq);
    }
    fclose(arq);

    carregaAutomato();
    resetContTokens();
    vector<string> Erros = AnaliseSintatica();
    if(Erros.size()!=0){
        for(int i = 0 ;i<Erros.size();i++){
            cout<<Erros[i];
        }
        return -1;
    }
    
    // Traducao do codigo para c++
    string nomeCode = string(argv[1],argv[1]+len-2)+"cpp";
    traducao(nomeCode);
    // Compilar o codigo traduzido utilizando o gcc
    system(string("g++ "+nomeCode).c_str());


    return 0;
}

