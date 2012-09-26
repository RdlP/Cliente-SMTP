Pequeño cliente que muestra el funcionamiento del protocolo SMTP con autenticación.

Para que funcione correctamente hay que modificar las siguientes líneas

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

 - Esto es, elegir que servidor vamos a usar (Hotmail, Gmail, Universidad de Murcia, etc) comentando o descomentando las líneas de código
 - En from_id el vuestro correo en base64
 - En password vuestra contraseña en base64
 - En to_id hacia la persona ala que va dirigida el correo (sin base64)
 - Y el asunto y el cuerpo del mensaje

Para compilarlo basta ejecutar

g++ mail.cpp -lssl -o mail

Para que permita compilarlo hay que instalar la libreria openssl (en su versión de desarrollo [DEV o DEVEL])

Para ejecutarlo basta con ejecutar ./mail

LICENCIA

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details at
   http://www.gnu.org/copyleft/gpl.html
   AUTOR: Ángel Luis Perales Gómez
