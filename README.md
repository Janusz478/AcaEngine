# AcaEngine
The Acagamics Game Engine Course

Das Projekt baut mit Visual Studio 2019 16.8.4, aber nicht mit 16.9.1 wegen diesem MSVC Bug: https://developercommunity2.visualstudio.com/t/MSVC:-error-C2131-occurs-in-169-version/1358064?q=c2131+optional&viewtype=all

## GUI-Toolkit

Das GUI-Toolkit stellt 6 Grundlegende Arten von GUI-Objekten zur Verfügung: Slider, Checkboxes, Buttons, Textdisplays, Textfields und Container. Alle GUI-Objekte brauchen eine Entität. Weitere Parameter wie z.B. Position, Skalierung, Funktion usw. sind abhängig vom konkreten Objekt-Typ. Wenn man einen Container verwendet, dann werden Positionierung und Skalierung der Objekte über den Container verwaltet, man kann aber dennoch einen Positions- und Skalierungswert übergeben. Dieser wird dann vom Container automatisch angepasst.

Alle Objekte außer Slider und Container haben eine Textausgabemöglichkeit. Der Text muss als String übergeben werden. Beim Textfeld wird der Text (Standard-Text) so lange angezeigt bis man auf das Textfeld klickt. Dann kann der Nutzer per Tastatureingabe einen neuen Text eingeben. Außerdem kann die Textfarbe übergeben werden.

Textdisplays brauchen eine Textur als Hintergrundfarbe.

Buttons, Textfelder und Checkboxen brauchen zwei Texturen als Parameter. Die erste Textur dient als Standardfarbe des Buttons und des Textfeldes und als False-State der Checkbox. Die zweite Textur erscheint, wenn man über den Button mit der Maus fährt, auf das Textfeld klickt oder drüberfährt und beim True State der Checkbox.

Slider brauchen keine Texturen, sondern eine Anzahl an möglichen Sliderpositionen und eine Startposition, beide als Integer. Ein Boolean beim Slider richtet den Slider vertikal (true) oder horizontal (false) aus.

Buttons, Checkboxen und Textfelder können eine Funktion bekommen. Bei Buttons und Checkboxen wird die Funktion durch ein anklicken ausgelöst. Die Funktionen erhalten die gewünschte Entität und das System als Parameter. Bei Textfeldern wird die Funktion durch einen Enter Tastendruck während man auf das Feld geklickt hat ausgelöst. Der Standard-Text wird danach wieder angezeigt. Durch Drücken von Back-Space können einzelne Zeichen gelöscht werden. Durch Drücken von Escape wird der gesamte neue Text gelöscht.

Textfelder passen die Größe des eingegebenen Textes automatisch an, sodass die Box nicht überschritten wird.

Die getLevel Funktion gibt den Aktuellen Sliderstand als Integer zurück.

Um komplexere Funktionen zu schreiben, zum Beispiel eine Buttonfunktion welche vom Zustand einer Checkbox abhängt, kann man entweder über die ID der Checkbox auf den Boolean via System zugreifen
oder man übergibt eine leere Funktion und schreibt eine neue Funktion, welche die gewünschten Entitäten mitbekommt. Die "Hitbox" des Buttons muss mit der Position und Skalierung berechnet werden.

## Container

Container übernehmen die Positionierung, Anordnung und Skalierung der einzelnen Objekte. Container haben eine Position, eine Skalierung, eine
Reihen und Spalten Zahl, einen Abstandparameter, eine Hintergrundtextur, zwei Booleans und einen Vector an Entitäten.

Der erste Boolean gibt an ob die Angegebe Textur als Hintergrund gesetzt wird. 
Die Textur muss in jedem Fall angegeben werden, wird aber nur bei True verwendet.
Der zweite Boolean bietet die möglichkeit die Container zu ziehen. Diese Funktion ist recht rechenaufwendig.

Es ist wichtig, dass der übergebene Vector zuerst befüllt wird. Weiterhin ist es auch möglich Container in Container zu packen.
Hier muss der Vector des innere Container allerding auch zuerst befüllt werden und dann erst sollte der innere Container in den Vector des äußeren gepackt werden.

Der innere Container kann zwar unabhängig vom äußeren bewegt werden, aber wenn der äußere bewegt wird geht der innere an seine Position im äußeren zurück.

Die "hitbox" für GUI-Objekte und sämtliche Texte werden automatisch mitbewegt.

## Gamestates

Um die GUI-Objekte sowie die bisherigen Funktionen zu zeigen, haben wir uns für ein Startmenü entschieden.
Mit diesem Menü können die Gamestates durch Buttons aufgerufen werden.
Weiterhin kann ein Settingsmenü aufgerufen werden.

Das Settingsmenü bietet die Möglichkeit die Hintergrundfarbe für alle Gamestates und Menüs mit Slidern einzustellen, VSync mit einer Checkbox an und aus zu schalten, einen Name in ein Textfeld einzugeben und gibt einige Tastaturfunktionen für alle Gamestates an.  

Die einzelnen Gamestates haben zusätzlich neue Funktion, welche unsere Gui Elemente einbinden.
Jeder Gamestate hat einen Button, welcher zurück zu Menü führt.
Sollte dieser Button nicht getätigt werden, wird der Gamestate nach 60 Sekunden automatisch beendet.

Neben den zwei reinen GUI Gamestates gibt es 5 Gamestates mit GUI Elementen und weiteren Game Engine Funktionen, nämlich Shooter, Lights, Spring, UpAndDown und FreeFall.

Beim Shooter werden Planeten auf Crates geschossen. Bei einem Treffer werden die Crates zerstört. Mit einem Button können neue Crates erstellt werden und mit einer Checkbox kann die Kollisionsdetection an und aus gestellt werden.

Lights ist ein Gamestate bei welchem sich ein Planet durch eine Reihe von farbigen Punktlichtquellen bewegt. Diese Quellen können mit Checkboxen an und aus gemacht werden.

Spring ist ein Planet welcher sich vertikal hoch und runter bewegt. Die Bewegung wird durch eine Feder, welche den Planeten in der Mitte fixiert beschrieben.
Mit einem Slider wird die Geschwindigkeit des Planetens modifiziert.

UpAndDown ist ein Planet der erst eine Geschwindigkeit nach oben besitzt, die nach 2 Sekunden in eine Geschwindigkeit nach unten geändert wird und nach wieder 2 Sekunden zurückgeändert wird.

FreeFall ist ein Planet und ein Crate, die erst eine Geschwindigkeit nach oben besitzen und dann durch ihre, die Geschwindigkeit verändernde Beschleunigung nach unten fallen.


## Entity-Creation-Interface
Das Entity-Creation-Interface kann genutzt werden um einfach neue Entitäten zu erstellen. Dieses Interface muss einmal mit der Funktion initialize initialisiert werden, was automatisch im Game Konstruktor geschieht. 

Das Interface bietet Funktionen um häufig genutzte Entitäten zu erstellen, wie beispielsweise einfach bewegende, rotierende, springartige oder schießbare Entitäten. 
Außerdem können mit generischen Funktionen, mit booleans zum Auswählen, beliebige Entitäten erstellt werden. Pointlights und Rectangles können auch damit erstellt werden.

Dieses Interface kann genutzt werden um beim Buttondrücken neue Entitäten zu erstellen, wie es beispielsweise im Shooter mit dem Button create Targets der Fall ist.

## Andere Teile der Gameengine
Die Gameengine benutzt ein Entity-Component-System mit einer Registry. Ein System greift auf die Registry zu und verändert mit einem Physik- und Zugriffssytem (Entitysystem, Componentsystem) Entitäten und Komponenten. Außerdem stellt das System ein Drawsystem bereit, bei welchem verschiedene Renderer verwendet werden. 

Eine Game Klasse verwaltet alle Gamestates und ruft für sie update und draw Methoden mit der Zeit als Parametern auf, wobei diese das System aufrufen. 

Kollisionserkennung kann mit der Systemfunktion removeIntersecting genutzt werden, wobei vorher Boxen mit dem boolean isTarget als true oder false hinzugefügt werden müssen. Es können mit dem System Pointlights hinzugefügt werden, wobei erst, mit dem System, Lightconstants hinzugefügt werden müssen oder es wird das Entity-Creation-Interface genutzt. 
