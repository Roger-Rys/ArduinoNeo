/*******FUNC DATOS_GPS********************************************************/
String SerialDatosGPS() { //La funcion devuelve un string
  digitalWrite(ledGPSon, HIGH); //LED para GPSon
  String enviar = "";
  //Mientras serial del modulo A7 esta habilitado
  neo6.listen();
  while (neo6.available() > 0) {
    //Codifica lo que provanga del modulo A7
    GPS.encode(neo6.read());
    //Si existe nuevos valores ejecutara este codigo
    if (GPS.location.isUpdated()) {
        //Serial.println(GPS.location.lat(), 6);
        //Serial.println(GPS.location.lng(), 6);
      //Trama a enviar con datos de posicion en los campos field1 y field2
      enviar = "&field1=" + String(GPS.location.lat(), 6) + "&field2=" + String(GPS.location.lng(), 6);
      //Si "enviar" tiene una longitud mayor a 32 caracteres
      if (enviar.length() >= 32) {
        //Detectar si recibe llamada
        serialA7();
        //La funcion devuelve lo que contiene "enviar"
        return enviar;
      }
    }
  }
  serialA7();//___
}
