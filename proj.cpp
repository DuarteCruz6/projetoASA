#include <vector>
#include <sstream>
using namespace std;

int numLinesMatrix, totalNumbersEquation;
vector<vector<int> > Matrix;
vector<int> Numbers;
int resultadoEquacao;


struct Celula{
    int result; //resultado que esta célula deu
    Celula* primeiraCelula;  //ponteiro para a primeira célula que foi usada para criar esta
    Celula* segundaCelula;  //ponteiro para a segunda célula que foi usada para criar esta
};

vector<vector<vector<Celula*> > > Res;
Celula* solucao;

void createMatrix(){

    for (int i = 0; i < numLinesMatrix; ++i) {
        for (int j = 0; j < numLinesMatrix; ++j) {
            scanf("%d",&Matrix[i][j]);
        }
    }
}

void createEquation(){
    //cria a equacao e a tabela com a diagonal 0 preenchida
    for (int i = 0; i < totalNumbersEquation; ++i) {
        scanf("%d",&Numbers[i]);
        Celula* novaCelula = new Celula{Numbers[i],nullptr,nullptr};    //celula da diagonal 0: é o numero da equacao 
                                                                        //(nao tem conta nem parentesis)
        Res[i][i].push_back(novaCelula); 
    }
}

void calculaCelula(int linha, int diagonal){
    int colunaAtual = linha+diagonal;
    int possibilidadesCelula = 0;
    vector<bool> resultadosCelula(numLinesMatrix, false);    //para verificar se o resultado da equacao ja esta usado

    for(int a=0;a<diagonal;a++){
        //celula atual = Res[linha][colunaAtual]
        //celula esquerda = Res[linha][colunaAtual-1-a]
        //celula baixo = Res[colunaAtual-a][colunaAtual]
        for(Celula* celulaEsquerda : Res[linha][colunaAtual-1-a]){  
            for(Celula* celulaBaixo : Res[colunaAtual-a][colunaAtual]){
                int resultado= Matrix[celulaEsquerda->result-1][celulaBaixo->result-1];
                if(!resultadosCelula[resultado-1]){
                    //ainda nao foi usado
                    resultadosCelula[resultado-1]=true;
                    possibilidadesCelula++;
                    Res[linha][colunaAtual].push_back(new Celula{resultado,celulaEsquerda,celulaBaixo}); 
                    if(possibilidadesCelula==numLinesMatrix){
                        //ja tem todas as possibilidades
                        return;
                    }
                }
            }
        }
    }
}

void updateMatrixCheat(){
    for(int diagonal=1;diagonal<totalNumbersEquation;diagonal++){
        for(int linha=0;linha<totalNumbersEquation-diagonal;linha++){
            //celula atual = Res[linha][linha+diagonal]
            calculaCelula(linha, diagonal);
        }
    }     
}

Celula* choseSolution(){
    for(Celula* celula : Res[0][totalNumbersEquation-1]){
        if(celula->result==resultadoEquacao){
            return celula;      //retorna a celula cujo resultado deu correto. as celulas
                                //ja tao ordenadas pela parentizacao mais a esquerda
        }
    }
    return nullptr;
}

string meteParentesis(Celula* celula){
    if(celula -> primeiraCelula == nullptr){
        return to_string(celula -> result);
    }
    return "(" + meteParentesis(celula -> primeiraCelula) + " " + meteParentesis(celula -> segundaCelula) + ")";
}

void clearMemory(){
    for(int i=0; i<totalNumbersEquation;i++){
        for(int j=0; j<totalNumbersEquation;j++){
            for(Celula* celula : Res[i][j]){
                delete celula;
            }
            Res[i][j].clear();
        }
        Res[i].clear();
    }
    Res.clear();
}

int main(){
    scanf("%d %d",&numLinesMatrix,&totalNumbersEquation);

    Matrix.resize(numLinesMatrix, vector<int>(numLinesMatrix));
    Numbers.resize(totalNumbersEquation);
    Res.resize(totalNumbersEquation, vector<vector<Celula*> >(totalNumbersEquation));

    createMatrix();       //cria a matriz com os resultados das operacoes
    createEquation();     //cria uma lista com os numeros da equacao e 
                          //também a tabela com a diagonal 0 preenchida

    scanf("%d",&resultadoEquacao);

    updateMatrixCheat();    //atualiza a minha tabela até ao fim

    solucao = choseSolution();

    if(solucao){
        printf("%d\n",1);
        printf("%s\n",meteParentesis(solucao).c_str());
    }else{
        printf("%d\n",0);
    }

    clearMemory();
}