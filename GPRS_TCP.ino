bool TCP_GPRS(bool aRed, bool aServer) { //voil TCP_GPRS()
  
  if (aRed) { //!aIniciado = false al inicio
    bool creg = false, cgatt = false, cgdcont = false, cgact = false;
    creg = escribirComando("AT+CREG?", 2000, false); //Query network registration
    delay(100);
    serialA7();//___
    cgatt = escribirComando("AT+CGATT=1", 6000, false);
    delay(100);
    serialA7();//___
    cgdcont = escribirComando("AT+CGDCONT=1,\"IP\",\"CMNET\"", 2000, false); //setting PDP parameter //PDP - The process on the Security Gateway responsible for collecting and sharing identities.(URL= https://sc1.checkpoint.com/documents/R76/CP_R76_CLI_WebAdmin/66477.htm)
    delay(100);
    serialA7();//___
    cgact = escribirComando("AT+CGACT=1,1", 10000, false); //Activate PDP, open Internet service
    delay(100);
    serialA7();//___
    if (creg && cgatt && cgdcont && cgact && !aServer) {
      Serial.println("Conectado a Red");
      return true;
    }
  }
  if (aServer) {
    bool confirmar = false; 
    serialA7();//___
    confirmar = escribirComando("AT+CIPSTART=TCP,api.thingspeak.com,80", 10000, true);
    if (confirmar) {
      digitalWrite(onRed, HIGH);
      return true;
    }
    digitalWrite(onRed, LOW);
    return false;
  }

}



///////////FUNCION PARA CONECTAR A LA RED
bool coneccion_a_Red (bool aRed, bool aServer) {
  //Comprobar si activa todo de TCP_GPRS
  while (TCP_GPRS(aRed, aServer) != true) {
    ////////ACTIVA TCP_GPRS
    escribirComando("AT+CIPCLOSE", 4000, true);
    delay(100); //1000
    serialA7();//___
    escribirComando("AT+CIPSHUT", 4000, true);
    delay(100);
    serialA7();//___
    Serial.println("Desactivado TCP_GPRS");
    digitalWrite(onRed, LOW);
  }
  digitalWrite(onRed, HIGH);
  delay(500);
  Serial.println("Activado TCP_GPRS!!!");
  return true; //DEVUELVE TRUE POR QUE YA EXISTE CONECCION
}

