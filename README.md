# RBCX-BRNO-2026-VICEBOJ-REPAS

- Opět uveden do provozu z loňského roku (2025).

Původní robot z roku 2025: https://github.com/ZemanNz/RBCX-ROBOT-BRNO-2025

- Hlavní změnou je knihovna a styl jízdy.

![Robot](/photo/BRNO-VICEBOJ-2026-REPAS-FOTO.jpg)


## Pokyny před startem:

1) Vložte baterii.
2) Zmáčkněte tlačítko „**ON**“ (rozsvítí se 2 červené LEDky na desce -> deska je zapnutá).
3) Zkontrolujte, zda se klepeto a rameno posunula do výchozí polohy.
4) Zmáčkněte „**RESET**“.
5) Spusťte některý z programů (B1, B2, B3, B4, ON, OFF, TL1, TL2). Pokud se rozsvítí LEDky / robot se začne pohybovat -> ROBOT JE PŘIPRAVEN K JÍZDĚ.
6) Vložte robota na start soutěže a spusťte „**B1 - VYHREJ**“.

**POKUD NĚKTERÝ Z KROKŮ NEFUNGOVAL, ZKUSTE VYPNOUT A OPĚT ZAPNOUT DESKU, PŘÍPADNĚ VYMĚNIT BATERII.**


## Ovládací tlačítka:

!["Ovládací tlačítka"](/photo/BRNO-VICEBOJ-2026-REPAS-BUTTONS_SETUP.JPG)

### B1 - VYHREJ
- Projede všechny disciplíny, tak jak jsou za sebou.
- Sprint -> Slalom -> Bludiště -> Medvěd -> Kuličky --> Cíl

### B2 - KULIČKY

### B3 - SLALOM

### B4 - BLUDIŠTĚ

### ON - SPRINT
- Provede Kombinaci 0 (vše po Sprintu):
- Slalom -> Bludiště -> Medvěd -> Kuličky --> Cíl

### OFF - MEDVĚD    

### TL1 - KOMBINACE 1
- Provede Kombinaci 1 (vše po Slalomu):
- Bludiště -> Medvěd -> Kuličky --> Cíl

### TL2 - KOMBINACE 2
- Provede Kombinaci 2 (vše po Bludišti):
- Medvěd -> Kuličky --> Cíl


## SOUTĚŽE:

### B1 - VYHREJ:
- Rozsvítí zelenou LEDku.
- Projede všechny disciplíny, tak jak jsou za sebou. Po dokončení každé z disciplín (stojí na červené ploše) vydá tón a poté čeká 10 s. 

### B2 - KULIČKY:
- Rozsvítí modrou LEDku.
- Čeká 10 s a poté provede disciplínu.
- Popojede vpřed a shodí míček (3x), poté couvne, otočí se, srovná o zeď a jede do cíle.

### B3 - SLALOM:
- Rozsvítí červenou a žlutou (oranžovou) LEDku.
- Čeká 10 s.
- Může začínat ze dvou směrů (nastavuje se v kódu).
- Projede slalom tím, že jede přesné obloučky.

**TRUE:**

```cpp
slalom(true);
```
===> Slalom začíná zatáčením na pravou stranu.

**FALSE:**

```cpp
slalom(false);
```

===> Slalom začíná zatáčením na levou stranu.

### BLUDIŠTĚ:
- Rozsvítí žlutou LEDku.
- Čeká 10 s.
- Jede vpřed a kontroluje, kam může jet dle priority (pravá, rovně, levá, zpět).
- Jede-li zpět, tak se rovná podle tlačítek.
- Pro prvních 5 kontrol nekontroluje, zda je již v cíli, ale má lehce odlišný kód, aby robot omylem nenajel zpět do startu.
- Pro další kontroly vždy nejprve kontroluje, zda vidí cíl (červenou barvu).

### SPRINT:
- Rozsvítí červenou a modrou (fialovou) LEDku.
- Čeká 10 s.
- Změří vzdálenost od pravé zdi.
- Změří vzdálenost před sebou a jede tuto vzdálenost.
- Během jízdy měří aktuální vzdálenost od pravé zdi a podle toho upravuje kurz.
- Po zastavení se srovná v rohu a otočí se na pravou stranu od původní jízdy.

### MEDVĚD:
- Rozsvítí červenou LEDku.
- Čeká 10 s.
- Otevře klepeto, tlačí medvěda a jede vzdálenost 2 polí, poté zatáčí na pravou stranu a pokračuje vpřed až na poslední pole.
- Po odtlačení medvěda couvá a rovná se v rohu, poté jede do cíle.


## Návaznosti:

### SPRINT (Kombinace 0):
- Rozsvítí zelenou a žlutou LEDku.
- Čeká 10 s.
- Provádí nastavenou kombinaci disciplín po dokončení Sprintu.
- Slalom -> Bludiště -> Medvěd -> Kuličky --> Cíl

### KOMBINACE 1:
- Rozsvítí zelenou a žlutou LEDku.
- Čeká 10 s.
- Provádí nastavenou kombinaci disciplín po dokončení Slalomu.
- Bludiště -> Medvěd -> Kuličky --> Cíl

### KOMBINACE 2:
- Rozsvítí červenou, zelenou, modrou a žlutou (všechny barvy) LEDku.
- Čeká 10 s.
- Provádí nastavenou kombinaci disciplín po dokončení Bludiště.
- Medvěd -> Kuličky --> Cíl


## KONTROLA KÓDU PŘED SOUTĚŽÍ:

1) Zkontrolovat, zda je správně nastaven **DelayOnRed** (čekání mezi disciplínami --> 10 s == (10000) ms).
2) Nastavit správně všechny kombinace, včetně otáčení mezi nimi pokud je potřeba.
3) Nastavit SPRINT (Kombinaci_0), Kombinaci_1 a Kombinaci_2. Ideálně po disciplínách, které jsou problémové a mohly by se opakovaně kazit.
4) Otestovat všechny disciplíny, alespoň samostatně, případně upravit/optimalizovat kód. 
5) Kontrolujte, zda se neobjevují náhlé chyby.