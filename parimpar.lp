inicioPrograma{
	inteiro x;
	inteiro v;
	sai "Digite um numero\n";
	entra x;
	v = x resto 2;
	se(v == 0){
		sai "O numero " sai x sai " eh par";
	}
	senao{
		sai "O numero " sai x sai " eh impar";
	}
}