# RBCX-BRNO-2026-VICEBOJ-REPAS

Opět uveden do provozu z loňského roku (2025).
Hlavní změnou je knihovna a styl jízdy.

![Robot](/photo/BRNO-VICEBOJ-2026-REPAS-FOTO.jpg)


## Pokyny před startem:

1) Vložte baterii
2) Zmáčknout tlačítko ,,**ON**,, (rozsvítí se 2 červené ledky na desce ===> deksa je zapnutá)
3) Zkontrolovat zda se klepeto a rameno posunuly no výchouí polohy.
4) Zmáčknout ,,**RESET**,,
5) Spustit některý z programů (B1, B2, B3, B4, B5, ON, OFF, TL1, TL2), pokud se rozsvítí ledky/robot se začne pohybovat ===> ROBOT JE PŘIPRAVEN K JÍZDĚ
6) Vložte robota na start soutěže a spusťte ,,**B1 - VYHREJ**,,

**POKUD NĚKTERÝ Z KROKŮ NEFUNGOVAL ZKUSTE VYPNOUT A OPĚT ZAPNOUT DESKU, PŘÍPADNĚ VYMĚNIT BATERII**


## Ovládací tlačítka:

!["Ovládací tlačítka"](/photo/BRNO-VICEBOJ-2026-REPAS-BUTTONS_SETUP.JPG)

**B1 - VYHREJ**

**B2 - KULIČKY**

**B3 - SLALOM**

**B4 - BLUDIŠTĚ**

**ON - SPRINT**

**OFF - MEDVĚD**    

**TL1 - KOMBINACE 1**

**TL2 - KOMBINACE 2**


## SOUTĚŽE:

### B1 - VYHREJ:
- Rozsvítí zelenou LEDku.
- Projede všechny soutěže, tak jak jsou jsou za sebou. Po dokončení každé z disciplín (stojí na červené ploše) vydá tón, a poté čeká 10s. 

### B2 - KULIČKY:
- Rozsvítí modrou LEDku.
- Čeká 10s a poté provede disciplínu.
- Popojede vpřed a shodí míček (3x), poté couvne, otočí se a srovná o zeď. A jede do cíle.

### B3 - SLALOM:
- Rozsvítí červenou a žlutou (oranžová) LEDku.
- Čeká 10s
- Může začínat ze dvou směrů (nastavuje se v codu).
- Projede slalom, tím že jede přesné obloučky.

**TRUE:**

```cpp
slalom(true);
```
===> Slalom začíná zatáčením na pravou stranu

**FALSE:**

```cpp
slalom(false);
```

===> Slalom začíná zatáčením na levou stranu

### B4 - BLUDIŠTĚ:
- Rozsvítí žlutou LEDku.
- Čeká 10s
- Jede vpřed a kontroluje kam může jet dle priority (pravá, rovně, levá, zpět)
- Jede-li zpět, tak se rovná podle tlačítek
- Pro prvních 5 kontrol nekontroluje, zda je již v cíli, ale má lehce odlišný code, aby robot omylem nenajel zpět do startu.
- Pro další kontroly vždy prvně kontroluje, zda vidí cíl (červenou barvu).

### ON - SPRINT:
- Rozsvítí červenou a modrou (fialová) LEDku.
- Čeká 10s
- Změří vzdálenost od pravé zdi.
- Změří vzdálenost před sebou a jede tuto vzdálenost.
- Během jízdy měří aktuální vzdálenost od pravé zdi a dle toho upravuje kurz.
- Po zastavení se srovná v rohu a otočí se na pravou stranu od původní jízdy.

### OFF - MEDVĚD:
- Rozsvítí červenou LEDku.
- Čeká 10s
- Otevře klepeto, tlačí medvěda a jede vzdálenost 2 polí, poté zatáčí na pravou stranu a pokračuje vpřed až na poslední pole.
- Po odtlačení medvěda couvá a rovná se v rohu, poté jede do cíle.

### TL1 - KOMBINACE 1:
- Rozsvítí zelenou a žlutou LEDku.
- Čeká 10s
- Prování nastavenou kombinaci disciplín.

### TL2 - KOMBINACE 2:
- Rozsvítí červenou, zelenou, modrou a žlutou (všechny barvy) LEDku.
- Čeká 10s
- Prování nastavenou kombinaci disciplín.


## KONTROLA CODU PŘED SOUTĚŽÍ:

1) Zkontrolovat zda je správně nastaven DelayOnRed (čekání mezi disciplínama --> 10s == (10000)ms).
2) Nastavit správně VYHREJ kombinaci.
3) Nastavit Kombinaci_1 a Kombinaci_2. Ideálně po disciplínách, které jsou problémové a mohly by se opakovaně kazit.
4) Otestova všechny disciplíny, alespoň samostatně, případně upravit/optimalizovat code. 