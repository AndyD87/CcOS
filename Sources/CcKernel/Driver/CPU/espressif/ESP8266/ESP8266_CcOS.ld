SECTIONS
{
  .MySection 0x403FB000 :
  {
    KEEP(*(.ESP8266Eeprom_Section))
  }
}
