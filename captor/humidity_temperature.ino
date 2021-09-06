String humTemp(){
  //reset sensor by sending 0xFE command to the Si7021 address
    Wire1.beginTransmission(si7021Addr);
    Wire1.write(0xFE); // Write reset command
    Wire1.endTransmission();
    delay(15); // Default = 15ms

   
  
   unsigned int data[2];
  //Send humidity measurement command and get response into the array 'data'
  getSiData(data, 0xE5);
  // Convert the data
  float humidity  = ((data[0] * 256.0) + data[1]);
  humidity = ((125 * humidity) / 65536.0) - 6;
  // Send temperature measurement command
  getSiData(data, 0xE3);
  /* re-use temperature already measured by humidity measurement some milliseconds before command -> E0
    *getSiData(data, 0xE0); 
    *
    *However, does not report valid results
    *
    *Datasheet: 
    *Each time a relative humidity measurement is made a temperature measurement is also made for the purposes of
    *temperature  compensation  of  the  relative  humidity  measurement.  If  the  temperature  value  is  required,  
    *it  can  be read  using  command  0xE0;  this  avoids  having  to  perform  a  second  temperature  measurement.  
    *The  measure temperature  commands 0xE3  and  0xF3  will  perform  a  temperature measurement  and  return  the  
    *measurement value, command 0xE0 does not perform a measurement but returns the temperature value measured during the
    *relative humidity measurement. 
  */ 
//   Convert the data
  float temp  = ((data[0] * 256.0) + data[1]);
  float celsTemp = ((175.72 * temp) / 65536.0) - 46.85;
  float fahrTemp = celsTemp * 1.8 + 32;
  // Output data to serial monitor
  return String(humidity)+";"+String(celsTemp);
  
}

void getSiData(unsigned int *_ret_data, byte _i2c_command)
{
  // start i2c communication 
  Wire1.beginTransmission(si7021Addr);
  //send i2c command to sensor
  Wire1.write(_i2c_command);
  // we are done with our transmission...close i2c communication
  Wire1.endTransmission();
  delay(85);
  // Request 2 bytes of data
  Wire1.requestFrom(si7021Addr, 2);
  // Read 2 bytes of data and save it to _ret_data which points to 'data[2]'
  if(Wire1.available() == 2)
  {
    _ret_data[0] = Wire1.read();
    _ret_data[1] = Wire1.read();
  }
}
