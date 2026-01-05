--- 
authors: Freek Pols
updated: december 3, 2025    
---
# Portfolio opzetten (1h)

Dit is de GitHub repository voor het thermodynamica deel van IP2.
De bedoeling is dat je deze repository kloont en je vervolgens samen met je labpartner zelf een 'website / portfolio' bouwt op basis van jullie werk. 

Per duo volgt een de onderstaande stappen waarna de anderen uitgenodigd worden om bij te dragen aan deze repository.

```{mermaid}
graph TD;
    A["Kloon deze repository"] --> B["Verbreek de link met deze repository"];
    B --> C["Maak een nieuwe repository aan"];
    C --> D["Push de bestanden naar je nieuwe repository"];
    D --> E["Je partner(s) uitnodigen"]; 
    E --> F["samenwerken"] --> G;
    
    P1["Uitnodiging ontvangen"] --> P2["Uitnodiging accepteren"]; 
    P2 --> P3["De repo lokaal beschikbaar maken"] --> F; 

    G["Jullie eerste wijzigingen doorvoeren"];
```


## Klonen en opzetten van je eigen repository
Volg onderstaande instructie om je eigen repository op te stellen.

1. Ga naar de [repository](https://github.com/Contemporary-Physicslab/thermolab)
2. Klik op de groene knop `code` en kopier de url.
3. Open VSC en open een terminal (via `ctrl + ~` of via `view` $\rightarrow$ `terminal`).

````{note} Gebruik van de terminal
:class: dropdown
Je bent waarschijnlijk gewend om met een grafische interface (en je muis) te werken: Je klikt naar de locatie waar je heen wilt.
In de terminal werk je met commando's.
Navigeren door de terminal doe je met `cd <folder>` (waarbij `<folder>` de folder is waar je naartoe wilt gaan). 

- Met `cd ..` ga je een folder omhoog.
- Met `ls` of `dir` zie je de inhoud van de folder waar je in zit.
- Wil je naar een andere drive, dan typ je de drive letter gevolgd door een dubbele punt (bijv. `D:`).
- Met `mkdir <folder>` maak je een nieuwe folder aan (waarbij `<folder>` de naam is van de nieuwe folder).
- Met `rmdir <folder>` verwijder je een lege folder (waarbij `<folder>` de naam is van de folder die je wilt verwijderen). 
- Met `rm <file>` verwijder je een bestand (waarbij `<file>` de naam is van het bestand dat je wilt verwijderen). 
- Met `code .` open je de huidige folder in VSC.

```{warning}
Het gebruik van spaties in mapnamen is toegestaan op Linux en macOS, maar wanneer je die mappen in de terminal aanspreekt, moeten spaties voorafgegaan worden door een escape-teken (bijv. backslash: cd Documents/My\ Folder).
```
````

4. Navigeer in de terminal naar de locatie waar je je repository wilt opslaan (bijvoorbeeld `cd Documents/studie/jaar 1/IP2/`). Maak een nieuwe folder aan met `mkdir Project` en ga naar deze folder met `cd Project`.

5. Typ in de terminal `git clone <url>` (waarbij `<url>` de url is die je gekopieerd hebt (waarschijnlijk `https://github.com/Contemporary-Physicslab/thermolab.git`)) en druk op enter.

Nu worden alle bestanden van de repository gedownload naar je computer, maar deze zijn nog steeds gelinkt aan de originele repository (dus als je nu iets pusht, komt het in de originele repository terecht - dat mag echter niet want je hebt geen schrijfrechten).
We moeten dus de link met de originele repository verbreken en een nieuwe repository aanmaken.

6. Gebruik nu `cd thermolab` om in de thermolab folder te komen.

7. Als dat gelukt is typ `git remote remove origin` (om de link met de originele repository te verbreken).

Je kunt zowel via de terminal een nieuwe repository aanmaken als via de website van GitHub.
Hieronder staat de methode via de website van GitHub beschreven.
Ga naar [GitHub](https://github.com/) en doorloop onderstaande stap.

8. Maak een nieuwe repository aan op GitHub (via de `+` rechtsboven in je scherm en dan `new repository`).  
9. Kies jullie groepsnummer als naam voor je repository (dit zal ook deel uitmaken van je URL!), verifieer dan dat `Choose visibility *
` op `public` staat. Klik daarna op de groene knop `Create repositorty`.
10. Ga in je repository naar `settings` en in het linkermenu naar `Pages` en kies onder het kopje `Source` de optie `Github actions`.
11. Klik op `code` (linksboven).
12. Kopier de url van je nieuwe repository (deze heb je zo dadelijk nodig).

We hebben nu een nieuwe repository aangemaakt, maar deze is nog leeg.
We moeten nu de bestanden die we gedownload hebben naar deze nieuwe repository pushen.

13. Ga terug naar VSC.
14. Typ in de terminal `git remote add origin <url>` (waarbij `<url>` de url is van je nieuwe repository).

Nu zijn de bestanden op je computer gelinkt aan je nieuwe repository, maar staan ze nog niet in je nieuwe repository.
We moeten ze nu pushen.

15. Typ in de terminal `git push -u origin main` (om de wijzigingen naar je nieuwe repository te pushen).


Als je nu naar je nieuwe repository op GitHub gaat en de pagina ververst (via `F5` of via de `reload` knop in je browser), zie je dat alle bestanden zijn geupload. 

16. Klik op het `gear-icon` (naast **About**) aan de rechterkant van de pagina.
17. Vink het vakje **Use your GitHub Pages website** aan.

Je kunt nu ook de output zien op je eigen GitHub website!
Klik daarvoor de link die rechts staat onder `code` $\rightarrow$ onder **About**.


## Je partner(s) uitnodigen
Bij IP2 werk je in tweetallen of drietallen. Je kunt je partner(s) uitnodigen om mee te werken aan jouw repository.
Ga daarvoor naar je nieuwe repository op GitHub en doorloop onderstaande stappen.

1. Ga naar je nieuwe repository op GitHub.
2. Klik op `settings` (rechtsboven in je scherm).
3. Klik in het linkermenu op `Collaborators`.
4. Klik op de knop `Add people` onder het kopje `manage access`.
5. Typ de gebruikersnaam van je partner(s) en klik op `add <username> to this repository` (waarbij `<username>` de gebruikersnaam is van je partner).

Als partner krijg je een mailtje met een uitnodiging om mee te werken aan de repository.
Als je deze accepteert, kun je allebei wijzigingen aanbrengen in de repository.

## Partner accepteert uitnodiging
Je partner krijgt een mailtje met een uitnodiging om mee te werken aan de repository.
Als je deze accepteert, kun je allebei wijzigingen aanbrengen in de repository.

Volg stappen 1 tot en met 6, maar dan op basis van de url van de repository waarop je bent uitgenodigd.
Ga NIET verder met stap 7 want de koppeling met de bestaande repository wil je behouden.


## Je eerste wijzigingen doorvoeren
Je hebt nu een eigen repository met alle bestanden die nodig zijn om een website te bouwen.
Je kunt nu zelf aan de slag om de inhoud van de website aan te passen.
Belangrijkste is om eerst even de URL aan te passen in het hoofdbestand.

1. Open de `myst.yml` file in VSC. Daar zie je op twee plekken een URL naar github. Pas deze URL aan naar de URL van je eigen repository. 
2. Je kunt nu de repository bijwerken via de terminal, een andere optie is links in VSC te kliken op het `source control` icoon (het derde icoon van boven, dat eruit ziet als een vork met drie punten). Daar geef je een samenvatting van je wijzigingen en klik je op het vinkje (bovenaan) om de wijzigingen toe te voegen. Daarna klik je op de drie puntjes (bovenaan) en kies je `push` om de wijzigingen naar je repository te pushen.

```{warning} Let op
Nu anderen ook toegang hebben tot je repository, is het belangrijk dat je regelmatig kijkt of er wijzigingen zijn doorgevoerd.
Dit doe je door een `pull` uit te voeren (via de drie puntjes bovenaan in het `source control` menu en dan `pull` te kiezen).
Als je dit niet doet, kan het zijn dat je wijzigingen overschreven worden door die van een ander. 

Dus de volgende workflow is handig:
Als je start met werken: eerst een `pull` uitvoeren. Daarna je wijzigingen doorvoeren en deze `committen` en `pushen`.

Eventuele conflicten die ontstaan door gelijktijdig werken, kun je het systeem 'redelijk eenvoudig' oplossen.
```

```{note}
In de terminal kun je ook `git pull` typen om een pull uit te voeren, `git add .` om alle wijzigingen toe te voegen, `git commit -m "message"` om de wijzigingen te committen (waarbij `"message"` een korte omschrijving is van de wijzigingen die je doorvoert) en `git push` om de wijzigingen naar je repository te pushen.
```
