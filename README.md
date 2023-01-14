# cppproject - Schiffe Versenken

## Anleitung zum Bauen
Build-Command: g++ Main.cpp Brett.cpp Schiff.cpp

## Spielprinzipien
Unser Spiel basiert weitestgehend auf den Standard-Regeln für Schiffeversenken. 
Der Spieler kämpft hierbei gegen eine KI.
Schiffe müssen auf einem 10x10-Feld platziert werden, ohne sich zu berühren oder zu kreuzen.
In unserer Konfiguration gibt es: 1x 5er-Schiff, 1x 4er-Schiff, 3x 3er-Schiff, 1x 2er-Schiff.
Spieler und KI schießen abwechselnd; bei einem Treffer ist derselbe Spieler solange dran, bis er daneben trifft. 

Die Werte auf den Spielfeldern haben dabei folgende Bedeutungen:
0: Wasser
1: Schiff
2: Getroffen
3: Versenkt
4: Daneben

Verloren hat der Spieler, welcher keine Schiffe mehr auf seinem Feld besitzt.


## Vorgehen
Der Spielablauf wird durch die Main-Klasse gesteuert. 
Zunächst werden jeweils ein spielerBrett und ein kiBrett erstellt.

Die KI setzt in der Methode kiSchiffeSetzen(shipSize) nach dem Zufallsprinzip jeweils ein Schiff der übergebenen Größe auf ihr Feld.
Der Spieler kann seine Schiffe in der Methode spielerSchiffeSetzen(shipSize) anhand einer Konsoleneingabe setzen. Dazu muss zunächst
die Orientierung mit h (=horizontal) bzw. v (=vertikal) angegeben werden und danach die Position des Kästchens des Schiffes, das am weitesten links unten sein soll.

Am Ende der beiden Methoden werden (nach Überprüfung der Koordinaten auf Korrektheit) 

