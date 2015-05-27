#Hamilton Kreis 

###Spezifikation 
Der Hamiltion Kreis ist ein mathematisches Graphenproblem. Der Hamiltonkreis ist ein geschlossener Pfad in einem Graph, bei dem jeder Knoten nur einmal besucht wird. 
Es wird ein Programm entwickelt, welches einen Graph im DIMACS Format einliest. Diese Problematik in eine aussagenlogische Formel umwandelt und dann die Erfüllbarkeit dieser Formel mit dem MiniSAT Solver errechnet. 

---

    

###Annahmen 
    - Jeder Knoten wird nur einmal besucht -> Jede Kante wird nur einmal benutzt
    - Jedem Zeitpunkt wird nur ein Knoten zugeordnet 
    - Startknoten ist gleich Endknoten (Kreis) 


---
###Knoten 
    - Zusammgesetzer String aus id + time
    
###Kante 
    - Implikation  id + time -> id  + time+1

###DIMACS FORMAT
    - "c" Kommentar 
    - "p Kanten Knoten" Spezifikation des Problems
    - "e Knoten Knoten" Spezifkikation einer Kante 
    
### MiniSAT 
1.Installiere Cygwin

2.Füge "C:/cygwin/bin/ zu deinem Root-Path hinzu 


3.Um MiniSAT im Programm aufzurufen muss die Cygwin Version von MiniSAT herunterladen 
[MiniSAT](http://minisat.se/MiniSat.html)
4.Benenne die Cygwin Datei in MiniSat.exe um und schiebe sie in den Projektordner

5.Fertig



---

###To Do's

    -Generierung der Formel 
    -Minisat öffnen
    -MiniSat starten
    -MiniSat auslesen 

###References 
    -Visual Studio 2012 
    -GitHub
    -MiniSat
    -dillinger.io
    
    
