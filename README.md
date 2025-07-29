# Tarification des CDNs

Ce répertoire présente le code utilisé pour les simulations de mon sujet de PFE et de l'article écrit durant le stage.

## Lancer une simulation

Pour lancer un programme, il faut se placer dans le dossier correspondant à la version souhaitée, puis utiliser les commandes suivantes :

```
make
./choix_prix_cdn
```

Et à la fin de la simulation, la commande suivante permet d'éliminer les fichiers désormais inutiles :

```
make clean
```

Le code contenu dans "with_g" correspond au code dans le cas où les équations prennent en compte des coûts de stockage.
\
Le code contenu dans "no_g" correspond au code dans le cas où les équations ne prennent pas en compte de coûts de stockage.
\
Le code contenu dans "no_g_1_CP" correspond au code dans le cas où il n'y a qu'un seul CP dans l'analyse.
\
Le code contenu dans "no_g_omega" correspond au code dans le cas où on suppose que les CPs n'ont pas les mêmes contenus.

Les trois derniers dossiers contiennent le code utilisé dans l'article.

## Modifier une simulation

Actuellement, lancer une simulation tracera toutes les courbes de sortie en fonction de __V__, le volume total de contenus pour lesquels il y a de la demande, entre __V = 10__ et __V = 300__.
\
Pour tracer les résultats en fonction d'une autre variable ou pour un autre plage de valeurs, il faut modifier les premières lignes de la fonction `main` de *choix_prix_cdn.c*.

Les explications détaillées du fonctionnement du code se trouvent dans les fichiers du dossier *with_g*. Toutes les versions ont le même fonctionnement : ce sont seulement les équations, les valeurs et les structures qui changent (sauf pour le cas de *no_g_1_CP*, où tout l'équilibre de Nash est supprimé).
