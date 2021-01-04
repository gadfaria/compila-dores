#include<iostream>
#include<cstdio>
using namespace std;

int main() { 
cout<< "\nPrograma calcular a sequencia de Fibonacci\n\n" ; 
int a = 1 , b = 1 ; 
int num , aux ; 
cout<< "Digite um numero \n" ; 
cin>> num ; 
if ( num > 2 ) { 
cout<< "1 " << " 1  " ; 
} 
while ( num > 2 ) { 
aux = a ; 
a = b ; 
b = b + aux ; 
cout<< b << "  " ; 
num = num - 1 ; 
} 
cout<< "\n" ; 
} 
