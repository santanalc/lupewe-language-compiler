# lupewe-language-compiler

Lingaguem lupewe: 

Pedro Paulo do Prado Neto - 2017014201
Lucas Santana Santos - 2017003173
Weslei Carlos de Melo - 2017018004


Antes de compilar a linguagem lupewe é necessário baixar a seguinte lib no Ubuntu: sudo apt install libboost-regex1.48-dev

Primeiramente é necessario compilar o codigo fonte, usando:

g++ trabalho.cpp -lboost_regex -o lupewe

Apos isso rodar o seguinte comando para rodar o arquivo.lp

./lupewe arquivo.lp

Após a execução do compilador, um executavel com o nome padrão 'a.out' sera criado. Em seguida rodar o seguinte código

./a.out