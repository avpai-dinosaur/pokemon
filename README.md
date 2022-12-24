# pokemon
Pokemon game simulation implementing MST, Branch and Bound, and TSP heuristics.

With just a Pikachu , you, a Pokemon Trainer, have left Pallet Town to fill up your PokeDex and catch all the
pokemon! Pokemon are located all over, and you can walk, surf, or fly to get from pokemon to pokemon. On
your journey, you may face other Pokemon Trainers, who make traveling more difficult, but once you fight a
trainer, you never have to fight him/her again. Your task is to identify the most efficient routes for you to travel.
In Trainer Evasion Mode (Part A), we assume you have not faced any trainers and you must fight some of
them such that all pokemon in the country are reachable. You need to create Trainer-Free paths by fighting as
few trainers as possible and by traveling the least distance (one metric will account for both of these).
In Champion Mode (Parts B and C), we assume you have visited all of the country, beaten every Trainer, and
have access to Fly, meaning you can skip fighting trainers altogether and fly from pokemon to pokemon. You
will be constructing a path (rather, a cycle, as you return home) to catch every pokemon while minimizing the
distance traveled.
