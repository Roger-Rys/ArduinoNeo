/******ENVIA EL MENSAJE DE POSICION POR DATOS************/
void enviarDatosGPS(String posicion) {
  //CONECTANDO CON SERVIDOR A LA RED
  byte cont = 0;
  bool aRed = false; // No conectar a la red
  bool aServer = true; // Para conectar al servidor
  //Llamar a la funcion "TCP_GPRS"
  while (TCP_GPRS(aRed, aServer) != true) {
    delay(500);
    cont++;
    if (cont % 5 == 0) {
      //Reintentar conexion con modulo A7
      proA7.println("AT+CPOF"); delay(1000);
      configuracionInicial(); // Configuracion inicial
    }
  }
  serialA7();//___

  //ENVIANDO DATOS AL SERVIDOR
  if (posicion.length() > 32) {
    //Llave API del servidor ThingSpeak
    String dato = "GET https://api.thingspeak.com/update?api_key=2CNOXEGVDQ3X35K7";
    dato += String(posicion); //Agrego los valores de posicion obtenidas por el Neo6
    serialA7();//___
    Serial.println(dato);
    serialA7();//___
    delay(200);
    analogWrite(ledSend, encender); //LED "Send" encendido
    proA7.println("AT+CIPSEND"); // Comando AT para enviar 
    delay(500);
    serialA7();
    proA7.println(dato);//Especifivar la variable a enviar
    delay(500);
    serialA7();
    proA7.println((char)26);//Indicador de fin de mensaje
    delay(6000);
    serialA7();
    
    escribirComando("AT+CIPCLOSE", 5000, true); //Cierro conexion con servidor
    serialA7();//___
    delay(500);
    Serial.println("Enviado");//Indicando que se ha enviado el mensaje al servidor
    serialA7();//___
    analogWrite(ledSend, apagar); //Apagar LED Send Indicando el envio del mensaje
    delay(100);
  }
}
