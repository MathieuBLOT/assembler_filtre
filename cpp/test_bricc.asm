; Fichier assembleur de test, pour l'implémentation CPP
foo:
add r1, r1, r1
add r1, r1, r1

bar:
add r2, r2, r1
briump foo
briump bar

briump schtroumpf
add r0, r0, r0

schtroumpf:
briump non_exist
