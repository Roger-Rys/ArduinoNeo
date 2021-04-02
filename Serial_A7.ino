/*********FUNCION SERIALA7******************************/
void serialA7() {
  proA7.listen();
  while (proA7.available() > 0) {
    valorChar = proA7.read(); // Recepta valores Char
    valorStr += valorChar; //valorStr recoge los valores char de valorChar
    if (valorChar == '\n') { // Detecta "\n" (salto de linea)
      Serial.print("c_s:"); Serial.println(valorStr);
      valorStr.toCharArray(trama1, 100); //ValorStr se almacena en arregloChar
      ///////DETECTA EL RING/////////
      if ((strstr(trama1, " RING") != NULL)) { // Detecta si trama1 = RING
        analogWrite(ledLlamada, encender);  //Indicador de llamada entrante        
        valorStr = ""; // Limpia valorStr
        memset(trama1, '\0', 100); // Limpia Trama
        conteoRing++; // Aumenta conteoRing
        //Detectar si conteoRing es igual a uno
        if(conteoRing==1){
          //Se ejecuta cada vez que inicializa el dispositivo
          proA7.println("AT+CLCC"); // Identifica que numero llamada
          delay(100);  
        }        
        proA7.println("ATH"); // Cuelga la llamada
        delay(100); 
        analogWrite(ledLlamada, apagar); //Indicador de llamada entrante         
        delay(100); 
      }
      ///////DETECTA EL +CLCC contiene el numero telefonico/////////
      // Detecta si trama1 = +CLCC:...
      else if ((strstr(trama1, "+CLCC: 1,1,4,0,0,") != NULL) && NumTelSav.length() <= 11) { 
        // num_telefoYmas contiene el numero telefonico y mas valores
        valorStr.toCharArray(num_telefoYmas, 100); 
        //Llamar la funcion "detectarNumTel", devolvera solo el numero telefonico
        NumTelRec = detectarNumTel(num_telefoYmas);
        valorStr = " "; // Limpia valorStr
        memset(trama1, '\0', 100); // Limpia Trama
        analogWrite(ledLlamada, apagar); //Indicador de fin de llamada
      }
      ///////BORRA STRING Y TRAMA1/////////
      else {
        valorStr = " "; // Limpia valorStr
        memset(trama1, '\0', 100); // Limpia Trama
      }
    }
  }
}

