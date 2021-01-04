#include<iostream>
#include<cstdio>
#include<string>
using namespace std;

int main() { 
int x ; 
int v ; 
cout<< "Digite um numero\n" ; 
cin>> x ; 
v = x % 2 ; 
if ( v == 0 ) { 
cout<< "O numero " << x << " eh par" ; 
} 
else { 
cout<< "O numero " << x << " eh impar" ; 
} 
} 
