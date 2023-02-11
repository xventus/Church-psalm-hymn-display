# Church psalm hymn display, display unit

Project for displaying hymn numbers and lyrics. This is the latest version and cheaper variant of the original FPGA VGA version. Solution based on a VGA LCD display and a remote keyboard with wireless / wired connection. Here are the complete resources including schematics and links to the modules. 

The old types of displays based on LED displays are compatible, the transmission protocol has not changed. 


# Číselník a zpěvník do kostelů

Jde o inovaci původní verze číselníku s LED i FPGA VGA zobrazovači. Protokol je zpětně kompatibilní. 
Zobrazovač je založen na VGA LCD monitoru a vzdáleného ovladače. Projekt je koncipován tak, aby jej byl schopen kdokoliv replikovat za pomocí dostupných modulů Arduino / ESP32 a nemusel vytvářet vlastní DPS.

VGA zobrazovač je založen na ESP32, konkretně levném modulu LILYGO® TTGO VGA32 V1.4  (s SD kartou)
http://www.lilygo.cn/prod_view.aspx?TypeId=50033&Id=1083


Klávesnici lze připojit buď lokálně přímo k TTGO VGA32 (PS/2), nebo přes vzdáleně přes RF modul, popřípadě RS485.
RF/RS modul se připojuje na PS/2 Mouse konektor Data (27).  

Vzdálená klávesnice je založena na ATMEGA328 v základním zapojení s 16MHz krystalem. Schéma a PCB design přiložen. Popřípadě lze použít jakékoliv Arduino. 
Oproti původní verzi nepoužívá modul MAX3421, ale využívá možnosti některých USB klávesnic pracovat v režimu PS/2. Pro zobrazení zadávaného čísla se používá modul LED displeje založený na TM1637. Pro bezdrátovou komunikaci lze využít modul APC 220. 

Kompatibilní klávesnice např. CONNECT IT NumCALC CKB-0060-BK
Nekompatibilní např. Genius NumPad i110


# Data pro SD kartu

Adresář scripts obsahuje pomocné programy pro převod žalmů a kancionálu do formátu pro VGA displej. 
Data z kancinálu byla získána stažením obsahu webu www.kancional.cz a převodem z html do vlastních dat. 
Texty žalmů jsou opisem z příšlušných knih. 


# Režimy zobrazení

V základním režimu zobrazovač nahrazuje tříciferný zobrazovač (000-996) s možností zobrazení slok (1-9).
Pokud má ESPšě modul připojenou SD kartu, lze zobrazovat žalmy, popřípadě i texty z kancionálu. 

Nastavemní viz. /doc/user-manual-csy.pdf 


# Licence

Volně k použití pro kostely, nekomerční účely ...

# Pár obrázků
![screen](/display/images/IMG_4641.jpg)
![main board](/display/images/IMG_4636.jpg)
![remote](/controller/DOC/images/IMG_4635.jpg)
![case](/controller/case%203d%20print/overall_keypad.png)

# Nastavení knihoven
```
Platform espressif32 @ 6.0.1 (required: espressif32)
├── framework-arduinoespressif32 @ 3.20006.221224 (required: platformio/framework-arduinoespressif32 @ ~3.20006.0)
├── tool-esptoolpy @ 1.40400.0 (required: platformio/tool-esptoolpy @ ~1.40400.0)
├── tool-mkfatfs @ 2.0.1 (required: platformio/tool-mkfatfs @ ~2.0.0)
├── tool-mklittlefs @ 1.203.210628 (required: platformio/tool-mklittlefs @ ~1.203.0)
├── tool-mkspiffs @ 2.230.0 (required: platformio/tool-mkspiffs @ ~2.230.0)
├── tool-openocd-esp32 @ 2.1100.20220706 (required: platformio/tool-openocd-esp32 @ ~2.1100.0)
└── toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5 (required: espressif/toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch5)

Libraries
└── FabGL @ 1.0.9 (required: fdivitto/FabGL @ ^1.0.9)
```
