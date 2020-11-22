/******ENVIA EL MENSAJE DE POSICION POR DATOS*************/
void enviarDatosGPS() {
  String posicion = "";
  delay(500);

  ////////RECEPTAR DATOS DE GPS
  serialA7();//___
  delay(500);
  //GPS ACTIVADO
  Serial.println("GPS on");
  do {
    posicion = SerialDatosGPS(); // Lee los datos GPS y guardar valores en variable
    delay(500); //500
  } while (posicion.length() <= 32);
  delay(250);//500
  serialA7();//__
  ////////DESACTIVA GPS
  delay(100);
  digitalWrite(ledGPSon, LOW); //LED para GPSon
  delay(500);//1000
  serialA7();//___

  //CONECTANDO CON SERVIDOR A LA RED
  byte cont = 0;
  bool aRed = false;
  bool aServer = true;
  while(TCP_GPRS(aRed, aServer)!=true){
    delay(500);
    cont++;
    if(cont%5==0){
      //Reintentar conexion con modulo A7
      proA7.println("AT+CPOF"); delay(1000);
      configuracionInicial(); // Configuracion inicial
    }
  }
  serialA7();//___
  if (posicion.length() > 32) {
    String dato = "GET https://api.thingspeak.com/update?api_key=2CNOXEGVDQ3X35K7";
    dato += String(posicion);
    serialA7();//___
    Serial.println(dato);
    serialA7();//___
    delay(200);
    digitalWrite(ledSend, HIGH); //LED para Send
    proA7.println("AT+CIPSEND");
    delay(500);
    serialA7();
    proA7.println(dato);
    delay(500);
    serialA7();
    proA7.println((char)26);
    delay(5000);
    serialA7();

    escribirComando("AT+CIPCLOSE", 5000, true);
    serialA7();//___
    delay(500);
    digitalWrite(ledSend, LOW); //LED para Send Indicador de envio de mensaje
    delay(500);
    serialA7();//___
    
    Serial.println("Enviado");
    serialA7();//___
    digitalWrite(ledSend, LOW);  //Apagar indicador de envio de mensaje
    delay(100);
  }
}
