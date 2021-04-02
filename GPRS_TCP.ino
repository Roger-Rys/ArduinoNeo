bool TCP_GPRS(bool aRed, bool aServer) {
  if (aRed) { //Condicion para conectar a la red
    bool creg = false, cgatt = false, cgdcont = false, cgact = false;
    creg = escribirComando("AT+CREG?", 2000, false); //Consultar regristro de red
    delay(100);
    serialA7();//___
    cgatt = escribirComando("AT+CGATT=1", 6000, false);//
    delay(100);
    serialA7();//___
    //Configuracion del paramtro PDP
    cgdcont = escribirComando("AT+CGDCONT=1,\"IP\",\"CMNET\"", 2000, false); //PDP - The process on the Security Gateway responsible for collecting and sharing identities.(URL= https://sc1.checkpoint.com/documents/R76/CP_R76_CLI_WebAdmin/66477.htm)
    delay(100);
    serialA7();//___
    //Activar PDP, Abrir servicios de internet
    cgact = escribirComando("AT+CGACT=1,1", 10000, false);              //Activate PDP, open Internet service
    delay(100);
    serialA7();//___
    if (creg && cgatt && cgdcont && cgact && !aServer) {
      Serial.println("Conectado a Red");
      return true;
    }
  }
  if (aServer) { //Condicion para conectar al servidor
    bool confirmar = false;
    serialA7();
    //Establecer conexion con servidor ThingSpeak
    confirmar = escribirComando("AT+CIPSTART=TCP,api.thingspeak.com,80", 10000, true);
    if (confirmar) {
      analogWrite(onRed, encender);//Enciende Led "RED"
      return true; //Retorna Verdadero
    }
    analogWrite(onRed, apagar); // Apaga Led "RED"
    return false; //Retorna Falso
  }
}

///////////FUNCION PARA CONECTAR A LA RED
bool coneccion_a_Red (bool aRed, bool aServer) {
  //Comprobar si activa todo de TCP_GPRS
  while (TCP_GPRS(aRed, aServer) != true) {
    ////////ACTIVA TCP_GPRS
    escribirComando("AT+CIPCLOSE", 4000, true);//Cierra la conexion al servidor
    delay(100); //1000
    serialA7();//___
    escribirComando("AT+CIPSHUT", 4000, true); //Cierra la conexion a la red
    delay(100);
    serialA7();//___
    Serial.println("Desactivado TCP_GPRS");
    analogWrite(onRed, apagar); //Apaga Led "RED"
  }
  analogWrite(onRed, encender); //Enciende Led "RED"
  delay(500);
  Serial.println("Activado TCP_GPRS!!!");
  return true; // Devuelve true por que ya existe conexion
}

