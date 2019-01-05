La version actuellement en test est la **0.9.2**

N.B. : merci de me contacter directement en cas de problème avec les instructions.


**Pour l'installer sous Windows**

**INSTALLATION SOUS WINDOWS 7** ou plus récent :
* **INSTALLER le pilote OpenGL de votre machine** : soit il est fourni avec le CD/DVD fourni par le constructeur, soit il faut le télécharger sur son site (sans ce pilote, rien ne fonctionnera);
* **télécharger l'archive miniDart_0.9.2_test.exe**
* placer cette archive dans un endroit facile d'accès, par exemple sur le bureau ;
* double-cliquer dessus pour lancer l'installeur ;
* suivre les instructions.

**En particulier**

* il est recommandé d'**ajouter un raccourci** sur le bureau pour l'utiliser plus facilement.
* il est recommandé de choisir un répertoire de travail dans lequel on peut enregistrer des documents ( très utile pour l'enregistrement des séquences vidéos)
* SEUL l'enregistrement en 1280 x 720 (30 images par seconde) fonctionne actuellement.

**INSTALLATION SOUS LINUX INTEL**  (64 bits seulement)

**Ne concerne que les développeurs** Il n'y a pas d'archive pour débutant sous Linux pour l'instant, désolé
 
- PROCÉDURE et INSTRUCTIONS EN COURS D'ÉCRITURE, MERCI DE PATIENTER 
- Actuellement : toutes les instructions sont données sur le wiki [ICI](https://framagit.org/ericb/miniDart/wikis/Cr%C3%A9ation-de-l'environnement-de-compilation-sous-Linux)
- Le binaire testable avec LinuxMint s'appelle miniDart_0.9.2_LinuxMint.tar.gz (dans le dossier testing Linux)
- à venir : la version Ubuntu 18-04
- une fois téléchargée, décompresser l'archive dans un répertoire dans lequel vous avez le droit d'écrire.
- l'arborescence tient compte des dépendances, et si vous la modifiez, cela ne fonctionnera pas.
- pour tester, ouvrir un terminal et se placer dans le répertoire qui contient le binaire miniDart :

=> ensuite, il suffit de taper la ligne de commande :

**./miniDart_0.9.2_LinuxMint**

N.B.: il est préférable que la webcam (C920 fortement suggérée) soit BRANCHÉE AVANT DE LANCER miniDart (sous Linux)


**Bugs connus**
(en cours de mise à jour, en fonction des retours)
On peut quitter l'application dans certaines conditions, lors de la fermeture d'une vidéo en fin de lecture (résolu, sera dans la prochaine version)

SPÉCIFIQUES WINDOWS
ENREGISTREMENT : 1280 x 720 à 30 images par secondes est le SEUL FORMAT QUI DEVRAIT FONCTIONNER sous Windows.

(et 24 images par seconde sous Linux)

=> Je n'ai pas assez de retours utilisateurs pour en conseiller d'autres pour l'instant.

Le numéro des joueurs ne FONCTIONNE PAS CORRECTEMENT (le même numéro est attribué à tous les joueurs). Cela sera corrigé dans une version à venir.


Enfin, merci d'avance pour tout retour et n'oubliez pas de tester l'intégrité de l'archive téléchargée 

Have Fun ;-)

Eric Bachard


************************************************************************************************************

                   Somme de contrôle (sha 256)                                   Version

************************************************************************************************************

     204478fc7892d71e51272c0fff8e64988063493ec1b522f848f7e75fee8d77f3  miniDart_0.9.2_test.exe

************************************************************************************************************

