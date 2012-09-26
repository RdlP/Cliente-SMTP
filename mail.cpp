/* This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details at
   http://www.gnu.org/copyleft/gpl.html
   AUTOR: Ángel Luis Perales Gómez
*/



#include<iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


using namespace std;

int sock; /*Socket inseguro*/
SSL *ssl; /*Socket seguro*/
SSL_CTX *ctx;
struct sockaddr_in server;
struct hostent *hp, *gethostbyname();
char buf[BUFSIZ+1];
int len;
//char *host_id="65.55.172.254"; //hotmail también puede ser "smtp.live.com"
char *host_id="smtp.gmail.com"; //gmail
//char *host_id="smtp.um.es"; //Universidad de Murcia
//TODO
//Implementar métodos de encode y decode en base64
char *from_id="AQUI EL CORREO EN BASE64";
char *password="AQUI LA PASSWORD EN BASE64";
char *to_id="CORREO DEL DESTINATARIO sin base64";
char *asunto="Prueba\r\n";
char cuerpo[100]="CORREO\r\n";

/*=====Enviar una cadena al socket=====*/

void send_socket(char *s)
{
    write(sock,s,strlen(s));
    write(1,s,strlen(s));
}

/*=====Enviar una cadena al socket cifrado=====*/
void send_SSLsocket(char *s)
{
    write(1,s,strlen(s));
	SSL_write(ssl,s,strlen(s));
}

//=====Leer una cadena desde el socket=====*/

void read_socket()
{
    len = read(sock,buf,BUFSIZ);
    write(1,buf,len);
}

//=====Leer una cadena desde el socket cifrado=====*/

void read_SSLsocket()
{
	len = SSL_read(ssl,buf,BUFSIZ);
    write(1,buf,len);
}


    /*=====MAIN=====*/

int main(int argc, char* argv[])
{

	/*====Variables necesarias para la conexión SSL=====*/
	SSL_library_init ();
    SSL_load_error_strings();

	ctx=SSL_CTX_new(SSLv3_client_method());
	ssl = SSL_new(ctx);
	 
    /*=====Creamos el socket=====*/
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock==-1)
    {
     perror("No se puede crear al socket");
     exit(1);
    }
    else
      cout << "socket creado\n";
    /*=====Verificamos el host=====*/
    server.sin_family = AF_INET;
    hp = gethostbyname(host_id);
    if (hp==(struct hostent *) 0)
    {
     fprintf(stderr, "%s: host desconocido\n", host_id);
     exit(2);
    }

    /*=====Conectamos con el puerto SMTP (25)=====*/
	cout << "Host ID: " << host_id << "\n";
	
	//cout << "IP: " << inet_ntoa(hp->h_addr) << "\n";
	
    memcpy((char *) &(server.sin_addr), (char *) hp->h_addr, hp->h_length);
    server.sin_port=htons(25); /* SMTP PORT */
    if (connect(sock, (struct sockaddr *) &server, sizeof server)==-1)
    {
     perror("conectado socket");
     exit(1);
    }
    else
    	cout << "Conectado!\n";
    //PROTOCOLO SMTP http://es.wikipedia.org/wiki/Simple_Mail_Transfer_Protocol
	//Autenticación en SMTP http://www.fehcom.de/qmail/smtpauth.html
    read_socket(); /* Primero se comunica con nosotros el Servidor */
    send_socket("EHLO\r\n"); /* Le respondemos */
    read_socket(); /*Leemos su respuesta */
    send_socket("STARTTLS"); /*Iniciamos el TLS */
    send_socket("\r\n");
	read_socket();
	SSL_set_fd(ssl, sock); /*Asociamos el socket a la conexión ssl*/
	SSL_connect(ssl);
	send_SSLsocket("EHLO\r\n"); /*Al estar sobre conexión segura tenemos que empezar la comunicación desde 0*/
	read_SSLsocket();
	send_SSLsocket("AUTH LOGIN\r\n");
	read_SSLsocket(); /*Aquí el servidor nos devolverá "username: " cifrado en base64*/
	send_SSLsocket("%s\r\n",from_id);
	read_SSLsocket(); /*Aquí el servidor nos devolverá "password: " cifrado en base64*/
	send_SSLsocket("%s\r\n",password);
	read_SSLsocket(); /*Si todo ha ido correcto nos autenticaremos en el servidor SMTP*/
    send_SSLsocket("MAIL FROM: <"); /*Correo de*/
    send_SSLsocket(from_id);
    send_SSLsocket(">\r\n");
    read_SSLsocket(); /* Todo OK */
    /*send_SSLsocket("VRFY "); ¿OBSOLETO?
    send_SSLsocket(from_id);
    send_SSLsocket("\r\n");
    read_SSLsocket(); // Todo OK */
    send_SSLsocket("RCPT TO: <"); /*Correo para */
    send_SSLsocket(to_id);
    send_SSLsocket(">\r\n");
    read_SSLsocket(); // Remitente OK */
    send_SSLsocket("DATA\r\n");// Cuerpo del mensaje */
	read_SSLsocket();
    send_SSLsocket("Subject: ");
    send_SSLsocket(asunto);
    //read_SSLsocket(); // Remitente OK*/
    send_SSLsocket(cuerpo);
    send_SSLsocket(".\r\n");
    read_SSLsocket(); 
    send_SSLsocket("QUIT\r\n"); /* Salimos */
    read_SSLsocket(); // Salimos */

    //=====Salimos y finalizamos=====*/
    close(sock);
    exit(0);
  }
