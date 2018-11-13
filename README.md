# MixHit auf IDF-Basis
Cocktail-Mixer HS-Karlsruhe

Dies ist das Github-Repository für den Cocktail-Mixer an der Hochschule unter der Betreuung von Jürgen Walter.

## Code Convention
Damit der erstellte Code hier einheitlich aussieht, wird hier die Code Convention von Google genutzt:
https://google.github.io/styleguide/cppguide.html

## Klonen des Projektes:
Das Klonen des Github-Repository wird hier beschrieben: https://help.github.com/articles/cloning-a-repository/
Eine Einleitung zu Git findet sich hier: http://rogerdudler.github.io/git-guide/index.de.html
Benutze für das Klonen die Option "--recursive", damit auch die MixHit-Libraries als Submodul geladen werden

Der geeignete Befehl für das Klonen ist: git clone https://github.com/Mechatronikwelt/mixhit-idf.git --recursive
Der Befehl kann in der mingw32-Konsole ausgeführt werden, wenn man im Ordner ~/esp/ ist.

## Installation
1. Befolge diese Anweisungen: https://docs.espressif.com/projects/esp-idf/en/latest/get-started/windows-setup.html
2. Setze das Esp32-Framework auf: https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#get-started-get-esp-idf
    Benutze dabei die Git-Variante, um die aktuellste Framework-Variante zu erhalten.
3. Klone dieses Github-Projekt in dein Working Directory (Dort, wo auch der Ordner esp-idf zu finden ist)
4. Füge das Projekt in Eclipse zu (siehe: https://docs.espressif.com/projects/esp-idf/en/latest/get-started/eclipse-setup-windows.html#eclipse-windows-setup )
5. Starte das Tool "mingw64.exe" im Ordner C:/msys32.
  - Navigiere per "cd <Ordnername>" in den mixhit-Ordner.
  - Führe make menuconfig aus
  - Passe den COM-PORT für den ESP32 an
6. Öffne die Projekteigenschaften, 
  - Navigiere zu C/C++ General -> Paths and Symbols -> Tab Includes
  - Import Settings -> Importiere die c_includes_windows.xml
7. Nun solltest du in Eclipse das Projekt bauen können. Du kannst in den Fenster "Build Targets" auch einen Task mit "flash" anlegen, wodurch es möglich ist, direkt aus Eclipse zu flashen.


Das hier benutzte Framework heißt "esp-idf". Zu finden ist das Framework auf Github.
