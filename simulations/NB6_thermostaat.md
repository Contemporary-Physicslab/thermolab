# NB 6: Thermostaat

```{warning}
Vergeet niet naar main te gaan, te pullen, een nieuwe branch te maken vanaf main en daarin verder te werken!
```

In het vorige werkblad heb je gezien hoe een gas zich gedraagt in een zuiger waarbinnen wordt voldaan aan de wet van behoud van energie.
In dat geval geldt $p \propto \alpha V^{-k}$, waarbij $k$ een exponent is die wordt bepaald door de vrijheidsgraden van het gas - in onze simulatie simuleren we mono-atomaire gassen in 2D. 

We hadden kunnen denken dat we de ideale gaswet zouden kunnen gebruiken.
Echter, de temperatuur van het gas verandert door de werking van de zuiger - deze verricht arbeid. 

Arbeid uitgevoerd door (of op) het gas is een belangrijke interactie van het systeem met de omgeving. Uitwisseling van energie (toevoegen of afvoeren van warmte) is een tweede uitbreiding.
In deze simulatie onderzoeken we die uitbreiding waarbij we warmte aan- en afvoeren uit het controlevolume.
Op die manier kunnen we ook isotherme simulaties doen.

Eerst herhalen we de nodige ingrediënten:

- klasse voor het deeltje met bijbehorende functies
- variabelen en randcondities van controle volume
- functies voor een lijst deeltjes

Daarna voegen we de code toe voor het warmtecontact:

- introduceren thermostaat

En vervolgens
- bestuderen isotherm proces

## Laden van eerdere code

De pakketten van Python en de constanten voor de simulatie. Hierbij is de snelheid van de zuiger verder verlaagd.

% ```{exercise} constanten
% :label: ex-thermostat-01
% Neem de constanten die je in het vorige werkblad hebt gekozen hieronder over.
% ```


```python tags=["NB6_1"]
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.optimize import curve_fit

BOX_SIZE_0 = 10              # Hoogte en lengte startvolume in nm
N = 40                       # Aantal deeltjes
V_0 = 0.4                    # Startsnelheid van deeltjes in nm per ps
RADIUS = 0.3                 # Straal van moleculen in nm
DT = 0.1 * RADIUS / V_0      # Tijdstap in ps (kort om geen botsing te missen)
V_PISTON_0 = -0.02 * V_0      # Startsnelheid van zuiger 
# (negatief betekent zowel links als rechts naar binnen gericht)
k_B = 1.38E-23
```

De klasse voor het gasmolecuul met de interacties:

```python tags=["NB6_2"]
class ParticleClass:
    def __init__(self, m, v, r, R):
        """ maakt een deeltje (constructor) """
        self.m = m                         
        self.v = np.array(v, dtype=float)  
        self.r = np.array(r, dtype=float)  
        self.R = R

    def update_position(self):
        """ verandert positie voor één tijdstap """
        self.r += self.v * DT 
            
    @property
    def momentum(self):
        return self.m * self.v
    
    @property
    def kin_energy(self):
        return 1/2 * self.m * np.dot(self.v, self.v)
    
def collide_detection(p1: ParticleClass, p2: ParticleClass) -> bool:
    """ Geeft TRUE als de deeltjes overlappen """
    return np.linalg.norm(p1.r - p2.r) < (p1.R + p2.R)


def particle_collision(p1: ParticleClass, p2: ParticleClass):
    """ past snelheden aan uitgaande van overlap """
    m1, m2 = p1.m, p2.m
    delta_r = p1.r - p2.r
    delta_v = p1.v - p2.v
    dot_product = np.dot(delta_r, delta_v)
    # Als deeltjes van elkaar weg bewegen dan geen botsing
    if dot_product >= 0: # '='-teken voorkomt ook problemen als delta_r == \vec{0}
        return
    distance_squared = np.dot(delta_r, delta_r) 
    # Botsing oplossen volgens elastische botsing in 2D
    p1.v -= 2 * m2 / (m1 + m2) * dot_product / distance_squared * delta_r
    p2.v += 2 * m1 / (m1 + m2) * dot_product / distance_squared * delta_r
```

Voor de botsingen met de wanden moeten we nu de functies gebruiken zoals je die in notebook 5 hebt ontwikkeld.

```{exercise} functies
:label: ex-thermostat-01
Neem de functie die je in het vorige werkblad hebt geprogrammeerd voor `left_right_colission`over in onderstaande code:
```

```python tags=["NB6_3"]
def top_down_collision(particle: ParticleClass):
    global impulse_outward, box_height
    if abs(particle.r[1]) + particle.R > box_height / 2:
        particle.r[1] = np.sign(particle.r[1]) * (box_height/2 - particle.R)
        impulse_outward += abs(particle.momentum[1]) * 2
        particle.v[1] *= -1

def left_right_collision(particle: ParticleClass):
    """ verzorgen van botsingen met wand links en rechts, die als zuiger kunnen bewegen """
    global box_length, v_piston, impulse_outward, work
    if particle.r[0] > box_length/2 - particle.R:
        # kopieer de correcte code van de functie zoals je die in het vorige werkblad hebt opgesteld
        ### YOUR CODE HERE ###
    elif particle.r[0] < -box_length/2 + particle.R:
        # kopieer de correcte code van de functie zoals je die in het vorige werkblad hebt opgesteld
        ### YOUR CODE HERE ###

    return temp
        
def handle_collisions(particles):
    """ alle onderlinge botsingen afhandelen voor deeltjes in lijst """
    num_particles = len(particles)
    for i in range(num_particles):
        for j in range(i+1, num_particles):
            if collide_detection(particles[i], particles[j]):
                particle_collision(particles[i], particles[j])

def handle_walls(particles):
    """ botsing met wanden controleren voor alle deeltjes in lijst en gemiddeld bepaling druk """
    global pressure, impulse_outward, box_height, box_length   # om pressure buiten de functie te kunnen gebruiken
    impulse_outward = 0.0
    for p in particles:
        left_right_collision(p)
        top_down_collision(p)    
    pressure = 0.95 * pressure + 0.05 * impulse_outward / ((2 * box_length + 2 * box_height) * DT) 
    
def take_time_step(particles):
    """ zet tijdstap voor een lijst deeltjes en verwerk alle botsingen onderling en met wanden """
    global box_length, v_piston
    box_length += 2 * v_piston * DT # zowel links als rechts zuiger
    for p in particles:
        p.update_position()
    handle_collisions(particles)
    handle_walls(particles)  

```

## Test code

Voordat we de code aanpassen controleren we eerst of alles het doet. Hiervoor maken we zowel een $P,V$-diagram als een $T,V$-diagram (met als toevoeging een $P,T$-diagram) tijdens de werking van de zuiger. Let op! De eenheden van deze grafiek kunnen niet kloppen omdat er niet in verrekend zit welke constanten jij hebt gekozen. 

```python tags=["NB6_4"]
stepcount = 4000
particles = []
volumes = np.zeros(stepcount, dtype=float)
pressures = np.zeros(stepcount, dtype=float)
temperatures = np.zeros(stepcount, dtype=float)
# times = np.linspace(1, 100, 100)

pressure = 0.0
work = 0.0
box_height = BOX_SIZE_0
box_length = BOX_SIZE_0         # zetten zuiger terug
v_piston = V_PISTON_0
create_particles(particles)     # resetten deeltjes 
for i in range(stepcount):
    take_time_step(particles)
    volumes[i] = box_length * box_height
    pressures[i] = pressure * 28 * 1.7E-27 * 1E24
    temperatures[i] = temperature(particles)

fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(12, 3))
ax1.set_xlabel('Volume')
ax1.set_ylabel('Pressure')
ax2.set_xlabel('Volume')
ax2.set_ylabel('Temperature')
ax3.set_xlabel('Pressure')
ax3.set_ylabel('Temperature')

fig.tight_layout

ax1.plot(volumes, pressures, '-r')
ax2.plot(volumes, temperatures, '-b')
ax3.plot(pressures, temperatures, 'r--')
plt.subplots_adjust(wspace=0.3)      # afstand tussen subplots
plt.show()
```

Alles alle code netjes is overgenomen, dan zien we de druk, $P \propto V^{-k}$, met een exponent $k > 1$. In het vorige notebook hebben we gezien dat dit komt doordat de zuiger arbeid verricht op het gas en de temperatuur daardoor verhoogt. 

Maar wat gebeurt er wanneer we de temperatuur van het gas constant houden? Als we dat doen kunnen we controleren of de druk inderdaad invers proportioneel is met het volume, zoals de gaswet voorschrijft. 


## De thermostaat

In de werkelijkheid is de temperatuur van de wand bepaald door de amplitude van de trillingen van de deeltjes waaruit de wand bestaat.
De wederzijdse overdracht van de energie van die trillingen naar de kinetische energie van de gasmoleculen bepaalt het thermische contact tussen de wanden van het volume en het gas.
In ons model bestaat de wand echter helemaal niet uit deeltjes maar hebben we een denkbeeldige lijn getrokken in de ruimte.
We moeten daarom een wiskundige methode toepassen om de temperatuur van het gas te beïnvloeden. 

Er zijn in de literatuur verschillende van dit soort methodes bedacht.
Zo'n methode wordt een 'thermostaat' genoemd.
Elk van die type trucs hebben hun voor- en nadelen.
In ons geval houden we het simpel: Op het moment dat een gasmolecuul botst met de wand, dan schalen we de snelheid van dit molecuul met (de wortel van) de verhouding tussen de veronderstelde temperatuur van de wand en de gemiddelde temperatuur die het gas op dat moment heeft:

$$
  v_{\text{na}} = \sqrt{ \frac{T_{\text{wand}}} {T_{\text{gas}}} }v_{\text{voor}}
$$

Voor het gemak houden we de linker en rechter wand van het volume als zuiger en maken we het thermische contact aan de onder- en bovenwand.


```{exercise} code thermostaat
:label: ex-thermostat-03
Hieronder vind je een groot deel van de code voor de botsing van de deeltjes met de onder- en bovenwand. 
De functie is nu gesplitst in een apart deel voor de twee wanden apart om het makkelijker te maken met de plus- en mintekens. 
Voeg de code toe die de snelheid van het deeltje berekent ten gevolge van de thermostaat, zoals hierboven staat uitgelegd. De temperatuur van de wand is in deze functie gegeven door de variabele `set_temp`. Als deze een positieve waarde is, dan staat de thermostaat aan en moet de snelheid van de deeltjes na botsing correct worden geschaald. Als deze waarde negatief is, staat de thermostaat uit en blijft de absolute waarde van de snelheid contstant. 
Bereken hierna ook de totale stoot (`impulse-outward`) en de warmte (`heat`). 
Let op dat het teken van de warmte correct is. Warmte is positief als het stroomt van de omgeving naar het systeem.
Doe dit voor beide wanden.
```



```python tags=["NB6_5"]
def top_down_collision(particle: ParticleClass) -> None:
    """ verzorgen van botsingen met wand boven en onder, die als thermostaat kunnen werken """
    global box_height, set_temp, impulse_outward, heat
    if particle.r[1] > box_height/2 - particle.R:
        v_voor = particle.v
        kin_voor = particle.kin_energy
        particle.r[1] = box_height/2 - particle.R
        particle.v[1] *= -1
        ### YOUR CODE HERE ###
    elif particle.r[1] < -box_height/2 + particle.R:
        v_voor = particle.v
        kin_voor = particle.kin_energy
        particle.r[1] = -box_height/2 + particle.R
        particle.v[1] *= -1
        ### YOUR CODE HERE ###
```

Met deze nieuwe definitie van de functies, draaien we een simulatie waarin we zowel de temperatuur als de druk plotten als functie van het volume.

```{warning}
Deze simulatie bevat vanwege de tragere zuiger een veel groter aantal tijdstappen .
Daarom zal deze simulatie ook langer duren.
Maak je geen zorgen als het een minuut duurt om de berekening te voltooien.
```

Om verdere belasting van de processor tot een minimum te beperken, berekent deze simulatie ook alvast de totale warmte $Q$ en de totale arbeid $W$ tijdens het proces.
Deze worden opgeslagen in de arrays `heats` en `works`.
We zullen de resultaten van deze simulatie voor een aantal vervolgstappen gebruiken. 

```python tags=["NB6_6"]
particles = []
N_steps = 5000
volumes = np.zeros(N_steps, dtype=float)
pressures = np.zeros(N_steps, dtype=float)
temperatures = np.zeros(N_steps, dtype=float)
heats = np.zeros(N_steps, dtype=float)
works = np.zeros(N_steps, dtype=float)

pressure = 0.0
work = 0.0
heat = 0.0
box_height = BOX_SIZE_0
box_length = BOX_SIZE_0         # zetten zuiger terug
v_piston = V_PISTON_0
create_particles(particles)     # resetten deeltjes 
set_temp = temperature(particles)

for i in range(N_steps):
    take_time_step(particles)
    volumes[i] = box_length * box_height
    pressures[i] = pressure
    temperatures[i] = temperature(particles)
    heats[i] = heat
    works[i] = work
    if i%500==0:
        print(i)


fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(9, 3))
ax1.set_xlabel('Volume')
ax1.set_ylabel('Pressure')
ax2.set_xlabel('Volume')
ax2.set_ylabel('Temperature')
fig.tight_layout

ax1.plot(volumes, pressures, '-r')
ax2.plot(volumes, temperatures, '-b')

plt.subplots_adjust(wspace=0.3)   
plt.show()
```

```{exercise} fit P,V-diagram
:label: ex-thermostat-04
Maak een fit van het $(P,V)$-diagram uit de laatste simulatie.
(Maak dus niet een nieuwe simulatie, maar gebruik de gegevens van de vorige) De fit-functie hiervoor is alvast gegeven.
We kiezen hier voor een andere fitfunctie als bij het vorige werkblad. De formule die we hier gebruiken is een vereenvoudigde versie van de formule van Van der Waals.
Maak daarom weer gebruik van van de extra optie `p0=` in de argumenten van de functie `curve_fit`.
```

````{tip}
:class: dropdown
Het zelfstandig afschatten van `p0` is lastig, maar omdat de relatie tussen $p$ en $V$ bekend is, kunnen we wel een eerste stap maken door twee van onze metingen te nemen ($p_0$ en $p_1$ en $V_0$ en $V_1$), delen we die waarden door elkaar en nemen we links en rechts de logaritme krijgen we:

$$
  log(\frac{p_0}{p_1})=log(\frac{V_0}{V_1})(-n)
$$

Zo hebben we een schatting ($n_{init}$) voor $n$ gevonden. Door die waarde nu te substitueren vinden we een initiele guess ($a_{init}$) voor de waarde van $a$:

$$
  a = p_0V_0^{n_{init}}
$$

Let op het - teken!
Waar we nog verder rekening mee moeten houden is dat een verkeerde waarde voor $n_{init}$ kan zorgen voor hele grote fouten in $a_{init}$ omdat die exponentieel mee telt.
Omdat $p$ en $V$ fluctueren door het gering aantal deeltjes kunnen we beter waarden voor sampelen waarbij we een keuze maken moeten tussen grotere sample size voor reduceren van ruis en kleine sampele size om het effect van de zuiger klein te houden:

```python tags=["NB6_7"]
p0 = np.mean(pressures[100:110])
p1 = np.mean(pressures[N:N+10])
V0 = np.mean(volumes[100:110])
V1 = np.mean(volumes[N:N+10])

n_init = -np.log(p0/p1) / np.log(V0/V1)
a_init = p0 * V0**(-n_init)
```

```` 


```python tags=["NB6_8"]
p0 = np.mean(pressures[100:110])
p1 = np.mean(pressures[-10:])
V0 = np.mean(volumes[100:110])
V1 = np.mean(volumes[-10:])

n_init = -np.log(p0/p1) / np.log(V0/V1)
a_init = p0 * V0**(n_init)
print(n_init)
print(a_init)
```

```python tags=["NB6_9"]
def power_law(vol, a, b, n):
    return a * (vol-b)**n    

### YOUR CODE HERE ###

```

Als alles klopt, zie je dat de exponent van dit $(P,V)$-diagram heel dicht in de buurt zit van het gedrag dat je verwacht. Als je goed kijkt zie je dat de temperatuur voor kleinere volumes een steeds grotere afwijking vertoont. 

```{exercise} falende thermostaat
:label: ex-thermostat-06
Als je ervan uit kan gaan dat de afvoer van warmte evenredig is met het verschil tussen de starttemperatuur en de actuele temperatuur, verklaar dan waarom de temperatuur toeneemt voor kleine volumes.
```

```{solution} ex-thermostat-06
### YOUR CODE HERE ###
```


Al met al lijkt het resultaat van de simulatie er redelijk uit te zien.
Maar om een sterkere indicatie te hebben dat de simulatie correct is, moeten we weer een goede test verzinnen om de simulatie te verifiëren.
In dit geval kunnen we opnieuw controleren of de simulatie voldoet aan de eerste hoofdwet.

```{exercise} eerste hoofdwet herhaald
:label: ex-thermostat-07
De interne energie, de arbeid en de warmte (mits met de juiste voorfactoren en de juiste tekens) vormen een contstante som volgens formule 2.37 van het boek. Maak een grafiek die deze som plot als functie van het volume en laat zien dat deze inderdaad vlak is. Let hierbij vooral goed op je eenheden. Deze zijn namelijk gekozen bij het bepalen van de constanten aan het begin van dit werkblad. 
Om het jezelf makkelijker te maken kun je de verschillende grootheden onafhankelijk van elkaar plotten en de resultaten controleren, voordat je de definitieve grafiek opstelt.  
```

```python tags=["NB6_10"]
plt.figure()
plt.xlabel('Volume [nm$^{2}$]')
plt.ylabel('Energy [J]')

# maak de juiste combinatie van `temperatures`, `works`, en `heats` die contstant is. Let daarbij netjes op de eenheden. 
# plot deze combinatie als functie van de `volumes`.

### YOUR CODE HERE ###
```

In het boek wordt uitgelegd dat er maar twee vormen van energieoverdracht zijn van een systeem naar de omgeving. Dit kan via warmteoverdracht of via arbeid.
Bijzonder is dat deze beide grootheden alleen de snelheid van de moleculen beïnvloeden.
Toch zullen we in het volgende werkblad zien dat er een fundamenteel verschil zit in de werking van deze twee macroscopische grootheden. 

```{exercise}
Klaar? Bekijk de twee versies, kies de beste en doe een `Pull Request` naar `Main`. Je partner reviewt de code, doet eventueel suggesties. Je partner is ook degene die uiteindelijke  de Pull Request goed keurt.
```

```{exercise} 🌶 Uitbreiding
In bovenstaande simulatie zijn de onder- en bovenwand allebei een thermostaat die naar dezelfde temperatuur toewerken.
Als we die temperaturen verschillend maken, kunnen we kijken naar de thermische geleiding van het gas. 

- Maak nu een nieuwe simulatie met een volume met dezelfde breedte maar vier keer de hoogte, waarin de zuiger stilstaat. 
- Hou de dichtheid van de deeltjes hetzelfde, dus breid het aantal deeltjes uit naar 160. 
- Zet de temperatuur van de bovenwand op 150% en die van de onderwand op 50% van de starttemperatuur. 
- Door het warmtecontact via de bovenwand en de onderwand apart te rekenen, kan je zien hoeveel warmte er door het volume heenstroomt.
- Controller of deze warmtestroom evenredig is met het temperatuurverschil en omgekeerd evenredig met de afstand. (nogmaals: hou de dichtheid van de deeltjes constant)
- Geef de warmtegeleidingscoëfficiënt in W/K van het ideaalgas in je simulatie. (Opmerking: De warmtegeleidingscoëfficiënt wordt normaal gegeven in W/mK, maar in onze tweedimensionale simulatie valt de lengteafhankelijkheid er uit)
```

```python tags=["NB6_11"]

```
