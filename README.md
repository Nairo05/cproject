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

Immer am Ende der beiden Methoden werden (nach Überprüfung der Koordinaten auf Korrektheit) neue Schiff-Objekte erstellt, deren Positionen gesetzt und anschließend in einen Vektor abgelegt. Zudem wird das betroffene Spielbrett (kiBrett bzw. spielerBrett) an den entsprechenden Positionen auf 1 gesetzt (Methode registerToField()).

Danach beginnt das Spiel (Aufruf der Methode spielStarten()). Solange der bool "spielEnde" noch nicht "true" ist, wird die Methode spielZug(naechster) aufgerufen.
Diese Methode bestimmt letztendlich, auf welche Koordinate geschossen wird und ob getroffen wurde. Zudem wird per return-Statement definiert, wer als nächstes ziehen darf. 
Wird spielZug(1) aufgerufen, ist der Spieler dran. Es wird eine Koordinateneingabe der Form <Zeile><Spalte> (z.B. B4) erwartet. Es wird kurz überprüft, ob die Koordinate zulässig ist. Wenn dies so ist, wird über ein switch-case geregelt, was passiert: 
  - Bei Wasser bzw. 0 wird das Feld auf dem kiBrett als "Verfehlt" bzw. 4 markiert und die KI ist als nächstes dran.
  - Bei Treffer bzw. 1 wird mit Hilfe des kiSchiffe-Vektors überprüft, welches Schiff aktuell beschossen wird und ob es vollständig versenkt wurde. Ist dies der Fall,     wird das Schiff aus dem kiSchiffe-Vektor gelöscht. Der Spieler darf nun auch wieder den nächsten Zug machen (return 1).
  - Bei allen anderen Fällen wird ausgegeben, dass der Spieler dieses Feld bereits beschossen hat, und darf nochmal ziehen (return 1).
  
Wird spielZug(2) aufgerufen, passiert an sich dasselbe. Der entscheidende Unterschied ist, dass die KI eine Schussstrategie abarbeitet. Diese ist in einem Enum definiert und aufgegliedert in "zufall", "west", "ost", "nord" und "sued". Bei Spielbeginn ist immer "zufall" ausgewählt und die KI bestimmt die Target-Koordinate per Random-Algorithmus. Hierbei wird durch verschiedene Überprüfungen wieder sichergestellt, dass der generierte Wert zulässig ist. 
- Trifft die KI mit Strategie=zufall daneben, bleibt die Strategie "zufall" und der Spieler ist erstmal wieder an der Reihe. 
- Trifft die KI, wird die Strategie auf "westen" gesetzt, alles andere ist analog zum Treffer-Fall beim Spieler. Die Strategie bleibt nun solange gleich, bis die KI      auf eine 0 (Wasser) oder 4 (bereits daneben geschossen) trifft:
- Trifft die KI daneben oder auf ein bereits beschossenes Feld, wird die Strategie in Abhängigkeit der aktuellen Strategie umgeändert: zufall=>zufall, west=>ost,   
  ost=>nord und nord=>sued. So werden Schiffe, die an einer Stelle getroffen sind, Systematisch versenkt.
Wichtig: Beim Verfolgen einer konkreten Strategie oder Strategiefolge kann es zu 2 größeren Problemen kommen:
  1. Die Target-Koordinaten "wandern aus dem Spielfeld heraus": Hierzu wird die Strategie am Anfang der spielZug()-Methode ggfs. "umgekehrt/gespiegelt", wenn die   
     aktuelle Target-Koordinate am Rand liegt.
  2. Bei bestimmten Schiffskonstellationen kann es passieren, dass durch den Strategiewechsel (hervorgerufen durch das Treffen auf ein Feld mit Wert 4) eine  
     Endlosschleife des Strategiewechsels hervorgerufen wird. Deshalb gibt es einen cyclicCounter, der beim Auftreffen auf ein "4-Feld" inkrementiert wird und  
     ansonsten (Wasser oder Treffer) zurückgesetzt wird. Erreicht cyclicCounter den Wert 5 oder höher, wird die Strategie auf zufall zurückgesetzt.
  
Nach jedem Zug erfolgt die Win-Detection, die dann die while-Schleife in der Methode "spielStarten()" unterbricht. 
In der main()-Methode wird dann letztendlich der Gewinner ausgegeben und Heap-Speicher freigegeben.

##Schwierigkeiten
- Out of Playfield:
- Random Algorithmus: 

