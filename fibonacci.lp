inicioPrograma{
  
  inteiro i;
  inteiro x;
  inteiro soma;
  inteiro fib1;
  inteiro fib2;
  cadeia a;

  fib1 = 1;
  fib2 = 1;
  
  a = "Digie a posicao da sequencia de fibonacci\n";
  sai a;
  entra x;
  
  x = x - 2;

  para(i=0;i<x;i= i+1){
    soma = fib1 + fib2;
    fib1 = fib2;
    fib2 = soma;
  }

  sai fib2;
}