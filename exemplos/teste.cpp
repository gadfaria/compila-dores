#include<iostream>
#include<cstdio>
using namespace std;

int main() { 
cout<< "Programa teste para o Compila Dores\n\n" ; 
int a , b , c ; 
int d = 0 ; 
a = 10 ; 
cout<< "Digite um valor para b \n" ; 
cin>> b ; 
c = a + b ; 
cout<< "\na = " << a << "\nb = " << b << "\nc = a + b = " << c ; 
cout<< "\n\n" ; 
cout<< "Testando o PARA usando o valor digitado para b \n" ; 
for ( int x = 1 ; 
x <= b ; 
x = x + 1 ) { 
cout<< x << "\n" ; 
} 
cout<< "\n\n" ; 
cout<< "Testando o ENQUANTO usando o valor atribuido para a \n" ; 
while ( d < a ) { 
cout<< d << "\n" ; 
d = d + 1 ; 
} 
cout<< "\n" ; 
if ( b > 10 ) { 
cout<< "B maior que 10" ; 
} 
else { 
if ( b == 10 ) { 
cout<< "B igual a 10" ; 
} 
else { 
cout<< "B menor que 10" ; 
} 
} 
cout<< "\n" ; 
} 
