# NB 7: Cycli 

```{warning}
Vergeet niet naar main te gaan, te pullen, een nieuwe branch te maken vanaf main en daarin verder te werken!
```

In de vorige werkbladen heb je gezien hoe een gas zich gedraagt als het controlevolume wordt aangepast. Daarbij kan er arbeid worden uitgevoerd of warmte stromen. Door deze processen te combineren kunnen we ook thermodynamische processen in een zuiger simuleren. We gaan kijken of die voldoen aan de theorie uit het boek en waar deze afwijken. 

Eerst herhalen we de ingrediënten van de vorige werkbladen die we nodig hebben:

- klasse voor het deeltje met bijbehorende functies
- variabelen en randcondities van controle volume
- functies voor een lijst deeltjes

Daarna gaan we verschillende processen combineren en proberen we cyclische processen te bestuderen.


## Laden van eerdere code


De pakketten van Python en de constanten voor de simulatie.

```python tags=["NB7_cell1"]
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.optimize import curve_fit

BOX_SIZE_0 = 10              # Hoogte en lengte startvolume in nm
N = 40                       # Aantal deeltjes
V_0 = 0.4                    # Startsnelheid van deeltjes in nm per ps
RADIUS = 0.3                 # Straal van moleculen in nm
DT = 0.1 * RADIUS / V_0      # Tijdstap in ps (kort om geen botsing te missen)
V_PISTON_0 = -0.02 * V_0     # Startsnelheid van zuiger 
# (negatief betekent zowel links als rechts naar binnen gericht)
k_B = 1.38E-23               # constante van Boltzmann in J/K
```

De klasse voor het gasmolecuul en de interacties:

```python tags=["NB7_cell2"]
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

Voor de botsingen met de wanden moet je nu de code uit de vorige werkbladen kopieëren.

```{exercise} functies
:label: ex-cycli-01
Neem de functies die je in werkblad 5 hebt geprogrammeerd voor `left_right_colission` en in Notebook 6 hebt geprogrammeerd voor `top_down_collision` over in onderstaande code.
```


```python tags=["NB7_cell3"]
def top_down_collision(particle: ParticleClass):
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
        
def left_right_collision(particle: ParticleClass):
    """ verzorgen van botsingen met wand links en rechts, die als zuiger kunnen bewegen """
    global box_length, v_piston, impulse_outward, work
    if particle.r[0] > box_length/2 - particle.R:
        # kopieer de correcte code van de functie zoals je die in werkblad 5 hebt opgesteld
        ### YOUR CODE HERE ###
    elif particle.r[0] < -box_length/2 + particle.R:
        # kopieer de correcte code van de functie zoals je die in werkblad 5 hebt opgesteld
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

Om er zeker van te zijn dat de overgenomen code correct is, controleren we de werking met een eenvoudige functie die de zuiger verschuift bij constante temperatuur.

```python tags=["NB7_cell5"]
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
        print('busy, step:', i)


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

Als de code nu functioneert zien we een drukafhankelijkheid die in de buurt komt van het gedrag van een ideaal gas en een grotendeels constante temperatuur.

```{exercise} temperatuur
:label: ex-cycli-03
Overtuig je ervan dat de code werkt en pas eventuele onvolkomenheden aan.
```


## Aanpassingen voor warmtepomp

We gaan nu een warmtepomp maken. De warmtepomp transporteert warmte van een koud warmtebad op temperatuur `lo_temp` naar een warm warmtebad op temperatuur `hi_temp`. Het koude warmte bad zit vast aan de onderste wand, het warme warmtebad zit vast aan de bovenste wand van ons controlevolume. Om dit mogelijk te maken gaan we de functie `top_down_collision` volledig splitsen in een functie voor de twee wanden apart:

```{exercise} twee thermostaten
:label: ex-cycli-04
Splits je code voor de functie `top_down_collision` in twee functies. Een begin is hieronder al gemaakt. Om seperaat bij te houden wat er aan de koude kant en de warme kant gebeurt, hebben de globale variabelen voor de twee functies verschillende namen gekregen. Pas jouw code hierop aan. 
```

De functie `handle_walls` wordt hieronder ook veranderd. Dat is nodig om bij elke tijdstap de botsingen bij de boven- en onderwand via de nieuwe functies te berekenen. Aan deze functie hoef je niks te veranderen. 

```python tags=["NB7_cell6"]
def top_collision(particle: ParticleClass):
    """ verzorgen van botsingen met wand boven, die als thermostaat naar een hoge temperatuur werkt """
    global box_height, hi_temp, impulse_outward, hi_heat
    if particle.r[1] > box_height/2 - particle.R:
        v_voor = particle.v
        kin_voor = particle.kin_energy
        particle.r[1] = box_height/2 - particle.R
        particle.v[1] *= -1
        ### YOUR CODE HERE ###

def down_collision(particle: ParticleClass):
    """ verzorgen van botsingen met wand onder, die als thermostaat naar een lage temperatuur werkt """
    global box_height, lo_temp, impulse_outward, lo_heat
    if particle.r[1] < -box_height/2 + particle.R:
        v_voor = particle.v
        kin_voor = particle.kin_energy
        particle.r[1] = -box_height/2 + particle.R
        particle.v[1] *= -1
        ### YOUR CODE HERE ###

# pas onderstaande functie niet aan
def handle_walls(particles):
    """ botsing met wanden controleren voor alle deeltjes in lijst en gemiddeld bepaling druk """
    global pressure, impulse_outward, box_height, box_length   # om pressure buiten de functie te kunnen gebruiken
    impulse_outward = 0.0
    for p in particles:
        left_right_collision(p)
        top_collision(p)    
        down_collision(p)
    pressure = 0.95 * pressure + 0.05 * impulse_outward / ((2 * box_length + 2 * box_height) * DT) 
```

### Creatie en Compressie

We beginnen nu met hetzelfde gas als bij de simulaties hiervoor, in een ruimte van $10 \;\mathrm{nm} * 10 \;\mathrm{nm}$. Aan het begin van de simulatie zetten we de temperatuur op de onderste wand exact gelijk aan de starttemperatuur van het gas. De temperatuur voor de bovenste wand kiezen we exact twee keer zo hoog. We maken deze temperaturen allebei negatief om geen warmtecontact te hebben. Dan verkleinen we het volume tot $5 \;\mathrm{nm} * 10 \;\mathrm{nm}$ met behulp van de zuiger.

```python tags=["NB7_cell7"]
particles = []
# we kiezen het aantal tijdstappen zo dat de elke zuigerwand het volume met een kwart verkleind
N_steps = int(BOX_SIZE_0 / (4 * abs(V_PISTON_0) * DT))

volumes = np.zeros(N_steps, dtype=float)
pressures = np.zeros(N_steps, dtype=float)
temperatures = np.zeros(N_steps, dtype=float)
lo_heats = np.zeros(N_steps, dtype=float)
hi_heats = np.zeros(N_steps, dtype=float)
works = np.zeros(N_steps, dtype=float)

pressure = 0.0
work = 0.0
lo_heat = 0.0
hi_heat = 0.0
box_height = BOX_SIZE_0
box_length = BOX_SIZE_0           # zetten zuiger (terug) op beginwaarde
v_piston = V_PISTON_0
create_particles(particles)       # resetten deeltjes 
# zetten de thermostaat van de onderste wand op de starttemperatuur van het gas (negatief)
lo_temp = -temperature(particles) 
hi_temp = 2 * lo_temp


for i in range(N_steps):
    take_time_step(particles)
    volumes[i] = box_length * box_height
    pressures[i] = pressure
    temperatures[i] = temperature(particles)
    lo_heats[i] = lo_heat
    hi_heats[i] = hi_heat
    works[i] = work
    if i%500==0:
        print('busy, step:', i)


fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(12, 3))
ax4 = ax3.twinx()
ax1.set_xlabel('Volume')
ax1.set_ylabel('Work')
ax2.set_xlabel('Volume')
ax2.set_ylabel('Temperature')
ax3.set_xlabel('step')
ax3.set_ylabel('Temperature')
ax4.set_ylabel('Volume')
fig.tight_layout

ax1.plot(volumes, works, '-r')
ax2.plot(volumes, temperatures, '-b')
ax3.plot(temperatures, '-g')
ax4.plot(volumes, '-r')

line1, = ax3.plot(temperatures, '-g', label='Temperature')
line2, = ax4.plot(volumes, '-r', label='Volume')

ax3.legend(handles=[line1, line2], loc='best')
plt.subplots_adjust(wspace=0.3)   
plt.show()
```

### Warmteoverdracht

Als de vorige simulatie gelukt is, dan moet de temperatuur gedurende de compressie meer dan twee keer zo hoog zijn geworden (beredeneer waarom dit zo is). Dat betekent dat we nu warmte kunnen overdragen van ons gas naar het warmtebad aan de bovenste wand. De volgende simulatie doet dit door de zuiger stil te zetten en de waarde van de thermostaat van de bovenwand positief te maken. We  plotten vervolgens de temperatuur en de warmtestroom door de bovenwand per tijdsstap. 

```{exercise} Warmte afvoeren
:label: ex-cycli-05
Pas het aantal tijdstappen proefondervindelijk aan totdat jij vindt dat het proces lang genoeg is om evenzicht te bereiken.
Hint: Als je het aantal stappen wilt aanpassen moet je bovenstaande simulate natuurlijk opnieuw uitvoeren om de zuiger weer in dezelfde toestand te krijgen. 
```

```python tags=["NB7_cell8"]
N2_steps = 0 #kies je waarde voor het aantal tijdstappen
### YOUR CODE HERE ###

# we maken de arrays met de meetwaarden leeg voor de nieuwe simulatie 
volumes = np.zeros(N2_steps, dtype=float)
pressures = np.zeros(N2_steps, dtype=float)
temperatures = np.zeros(N2_steps, dtype=float)
lo_heats = np.zeros(N2_steps, dtype=float)
hi_heats = np.zeros(N2_steps, dtype=float)
works = np.zeros(N2_steps, dtype=float)

# en zetten de zuiger stil en de thermostaat van de bovenwand aan
v_piston = 0
hi_temp = abs(hi_temp)

# we passen de waardes voor lo_heat, hi_heat, box_height, box_width, work, lo_heat en hi_heat niet aan.
# we onthouden daarmee de geschiedenis van het vorige proces

for i in range(N2_steps):
    take_time_step(particles)
    volumes[i] = box_length * box_height
    pressures[i] = pressure
    temperatures[i] = temperature(particles)
    lo_heats[i] = lo_heat
    hi_heats[i] = hi_heat
    works[i] = work
    if i%500==0:
        print('busy, step:', i)
        
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(9, 3))
ax1.set_xlabel('Volume')
ax1.set_ylabel('Heat')
ax2.set_xlabel('Volume')
ax2.set_ylabel('Temperature')
fig.tight_layout

ax1.plot(hi_heats, '-r')
ax2.plot(temperatures, '-b')

plt.subplots_adjust(wspace=0.3)   
plt.show()
```

### Decompressie

Als de temperatuur van het gas in evenwicht is met de temperatuur van de bovenwand, dan is de maximale hoeveelheid warmte getransporteerd en moeten we terug naar de begintoestand. De eerste stap in dat proces is om de zuigerwand terug te bewegen naar de startpositie. Dat doen we door het warmtecontact in de bovenste wand uit te zetten, de zuiger naar buiten te laten bewegen en net zoveel stappen terug te zetten als in de eerste fase van de simulatie. 

```{exercise} Decompressie
:label: ex-cycli-06
Zet de cyclus nu voort met decompressie. Maak gebruik van de code die we voor de compressie (de eerste fase van de cyclus) hebben gebruikt. Plot dezelfde grafieken als bij de eerste fase van de cyclus (arbeid tegen volume en temperatuur tegen volume).
```

```python tags=["NB7_cell9"]
N3_steps = 0
# Hier jouw code

### YOUR CODE HERE ###
```

### Warmteoverdracht vanuit het koude bad

Omdat het volume een factor twee groter wordt en er tijdens de decompressie geen warmtecontact is, moet de eindtemperatuur lager zijn dan de temperatuur van de koude (onder-) wand. We kunnen nu dus warmte uit de koude omgeving onttrekken en deze transporteren naar de warme kant, precies zoals je voor een warmtepomp verwacht. 

```{exercise} Warmte opnemen
:label: ex-cycli-07
Gebruik nu de code die we in de tweede fase van de cyclus gebruikt hebben en pas deze aan. Deze keer moet de zuiger weer worden uitgezet en de thermostaat aan de koude kant juist aan gezet worden. Kies voor het hele proces weer een aantal tijdsstappen waarbij het gas duidelijk in evenzicht is met de koude wand. 
```

```python tags=["NB7_cell10"]
N4_steps = 0 #kies je waarde voor het aantal tijdstappen
### YOUR CODE HERE ###
```

### Cyclus en efficiëntie

Als je de fases van het proces goed hebt doorlopen, dan bevat de variabele `work` nu de totale arbeid die tijdens het proces is verricht, `hi_heat` de warmteoverdracht aan de warme wand en `lo_heat` de warmteoverdracht aan de koude wand.

```{exercise} Coefficient
:label: ex-cycli-08
Bereken uit deze getallen de COP van de warmtepomp.
```

```python tags=["NB7_cell11"]
COP = 0 # hier jouw berekening

### YOUR CODE HERE ###

print(COP)
```

```{exercise}
Klaar? Bekijk de twee branches, kies de beste en doe een `Pull Request` naar `Main`. Je partner reviewt de code, doet eventueel suggesties. Je partner is ook degene die uiteindelijke de Pull Request goedkeurt. Ga je voor excellent? Ga dan door met onderstaande opdracht.
```


## De Carnot-cyclus (uitbreiding 🌶) 

Nu dat we een volledige cyclus met de wamrtepomp hebben gemaakt is het interessant om te zien of we ook de efficiëntie van de pomp kunnen verhogen. Zoals vermeld in het theorie boek is de meest efficiënte cyclus de Carnot-cyclus. 

```{exercise} Warmte opnemen
:label: ex-cycli-10
Vul onderstaande code aan en simuleer een Carnot-cyclus met dezelfde temperaturen voor de twee warmtebaden (boven- en onderwand) en dezelfde bewegingsvrijheid voor de zuiger (van $10 \; \mathrm{nm} * 10 \; \mathrm{nm}$ naar $5 \; \mathrm{nm} * 10 \; \mathrm{nm}$).  
Let op dat de cyclus niet sluit na de eerste keer door de 4 fases heen te lopen. Je moet de cylus dus tweemaal doorlopen om echt in dezelfde toestand terug te komen. 
```

```python tags=["NB7_cell12"]
# We weten het aantal tijdstappen, omdat de zuiger tijdens een Carnot-cyclus telkens beweegt.
# Het totaal aanstappen is dus precies dat wat nodig is om de zuiger helemaal heen en terug te bwegen.
NC_steps = 2 * int(BOX_SIZE_0 / (4 * abs(V_PISTON_0) * DT)) + 4

volumes = np.zeros(NC_steps, dtype=float)
pressures = np.zeros(NC_steps, dtype=float)
temperatures = np.zeros(NC_steps, dtype=float)
lo_heats = np.zeros(NC_steps, dtype=float)
hi_heats = np.zeros(NC_steps, dtype=float)
works = np.zeros(NC_steps, dtype=float)

pressure = 0.0
work = 0.0
lo_heat = 0.0
hi_heat = 0.0
box_height = BOX_SIZE_0
box_length = BOX_SIZE_0           # zetten zuiger (terug) op beginwaarde
v_piston = V_PISTON_0
create_particles(particles)       # resetten deeltjes 
# zetten de thermostaat van de onderste wand op de starttemperatuur van het gas (negatief)
lo_temp = -temperature(particles) 
# zetten de thermostaat van de bovenste wand op het dubbele daarvan
hi_temp = 2 * lo_temp


i = 0 # counter voor het bijhouden van de huidige stap

# nu maken we de tijdsstappen en doorlopen we de vier fasen van de cyclus

# de eerste fase is een compressie zonder contact naar de warmtebaden 
# totdat het gas de temperatuur van de bovenwand bereikt

# de tweede fase is een compressie met thermisch contact aan debovenwand
# totdat de zuigerwand het minimale volume heeft bereikt

# de derde fase is een decompressie zonder contact naar de warmtebaden
# totdat het gas de temperatuur van de onderste wand bereikt

# de vierde fase is een decompressie met thermisch contact aan de onderwand 
# tot de zuigerwand het maximale volume heeft bereikt

### YOUR CODE HERE ###

# nu plotten we twee grafieken
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(9, 3))
ax1.set_xlabel('Volume')
ax1.set_ylabel('pressure')
ax2.set_xlabel('Volume')
ax2.set_ylabel('Temperature')
fig.tight_layout


ax1.plot(volumes[volumes>0], pressures[volumes>0], '-r')
ax2.plot(volumes[volumes>0], temperatures[volumes>0], '-b')

plt.subplots_adjust(wspace=0.3)   
plt.show()

```

Als je de fases van het proces goed hebt doorlopen, dan bevat de variabele `work` nu de totale arbeid die tijdens het proces is verricht, `hi_heat` de warmteoverdracht aan de warme wand en `lo_heat` de warmteoverdracht aan de koude wand.

```{exercise} COP Carnot
:label: ex-cycli-11
Bereken uit deze getallen de COP van de warmtepomp.
```

```python tags=["NB7_cell13"]
COP = 0 # hier jouw berekening

### YOUR CODE HERE ###

print(COP)
```

Je zult zien dat de COP niet overeenkomt met de theorie. 

```{exercise} COP Carnot
:label: ex-cycli-12
Welke waarde verwacht je voor de COP en waarom krijgen we die hier niet uit de simulatie? 
```

```python tags=["NB7_cell14"]

```
