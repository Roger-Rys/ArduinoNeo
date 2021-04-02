/*******FUNC CONFIGURACION_INIC*******************************************************/
void configuracionInicial() {
  digitalWrite(p_PWRKEY, HIGH); delay(3000); // 2 seg para Activar la red GSM
  digitalWrite(p_PWRKEY, LOW); delay(2000); //10seg
  Serial.println("PWR_KEY Activado"); // Mensaje que la red esta activada

  Serial.println("--A7 desconectado--");
  int count = 0;
  
  while (escribirComando("AT", 3000, false) != true) { // Envia AT y espera respuesta OK
    count++;
    if (count % 10 == 0) {
      digitalWrite(p_reset_A7, HIGH); delay(500);
      digitalWrite(p_reset_A7, LOW);
    }
    if (count % 15 == 0 ) {
      digitalWrite(power_A7, LOW); delay(2000);
      digitalWrite(power_A7, HIGH);
    }
    digitalWrite(p_PWRKEY, HIGH); // Pin activa PWRKEY de A7
    analogWrite(ledPWRKEY, encender); //Led indicador 
    delay(3000); // 3 seg para Activar la red GSM
    digitalWrite(p_PWRKEY, LOW);
    analogWrite(ledPWRKEY, apagar); //Led indicador 
  }
  Serial.println("---Conectado---");
  delay(2000);
  escribirComando("AT+CIPSHUT", 3000, false);
  
  //CONECTADO A RED
  bool server = false;
  bool red = true;
  //Solo Conectar a la red y no al servidor
  coneccion_a_Red(red, server);
  delay(1000);  
  
}
