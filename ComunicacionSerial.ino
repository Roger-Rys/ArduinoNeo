/*******FUNC COMUNICACION_SERIAL*****************************************/
void ComunicacionSerial() {
  ////////FUNCION SERIALA7//////////////////////////////////
  serialA7();
  ///////SERIAL DE ARDUINO/////////
  while (Serial.available()) {
    //Serial arduino lee, modulo A7 escribe comando
    proA7.write(Serial.read()); 
  } delay(50);
}

