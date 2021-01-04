#include<iostream>
#include<cstdio>
#include<string>
using namespace std;

int main() { 
int i ; 
int x ; 
int soma ; 
int fib1 ; 
int fib2 ; 
string a ; 
fib1 = 1 ; 
fib2 = 1 ; 
a = "Digie a posicao da sequencia de fibonacci\n" ; 
cout<< a ; 
cin>> x ; 
x = x - 2 ; 
for ( i = 0 ; 
i < x ; 
i = i + 1 ) { 
soma = fib1 + fib2 ; 
fib1 = fib2 ; 
fib2 = soma ; 
} 
cout<< fib2 ; 
} 
