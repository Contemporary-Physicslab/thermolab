---
jupytext:
  formats: ipynb,md:myst
  text_representation:
    extension: .md
    format_name: myst
    format_version: 0.13
    jupytext_version: 1.19.5
kernelspec:
  display_name: base
  language: python
  name: python3


--- 

# Koelen van metalen buizen
<!-- vervang bovenstaande titel -->

+++

# Introductie

In het boek wordt in hoofdstuk 2 geschreven over warmtetransport. Dat kan op drie manieren plaatsvinden. Het is niet eenvoudig om deze drie verschillende vormen uit elkaar te houden. In het vak 'Fysische Transportverschijnselen', dat in het tweede jaar wordt gegeven, zal je zien dat de natuurkunde achter deze verschillende vormen van warmtetransport ook best ingewikkeld is. 

```{exercise}
Welke vormen van warmtetransport bestaan er?
```

In deze proef proberen we een inschatting te maken van de ordegrootte van de verschillende vormen van warmtetransport bij de koeling van een metalen buis aan lucht. 

+++

# Theorie

+++

Volgens [Newton's wet van afkoeling](https://nl.wikipedia.org/wiki/Afkoelingswet_van_Newton) is de snelheid waarmee een voorwerp afkoelt evenredig met het verschil in de temperatuur van het voorwerp ($T$) en de omgeving ($T_0$). We kunnen dit schrijven als:

$$
    \dot{Q} = -hA(T(t) - T_0),
$$

waarin 

- $\dot{Q}$ de warmtestroom in $\mathrm{W}$,
- $A$ het oppervlak waardoor koeling optreedt in $\mathrm{m}^2$,
- $h$ de warmteoverdrachtscoëfficiënt in $\mathrm{W/(m^2 K)}$.

Dit levert de differentiaalvergelijking

$$
    C\dot{T} = -hA(T(t) - T_0),
$$

met $C$ de warmtecapaciteit in $\mathrm{J/kg}$. Herschrijven met $\tau = \frac{C}{hA}$ levert:

$$
    -\tau\dot{T} = T(t) - T_0,
$$ (eq:dv1)

met als oplossing:

$$
    T(t) - T_0 = (T(0) - T_0)\text{e}^{-t/\tau}.
$$ (eq:dv1_opl)

We kunnen hieruit dus concluderen dat $\tau$ de karakteristieke tijdsduur is waarin de temperatuur van de buis een factor $\text{e}$ verlaagd ten opzichte van de omgevingstemperatuur. 

```{exercise} Oplossing DV controleren 
:label: ex-dvopl
Controleer bovenstaande door vergelijking [](#eq:dv1_opl) te substitueren in [](#eq:dv1).
```

+++

````{solution} ex-dvopl
#your code/answer
````

+++

We zijn hier voor het gemak uitgegaan van een $h$ die onafhankelijk is van de temperatuur. We weten echter dat warmtetransport door straling niet lineair gaat, maar als 

$$
    \dot{Q}_s = \epsilon \sigma A (T^4 - T_0^4).
$$ 

Voor kleine temperatuurverschillen ($\Delta T = T - T_0$) is dit te vereenvoudigen tot

$$
    \dot{Q}_s = \epsilon \sigma A ((T_0+\Delta T)^4 - T_0^4) \approx \epsilon \sigma 4A T_0^3 \Delta T.
$$

Zolang $\Delta T$ dus relatief klein is ten opzichte van $T_0$, kunnen we $h$ dus inderdaad als een constante beschouwen. 

```{exercise} Uitwerken
:label: ex_uitw

Laat zien dat bovenstaande geldt door eerst $(T_0+\Delta T)^4$ uit te schrijven, te substitueren en dan te bedenken dat $\Delta T$ klein is en $\Delta T^2$ dus nog kleiner.
```

```{solution} ex_uitw
#your code/answer


```


```{exercise} 
:label: ex_fout
Hoe groot is de fout in het warmtetransport door straling die we maken voor het temperatuurbereik waarin we deze proef uitvoeren?
```

```{solution} ex_fout
#your code/answer
```

+++

# Methode en materialen

## Ontwerp

## Materialen

- standaard met twee thermisch geïsoleerde grijparmen
- metalen buis me bijpassende dop
- thermometer (infrarood of thermokoppel)
- knijper voor bevestigen thermokoppel op buis
- warm water tussen 60 en 80 graden Celsius
- (evt) schuifmaat voor bepalen dimensies buis

## Procedure

Stop de buis in warm water en laat deze gedurende een paar minuten zitten om thermisch evenwicht te bereiken. Beantwoord ondertussen de volgende vragen met behulp van de tabel:

| Materiaal | $\rho$ in $\text{kg/m}^3$ | $C$ in $\text{J} / \text{(kg K)}$ |
| --------- | --------- | ------------------ |
| messing   | 8,73E3    | 3,8E2 |
| aluminium | 2,7E3     | 8,8E2 | 
| staal     | 7,9E3     | 4,7E2 | 


```{exercise} constanten
:label: ex_const
Wat is het buitenoppervlak van de buis?
Wat is de warmtecapaciteit van de buis?
Wat is de omgevingstemperatuur?
```



Pak de buis op met thermisch isolerende handschoenen (of direct met de geïsoleerde grijparm) en plaats deze in de grijparm met isolatieschoentjes.
Positioneer de thermometer voor optimale temperatuurlezing.
Meet als functie van tijd hoe lichaam koelt.
Wacht voldoende lang zodat je de karakteristieke tijd $\tau$ voor de afkoeling kan bepalen.

```{exercise} Configuratie
:label: ex_conf
Maakt het uit in welke richting je de (cilindrische) as van de buis legt? En waarom zit er een dop bij de buis?
```

```{solution} ex_conf

```

Doe dit voor twee of drie configuraties:

1. De buis met de as in verticale richting en afgesloten met dop.
2. De buis met de as in verticale richting *zonder* dop.
3. (alleen bij voldoende tijd) De buis met de as in horizontale richting en afgesloten met dop.

```{warning} Veiligheid
Zorg ervoor dat je de buis niet aanraakt terwijl deze warm is. De metalen buis geleidt warmte. Zelfs een temperatuur van 60 graden celsius is daarom al onaangenaam hoog. Hou de pan met het warme water ook op een veilige plek zodat deze niet kan omkiepen en het warme water over de grond opspat.
```


## Data analyse

- Bepaal de karakteristieke tijd $\tau$ waarin de temperatuur van buis afneemt. Deze kan verschillend zijn voor de drie bovenstaande configuraties. 
- Bereken hieruit de warmteoverdrachtscoëfficiënt. 
- Vergelijk je resultaten met je groepsgenoten die een vergelijkbare buis hebben gemeten (dit kan klassikaal).
- Welk deel van de warmteoverdrachtscoëfficiënt verwacht je dat gegeven is door de geleiding, straling en convectie? Onderbouw je redenering.

+++

# Resultaten

```{code-cell} ipython3
# Hier de data en de analyse

import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def exp_func(t, A, tau, T_omg):
    # A is verschiltemperatuur met omgeving aan start
    # tau is de karakteristieke tijd voor de koeling
    # T_omg is de omgevingstemperatuur
    return (A * np.exp(-t/tau) + T_omg)  

buitenoppervlak = 0.0 # bepaal zelf in m^2
warmtecapaciteit = 0.0 # bepaal de warmtecapaciteit in J/K

times = np.array([])
temps = np.array([])


# pas beginwaardes aan naar schatting
# Het aantal maxfev moet wellicht hoger voor goede convergentie van de waarde
popt, pocv = curve_fit(exp_func, times, temps, p0=[50, 1000, 20], maxfev=5000)

A_exp, tau_exp, T_omg_exp = popt

y_fit = exp_func(times, *popt)

plt.figure()
plt.xlabel('Time [s]')
plt.ylabel('Temperature [K]')

plt.plot(times, temps, 'bo', label='measurement')
plt.plot(times, y_fit, 'r-', 
         label='$T = %0.2f e^{-t/%0.4f} + %0.2f$' % (A_exp, tau_exp, T_omg_exp))

plt.legend()

plt.show()

h_exp = (warmtecapaciteit) / (tau_exp * buitenoppervlak)
 
print(h_exp) # warmteoverdrachtscoëfficiënt in W/m^2 K
# Sla figuren op met  
# 
# plt.savefig("Figuren/naam.png", dpi=450)

```

# Discussie en conclusie


#your code/answer
